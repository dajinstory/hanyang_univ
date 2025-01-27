import module.nn as nn
import numpy as np
np.random.seed(227)

# # 3. Train Model
def create_samples_sum(sample_num):
    X = np.random.uniform(-10, 10, (sample_num, 2))
    y = (np.sum(X, 1) >0).astype(float)
    return X, y

def run_practice_2(train_samples, test_samples, learning_rate, epochs, print_option='default'):
    
    train_X, train_y = create_samples_sum(train_samples)
    test_X, test_y = create_samples_sum(test_samples)

    train_X, train_y = train_X.reshape(-1,1,1,2), train_y.reshape(-1,1,1,1)
    test_X, test_y = test_X.reshape(-1,1,1,2), test_y.reshape(-1,1,1,1)
 
    # make model
    model = nn.Network(
        layers = [
            nn.Layer(INPUT_DIM=(1,1,2), kernel=(1,1,2,1), stride=(1,1), OUTPUT_DIM=(1,1,1)),
        ] 
    )
    if print_option != 'nothing': 
        print('train_samples : ', train_samples, train_X.shape)
        print('test_samples  : ', test_samples, train_y.shape)
        print('epochs        : ', epochs)
        print('learning_rate : ', learning_rate)
        model.print_layers();

    for epoch in range(epochs):
        model.train(train_X, train_y, learning_rate)

        if print_option == 'progress':
            print('\nepoch #' + str(epoch+1))
            print('w :' + str(model.layers[0].w.reshape((1,2))))
            print('b : ' + str(model.layers[0].b))
            
    return {'w': model.layers[0].w.reshape((1,2)), 
            'b': model.layers[0].b,
            'train_loss': model.loss(train_X, train_y),
            'test_loss': model.loss(test_X, test_y),
            'train_acc': 100 * np.mean(model.predict(train_X).astype(bool) == train_y.astype(bool)),
            'test_acc': 100 * np.mean(model.predict(test_X).astype(bool) == test_y.astype(bool))}


if __name__ == '__main__':
    train_samples = 1000 # m
    test_samples = 100 # n
    learning_rate = 1e-2
    epochs = 1000 # K
    result = run_practice_2(train_samples, test_samples, learning_rate, epochs)

    print('\nResult')
    for key in result:
        print(key, ":", result[key])
    print()
