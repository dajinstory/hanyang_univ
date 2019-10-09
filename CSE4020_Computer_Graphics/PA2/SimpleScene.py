import glfw
import sys
import pdb
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.arrays import ArrayDatatype
import time
import math, sys
import numpy as np
import ctypes
from PIL.Image import open
import OBJ
from Ray import *


# global variables
clickCount =0
wld2cam=[]
cam2wld=[]
cow2wld=None

#dajin
fixedPoints=[]
numOfPoints=-1
showAnimation=0
animStartTime=.0
catMullRomCourse=[[] for i in range(6)]
firstCow2wld=None
cycle=0
pointIdx=0
NUM_OF_FRAME = 44100
yaw_backup =None
clicked_x=0
clicked_y=0
stop_time=0
endAnimation=0
matrixHermite=np.array([
    [2., -2., 1., 1.],
    [-3., 3., -2., -1.],
    [0., 0., 1., 0.],
    [1., 0., 0., 0.]
    ])
matrixCatmullRom = np.array([
    [0., 1., 0., 0.],
    [0., 0., 1., 0.],
    [-0.5, 0., 0.5, 0.],
    [0., -0.5, 0., 0.5]
    ])

cursorOnCowBoundingBox=False
pickInfo=None
floorTexID=0
cameras= [
	[28,18,28, 0,2,0, 0,1,0],   
	[28,18,-28, 0,2,0, 0,1,0], 
	[-28,18,28, 0,2,0, 0,1,0], 
	[-12,12,0, 0,2,0, 0,1,0],  
	[0,100,0,  0,0,0, 1,0,0]
]
camModel=None
cowModel=None
H_DRAG=1
V_DRAG=2
# dragging state
isDrag=0

class PickInfo:
    def __init__(self, cursorRayT, cowPickPosition, cowPickConfiguration, cowPickPositionLocal):
        self.cursorRayT=cursorRayT
        self.cowPickPosition=cowPickPosition.copy()
        self.cowPickConfiguration=cowPickConfiguration.copy()
        self.cowPickPositionLocal=cowPickPositionLocal.copy()

def vector3(x,y,z):
    return np.array((x,y,z))
def position3(v):
    # divide by w
    w=v[3]
    return vector3(v[0]/w, v[1]/w, v[2]/w)

def vector4(x,y,z):
    return np.array((x,y,z,1))

def rotate(m,v):
    return m[0:3, 0:3]@v
def transform(m, v):
    return position3(m@np.append(v,1))

def getTranslation(m):
    return m[0:3,3]
def setTranslation(m,v):
    m[0:3,3]=v

def makePlane( a,  b,  n):
    v=a.copy()
    for i in range(3):
        if n[i]==1.0:
            v[i]=b[i];
        elif n[i]==-1.0:
            v[i]=a[i];
        else:
            assert(n[i]==0.0);
            
    return Plane(rotate(cow2wld,n),transform(cow2wld,v));

def onKeyPress( window, key, scancode, action, mods):
    global cameraIndex
    if action==glfw.RELEASE:
        return ; # do nothing
    # If 'c' or space bar are pressed, alter the camera.
    # If a number is pressed, alter the camera corresponding the number.
    if key==glfw.KEY_C or key==glfw.KEY_SPACE:
        print( "Toggle camera %s\n"% cameraIndex );
        cameraIndex += 1;

    if cameraIndex >= len(wld2cam):
        cameraIndex = 0;

def drawOtherCamera():
    global cameraIndex,wld2cam, camModel
    for i in range(len(wld2cam)):
        if (i != cameraIndex):
            glPushMatrix();												# Push the current matrix on GL to stack. The matrix is wld2cam[cameraIndex].matrix().
            glMultMatrixd(cam2wld[i].T)
            drawFrame(5);											# Draw x, y, and z axis.
            frontColor = [0.2, 0.2, 0.2, 1.0];
            glEnable(GL_LIGHTING);									
            glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);			# Set ambient property frontColor.
            glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);			# Set diffuse property frontColor.
            glScaled(0.5,0.5,0.5);										# Reduce camera size by 1/2.
            glTranslated(1.1,1.1,0.0);									# Translate it (1.1, 1.1, 0.0).
            camModel.render()
            glPopMatrix();												# Call the matrix on stack. wld2cam[cameraIndex].matrix() in here.

def drawFrame(leng):
    glDisable(GL_LIGHTING);	# Lighting is not needed for drawing axis.
    glBegin(GL_LINES);		# Start drawing lines.
    glColor3d(1,0,0);		# color of x-axis is red.
    glVertex3d(0,0,0);			
    glVertex3d(leng,0,0);	# Draw line(x-axis) from (0,0,0) to (len, 0, 0). 
    glColor3d(0,1,0);		# color of y-axis is green.
    glVertex3d(0,0,0);			
    glVertex3d(0,leng,0);	# Draw line(y-axis) from (0,0,0) to (0, len, 0).
    glColor3d(0,0,1);		# color of z-axis is  blue.
    glVertex3d(0,0,0);
    glVertex3d(0,0,leng);	# Draw line(z-axis) from (0,0,0) - (0, 0, len).
    glEnd();			# End drawing lines.

#*********************************************************************************
# Draw 'cow' object.
#*********************************************************************************/
def drawCow(_cow2wld, drawBB):

    glPushMatrix();		# Push the current matrix of GL into stack. This is because the matrix of GL will be change while drawing cow.

    # The information about location of cow to be drawn is stored in cow2wld matrix.
    # (Project2 hint) If you change the value of the cow2wld matrix or the current matrix, cow would rotate or move.
    glMultMatrixd(_cow2wld.T)

    drawFrame(5);										# Draw x, y, and z axis.
    frontColor = [0.8, 0.2, 0.9, 1.0];
    glEnable(GL_LIGHTING);
    glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);		# Set ambient property frontColor.
    glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);		# Set diffuse property frontColor.
    cowModel.render()	# Draw cow. 
    glDisable(GL_LIGHTING);
    if drawBB:
        glBegin(GL_LINES);
        glColor3d(1,1,1);
        cow=cowModel
        glVertex3d( cow.bbmin[0], cow.bbmin[1], cow.bbmin[2]);
        glVertex3d( cow.bbmax[0], cow.bbmin[1], cow.bbmin[2]);
        glVertex3d( cow.bbmin[0], cow.bbmax[1], cow.bbmin[2]);
        glVertex3d( cow.bbmax[0], cow.bbmax[1], cow.bbmin[2]);
        glVertex3d( cow.bbmin[0], cow.bbmin[1], cow.bbmax[2]);
        glVertex3d( cow.bbmax[0], cow.bbmin[1], cow.bbmax[2]);
        glVertex3d( cow.bbmin[0], cow.bbmax[1], cow.bbmax[2]);
        glVertex3d( cow.bbmax[0], cow.bbmax[1], cow.bbmax[2]);

        glColor3d(1,1,1);
        glVertex3d( cow.bbmin[0], cow.bbmin[1], cow.bbmin[2]);
        glVertex3d( cow.bbmin[0], cow.bbmax[1], cow.bbmin[2]);
        glVertex3d( cow.bbmax[0], cow.bbmin[1], cow.bbmin[2]);
        glVertex3d( cow.bbmax[0], cow.bbmax[1], cow.bbmin[2]);
        glVertex3d( cow.bbmin[0], cow.bbmin[1], cow.bbmax[2]);
        glVertex3d( cow.bbmin[0], cow.bbmax[1], cow.bbmax[2]);
        glVertex3d( cow.bbmax[0], cow.bbmin[1], cow.bbmax[2]);
        glVertex3d( cow.bbmax[0], cow.bbmax[1], cow.bbmax[2]);

        glColor3d(1,1,1);
        glVertex3d( cow.bbmin[0], cow.bbmin[1], cow.bbmin[2]);
        glVertex3d( cow.bbmin[0], cow.bbmin[1], cow.bbmax[2]);
        glVertex3d( cow.bbmax[0], cow.bbmin[1], cow.bbmin[2]);
        glVertex3d( cow.bbmax[0], cow.bbmin[1], cow.bbmax[2]);
        glVertex3d( cow.bbmin[0], cow.bbmax[1], cow.bbmin[2]);
        glVertex3d( cow.bbmin[0], cow.bbmax[1], cow.bbmax[2]);
        glVertex3d( cow.bbmax[0], cow.bbmax[1], cow.bbmin[2]);
        glVertex3d( cow.bbmax[0], cow.bbmax[1], cow.bbmax[2]);


        glColor3d(1,1,1);
        glVertex3d( cow.bbmin[0], cow.bbmin[1], cow.bbmin[2]);
        glVertex3d( cow.bbmin[0], cow.bbmax[1], cow.bbmin[2]);
        glVertex3d( cow.bbmax[0], cow.bbmin[1], cow.bbmin[2]);
        glVertex3d( cow.bbmax[0], cow.bbmax[1], cow.bbmin[2]);
        glVertex3d( cow.bbmin[0], cow.bbmin[1], cow.bbmax[2]);
        glVertex3d( cow.bbmin[0], cow.bbmax[1], cow.bbmax[2]);
        glVertex3d( cow.bbmax[0], cow.bbmin[1], cow.bbmax[2]);
        glVertex3d( cow.bbmax[0], cow.bbmax[1], cow.bbmax[2]);

        glColor3d(1,1,1);
        glVertex3d( cow.bbmin[0], cow.bbmin[1], cow.bbmin[2]);
        glVertex3d( cow.bbmin[0], cow.bbmin[1], cow.bbmax[2]);
        glVertex3d( cow.bbmax[0], cow.bbmin[1], cow.bbmin[2]);
        glVertex3d( cow.bbmax[0], cow.bbmin[1], cow.bbmax[2]);
        glVertex3d( cow.bbmin[0], cow.bbmax[1], cow.bbmin[2]);
        glVertex3d( cow.bbmin[0], cow.bbmax[1], cow.bbmax[2]);
        glVertex3d( cow.bbmax[0], cow.bbmax[1], cow.bbmin[2]);
        glVertex3d( cow.bbmax[0], cow.bbmax[1], cow.bbmax[2]);
        glEnd();
    glPopMatrix();			# Pop the matrix in stack to GL. Change it the matrix before drawing cow.
def drawFloor():

    glDisable(GL_LIGHTING);

    # Set color of the floor.
    # Assign checker-patterned texture.
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, floorTexID );

    # Draw the floor. Match the texture's coordinates and the floor's coordinates resp. 
    nrep=4
    glBegin(GL_POLYGON);
    glTexCoord2d(0,0);
    glVertex3d(-12,-0.1,-12);		# Texture's (0,0) is bound to (-12,-0.1,-12).
    glTexCoord2d(nrep,0);
    glVertex3d( 12,-0.1,-12);		# Texture's (1,0) is bound to (12,-0.1,-12).
    glTexCoord2d(nrep,nrep);
    glVertex3d( 12,-0.1, 12);		# Texture's (1,1) is bound to (12,-0.1,12).
    glTexCoord2d(0,nrep);
    glVertex3d(-12,-0.1, 12);		# Texture's (0,1) is bound to (-12,-0.1,12).
    glEnd();

    glDisable(GL_TEXTURE_2D);	
    drawFrame(5);				# Draw x, y, and z axis.

def CatMullRomSpline(P, idx, nPoints=NUM_OF_FRAME):
    #for other catmull method
    '''
    P0=P[(idx+6-1)%6]
    P1=P[(idx+6+0)%6]
    P2=P[(idx+6+1)%6]
    P3=P[(idx+6+2)%6]

    alpha = 0.5
    def tj(ti, Pi, Pj):
        xi, yi, zi= Pi
        xj, yj, zj = Pj
        return ( ( (xj-xi)**2 + (yj-yi)**2 + (zj-zi)**2)**0.5 )**alpha + ti

    t0=0
    t1 = tj(t0, P0, P1)
    t2 = tj(t1, P1, P2)
    t3 = tj(t2, P2, P3)

    t = np.linspace(t1,t2,nPoints)

    # Reshape so that we can multiply by the points P0 to P3
    # and get a point for each value of t.
    t = t.reshape(len(t),1)

    A1 = (t1-t)/(t1-t0)*P0 + (t-t0)/(t1-t0)*P1
    A2 = (t2-t)/(t2-t1)*P1 + (t-t1)/(t2-t1)*P2
    A3 = (t3-t)/(t3-t2)*P2 + (t-t2)/(t3-t2)*P3

    B1 = (t2-t)/(t2-t0)*A1 + (t-t0)/(t2-t0)*A2
    B2 = (t3-t)/(t3-t1)*A2 + (t-t1)/(t3-t1)*A3

    C  = (t2-t)/(t2-t1)*B1 + (t-t1)/(t2-t1)*B2
    return C
    '''
    global matrixHermite, matrixCatmullRom
    P0=P[(idx+6-1)%6]
    P1=P[(idx+6+0)%6]
    P2=P[(idx+6+1)%6]
    P3=P[(idx+6+2)%6]

    return matrixHermite@matrixCatmullRom@np.array((P0, P1, P2, P3))


def rotateHRTF(current, nextPoint):
    global yaw_backup
    hrtf = normalize(nextPoint - current)

    if math.fabs(hrtf[0]) <= sys.float_info.epsilon and math.fabs(hrtf[2])<=sys.float_info.epsilon:
        yaw=yaw_backup
        if yaw_backup==None:
            yaw=0
    else:
        yaw=math.atan2(-hrtf[2], hrtf[0])
        yaw_backup=yaw

    pitch=math.asin(hrtf[1])
    Rx = np.array([[1., 0., 0.],
                   [0., np.cos(pitch), -np.sin(pitch)],
                   [0., np.sin(pitch), np.cos(pitch)]])

    Ry = np.array([[np.cos(yaw), 0., np.sin(yaw)],
                   [0., 1., 0.],
                   [-np.sin(yaw), 0., np.cos(yaw)]])

    Rz = np.array([[np.cos(pitch), -np.sin(pitch), 0.],
                   [np.sin(pitch), np.cos(pitch), 0.],
                   [0., 0., 1.]])
    return Ry@Rz
    

def setCatMullPoint():
    global catMullRomCourse, fixedPoints
    for i in range(6):
        catMullRomCourse[i] = CatMullRomSpline(fixedPoints, i)

def getCatMullPoint():
    global endAnimation, animStartTime, NUM_OF_FRAME, cycle, pointIdx, showAnimation, catMullRomCourse
    animTime=glfw.get_time()-animStartTime
    if animTime>1.0:
        pointIdx=pointIdx+1
        if pointIdx == 6:
            pointIdx=0
            cycle=cycle+1
            if cycle==3:
                showAnimation=0
                endAnimation=1
        animStartTime=glfw.get_time()
        animTime=.0
    tspots = np.array((animTime**3, animTime**2, animTime, 1))
    return tspots@catMullRomCourse[pointIdx]


def display():
    global endAnimation, cameraIndex, cow2wld, firstCow2wld, showAnimation, fixedPoints
    glClearColor(0.8, 0.9, 0.9, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				# Clear the screen
    # set viewing transformation.
    glLoadMatrixd(wld2cam[cameraIndex].T)

    drawOtherCamera();													# Locate the camera's position, and draw all of them.
    drawFloor();													# Draw floor.
    cow2wldLOC=cow2wld.copy()

    # TODO: 
    # update cow2wld here to animate the cow.
    if endAnimation:
        return

    if showAnimation and stop_time==0:
        #print(getTranslation(cow2wld), catMullRomCourse[pointIdx][idxNum])
        #print("=================")
        nextCatMullPoint=getCatMullPoint()

        T=np.eye(4)
        #set rotation first and then add translation
        rotationFunc = rotateHRTF(getTranslation(cow2wldLOC), nextCatMullPoint)
        cow2wldLOC[0:3, 0:3] = rotationFunc
        setTranslation(T, nextCatMullPoint-getTranslation(cow2wldLOC))
        #move cow2wld to next
        #additional translation
        cow2wld=T@cow2wldLOC
    #you need to modify both the translation and rotation parts of the cow2wld matrix.
    elif stop_time==0:
        for i in range(len(fixedPoints)):
            tmpCow=firstCow2wld.copy()
            setTranslation(tmpCow, fixedPoints[i])
            drawCow(tmpCow, 1)
    
    drawCow(cow2wld, cursorOnCowBoundingBox);														# Draw cow.

    glFlush();

def reshape(window, w, h):
    width = w;
    height = h;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);            # Select The Projection Matrix
    glLoadIdentity();                       # Reset The Projection Matrix
    # Define perspective projection frustum
    aspect = width/(float)(height);
    gluPerspective(45, aspect, 1, 1024);
    matProjection=glGetDoublev(GL_PROJECTION_MATRIX).T
    glMatrixMode(GL_MODELVIEW);             # Select The Modelview Matrix
    glLoadIdentity();                       # Reset The Projection Matrix

def initialize(window):
    global cursorOnCowBoundingBox, floorTexID, cameraIndex, camModel, cow2wld, cowModel
    cursorOnCowBoundingBox=False;
    # Set up OpenGL state
    glShadeModel(GL_SMOOTH);         # Set Smooth Shading
    glEnable(GL_DEPTH_TEST);         # Enables Depth Testing
    glDepthFunc(GL_LEQUAL);          # The Type Of Depth Test To Do
    # Use perspective correct interpolation if available
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    # Initialize the matrix stacks
    width, height = glfw.get_window_size(window)
    reshape(window, width, height);
    # Define lighting for the scene
    lightDirection   = [1.0, 1.0, 1.0, 0];
    ambientIntensity = [0.1, 0.1, 0.1, 1.0];
    lightIntensity   = [0.9, 0.9, 0.9, 1.0];
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientIntensity);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);
    glEnable(GL_LIGHT0);

    # initialize floor
    im = open('bricks.bmp')
    try:
        ix, iy, image = im.size[0], im.size[1], im.tobytes("raw", "RGB", 0, -1)
    except SystemError:
        ix, iy, image = im.size[0], im.size[1], im.tobytes("raw", "RGBX", 0, -1)

    # Make texture which is accessible through floorTexID. 
    floorTexID=glGenTextures( 1)
    glBindTexture(GL_TEXTURE_2D, floorTexID);		
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, ix, ix, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    # initialize cow
    cowModel=OBJ.OBJrenderer("cow.obj")

    # initialize cow2wld matrix
    glPushMatrix();		        # Push the current matrix of GL into stack.
    glLoadIdentity();		        # Set the GL matrix Identity matrix.
    glTranslated(0,-cowModel.bbmin[1],-8);	# Set the location of cow.
    glRotated(-90, 0, 1, 0);		# Set the direction of cow. These information are stored in the matrix of GL.
    cow2wld=glGetDoublev(GL_MODELVIEW_MATRIX).T # convert column-major to row-major 
    glPopMatrix();			# Pop the matrix on stack to GL.


    # intialize camera model.
    camModel=OBJ.OBJrenderer("camera.obj")


    # initialize camera frame transforms.

    cameraCount=len(cameras)
    for i in range(cameraCount):
        # 'c' points the coordinate of i-th camera.
        c = cameras[i];										
        glPushMatrix();													# Push the current matrix of GL into stack.
        glLoadIdentity();												# Set the GL matrix Identity matrix.
        gluLookAt(c[0],c[1],c[2], c[3],c[4],c[5], c[6],c[7],c[8]);		# Setting the coordinate of camera.
        wld2cam.append(glGetDoublev(GL_MODELVIEW_MATRIX).T)
        glPopMatrix();													# Transfer the matrix that was pushed the stack to GL.
        cam2wld.append(np.linalg.inv(wld2cam[i]))
    cameraIndex = 0;

def onMouseButton(window,button, state, mods):
    global endAnimation, stop_time, stop_spot, clicked_x, clicked_y, isDrag, V_DRAG, H_DRAG, fixedPoints, numOfPoints, showAnimation, animStartTime, cow2wld, firstCow2wld
    GLFW_DOWN=1;
    GLFW_UP=0;
    x, y=glfw.get_cursor_pos(window)
    
    cow2wldLOC=cow2wld.copy()

    if endAnimation:
        return

    if button == glfw.MOUSE_BUTTON_LEFT:
        if state == GLFW_DOWN:

            #dajin
            if showAnimation:
                stop_time=1
                return
            clicked_x=x
            clicked_y=y
            isDrag = V_DRAG;

            ''' original
            if isDrag==H_DRAG:
                isDrag=0
            else:
                isDrag=V_DRAG;
            '''
            print( "Left mouse click at (%d %d)" % (x,y))
            # start vertical dragging
        elif state == GLFW_UP and isDrag!=0:
            #dajin
            if showAnimation:
                stop_time=1
                return
            isDrag = H_DRAG;

            # add fixed cowPickPosition
            if numOfPoints>=0:
                fixedPoints.append(getTranslation(cow2wldLOC))
            if numOfPoints == 0:
                firstCow2wld=cow2wldLOC
            numOfPoints = numOfPoints + 1
            print("push %d" %(numOfPoints-1))
            print("inputState:%d" %(numOfPoints))
            if len(fixedPoints) == 6:
                cow2wld=firstCow2wld
                setCatMullPoint()
                showAnimation=1
                animStartTime=glfw.get_time()
                #for i in range(6):
                #    print(fixedPoints[i])
            ''' original
            isDrag=H_DRAG;
            '''
            #print( "Left mouse up\n");
            # start horizontal dragging using mouse-move events.
    elif button == glfw.MOUSE_BUTTON_RIGHT:
        if state == GLFW_DOWN:
            print( "Right mouse click at (%d, %d)\n"%(x,y) );

def onMouseDrag(window, x, y):
    global endAnimation, clicked_x, clicked_y, isDrag,cursorOnCowBoundingBox, pickInfo, cow2wld, showAnimation, fixedPoints
    x, y=glfw.get_cursor_pos(window)

    cow2wldLOC=cow2wld.copy()
    if endAnimation:
        return

    if isDrag: 
        if showAnimation:
            return
        print( "in %d drag (%d %d)"% (isDrag, x-clicked_x, y-clicked_y));
        if  isDrag==V_DRAG:
            # isDrag = 2. vertical to the plane
            # vertical dragging
            # TODO:
            # create a dragging plane perpendicular to the ray direction, 
            # and test intersection with the screen ray.


            if cursorOnCowBoundingBox:
                ray=screenCoordToRay(window, x, y);
                pp=pickInfo;
                if x-clicked_x<=0:
                    p=Plane(np.array((1,0,0)), getTranslation(cow2wldLOC));
                    c=ray.intersectsPlane(p);
                    currentPos=ray.getPoint(c[1])
                    currentPos[2] = getTranslation(cow2wldLOC)[2]
                else:
                    p=Plane(np.array((0,0,1)), getTranslation(cow2wldLOC));
                    c=ray.intersectsPlane(p);
                    currentPos=ray.getPoint(c[1])
                    currentPos[0] = getTranslation(cow2wldLOC)[0]
                #print(pp.cowPickPosition, currentPos)
                #print(pp.cowPickConfiguration, cow2wld)

                
                T=np.eye(4)
                setTranslation(T, currentPos-getTranslation(cow2wldLOC))
                cow2wld=T@cow2wldLOC
                #print('Vdrag')

        else:
            # horizontal dragging
            # Hint: read carefully the following block to implement vertical dragging.
            if cursorOnCowBoundingBox:
                ray=screenCoordToRay(window, x, y);
                pp=pickInfo;
                p=Plane(np.array((0,1,0)), getTranslation(cow2wldLOC))#pp.cowPickPosition);
                c=ray.intersectsPlane(p);

                currentPos=ray.getPoint(c[1])
                #cow2wld = LOCAL
                #currentPos = 현재 마우스의 local위치
                #cow2wld = 바로 이전프레임에서의 마우스의 local 위치.
                #print(getTranslation(cow2wld))
                #print(pp.cowPickPosition, currentPos)
                #print(pp.cowPickConfiguration, cow2wld)
                #print("NEWNEWNEWNEWNEWNEWNEWNENWEWNENWENEWENWENEWNWE")
                #for i in range(len(fixedPoints)):
                #    print(fixedPoints[i])
                #    print("------------------")

                T=np.eye(4)
                setTranslation(T, currentPos-getTranslation(cow2wldLOC))#pp.cowPickPosition)
                cow2wld=T@cow2wldLOC#pp.cowPickConfiguration;
                #print('Hdrag')
    else:
        ray=screenCoordToRay(window, x, y)

        planes=[];
        cow=cowModel
        bbmin=cow.bbmin
        bbmax=cow.bbmax

        planes.append(makePlane(bbmin, bbmax, vector3(0,1,0)));
        planes.append(makePlane(bbmin, bbmax, vector3(0,-1,0)));
        planes.append(makePlane(bbmin, bbmax, vector3(1,0,0)));
        planes.append(makePlane(bbmin, bbmax, vector3(-1,0,0)));
        planes.append(makePlane(bbmin, bbmax, vector3(0,0,1)));
        planes.append(makePlane(bbmin, bbmax, vector3(0,0,-1)));


        o=ray.intersectsPlanes(planes);
        cursorOnCowBoundingBox=o[0]
        cowPickPosition=ray.getPoint(o[1])
        cowPickLocalPos=transform(np.linalg.inv(cow2wldLOC),cowPickPosition)
        pickInfo=PickInfo( o[1], cowPickPosition, cow2wldLOC, cowPickLocalPos)

def screenCoordToRay(window, x, y):
    width, height = glfw.get_window_size(window)

    matProjection=glGetDoublev(GL_PROJECTION_MATRIX).T
    matProjection=matProjection@wld2cam[cameraIndex]; # use @ for matrix mult.
    invMatProjection=np.linalg.inv(matProjection);
    # -1<=v.x<1 when 0<=x<width
    # -1<=v.y<1 when 0<=y<height
    vecAfterProjection =vector4(
            (float(x - 0))/(float(width))*2.0-1.0,
            -1*(((float(y - 0))/float(height))*2.0-1.0),
            -10)

    #std::cout<<"cowPosition in clip coordinate (NDC)"<<matProjection*cow2wld.getTranslation()<<std::endl;
	
    vecBeforeProjection=position3(invMatProjection@vecAfterProjection);

    rayOrigin=getTranslation(cam2wld[cameraIndex])
    return Ray(rayOrigin, normalize(vecBeforeProjection-rayOrigin))

def main():
    if not glfw.init():
        print ('GLFW initialization failed')
        sys.exit(-1)
    width = 800;
    height = 600;
    window = glfw.create_window(width, height, 'modern opengl example', None, None)
    if not window:
        glfw.terminate()
        sys.exit(-1)

    glfw.make_context_current(window)
    glfw.set_key_callback(window, onKeyPress)
    glfw.set_mouse_button_callback(window, onMouseButton)
    glfw.set_cursor_pos_callback(window, onMouseDrag)
    glfw.swap_interval(1)

    initialize(window);						
    while not glfw.window_should_close(window):
        glfw.poll_events()
        display()

        glfw.swap_buffers(window)

    glfw.terminate()
if __name__ == "__main__":
    main()
