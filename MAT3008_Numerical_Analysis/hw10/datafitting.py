#!/usr/bin/env python
# coding: utf-8

# In[374]:


import sys
import cv2
import numpy as np
import numpy.linalg as lin
import math
from PIL import Image
import matplotlib
from matplotlib import pyplot as plt


# In[375]:


distThresh=30
divisor=1.0


# ## Matched KeyPoint

# In[376]:


def MatchedKeyPoint(img1, img2):

    # Initiate ORB detector
    orb = cv2.ORB_create()
    
    kp1, des1 = orb.detectAndCompute(img1,None)
    kp2, des2 = orb.detectAndCompute(img2,None) 

    ## bf matchi g with norm hamming
    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
    matches = bf.match(des1, des2)
    matches = sorted(matches, key=lambda x:x.distance)
    
    ## filter some matches with far difference
    for item in matches:
        if item.distance>distThresh:
            matches.remove(item)
    
    ## return pairs
    Left=[]
    Right=[]
    for idx in range(len(matches)):
        Left.append(kp1[matches[idx].queryIdx].pt)
        Right.append(kp2[matches[idx].trainIdx].pt)

    return Left, Right


# ## Hessian Function

# In[377]:


def make_Hessians(a1,b1,c1, a2,b2,c2, p,q, Left,Right):
    H_x=np.array([[0.0 for _ in range(5)] for __ in range(5)])
    H_y=np.array([[0.0 for _ in range(5)] for __ in range(5)])
    for i in range(5):
        for j in range(5):
            H_x[i][j]=sums(i,j,a1,b1,c1,p,q,Left,Right)
            H_y[i][j]=sums(i,j,a2,b2,c2,p,q,Left,Right)
    return H_x, H_y


# In[378]:


def g(a,b,c,x,y):
    return a*x+b*y+c
def f(p,q,x,y):
    return p*x+q*y+1

def _p(a,b,c,p,q,x,y, use_value=None, rf=0,rg=0):
    global f,g
    if not use_value:
        rf=f(p,q,x,y)
        rg=g(a,b,c,x,y)
    return -x*rf/rg/rg
            
def _q(a,b,c,p,q,x,y, use_value=None, rf=0,rg=0):
    global f,g
    if not use_value:
        rf=f(p,q,x,y)
        rg=g(a,b,c,x,y)    
    return -y*rf/rg/rg

def _a(a,b,c,p,q,x,y, use_value=None, rf=0,rg=0):
    global f,g
    if not use_value:
        rf=f(p,q,x,y)
        rg=g(a,b,c,x,y)
    return x/rg

def _b(a,b,c,p,q,x,y, use_value=None, rf=0,rg=0):
    global f,g
    if not use_value:
        rf=f(p,q,x,y)
        rg=g(a,b,c,x,y)
    return y/rg

def _c(a,b,c,p,q,x,y, use_value=None, rf=0,rg=0):
    global f,g
    if not use_value:
        rf=f(p,q,x,y)
        rg=g(a,b,c,x,y)    
    return 1/rg

funcs=[_a,_b,_c,_p,_q]

def sums(i,j,a,b,c,p,q,Left,Right):
    global funcs, divisor
    re = 0.0  
    for idx in range(len(Left)):
        re+=funcs[i](a,b,c,p,q,Left[idx][0],Left[idx][1])*funcs[j](a,b,c,p,q,Left[idx][0],Left[idx][1])
    return re*2/divisor/divisor


# In[379]:


def get_gradients(a1,b1,c1,a2,b2,c2, p,q, Left,Right):
    global funcs, divisor,f,g
    gradient_x=[0.0 for _ in range(5)]
    gradient_y=[0.0 for _ in range(5)]
    for idx in range(len(Left)):
        x=Left[idx][0]
        y=Left[idx][1]
        nx=Right[idx][0]
        ny=Right[idx][1]
        rf=f(p,q,x,y)
        rg1=g(a1,b1,c1,x,y)
        rg2=g(a2,b2,c2,x,y)
        dx = nx - rg1/rf
        dy = ny - rg2/rf
        for gidx in range(5):
            gradient_x[gidx]+=dx*funcs[gidx](a1,b1,c1,p,q,x,y,"USE",rf,rg1)
            gradient_y[gidx]+=dy*funcs[gidx](a2,b2,c2,p,q,x,y,"USE",rf,rg2)
    
    for gidx in range(5):
        gradient_x[gidx]*=-2
        gradient_x[gidx]/=divisor
        gradient_x[gidx]/=divisor
        gradient_y[gidx]*=-2
        gradient_y[gidx]/=divisor
        gradient_y[gidx]/=divisor
            
    return np.array(gradient_x), np.array(gradient_y)

def get_errors(a1,b1,c1,a2,b2,c2, p,q, Left,Right):
    global divisor,f,g
    error_x=0.0
    error_y=0.0
    
    for idx in range(len(Left)):
        x=Left[idx][0]
        y=Left[idx][1]
        nx=Right[idx][0]
        ny=Right[idx][1]
        rf=f(p,q,x,y)
        rg1=g(a1,b1,c1,x,y)
        rg2=g(a2,b2,c2,x,y)
        dx = nx - rg1/rf
        dy = ny - rg2/rf
        error_x+=dx*dx
        error_y+=dy*dy

    error_x/=divisor
    error_x/=divisor
    #error_x/=len(Left)
    error_y/=divisor
    error_y/=divisor
    #error_y/=len(Left)
    return error_x, error_y


# In[380]:


img1 = cv2.imread('./data/goodLeft.jpg',0)
img2 = cv2.imread('./data/goodRight.jpg',0)
Left, Right = MatchedKeyPoint(img1, img2)


# #### Initial State

# In[381]:


x_trans=0.0
y_trans=0.0
for idx in range(len(Left)):
    x_trans+=Right[idx][0]-Left[idx][0]
    y_trans+=Right[idx][1]-Left[idx][1]
x_trans/=len(Left)
y_trans/=len(Left)
  
print(x_trans, y_trans)
    
a1=1.0
b1=0.0
c1=x_trans
a2=0.0
b2=1.0
c2=y_trans
p=0.0
q=0.0


error_x, error_y = get_errors(a1,b1,c1,a2,b2,c2,p,q,Left,Right)
print(error_x/len(Left))
print(error_y/len(Left))
print(error_x+error_y)


# #### Epoch

# In[382]:


epoch=30
Nlambda=10e+3
Mulambda=10

while epoch>0:
    H_x,H_y = make_Hessians(a1,b1,c1,a2,b2,c2,p,q,Left,Right)
    gradient_x, gradient_y = get_gradients(a1,b1,c1,a2,b2,c2, p,q, Left,Right)
    error_x, error_y = get_errors(a1,b1,c1,a2,b2,c2,p,q,Left,Right)
    flag=0
    #Nlambda=10e+6
    #print(gradient_x)
    while True:
        RH_x=lin.inv(H_x+np.eye(5)*Nlambda)
        RH_y=lin.inv(H_y+np.eye(5)*Nlambda)

        if np.linalg.det(RH_x)>1 and np.linalg.det(RH_y)>1:
            RH_x/=np.linalg.det(RH_x)**(1/5)
            RH_y/=np.linalg.det(RH_y)**(1/5)

        re_x=np.dot(RH_x,gradient_x)
        re_y=np.dot(RH_y,gradient_y)

        na1=a1-re_x[0]
        nb1=b1-re_x[1]
        nc1=c1-re_x[2]

        na2=a2-re_y[0]
        nb2=b2-re_y[1]
        nc2=c2-re_y[2]

        nnp=p-(re_x[3]+re_y[3])/2
        nnq=q-(re_x[4]+re_y[4])/2
        
        Nerror_x, Nerror_y = get_errors(na1,nb1,nc1,na2,nb2,nc2,nnp,nnq,Left,Right)
        #print(Nerror_x+Nerror_y, error_x+error_y, Nlambda)
        if Nerror_x+Nerror_y>=error_x+error_y:
            Nlambda*=Mulambda
            if Nlambda>10e60:
                Nlambda=10e60
                break
        else:
            Nlambda/=Mulambda
            if Nlambda<10e-60:
                Nlambda=10e-60
            break
            
    if epoch%1 == 0:
        print('{} {} with {}\n'.format(epoch, Nerror_x+Nerror_y,Nlambda), end='')
        #print(a1,b1,c1,a2,b2,c2,p,q)
        #print(re_x)
        sys.stdout.flush()
    
    a1=na1
    b1=nb1
    c1=nc1
    a2=na2
    b2=nb2
    c2=nc2
    p=nnp
    q=nnq
    epoch-=1

print(a1,b1,c1,a2,b2,c2,p,q)
    


# In[383]:


error_x, error_y = get_errors(a1,b1,c1,a2,b2,c2,p,q,Left,Right)
print(error_x/len(Left))
print(error_y/len(Left))
print(a1,b1,c1,a2,b2,c2,p,q)


# ### Results

# In[384]:


# Initiate ORB detector
orb = cv2.ORB_create()

kp1, des1 = orb.detectAndCompute(img1,None)
kp2, des2 = orb.detectAndCompute(img2,None) 

img1 = cv2.drawKeypoints(img1, kp1, None)
img2 = cv2.drawKeypoints(img2, kp2, None)

bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
matches = bf.match(des1, des2)
matches = sorted(matches, key=lambda x:x.distance)
img3 = cv2.drawMatches(img1, kp1, img2, kp2, matches[:30], None, flags=2)
#plt.figure(figsize=(20,10))
#plt.imshow(img3)
matplotlib.image.imsave("imgs/result_training_sample.jpg",img3)


# In[385]:


for idx in range(len(matches)):
    x,y=kp1[matches[idx].queryIdx].pt
    kp2[matches[idx].trainIdx].pt=(g(a1,b1,c1,x,y)/f(p,q,x,y), g(a2,b2,c2,x,y)/f(p,q,x,y))
    
img3 = cv2.drawMatches(img1, kp1, img2, kp2, matches[:30], None, flags=2)
#plt.figure(figsize=(20,10))
#plt.imshow(img3)
matplotlib.image.imsave("imgs/result_mine.jpg",img3)


# In[ ]:




