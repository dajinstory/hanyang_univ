#!/usr/bin/env python
# coding: utf-8

# ### Import Libraries

# In[1]:


import numpy as np
from PIL import Image


# ### Predefined Function

# In[2]:


def show_np_arr(np_arr):#np_arr should be 2-dim
    tmp = np_arr
    tmp = (tmp - np.min(tmp))/(np.max(tmp) - np.min(tmp))
    tmp = np.clip(255*tmp, 0, 255)
    tmp = Image.fromarray(np.uint8(tmp)).convert('RGB')
    tmp.show()  
    
def show_top_K_np_arr(np_arr, K):#np_arr should be 2-dim
    tmp = np_arr[:K]
    tmp = (tmp - np.min(tmp))/(np.max(tmp) - np.min(tmp))
    tmp = np.clip(255*tmp, 0, 255)
    tmp = tmp.reshape(K*28, 28)
    tmp = Image.fromarray(np.uint8(tmp)).convert('RGB')
    tmp.show()  
    
def show_top_40_np_arr(np_arr, filename):#np_arr should be 2-dim
    R=5
    C=8
    img = []
    for r in range(R):
        for c in range(C):
            tmp = np_arr[r*C+c]
            tmp = (tmp - np.min(tmp))/(np.max(tmp) - np.min(tmp))
            tmp = np.clip(255*tmp, 0, 255)
            tmp = tmp.reshape(28, 28)
            if c == 0:
                row_img = tmp
            else:
                row_img = np.concatenate((row_img, tmp), axis=1)
        if r==0:
            img = row_img
        else:
            img = np.concatenate((img, row_img), axis=0)
   
    
    img = Image.fromarray(np.uint8(img)).convert('RGB')
    img.save(filename)
    img.show()


# ### Load and pre-process dataset

# In[3]:


x_train = np.load("./x_train.npy")/255.
y_train = np.load("./y_train.npy")
x_test = x_train[2]

idx = np.argsort(y_train)
x_train = x_train[idx]
x_train = x_train[::200]
print(x_train.shape, x_test.shape)


# # Q1. Eigenface

# A : phi_flatten   
# v : eigenvec

# ### step1: compute mean of x_train

# In[4]:


### step1: compute mean of x_train
mu = np.mean(x_train, axis=0)

### step2: subtract the mean
phi = x_train - mu
phi_flatten = phi.reshape(phi.shape[0], -1)

### step3: compute covariance C
C = np.cov(phi_flatten.T)

### step4: Compute eigenvector of C, you don't need to do anything at step4.
eigenvalues, eigenvec = np.linalg.eig(C)
eigenvec = eigenvec.T
### Normalize eigenvec
# for r, e in enumerate(eigenvec):
#     eigenvec[r] /= np.sqrt(np.sum(e**2))
print("Shape of eigen vectors = ",eigenvec.shape)

### step5: choose K
ratio = 0
preserved_info_ratio = []
for eigenvalue in eigenvalues:
    ratio += eigenvalue
    preserved_info_ratio.append(ratio)
preserved_info_ratio /= np.sum(eigenvalues)

K = np.sum(preserved_info_ratio < 0.85)
print("K : ",K)

### step6: show top K eigenfaces. use show_np_arr func.
show_np_arr(eigenvec[0].reshape(28, 28))
#show_top_K_np_arr(eigenvec, K)


# ### Show Top 40 eigenfaces

# In[5]:


show_top_40_np_arr(eigenvec, "Top_40_EigenVec.png")


# # Q2. Image Approximation

# In[6]:


x = x_test - mu
x_flatten = x.reshape(-1, 1)

### step1: approximate x as x_hat with top K eigenfaces and show x_hat
projection_matrix = eigenvec[:K]
weights = np.dot(projection_matrix, x_flatten)
x_hat = mu + np.sum(weights*eigenvec[:K], axis=0).reshape(28, 28)
show_np_arr(x_hat)

### step2: compare mse between x and x_hat by changing the number of the eigenfaces used for reconstruction (approximation) from 1 to K
for k in range(1, K+1):
    projection_matrix = eigenvec[:k]
    weights = np.dot(projection_matrix, x_flatten)
    x_hat = mu + np.sum(weights*eigenvec[:k], axis=0).reshape(28, 28)
    
    mse = np.mean((x_hat - x_test)**2)
    print(k, mse)


# # Q3. Implement fast version of your algorithm in Q1. 

# ### Eigenfaces

# In[7]:


### step1: compute mean of x_train
mu = np.mean(x_train, axis=0)

### step2: subtract the mean
phi = x_train - mu
phi_flatten = phi.reshape(phi.shape[0], -1)

### step3: compute covariance C
C = np.cov(phi_flatten)

### step4: Compute eigenvector of C, you don't need to do anything at step4.
eigenvalues, eigenvec = np.linalg.eig(C)
eigenvec = eigenvec.T
eigenvec = np.dot(eigenvec, phi_flatten)
### Normalize eigenvec
for r, e in enumerate(eigenvec):
    eigenvec[r] /= np.sqrt(np.sum(e**2))
print("Shape of eigen vectors = ",eigenvec.shape)

### step5: choose K
ratio = 0
preserved_info_ratio = []
for eigenvalue in eigenvalues:
    ratio += eigenvalue
    preserved_info_ratio.append(ratio)
preserved_info_ratio /= np.sum(eigenvalues)

K = np.sum(preserved_info_ratio < 0.85)
print("K : ",K)

### step6: show top K eigenfaces. use show_np_arr func.
show_np_arr(eigenvec[0].reshape(28, 28))
#show_top_K_np_arr(eigenvec, K)


# ### Show Top 40 eigenfaces

# In[8]:


show_top_40_np_arr(eigenvec, "Fast_Top_40_EigenVec.png")


# ### Image Approximation

# In[9]:


x = x_test - mu
x_flatten = x.reshape(-1, 1)

### step1: approximate x as x_hat with top K eigenfaces and show x_hat
projection_matrix = eigenvec[:K]
weights = np.dot(projection_matrix, x_flatten)
x_hat = mu + np.sum(weights*eigenvec[:K], axis=0).reshape(28, 28)
show_np_arr(x_hat)

### step2: compare mse between x and x_hat by changing the number of the eigenfaces used for reconstruction (approximation) from 1 to K
for k in range(1, K+1):
    projection_matrix = eigenvec[:k]
    weights = np.dot(projection_matrix, x_flatten)
    x_hat = mu + np.sum(weights*eigenvec[:k], axis=0).reshape(28, 28)
    
    mse = np.mean((x_hat - x_test)**2)
    print(k, mse)


# # Q4 Time Comparison

# In[10]:


import time


# ### Q1 algorithm

# In[11]:


start = time.time()

### step1: compute mean of x_train
mu = np.mean(x_train, axis=0)

### step2: subtract the mean
phi = x_train - mu
phi_flatten = phi.reshape(phi.shape[0], -1)

### step3: compute covariance C
C = np.cov(phi_flatten.T)

### step4: Compute eigenvector of C, you don't need to do anything at step4.
eigenvalues, eigenvec = np.linalg.eig(C)
eigenvec = eigenvec.T
### Normalize eigenvec
# for r, e in enumerate(eigenvec):
#     eigenvec[r] /= np.sqrt(np.sum(e**2))
print("Shape of eigen vectors = ",eigenvec.shape)

### step5: choose K
ratio = 0
preserved_info_ratio = []
for eigenvalue in eigenvalues:
    ratio += eigenvalue
    preserved_info_ratio.append(ratio)
preserved_info_ratio /= np.sum(eigenvalues)

K = np.sum(preserved_info_ratio < 0.85)
print("K : ",K)

print(time.time() - start)


# ### Q3 algorithm

# In[12]:


start = time.time()

### step1: compute mean of x_train
mu = np.mean(x_train, axis=0)

### step2: subtract the mean
phi = x_train - mu
phi_flatten = phi.reshape(phi.shape[0], -1)

### step3: compute covariance C
C = np.cov(phi_flatten)

### step4: Compute eigenvector of C, you don't need to do anything at step4.
eigenvalues, eigenvec = np.linalg.eig(C)
eigenvec = eigenvec.T
eigenvec = np.dot(eigenvec, phi_flatten)
### Normalize eigenvec
for r, e in enumerate(eigenvec):
    eigenvec[r] /= np.sqrt(np.sum(e**2))
print("Shape of eigen vectors = ",eigenvec.shape)

### step5: choose K
ratio = 0
preserved_info_ratio = []
for eigenvalue in eigenvalues:
    ratio += eigenvalue
    preserved_info_ratio.append(ratio)
preserved_info_ratio /= np.sum(eigenvalues)

K = np.sum(preserved_info_ratio < 0.85)
print("K : ",K)

print(time.time() - start)

