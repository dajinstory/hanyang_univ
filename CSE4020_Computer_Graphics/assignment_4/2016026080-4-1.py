import glfw
from OpenGL.GL import *
from OpenGL.GLU import *
import numpy as np

# draw a cube of side 1, centered at the origin.
def drawUnitCube():
    glBegin(GL_QUADS)
    glVertex3f( 0.5, 0.5,-0.5)
    glVertex3f(-0.5, 0.5,-0.5)
    glVertex3f(-0.5, 0.5, 0.5)
    glVertex3f( 0.5, 0.5, 0.5) 
                             
    glVertex3f( 0.5,-0.5, 0.5)
    glVertex3f(-0.5,-0.5, 0.5)
    glVertex3f(-0.5,-0.5,-0.5)
    glVertex3f( 0.5,-0.5,-0.5) 
                             
    glVertex3f( 0.5, 0.5, 0.5)
    glVertex3f(-0.5, 0.5, 0.5)
    glVertex3f(-0.5,-0.5, 0.5)
    glVertex3f( 0.5,-0.5, 0.5)
                             
    glVertex3f( 0.5,-0.5,-0.5)
    glVertex3f(-0.5,-0.5,-0.5)
    glVertex3f(-0.5, 0.5,-0.5)
    glVertex3f( 0.5, 0.5,-0.5)
 
    glVertex3f(-0.5, 0.5, 0.5) 
    glVertex3f(-0.5, 0.5,-0.5)
    glVertex3f(-0.5,-0.5,-0.5) 
    glVertex3f(-0.5,-0.5, 0.5) 
                             
    glVertex3f( 0.5, 0.5,-0.5) 
    glVertex3f( 0.5, 0.5, 0.5)
    glVertex3f( 0.5,-0.5, 0.5)
    glVertex3f( 0.5,-0.5,-0.5)
    glEnd()

def drawCubeArray():
    for i in range(5):
        for j in range(5):
            for k in range(5):
                glPushMatrix()
                glTranslatef(i,j,-k-1)
                glScalef(.5,.5,.5)
                drawUnitCube()
                glPopMatrix()

def drawFrame():
    glBegin(GL_LINES)
    glColor3ub(255, 0, 0)
    glVertex3fv(np.array([0.,0.,0.]))
    glVertex3fv(np.array([1.,0.,0.]))
    glColor3ub(0, 255, 0)
    glVertex3fv(np.array([0.,0.,0.]))
    glVertex3fv(np.array([0.,1.,0.]))
    glColor3ub(0, 0, 255)
    glVertex3fv(np.array([0.,0.,0]))
    glVertex3fv(np.array([0.,0.,1.]))
    glEnd()

def myLookAt(eye, at, up):
    zz=eye-at
    zz=zz/np.sqrt(np.sum(zz**2))
    yy=up
    yy=yy/np.sqrt(np.sum(yy**2))
    xx=np.cross(yy,zz)
    xx=xx/np.sqrt(np.sum(xx**2))
    yy=np.cross(zz,xx)
    yy=yy/np.sqrt(np.sum(yy**2))
    pos = np.array([-np.dot(eye, xx), -np.dot(eye, yy), -np.dot(eye, zz)])
    newV=np.array([
        [xx[0],xx[1],xx[2],pos[0]],
        [yy[0],yy[1],yy[2],pos[1]],
        [zz[0],zz[1],zz[2],pos[2]],
        [0.,0.,0.,1.]
        ])
    glMultMatrixf(newV.T)


def myOrtho(left, right, bottom, top, near, far): 
    x=(right-left)/2
    y=(top-bottom)/2
    z=(near-far)/2
    mx=(left+right)/2
    my=(bottom+top)/2
    mz=(near+far)/2
    glScalef(1/x,1/y,1/z)
    glTranslatef(-mx, -my, -mz)
    

def render():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) 
    glEnable(GL_DEPTH_TEST) 
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ) 
    glLoadIdentity()

    myOrtho(-5,5, -5,5, -8,8)
    myLookAt(np.array([5,3,5]), np.array([1,1,-1]), np.array([0,1,0]))
    # Above two lines must behaves exactly same as the below two lines

    #glOrtho(-5,5, -5,5, -8,8) 
    #gluLookAt(5,3,5, 1,1,-1, 0,1,0)
    drawFrame()

    glColor3ub(255, 255, 255) 
    drawCubeArray()


def main():
    if not glfw.init():
        return
    window = glfw.create_window(480, 480,"2016026080-4-1", None,None)
    if not window:
        glfw.terminate()
        return


    glfw.make_context_current(window)
    while not glfw.window_should_close(window):
        glfw.poll_events()
        render()
        glfw.swap_buffers(window)

    glfw.terminate()

if __name__ == "__main__":
    main()
