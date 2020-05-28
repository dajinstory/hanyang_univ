#!/usr/bin/env python
# coding: utf-8

# ### Import Libraries

# In[1]:


import numpy as np
import time
import tensorflow as tf
import keras

np.random.seed(97)


# ### Create dataset

# In[2]:


def create_samples(sample_num):
    X = np.random.uniform(-2, 2, (sample_num, 2))
    y = (X[:,0]*X[:,0] > X[:,1]).astype(float)
    return X, y


# In[3]:


train_samples = 1000
test_samples = 100


# In[4]:


train_X, train_y = create_samples(train_samples)
test_X, test_y = create_samples(test_samples)


# ### Build model

# In[5]:


model = tf.keras.models.Sequential([
    tf.keras.layers.Dense(3, input_shape=(2,), activation=tf.nn.sigmoid),
    tf.keras.layers.Dense(1, activation=tf.nn.sigmoid)
])

model.compile(
    optimizer=tf.keras.optimizers.Adam(1),
    loss=tf.keras.losses.MeanSquaredError(),
    metrics=[tf.keras.metrics.BinaryAccuracy()] #tf.keras.metrics.Accuracy()]
)


# ### Train model

# In[6]:


train_time=0
train_loss=0
train_acc=0

test_time=0
test_loss=0
test_acc=0

run_nums=30


# In[7]:


for _ in range(run_nums):
    print(_)
    ## train model
    time_start = time.time()
    result = model.fit(
        train_X, train_y, 
        batch_size=1000, 
        epochs=1000,
        verbose=False
    )

    train_time += time.time()-time_start
    train_loss += result.history['loss'][-1]
    train_acc += result.history['binary_accuracy'][-1]
    
    ## test model
    time_start = time.time()
    model.evaluate(test_X, test_y, 
                   verbose=False
    )

    test_time += time.time()-time_start
    test_loss += result.history['loss'][-1]
    test_acc += result.history['binary_accuracy'][-1]


# In[8]:


train_time /= run_nums
train_loss /= run_nums
train_acc /= run_nums

test_time /= run_nums
test_loss /= run_nums
test_acc /= run_nums


# ### Results

# In[9]:


print('# Train')
print('loss \t:\t', train_loss)
print('acc \t:\t', train_acc)
print('time \t:\t', train_time)

print('\n# Test')
print('loss \t:\t', test_loss)
print('acc \t:\t', test_acc)
print('time \t:\t', test_time)


# In[ ]:




