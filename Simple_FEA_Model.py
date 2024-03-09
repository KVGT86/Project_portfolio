import numpy as np
import matplotlib.pyplot as plt

#SUPPLIMENTAL QUESTION

#-------------------Globals-----------------------------------#
DISP_MAG = 1

Q = np.array([[0],
              [0],
              [0],
              [0],
              [-150e3],
              [0]])

#-- If Youngs/Area/Length are universal (vertical element go node 2 -> node 1) --#
YOUNGS = 200e9
AREA = 2e-4
# LENGTH = 10
SECOND_MOMENT = 640e-6
#-- node matrices --#
NODES1 = np.array([[0, 5],
                   [2, 2]])

NODES2 = np.array([[5, 7.5],
                   [2, 2]])

NODES3 = np.array([[5, 5],
                   [2, 0]])
#--------------------------------------------------------------#

class element:
    def __init__(self, name, youngs, area, second_moment, length, alpha, coords, assembly):
        self.name = name
        self.youngs = youngs
        self.area = area
        self.second_moment = second_moment
        self.length = length
        self.alpha = alpha
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
        return np.round(L_e, 5)

    def K(self):
        """Element Stiffness Matrix (in element co-ords)"""
        beta = (self.area * (self.length ** 2)) / self.second_moment
        K_e = ((self.youngs * self.second_moment) / (self.length ** 3)) * (np.array([[beta, 0, 0, -beta, 0, 0],
                                                                                   [0, 12, 6 * self.length, 0, -12, 6 * self.length],
                                                                                   [0, 6 * self.length, 4 * (self.length ** 2), 0, -6 * self.length, 2 * (self.length ** 2)],
                                                                                   [-beta, 0, 0, beta, 0, 0],
                                                                                   [0, -12, -6 * self.length, 0, 12, -6 * self.length],
                                                                                   [0, 6 * self.length, 2 * (self.length ** 2), 0, -6 * self.length, 4 * (self.length ** 2)]]))
        return np.round(K_e, 5)
        
    def K_hat(self):
        """Element Stiffness Matrix (in global co-ords)"""
        return (self.L()).T @ (self.K()) @ (self.L())

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
        return (self.d()[1] - self.d()[0]) / self.length
    
    def stress(self):
        """Element Axial Stress"""
        return self.youngs * self.strain()
    
    def print_all(self):
        """Prints All Calculated Data for Element"""
        np.set_printoptions(precision = 5)
        print("Name: ", self.name, "\n\n",
              "Transformation Matrix: \n", self.L(), "\n\n",
              "Element Stiffness Matrix (in element co-ords): \n", self.K(), "\n\n",
              "Element Stiffness Matrix (in global co-ords): \n", self.K_hat(), "\n\n",
              "Elements Contribution to Global Stiffness Matrix: \n", self.K_g(), "\n\n",
              "Element Nodal Forces (in global co-ords): \n", self.F(), "\n\n",
              "Element Internal Forces (in local co-ords): \n", self.f(), "\n\n",
              "Element Nodal Displacement (in global co-ords): \n", self.D(), "\n\n",
              "Element Nodal Displacement (in local co-ords): \n", self.d(), "\n\n",
              "Element Axial strain: \n", self.strain(), "\n\n",
              "Element Axial Stress: \n", self.stress(), "\n\n")

def global_calcs(element_array):
    """Calculates Gobal Stiffness Matrix and Structure Displacement"""
    K_g = 0
    for ele in element_array:
        K_g += ele.K_g()     
    q = np.linalg.solve(K_g, Q)
    
    return K_g, q

def plot_structure(element_array):
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
    
def main():
    element1 = element('Element One', YOUNGS, AREA, SECOND_MOMENT, 5, 0, NODES1, np.array([[0, 0, 0, 1, 0, 0],
                                                                                           [0, 0, 0, 0, 1, 0],
                                                                                           [0, 0, 0, 0, 0, 1],
                                                                                           [0, 0, 0, 0, 0, 0],
                                                                                           [0, 0, 0, 0, 0, 0],
                                                                                           [0, 0, 0, 0, 0, 0]]))
    
    element2 = element('Element Two', YOUNGS, AREA, SECOND_MOMENT, 2.5, 0, NODES2, np.array([[1, 0, 0, 0, 0, 0],
                                                                                             [0, 1, 0, 0, 0, 0],
                                                                                             [0, 0, 1, 0, 0, 0],
                                                                                             [0, 0, 0, 1, 0, 0],
                                                                                             [0, 0, 0, 0, 1, 0],
                                                                                             [0, 0, 0, 0, 0, 1]]))
    
    element3 = element('Element Three', 70e9, 400e-6, 1e-10, 2, -90, NODES3, np.array([[1, 0, 0, 0, 0, 0],
                                                                                       [0, 1, 0, 0, 0, 0],
                                                                                       [0, 0, 1, 0, 0, 0],
                                                                                       [0, 0, 0, 0, 0, 0],
                                                                                       [0, 0, 0, 0, 0, 0],
                                                                                       [0, 0, 0, 0, 0, 0]]))
    
    
    element_array = np.array([element1, element2, element3])
    
    
    #global stiffness - dof
    K_g, q = global_calcs(element_array)
    element1.q = element2.q = element3.q = q
    print("\n\n" , q, "\n\n", K_g)
    plot_structure(element_array)
    
main()