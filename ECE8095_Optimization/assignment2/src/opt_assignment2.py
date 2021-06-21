#!/usr/bin/env python
# coding: utf-8

# # Import Libraries

# In[1]:


import numpy as np
from scipy.optimize import linprog


# # Optimizer

# In[2]:


class Optimizer_ver_0:
    def __init__(self, matrix):
        self.A = matrix
        self.p_r = np.ones(self.A.shape[0]) / self.A.shape[0] 
        self.p_c = np.ones(self.A.shape[1]) / self.A.shape[1]
    
    def optimize_r(self):
        A_eq = np.ones(self.p_r.size)
        b_eq = np.ones(1)
        A_ub = self.A @ self.p_c.T
        b_ub = self.p_r @ self.A @ self.p_c.T

        c = A_ub.reshape(1,-1)
        A_eq = A_eq.reshape(1,-1)
        b_eq = b_eq
        A_ub = A_ub.reshape(1,-1)
        b_ub = b_ub
        
        print(c.shape, A_eq.shape, b_eq.shape, A_ub.shape, b_ub.shape)
        lp_result = linprog(c=c,
                               A_eq=A_eq,
                               b_eq=b_eq,
                               A_ub=A_ub,
                               b_ub=b_ub,
                               bounds=[(0,1)]*self.p_r.size)
        self.p_r = np.array(lp_result.x)

    def optimize_c(self):
        A_eq = np.ones(self.p_c.size)
        b_eq = np.ones(1)
        A_ub = self.p_r @ -self.A
        b_ub = self.p_r @ -self.A @ self.p_c.T

        c = A_ub.reshape(1,-1)
        A_eq = A_eq.reshape(1,-1)
        b_eq = b_eq
        A_ub = A_ub.reshape(1,-1)
        b_ub = b_ub
        
        #print(c.shape, A_eq.shape, b_eq.shape, A_ub.shape, b_ub.shape)
        lp_result = linprog(c=c,
                          A_eq=A_eq,
                          b_eq=b_eq,
                          bounds=[(0,1)]*self.p_c.size)
        self.p_c = np.array(lp_result.x)
    
    def optimize(self):
        cnt=10
        while cnt>0:
            self.optimize_r()
            self.optimize_c()
            print(self.p_r, self.p_c)
            cnt-=1


# In[3]:


class Optimizer:
    def __init__(self, matrix):
        self.A = matrix
        self.p_r = np.ones(self.A.shape[0]) / self.A.shape[0] 
        self.p_c = np.ones(self.A.shape[1]) / self.A.shape[1]
    
    def optimize_r(self):
        A_eq = np.ones((1, self.p_r.size))
        b_eq = np.ones(1)
        A_ub = self.A
        b_ub = np.zeros(self.p_c.size)

        # Set input parameters for linprog function
        c = np.zeros((1, self.p_r.size + 1))
        c[0, -1] = -1
        A_eq = np.concatenate((A_eq, np.zeros((1,1))), axis=1)
        b_eq = b_eq
        A_ub = np.concatenate((-A_ub, np.ones((1, self.p_c.size))), axis=0).T
        b_ub = b_ub
        
        #print(c.shape, A_eq.shape, b_eq.shape, A_ub.shape, b_ub.shape)
        lp_result = linprog(c=c,
                               A_eq=A_eq,
                               b_eq=b_eq,
                               A_ub=A_ub,
                               b_ub=b_ub,
                               bounds=[(0,1)]*self.p_r.size + [(np.min(self.A), np.max(self.A))])
        return lp_result

    def optimize_c(self):
        A_eq = np.ones((1, self.p_c.size))
        b_eq = np.ones(1)
        A_ub = self.A.T
        b_ub = np.zeros(self.p_r.size)

        # Set input parameters for linprog function
        c = np.zeros((1, self.p_c.size + 1))
        c[0, -1] = -1
        A_eq = np.concatenate((A_eq, np.zeros((1,1))), axis=1)
        b_eq = b_eq
        A_ub = np.concatenate((-A_ub, np.ones((1, self.p_r.size))), axis=0).T
        b_ub = b_ub
        
        #print(c.shape, A_eq.shape, b_eq.shape, A_ub.shape, b_ub.shape)
        lp_result = linprog(c=c,
                               A_eq=A_eq,
                               b_eq=b_eq,
                               A_ub=A_ub,
                               b_ub=b_ub,
                               bounds=[(0,1)]*self.p_c.size + [(np.min(self.A), np.max(self.A))])
        return lp_result
    
    def optimize(self):
        result_r = self.optimize_r().x
        result_c = self.optimize_c().x
        print("probability of row is ", result_r[:-1])
        print("value is ", result_r[-1])
        print("probability of col is ", result_c[:-1])
        print("value is ", result_c[-1])

    
    def print_saddle_point(self):
        mins = np.amin(self.A, axis=1)
        maxs = np.amax(self.A, axis=0)
        max_min = np.max(mins)
        min_max = np.min(maxs)

        if max_min != min_max :
            print("No Saddle Point")
        else :
            row_idxs = np.argwhere(self.A == max_min)
            for row_idx in row_idxs:
                print("(%f, %f)" %(row_idx[0]+1, row_idx[1]+1))
        


# In[4]:


matrix_0 = np.array([[1,2,3],
                     [4,5,6],
                     [7,8,9]])
problem_0 = Optimizer(matrix_0)

problem_0.optimize()
problem_0.print_saddle_point()


# In[5]:


matrix_1 = np.array([[4,3,1,4],
                     [2,5,6,3],
                     [1,0,7,0]])
problem_1 = Optimizer(matrix_1)

problem_1.optimize()
problem_1.print_saddle_point()


# In[6]:


matrix_2 = np.array([[0, 5, -2],
                     [-3, 0, 4],
                     [6, -4, 0]])
problem_2 = Optimizer(matrix_2)

problem_2.optimize()
problem_2.print_saddle_point()


# In[7]:


matrix_3 = np.array([[5,8,3,1,6],
                     [4,2,6,3,5],
                     [2,4,6,4,1],
                     [1,3,2,5,3]])
problem_3 = Optimizer(matrix_3)

problem_3.optimize()
problem_3.print_saddle_point()


# In[ ]:




