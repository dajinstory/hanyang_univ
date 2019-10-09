import glfw
from OpenGL.GL import *
import numpy as np

keys=[GL_POLYGON, GL_POINTS, GL_LINES,GL_LINE_STRIP,GL_LINE_LOOP,GL_TRIANGLES,GL_TRIANGLE_STRIP,GL_TRIANGLE_FAN,GL_QUADS,GL_QUAD_STRIP]
key_idx=4

def render():

	# enable depth test (we'll see details later)
	glClear(GL_COLOR_BUFFER_BIT)
	glLoadIdentity()
	glBegin(keys[key_idx])

	glColor3ub(255,255,255)
	for i in range(12):
		angle = np.pi*i/6
		glVertex2f(np.cos(angle), np.sin(angle))

	glEnd()


def key_callback(window, key, scancode, action, mods):
	global key_idx
	if key==glfw.KEY_0:
		if action==glfw.PRESS:
			key_idx=0
	elif key==glfw.KEY_1:
		if action==glfw.PRESS:
			key_idx=1
	elif key==glfw.KEY_2:
		if action==glfw.PRESS:
			key_idx=2
	elif key==glfw.KEY_3:
		if action==glfw.PRESS:
			key_idx=3
	elif key==glfw.KEY_4:
		if action==glfw.PRESS:
			key_idx=4
	elif key==glfw.KEY_5:
		if action==glfw.PRESS:
			key_idx=5
	elif key==glfw.KEY_6:
		if action==glfw.PRESS:
			key_idx=6
	elif key==glfw.KEY_7:
		if action==glfw.PRESS:
			key_idx=7
	elif key==glfw.KEY_8:
		if action==glfw.PRESS:
			key_idx=8
	elif key==glfw.KEY_9:
		if action==glfw.PRESS:
			key_idx=9




def main():
	# Initialize the library
	if not glfw.init():
		return

	# Create a windowed mode window and its OpenGL context
	window = glfw.create_window(480, 480, "2016026080-assignment2-1", None, None)
	if not window:
		glfw.terminate()
		return

	glfw.set_key_callback(window, key_callback)
	glfw.make_context_current(window)


	# Loop until the user closes the window
	while not glfw.window_should_close(window):
		# Poll events
		glfw.poll_events()
		render()
		# Swap front and back buffers
		glfw.swap_buffers(window)

	glfw.terminate()



if __name__ == "__main__":
	main()

