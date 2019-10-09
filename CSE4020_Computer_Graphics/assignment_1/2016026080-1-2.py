import numpy as np, OpenGL, glfw

#problem_A
M=np.arange(2,27,1)
print(M)

#problem_B
M=M.reshape(5,5)
print(M)

#problem_C
M[:,0]=0
print(M)

#problem_D
M=M@M
print(M)

#problem_E
v=M[0]
print(np.sqrt(v@v))
