# # Import Libraries
import numpy as np
import time

np.random.seed(97)


# # 1. Layer
class Layer:
    
    def __init__(self, INPUT_DIM=(1,2,1), kernel=(1,2,1,1), stride=(1,1), OUTPUT_DIM=(1,1,1)):
        self.INPUT_DIM = INPUT_DIM                                                                  # INPUT_DIM  == (row_i, column_i, channel_i)
        self.OUTPUT_DIM = OUTPUT_DIM                                                                # OUTPUT_DIM == (row_o, column_o, channel_o)
        self.KERNEL_DIM = kernel                                                                    # kernel     == (row_k, column_k, channel_i, channel_o)
        self.stride = stride                                                                        # stride     == (row_s, column_s)    

#         self.w = np.zeros(self.KERNEL_DIM, dtype=np.float64)                                        # w_dim      == (row_k, column_k, channel_i) * channel_o
#         self.b = np.zeros(self.OUTPUT_DIM[-1], dtype=np.float64)                                    # b_dim      == 1 * channel_o
        self.w = np.random.uniform(-1, 1, self.KERNEL_DIM)                                          # w_dim      == (row_k, column_k, channel_i) * channel_o
        self.b = np.random.uniform(-1, 1, self.OUTPUT_DIM[-1])                                      # b_dim      == 1 * channel_o

    def forward(self, X):
        MIN_MARGIN = 2 ** -53
                    
        for row_idx in range(self.OUTPUT_DIM[0]):
            row_s = row_idx * self.stride[0]
            row_e = row_s + self.KERNEL_DIM[0]                                              
            for column_idx in range(self.OUTPUT_DIM[1]):
                column_s = column_idx * self.stride[1]
                column_e = column_s + self.KERNEL_DIM[1]                                
                for channel_o_idx in range(self.OUTPUT_DIM[2]): 

                    tmp_z = self.w[:,:,:,channel_o_idx] * X[:, row_s:row_e, column_s:column_e, :]   #       (row_k, column_k, channel_i, 1)
                                                                                                    # * (-1, row_k, column_k, channel_i)
                                                                                                    # = (-1, row_k, column_k, channel_i)
                    tmp_z = np.sum(tmp_z, axis=(1,2,3)) + self.b[channel_o_idx]                     # (-1, 1)
                    if len(tmp_z.shape)==1:
                        tmp_z=tmp_z.reshape(-1,1)
                    if row_idx == 0 and column_idx == 0 and channel_o_idx == 0:
                        self.z = tmp_z
                    else:
                        self.z = np.concatenate((self.z, tmp_z), axis=1)
                                                                                                        
                            
        self.z = self.z.reshape((-1,)+self.OUTPUT_DIM)                                              # (-1, row_o , column_o , channel_o)

        
        self.a = 1 / (1 + np.exp(-self.z))                                                          # (-1, row_o , column_o , channel_o)
        self.a = np.maximum(np.minimum(1 - MIN_MARGIN, self.a), MIN_MARGIN)                         # (-1, row_o , column_o , channel_o)
        return self.a                                                                               # (-1, row_o , column_o , channel_o)


    def backward(self, X, dx_next, learning_rate):
        # x_next == a_now
        # dx_next == da_now
        # dx_next == d_loss / dx_next == d_loss / da_now
        # == da
        da = dx_next                                                                                # (-1, row_o , column_o , channel_o)
        dz = self.a * (1 - self.a) * da                                                             # (-1, row_o , column_o , channel_o)
        dx = np.zeros(X.shape, dtype=np.float64)                                                    # (-1, row_i , column_i , channel_i)
        # check each "filters" in total kernel!
        for channel_o_idx in range(self.OUTPUT_DIM[2]):
            for row_idx in range(self.OUTPUT_DIM[0]):
                row_s = row_idx * self.stride[0]
                row_e = row_s + self.KERNEL_DIM[0]                                              
                for column_idx in range(self.OUTPUT_DIM[1]):
                    column_s = column_idx * self.stride[1]
                    column_e = column_s + self.KERNEL_DIM[1]                                
                    
                    # set dw
                    X_selected = X[:, row_s:row_e, column_s:column_e, :]                            # (-1, row_k, column_k, channel_i)
                    dz_selected = dz[:, row_idx, column_idx, channel_o_idx].reshape(-1,1,1,1)       # (-1,     1,        1,         1)
                    
                    if row_idx == 0 and column_idx == 0:
                        dw = X_selected * dz_selected                                               #   (-1, row_k, column_k, channel_i) 
                    else:                                                                           # * (-1,     1,        1,         1)
                        dw += X_selected * dz_selected                                              # = (-1, row_k, column_k, channel_i) 
                    
                    # set dx
                    w_selected = self.w[:,:,:,channel_o_idx]                                        #     (row_k, column_k, channel_i, 1)
                    dx[:, row_s:row_e, column_s:column_e, :] += dz_selected * w_selected            # (-1, row_k, column_k, channel_i, 1)
            
            dw = np.mean(dw, axis=0)                                                                # (1, row_k, column_k, channel_i)
            db = np.mean(np.sum(dz[:,:,:,channel_o_idx], axis=(1,2)), axis=0)                       # (1, 1)
            
            # update weights of current "filter" in total kernel
            # current "filter" == channel_th filter of kernel
            self.w[:,:,:,channel_o_idx] -= learning_rate * dw
            self.b[channel_o_idx] -= learning_rate * db
        
        dx = dx.reshape((-1,)+self.INPUT_DIM)                                                       # (-1, row_i, column_i, channel_i)
        return dx                                                                                   # (-1, row_i, column_i, channel_i)     


# # 2. Interface of neural network
class Network:
    
    def __init__(self, layers):
        self.layers = layers
        self.inputs = ['tmp' for i in layers]
    
    def forward(self, X):
        input_mat = X
        for idx, layer in enumerate(self.layers):
            self.inputs[idx]=input_mat
            input_mat = layer.forward(input_mat)

        return input_mat
        
    def backward(self, dx_next, learning_rate):
        # dx_next == da_now (x->z->a)
        for layer, input_mat in zip(reversed(self.layers), reversed(self.inputs)):
            dx_next=layer.backward(input_mat, dx_next, learning_rate)
            
    def train(self, X, y, learning_rate):
        pred_y = self.forward(X)
        dy_of_dx = -y / pred_y + (1 - y) / (1 - pred_y)
        self.backward(dy_of_dx, learning_rate)
        
    def predict(self, X):
        return np.round(self.forward(X))
    
    def loss(self, X, y):
        pred_y = self.forward(X)
        return -np.mean(y * np.log(pred_y) + (1 - y) * np.log(1 - pred_y))
    
    def print_layers(self):
        for layer_idx, layer in enumerate(self.layers):
            print('Layer #{}.  input : {}\t kernel : {}\t output : {}'.format(layer_idx+1, layer.INPUT_DIM, layer.KERNEL_DIM, layer.OUTPUT_DIM))

