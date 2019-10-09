import glfw
from OpenGL.GL import *
import numpy as np

K=np.identity(3)

def render(T): 
	glClear(GL_COLOR_BUFFER_BIT) 
	glLoadIdentity() 
	
	# draw cooridnate 
	glBegin(GL_LINES) 
	glColor3ub(255, 0, 0) 
	glVertex2fv(np.array([0., 0.])) 
	glVertex2fv(np.array([1., 0.])) 
	glColor3ub(0, 255, 0) 
	glVertex2fv(np.array([0., 0.])) 
	glVertex2fv(np.array([0., 1.])) 
	glEnd() 

	# draw triangle 
	glBegin(GL_TRIANGLES) 
	glColor3ub(255, 255, 255) 
	glVertex2fv((T@np.array([.0, .5, 1.]))[:-1]) 
	glVertex2fv((T@np.array([.0, .0, 1.]))[:-1]) 
	glVertex2fv((T@np.array([.5, .0 , 1.]))[:-1]) 
	glEnd()

def key_callback(window, key, scancode, action, mods):
	global K
	degree=10.0*np.pi/180
	if key==glfw.KEY_Q:
		if action==glfw.PRESS:
			K[0][2]-=0.1
	elif key==glfw.KEY_E:
		if action==glfw.PRESS:
			K[0][2]+=0.1
	elif key==glfw.KEY_A:
		if action==glfw.PRESS:
			K=K@np.array([
				[np.cos(degree), -np.sin(degree), 0.],
				[np.sin(degree), np.cos(degree), 0.],
				[0.,0.,1.]
				])
	elif key==glfw.KEY_D:
		if action==glfw.PRESS:
			K=K@np.array([
				[np.cos(-degree), -np.sin(-degree), 0.],
				[np.sin(-degree), np.cos(-degree), 0.],
				[0.,0.,1.]
				])
	elif key==glfw.KEY_1:
		if action==glfw.PRESS:
			K=np.identity(3)
	elif key==glfw.KEY_W:
		if action==glfw.PRESS:
			K=np.array([
				[0.9, 0., 0.],
				[0., 1., 0.],
				[0., 0., 1.]
				])@K
	elif key==glfw.KEY_S:
		if action==glfw.PRESS:
			K=np.array([
				[np.cos(degree), -np.sin(degree), 0.],
				[np.sin(degree), np.cos(degree), 0.],
				[0.,0.,1.]
				])@K
def main():
	# Initialize the library
	if not glfw.init():
		return

	# Create a windowed mode window and its OpenGL context
	window = glfw.create_window(480, 480, "2016026080-assignment3-1", None, None)
	if not window:
		glfw.terminate()
		return

	# Set the key callback
	glfw.set_key_callback(window, key_callback)

	# Make the window's context current
	glfw.make_context_current(window)

	glfw.swap_interval(1)
	# Loop until the user closes the window
	while not glfw.window_should_close(window):
		# Poll events
		global K
		glfw.poll_events()
		
		render(K)
		# Swap front and back buffers
		glfw.swap_buffers(window)

	glfw.terminate()



if __name__ == "__main__":
	main()

