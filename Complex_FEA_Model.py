import numpy as np
import matplotlib.pyplot as plt

#PAGE 84

#-------------------Globals-----------------------------------#
DISP_MAG = 10
N_POINTS = 20

Q_VALS = np.array([[1e4],
                   [0],
                   [0],
                   [1e4],
                   [0],
                   [0]])

#-- If Youngs/Area/Length are universal --#
YOUNGS = 2e11
AREA = 5e-4
LENGTH = 3 #ele 1 & 3
SECOND_MOMENT = 1e-5
#-- node matrices (vertical element go node 2 -> node 1)--#
NODES1 = np.array([[0, 0],
                   [0, 3]])

NODES2 = np.array([[0, 4.50],
                   [3, 3]])

NODES3 = np.array([[4.5, 4.5],
                   [3, 0]])

#--------------------------------------------------------------#

class element:
    def __init__(self, name, youngs, area, second_moment, length, alpha, T_distributed, A_distributed, coords, assembly):
        self.name = name
        self.youngs = youngs
        self.area = area
        self.second_moment = second_moment
        self.length = length
        self.alpha = alpha
        self.T_distributed = T_distributed
        self.A_distributed = A_distributed
        self.x = coords[0]
        self.y = coords[1]
        self.assembly = assembly
        self.q = None        

    def L(self):
        """Transformation Matrix"""
        alpha = np.deg2rad(self.alpha)
        L_e = np.array([[np.cos(alpha), np.sin(alpha), 0, 0, 0, 0],
                        [-np.sin(alpha), np.cos(alpha), 0, 0, 0, 0],
                        [0, 0, 1, 0, 0, 0],
                        [0, 0, 0, np.cos(alpha), np.sin(alpha), 0],
                        [0, 0, 0, -np.sin(alpha), np.cos(alpha), 0],
                        [0, 0, 0, 0, 0, 1]])
        return L_e 

    def K(self):
        """Element Stiffness Matrix (in element co-ords)"""
        beta = (self.area * (self.length ** 2)) / self.second_moment
        K_e = ((self.youngs * self.second_moment) / (self.length ** 3)) * (np.array([[beta, 0, 0, -beta, 0, 0],
                                                                                   [0, 12, 6 * self.length, 0, -12, 6 * self.length],
                                                                                   [0, 6 * self.length, 4 * (self.length ** 2), 0, -6 * self.length, 2 * (self.length ** 2)],
                                                                                   [-beta, 0, 0, beta, 0, 0],
                                                                                   [0, -12, -6 * self.length, 0, 12, -6 * self.length],
                                                                                   [0, 6 * self.length, 2 * (self.length ** 2), 0, -6 * self.length, 4 * (self.length ** 2)]]))
        return K_e
        
    def K_hat(self):
        """Element Stiffness Matrix (in global co-ords)"""
        return (self.L().T) @ (self.K()) @ (self.L())

    def K_g(self):
        """Elements Contribution to Global Stiffness Matrix"""
        return self.assembly @ (self.K_hat()) @ ((self.assembly).T)

    def F(self):
        """Element Nodal Forces (in global co-ords)"""
        return (self.K_hat()) @ (self.assembly).T @ self.q
        
    def f(self):
        """Element Internal Forces (in local co-ords)"""
        return self.K() @ self.d()
    
    def D(self):
        """Element Nodal Displacement (in global co-ords)"""
        return (self.assembly).T @ self.q
    
    def d(self):
        """Element Nodal Displacement (in local co-ords)"""
        return self.L() @ self.D()
    
    def strain(self):
        """Element Axial strain"""
        return (self.d()[3] - self.d()[0]) / self.length
    
    def stress(self):
        """Element Axial Stress"""
        return self.youngs * self.strain()
    
    def axial_feq(self):
        """Axial Distrbuted Equivilant Force (in local co-ords)"""
        fe_total =  np.zeros((6,1))
        
        for i in range(0, len(self.A_distributed[0]) - 1):
            load_type = self.A_distributed[0, i]
            load_mag = self.A_distributed[1, i]
            
            match load_type:
                case 0: 
                    #No load 
                    fe_total +=  np.zeros((6,1))
                case 1:
                    #Distributed Axial Load
                    fe_total +=  load_mag * self.length * np.array([[1/2],
                                                                     [0],
                                                                     [0],
                                                                     [1/2],
                                                                     [0],
                                                                     [0]])
                
                case 2:
                    #Concentrated Axial Load
                    dist_total +=  load_mag * np.array([[1 - (load_mag[1][1] / 2)],
                                                        [0],
                                                        [0],
                                                        [load_mag[1][1] / self.length],
                                                        [0],
                                                        [0]])
        return fe_total
    
    def transverse_feq(self):
        """Transverse Distrbuted Equivilant Force (in local co-ords)"""
        fe_total = np.zeros((6,1))
        
        for i in range(0, len(self.T_distributed[0])):
            load_type = self.T_distributed[0, i]
            load_mag = self.T_distributed[1, i]
            
            match load_type:
                case 0: 
                    #No load 
                    fe_segment =  np.zeros((6,1))
                case 1:
                    #Uniformly distributed load
                    fe_segment = load_mag * self.length * np.array([[0],
                                                                   [1/2],
                                                                   [self.length / 12],
                                                                   [0],
                                                                   [1/2],
                                                                   [-self.length / 12]])
                case 2:
                    #Linearly varying load
                    fe_segment = load_mag * self.length * np.array([[0],
                                                                   [3 / 20],
                                                                   [self.length / 30],
                                                                   [0],
                                                                   [7 / 20],
                                                                   [-self.length / 20]])
                case 3:
                    #Concentrated point load
                    fe_segment = load_mag * np.array([[0],
                                                     [1 / 2],
                                                     [self.length / 8],
                                                     [0],
                                                     [1 / 2],
                                                     [-self.length / 8]])
            
            fe_total += fe_segment
        return fe_total
    
    def feq(self):
        """Distrbuted Equivilant Force (in local co-ords)"""
        return self.axial_feq() + self.transverse_feq()
    
    def Feq(self):
        """Distrbuted Equivilant Force (in global co-ords)"""
        return (self.L()).T @ self.feq()
    
    def Qeq(self):
        """Element Nodal Forces (in global co-ords)"""
        return (self.assembly).T @ self.Feq()

    def reacton_forces(self):
            """Calculates the Vector of Reaction Forces"""
            return self.F() + (-self.Feq())

    def print_all(self):
        """Prints All Calculated Data for Element"""
        np.set_printoptions(precision = 2)
        print("Name: ", self.name, "\n\n",
              "Transformation Matrix: \n", self.L(), "\n\n",
              "Element Stiffness Matrix (in element co-ords): \n", self.K(), "\n\n",
              "Element Stiffness Matrix (in global co-ords): \n", self.K_hat(), "\n\n",
              "Elements Contribution to Global Stiffness Matrix: \n", self.K_g(), "\n\n",
              "Element Nodal Forces (in global co-ords): \n", self.F(), "\n\n",
              "Element Internal Forces (in local co-ords): \n", self.f(), "\n\n",
              "Element Nodal Displacement (in global co-ords): \n", self.D(), "\n\n",
              "Element Nodal Displacement (in local co-ords): \n", self.d(), "\n\n",
              "Reaction Forces: \n", self.reacton_forces(), "\n\n",
              "Element Axial strain: \n", self.strain(), "\n\n",
              "Element Axial Stress: \n", self.stress(), "\n\n")

def global_calcs(element_array):
    """Calculates Gobal Stiffness Matrix and Structure Displacement"""
    K_g = 0
    global Q_VALS
    Q_e = Q_VALS
    for ele in element_array:
        K_g += ele.K_g()
        Q_e += ele.Qeq().astype(np.int32)     
    q = np.linalg.solve(K_g, Q_e)
    
    return K_g, q

def axial_shape(ele, x_e):
  
    return (((1 - x_e) / ele.length) * ele.d()[0]) + ((x_e / ele.length) * ele.d()[3])

def transverse_shape(ele, x_e):
    N_1 = 1 - (( 3 * x_e ** 2) / ele.length ** 2) + ((2 * x_e ** 3) / ele.length ** 3)
    N_2 = ((x_e ** 3) / ele.length ** 2) - ((2 * x_e ** 2) / ele.length) + x_e
    N_3 = (( 3 * x_e ** 2) / ele.length ** 2) - ((2 * x_e ** 3) / ele.length ** 3)
    N_4 = ((x_e ** 3) / ele.length ** 2) - ((x_e ** 2) / ele.length)
    
    return (N_1 * ele.d()[1] + N_2 * ele.d()[2] + N_3 * ele.d()[4] + N_4 * ele.d()[5])

def calc_deflections(ele):
    x_e = np.linspace(0, ele.length, N_POINTS)
    axial_displacement_e = axial_shape(ele, x_e)
    transverse_displacement_e = transverse_shape(ele, x_e)
    rad_alpha = np.deg2rad(ele.alpha)
    xe_deflections = axial_displacement_e * np.cos(rad_alpha) - transverse_displacement_e * np.sin(rad_alpha)
    ye_deflections = axial_displacement_e * np.sin(rad_alpha) + transverse_displacement_e * np.cos(rad_alpha)
    
    return xe_deflections, ye_deflections
    
def plot_simple_structure(element_array):
    """Plots the Original Structure and Displaced Structure"""
    fig, axes = plt.subplots()
    
    for ele in element_array:
        axes.plot(ele.x, ele.y, '-', color = 'k', label = ele.name)
        new_x_start = ele.x[0] + DISP_MAG * ele.D()[0]  
        new_y_start = ele.y[0] + DISP_MAG * ele.D()[1]
        new_x_end = ele.x[1] + DISP_MAG * ele.D()[3]
        new_y_end = ele.y[1] + DISP_MAG * ele.D()[4]
        axes.plot([new_x_start[0], new_x_end[0]], [new_y_start[0], new_y_end[0]], '-.', color = 'r', label = "Deformed " + ele.name)
    
    axes.legend(loc = (.025, 1.01), prop = {'size':7}, ncol = len(element_array))
    axes.set_xlabel("Global X [m]")
    axes.set_ylabel("Global Y [m]")
    axes.grid(True)
    plt.show()
    
def plot_complex_structure(element_array):
    """Plots the frame deformation of the structure"""
     
    fig, axes = plt.subplots()
     
    for ele in element_array:
        xe_deflections, ye_deflections = calc_deflections(ele)
        undeflected_x = np.linspace(ele.x[0], ele.x[1], N_POINTS)
        undeflected_y = np.linspace(ele.y[0], ele.y[1], N_POINTS)

        deflected_x = undeflected_x + DISP_MAG * xe_deflections
        deflected_y = undeflected_y + DISP_MAG *ye_deflections

        print(deflected_x[0], '\n\n')
        print(deflected_x[-1], '\n\n')
        axes.plot(undeflected_x, undeflected_y, '-', color = 'k', linewidth = 3, label = ele.name)
        axes.plot(deflected_x, deflected_y, '.-', color = 'r', label = "Deformed " + ele.name)
        
    axes.legend(loc = (.025, 1.01), prop = {'size':7}, ncol = len(element_array))
    axes.set_xlabel("Global X [m]")
    axes.set_ylabel("Global Y [m]")
    axes.grid(True)
    plt.show()

        
def main():
    #---Element Template elementx(name, youngs, area, second_moment, length, alpha, T_distributed, A_distributed, coords, assembly)
    element1 = element('Element One', YOUNGS, AREA, SECOND_MOMENT, 3, 90, np.array([[0], [0]]), np.array([[0], [0]], dtype = object), NODES1, np.array([[0, 0, 0, 1, 0, 0],
                                                                                                                                                             [0, 0, 0, 0, 1, 0],
                                                                                                                                                             [0, 0, 0, 0, 0, 1],
                                                                                                                                                             [0, 0, 0, 0, 0, 0],
                                                                                                                                                             [0, 0, 0, 0, 0, 0],
                                                                                                                                                             [0, 0, 0, 0, 0, 0]]))
    
    element2 = element('Element Two', YOUNGS, AREA, SECOND_MOMENT, 4.5, 0, np.array([[1, 3], [-1e4, -5e4]]), np.array([[0], [0]], dtype = object), NODES2, np.array([[1, 0, 0, 0, 0, 0],
                                                                                                                                                                     [0, 1, 0, 0, 0, 0],
                                                                                                                                                                     [0, 0, 1, 0, 0, 0],
                                                                                                                                                                     [0, 0, 0, 1, 0, 0],
                                                                                                                                                                     [0, 0, 0, 0, 1, 0],
                                                                                                                                                                     [0, 0, 0, 0, 0, 1]]))
    
    element3 = element('Element Three', YOUNGS, AREA, SECOND_MOMENT, 3, 270, np.array([[0], [0]]), np.array([[0], [0]], dtype = object), NODES3, np.array([[0, 0, 0, 0, 0, 0],
                                                                                                                                                                [0, 0, 0, 0, 0, 0],
                                                                                                                                                                [0, 0, 0, 0, 0, 0],
                                                                                                                                                                [1, 0, 0, 0, 0, 0],
                                                                                                                                                                [0, 1, 0, 0, 0, 0],
                                                                                                                                                                [0, 0, 1, 0, 0, 0]]))

    
    element_array = np.array([element1, element2, element3])
    
    #global stiffness - dof
    K_g, q = global_calcs(element_array)
    element1.q = element2.q = element3.q = q
    
    element1.print_all()
    plot_simple_structure(element_array)
    plot_complex_structure(element_array)
   
main()