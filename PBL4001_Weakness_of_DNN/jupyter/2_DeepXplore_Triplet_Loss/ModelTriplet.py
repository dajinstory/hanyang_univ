# usage: python MNISTModelTriplet.py

from __future__ import print_function

from keras.datasets import mnist
from keras.layers import Input, Conv2D, Lambda, Dense, Flatten,MaxPooling2D, concatenate
from keras.models import Model, Sequential
from keras.utils import to_categorical
import tensorflow as tf

from configs import bcolors
import numpy as np

def create_base_network(in_dims):
    """
    Base network to be shared.
    """
    #model = Sequential()
    #model.add(Conv2D(128,(7,7),padding='same',input_shape=(in_dims[0],in_dims[1],in_dims[2],),activation='relu',name='conv1'))
    #model.add(MaxPooling2D((2,2),(2,2),padding='same',name='pool1'))
    #model.add(Conv2D(256,(5,5),padding='same',activation='relu',name='conv2'))
    #model.add(MaxPooling2D((2,2),(2,2),padding='same',name='pool2'))
    #model.add(Flatten(name='flatten'))
    #model.add(Dense(4,name='embeddings'))
    
    input_tensor = Input(shape=in_dims)
    x = Conv2D(128,(7,7),padding='same',input_shape=(28,28,1,),activation='relu',name='conv1')(input_tensor)
    x = MaxPooling2D((2,2),(2,2),padding='same',name='pool1')(x)
    x = Conv2D(256,(5,5),padding='same',activation='relu',name='conv2')(x)
    x = MaxPooling2D((2,2),(2,2),padding='same',name='pool2')(x)
    x = Flatten(name='flatten')(x)
    x = Dense(4,name='embeddings')(x)
    model = Model(input_tensor, x, name='core')
    
    return model

def ModelTriplet(input_tensor=None, train=False):
    nb_classes = 10

    # tensor_0 : input
    # anchor_input = Input((28,28,1, ), name='anchor_input')

    # layer_0 : Shared embedding layer for positive and negative items
    Shared_DNN = create_base_network([28,28,1,])

    # tensor_1 : encoded vectors
    output_vector = Shared_DNN(input_tensor)

    model = Model(inputs=input_tensor, outputs=output_vector)
    model.load_weights('ModelTriplet.h5')

  
    return model

def get_distances(input_vector, centers):
    result=[]
    for label, center in enumerate(centers):
        dist_vector = input_vector - np.array(list(center.values()))
        pred = {
            'label':label, 
            'distance':np.sqrt(np.sum(dist_vector*dist_vector))
        }
        result.append(pred)
    
    return result