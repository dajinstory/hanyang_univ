#!/usr/bin/env python
# coding: utf-8

# # Import Libraries

# In[1]:


import numpy as np


# # Optimizer - MAX

# In[2]:


class Optimizer:
 
    def __init__(self, problem, init_alpha=1.0, beta=1e-4, rho=0.5, epsilon=1e-6):
        self.problem = problem         # problem to optimize
        self.init_alpha = init_alpha   # init_alpha to control step_size
        self.beta = beta               
        self.rho = rho                 # parameter to be multiplied to alpha
        self.epsilon = epsilon         # very small number to check if there is a significant change
    
    def optimize(self, x):
        while True:
            df = self.problem.df(x)
            alpha = self.init_alpha
            
            # Stopping Criterion 1.
            if abs(np.dot(df, alpha*df)) < self.epsilon:
                # No significant gradient on current point
                break
            
            # Find next_x (= current_x + vector)
            # Direction of vector is df, and size of vector is calculated below
            # Decrease alpha until next_x satisfying all conditions
            while True:
                p = df
                nx = x + alpha*p
                
                # Check if next_x is satisfying x's constraints
                if self.problem.check_constraints(nx):
                    # Check if next_x is satisfying Armijo's rule
                    if not self.problem.f(nx) <= self.problem.f(x) + alpha*self.beta*np.dot(p, df):
                        break
                alpha *= self.rho
            
            # Stopping Criterion 2.
            if np.sqrt(np.dot(nx-x, nx-x)) < self.epsilon:
                # Edge case that maximum point is on the edge of domain
                # Judge that the point is converged
                x = nx
                break
                
            x = nx

            
        return x
        
        


# # Problems

# In[3]:


class Problem1:
    def __init__(self, weights):
        w1, w2, w3 = weights
        self.w1 = w1
        self.w2 = w2
        self.w3 = w3

    # function f
    def f(self, x):
        x1, x2, x3 = x
        return self.w1*np.log(x1) + self.w2*np.log(x2) + self.w3*np.log(x3) - 1/((1-x1-x2-x3)**2)
        
    # differential function f about x
    def df(self, x):
        x1, x2, x3 = x
        return np.array([
            self.w1/x1 - 2/((1-x1-x2-x3)**3),
            self.w2/x2 - 2/((1-x1-x2-x3)**3),
            self.w3/x3 - 2/((1-x1-x2-x3)**3)
        ])
    
    # check if x satisfy constraints
    def check_constraints(self, x):
        x1, x2, x3 = x
        if x1<=0 or x2<=0 or x3<=0 or x1+x2+x3>=1:
            return False
        return True


# In[4]:


class Problem2:
    def __init__(self, weights):
        w1, w2, w3 = weights
        self.w1 = w1
        self.w2 = w2
        self.w3 = w3

    # function f
    def f(self, x):
        x1, x2, x3 = x
        return self.w1*np.log(x1) + self.w2*np.log(x2) + self.w3*np.log(x3) - 1/(1-x1-x2)**2 - 1/(1-x1-x3)**2
    
    # differential function f about x
    def df(self, x):
        x1, x2, x3 = x
        return np.array([
            self.w1/x1 - 2/((1-x1-x2)**3) - 2/((1-x1-x3)**3),
            self.w2/x2 - 2/((1-x1-x2)**3),
            self.w3/x3 - 2/((1-x1-x3)**3)
        ])
    
    # check if x satisfy constraints
    def check_constraints(self, x):
        x1, x2, x3 = x
        if x1<=0 or x2<=0 or x3<=0 or x1+x2>=1 or x1+x3>=1:
            return False
        return True


# ## Problem #1

# ### Case #1 - w1=1, w2=1, w3=1

# In[5]:


weights = np.array([1.0, 1.0, 1.0])
problem = Problem1(weights)
optimizer = Optimizer(problem)

x0 = np.array([0.01, 0.01, 0.01])
x_opt = optimizer.optimize(x0)
print("x1 : %.6f\nx2 : %.6f\nx3 : %.6f\n" % (x_opt[0], x_opt[1], x_opt[2]))


# ### Case #2 - w1=1, w2=2, w3=3

# In[6]:


weights = np.array([1.0, 2.0, 3.0])
problem = Problem1(weights)
optimizer = Optimizer(problem)

x0 = np.array([0.01, 0.01, 0.01])
x_opt = optimizer.optimize(x0)
print("x1 : %.6f\nx2 : %.6f\nx3 : %.6f\n" % (x_opt[0], x_opt[1], x_opt[2]))


# ### Case #3 - custom

# In[7]:


weights = np.array([1.0, 4.0, 7.0])
problem = Problem1(weights)
optimizer = Optimizer(problem)

x0 = np.array([0.01, 0.01, 0.01])
x_opt = optimizer.optimize(x0)
print("x1 : %.6f\nx2 : %.6f\nx3 : %.6f\n" % (x_opt[0], x_opt[1], x_opt[2]))


# ### Case #4 - custom2

# In[8]:


weights = np.array([1.0, -2.0, 3.0])
problem = Problem1(weights)
optimizer = Optimizer(problem)

x0 = np.array([0.01, 0.01, 0.01])
x_opt = optimizer.optimize(x0)
print("x1 : %.6f\nx2 : %.6f\nx3 : %.6f\n" % (x_opt[0], x_opt[1], x_opt[2]))


# ## Problem #2

# ### Case #1 - w1=1, w2=1, w3=1

# In[9]:


weights = np.array([1.0, 1.0, 1.0])
problem = Problem2(weights)
optimizer = Optimizer(problem)

x0 = np.array([0.01, 0.01, 0.01])
x_opt = optimizer.optimize(x0)
print("x1 : %.6f\nx2 : %.6f\nx3 : %.6f\n" % (x_opt[0], x_opt[1], x_opt[2]))


# ### Case #2 - w1=1, w2=2, w3=3

# In[10]:


weights = np.array([1.0, 2.0, 3.0])
problem = Problem2(weights)
optimizer = Optimizer(problem)

x0 = np.array([0.01, 0.01, 0.01])
x_opt = optimizer.optimize(x0)
print("x1 : %.6f\nx2 : %.6f\nx3 : %.6f\n" % (x_opt[0], x_opt[1], x_opt[2]))


# ### Case #3 - custom

# In[11]:


weights = np.array([1.0, 4.0, 7.0])
problem = Problem2(weights)
optimizer = Optimizer(problem)

x0 = np.array([0.01, 0.01, 0.01])
x_opt = optimizer.optimize(x0)
print("x1 : %.6f\nx2 : %.6f\nx3 : %.6f\n" % (x_opt[0], x_opt[1], x_opt[2]))


# ### Case #4 - custom2

# In[12]:


weights = np.array([1.0, -2.0, 3.0])
problem = Problem2(weights)
optimizer = Optimizer(problem)

x0 = np.array([0.01, 0.01, 0.01])
x_opt = optimizer.optimize(x0)
print("x1 : %.6f\nx2 : %.6f\nx3 : %.6f\n" % (x_opt[0], x_opt[1], x_opt[2]))

