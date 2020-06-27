#!/usr/bin/env python
# coding: utf-8

import tensorflow as tf
import keras
from tensorflow.keras.models import Model, Sequential
from tensorflow.keras.layers import Input, Dense, Dropout, Embedding, Reshape, Concatenate, Dot

import math
import numpy as np
import pandas as pd

import sys
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.patheffects as PathEffects

def load_dataset(filename_train, filename_test):
    dataset_train_df = pd.read_csv(filename_train, names=['user_id','movie_id','rating','time_stamp'], header=None, delimiter='\t')
    dataset_test_df = pd.read_csv(filename_test, names=['user_id','movie_id','rating','time_stamp'], header=None, delimiter='\t')
    return dataset_train_df, dataset_test_df  

class CFModel(Model):
    def __init__(self, num_of_users, num_of_items, num_of_factors):
        # input_user_id
        model_Embedding_user_id = Sequential([
            Embedding(num_of_users, num_of_factors, input_length=1, input_shape=(1,)),
            Reshape((num_of_factors,)),
        ])
        # input_item_id
        model_Embedding_item_id = Sequential([
            Embedding(num_of_items, num_of_factors, input_length=1, input_shape=(1,)),
            Reshape((num_of_factors,))
        ])
        # create base network
        input_user_id = Input((1,), name='input_user_id')
        input_item_id = Input((1,), name='input_item_id')
        embedded_user_id = model_Embedding_user_id(input_user_id)
        embedded_item_id = model_Embedding_item_id(input_item_id)
    
        output_label = Dot(axes=1)([embedded_user_id, embedded_item_id])
        
        super(CFModel, self).__init__(
            inputs=[input_user_id, input_item_id], 
            outputs=output_label
        )
        
    def score(self, y, pred_y):
        return np.sqrt( np.sum(np.square(pred_y.reshape(-1, 1) - y.reshape(-1,1))) / len(y) )
    

# 8.815, 6
if __name__ == '__main__':
    filename_train = sys.argv[1]
    filename_test = sys.argv[2]
    train_option = len(sys.argv) > 3
    test_num = int(filename_train[1:-5])
    filename_weights = 'weights/weights_CF_'+str(test_num)+'.h5'
    filename_prediction = filename_train+'_prediction.txt'
    
    # Load Dataset
    print('>> Load Dataset')
    dataset_train_df, dataset_test_df = load_dataset(filename_train, filename_test)

    train_x_user = dataset_train_df['user_id'].values
    train_x_item = dataset_train_df['movie_id'].values
    train_y = dataset_train_df['rating'].values
    test_x_user = dataset_test_df['user_id'].values
    test_x_item = dataset_test_df['movie_id'].values
    test_y = dataset_test_df['rating'].values

    num_of_users = max(dataset_train_df['user_id'].max(), dataset_test_df['user_id'].max()) + 1
    num_of_items = max(dataset_train_df['movie_id'].max(), dataset_test_df['movie_id'].max()) + 1
    num_of_factors = 100
    
    # Build Model
    print('>> Build Model')
    model = CFModel(num_of_users, num_of_items, num_of_factors)
    model.compile(
        loss='mse',
        #optimizer=tf.keras.optimizers.Adam(),
        optimizer='adamax',
    )
    
    
    if train_option:
        print('>> Train Model')
        callbacks = [
            tf.keras.callbacks.EarlyStopping('val_loss', patience=5),
        ]
        result = model.fit(
            [train_x_user, train_x_item], train_y,
            batch_size = 512,
            epochs = 1000,
            shuffle = True,
            verbose = True,
            validation_data = ([test_x_user, test_x_item], test_y),
            callbacks = callbacks,
        )
        model.save_weights(filename_weights)
    else:
        print('>> Load Model')
        model.load_weights(filename_weights)
    
    print('>> Prediction...')
    pred_y = model.predict([test_x_user, test_x_item]).reshape(-1,1)
    for idx, py in enumerate(pred_y):
        if py>5.0:
            pred_y[idx]=5.0
        elif py<1.0:
            pred_y[idx]=1.0
             
    print('RMSE : ', model.score(test_y, pred_y))
    
    print('>> Save results')
    dataset_pred_df = dataset_test_df
    dataset_pred_df['rating'] = pred_y
    dataset_pred_df.to_csv(filename_prediction, '\t', header=False, index=False)

