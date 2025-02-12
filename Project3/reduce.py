import sys

import numpy as np
import struct as st
import gzip
from keras.models import Model
from keras.optimizers import RMSprop
from keras.layers.normalization import BatchNormalization
from sklearn.model_selection import train_test_split
from matplotlib import pyplot as plt
from keras.layers import Input,Conv2D,MaxPooling2D,UpSampling2D,Dense,Flatten,Reshape,Conv2DTranspose

def read_data(filename):
    file = gzip.open(filename,'rb')
    file.seek(0)
    magic_number = st.unpack('>4B',file.read(4)) # read magic number
    number_of_images = st.unpack('>I',file.read(4))[0] # read number of images
    number_of_rows = st.unpack('>I',file.read(4))[0] #read number of rows
    number_of_columns = st.unpack('>I',file.read(4))[0] #read number of column
    with gzip.open(filename) as file_stream:
        file_stream.read(16)
        buf = file_stream.read(number_of_rows * number_of_columns * number_of_images)
        data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
        data = data.reshape(number_of_images, number_of_rows,number_of_columns)
        return data
    
def saveNewImages(compressedImgs,filename):
    f = open(filename,"w")
    #The first line will contain the number of images
    f.write(str(len(compressedImgs)) + "\n")
    #The second line will contain the number of rows
    f.write(str(len(compressedImgs[0])) + "\n")
    #The third line will contain the number of columns
    try:
        f.write(str(len(compressedImgs[0][0])) + "\n")
    except:
        f.write("1\n")
    #For the rest of the file each line contains the pixels of an image
    #seperated by spaces
    for image in compressedImgs:
        for pixel in image:
            strPixel = str(pixel)
            strPixel = strPixel.strip('[')
            strPixel = strPixel.strip(']')
            f.write(strPixel + " ")
        f.write("\n")
    f.close()
 
def normalize(x):
    return (25500*(x - np.min(x))/np.ptp(x)).astype(int)
    
#Check program arguments
if ((len(sys.argv))!=9):
    print("Usage is: python reduce.py -d <dataset> -q <queryset> -od <output_dataset_file> -oq <output_query_file>")
    sys.exit()
if sys.argv[1] != "-d":
    print("Usage is: python reduce.py -d <dataset> -q <queryset> -od <output_dataset_file> -oq <output_query_file>")
    sys.exit()
elif sys.argv[3] != "-q":
    print("Usage is: python3 reduce.py -d <dataset> -q <queryset> -od <output_dataset_file> -oq <output_query_file>")
    sys.exit()
elif sys.argv[5] != "-od":
    print("Usage is: python3 reduce.py -d <dataset> -q <queryset> -od <output_dataset_file> -oq <output_query_file>")
    sys.exit()
elif sys.argv[7] != "-oq":
    print("Usage is: python3 reduce.py -d <dataset> -q <queryset> -od <output_dataset_file> -oq <output_query_file>")
    sys.exit()

    
while 1:
    #Read Hyper parameters    
    batch_size = int(input("Please enter batch size: "))
    epochs = int(input("Please enter epochs number: "))
    inChannel = 1
    x, y = 28, 28
    input_img = Input(shape = (x, y, inChannel))

    #Read data file
    train_data = read_data(sys.argv[2])
    test_data = read_data(sys.argv[4])
    train_data = train_data.reshape(-1, 28,28, 1)
    test_data = test_data.reshape(-1, 28,28, 1)
    train_data = train_data / np.max(train_data)
    test_data = test_data / np.max(test_data)
    #Split dataset to train and validation sets
    train_set_X,validation_set_X,train_ground,validation_ground = train_test_split(train_data,train_data,test_size=0.2,random_state=13)

    #Model Complilation and Training
    #encoder
    conv1 = Conv2D(32, (2, 2), activation='relu', padding='valid',strides=2)(input_img) # 14 x 14 x 32
    conv2 = Conv2D(64, (2, 2), activation='relu', padding='valid',strides=2)(conv1) # 7 x 7 x 64
    conv3 = Conv2D(128, (2, 2), activation='relu', padding='valid',strides=2)(conv2) # 3 x 3 x 128

    # bottleneck
    n_bottleneck = 1
    bottleneck = Dense(n_bottleneck)(conv2)

    flatten = Flatten()(conv3)
    encoded = Dense(int(10), activation='relu')(flatten)
    decoded = Dense(units = 3*3*128, activation='relu')(encoded)
    reshape = Reshape(target_shape = (3,3,128))(decoded)  # 3 x 3 x 128


    #decoder
    conv4 = Conv2DTranspose(64, (3, 3), activation='relu', padding='valid',strides=2)(reshape) # 7 x 7 x 64
    conv5 = Conv2DTranspose(32, (2, 2), activation='relu', padding='valid',strides=2)(conv4) # 14 x 14 x 32
    conv6 = Conv2DTranspose(1, (2, 2), activation='sigmoid', padding='valid',strides=2)(conv5) # 28 x 28 x 1


    decoded = conv6
    
    autoencoder = Model(input_img, decoded)
    autoencoder.compile(loss='mean_squared_error', optimizer = RMSprop())

    autoencoder_train = autoencoder.fit(train_set_X, train_ground, batch_size=batch_size,epochs=epochs,verbose=1,validation_data=(validation_set_X, validation_ground))

    Ans = input("Choose: \n 1)TYPE \"AGAIN\" to set new hyperparameters and retrain algorithm \n 2)TYPE \"SAVE\" to save the model\n 3)TYPE \"EXIT\" to stop program\n")
    
    if Ans=="SAVE":
        encoder = Model(inputs=input_img, outputs=bottleneck)
        X_train_encode = encoder.predict(train_data)
        X_test_encode = encoder.predict(test_data)
        saveNewImages(normalize(X_train_encode),sys.argv[6])
        saveNewImages(normalize(X_test_encode),sys.argv[8])
        sys.exit()
    if Ans=="EXIT":
        sys.exit()
