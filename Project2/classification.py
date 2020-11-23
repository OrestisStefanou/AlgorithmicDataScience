import sys
from sklearn.metrics import classification_report
from sklearn.model_selection import train_test_split
from keras.models import load_model
import keras
from matplotlib import pyplot as plt
import numpy as np
import gzip
# %matplotlib inline
from keras.models import Model
from keras.optimizers import RMSprop
from keras.layers import Input,Dense,Flatten,Dropout,merge,Reshape,Conv2D,MaxPooling2D,UpSampling2D,Conv2DTranspose
from keras.layers.normalization import BatchNormalization
from keras.models import Model,Sequential
from keras.callbacks import ModelCheckpoint
from keras.optimizers import Adadelta, RMSprop,SGD,Adam
from keras import regularizers
from keras import backend as K
from keras.utils import to_categorical

def printHelp():
    print("Usage is python classification.py –d <training set> –dl <training labels> -t <testset> -tl <test labels> -model <autoencoder h5>")

if len(sys.argv) != 11:
    printHelp()
    exit(1)

#print(sys.argv)
train_data_file = ""
test_data_file = ""
train_labels_file = ""
test_labels_file = ""
autoencoder_file = ""

for i in range(len(sys.argv)):
    if sys.argv[i] == "-d":
        train_data_file = str(sys.argv[i+1])
        continue
    if sys.argv[i] == "-dl":
        train_labels_file = str(sys.argv[i+1])
        continue
    if sys.argv[i] == "-t":
        test_data_file = str(sys.argv[i+1])
        continue
    if sys.argv[i] == "-tl":
        test_labels_file = str(sys.argv[i+1])
        continue
    if sys.argv[i] == "-model":
        autoencoder_file = str(sys.argv[i+1])
        continue

"""
print("Train data file is:" + train_data_file)
print("Test data file is:" + test_data_file)
print("Train labels file is:" + train_labels_file)
print("Test labels file is:" + test_labels_file)
print("Model file is:" + autoencoder_file)"""


"""Function to read the data"""

def extract_data(filename, num_images):
    with gzip.open(filename) as bytestream:
        bytestream.read(16)
        buf = bytestream.read(28 * 28 * num_images)
        data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
        data = data.reshape(num_images, 28,28)
        return data

train_data = extract_data(train_data_file,60000)
test_data = extract_data(test_data_file, 10000)

"""Function to read labels"""

def extract_labels(filename, num_images):
    with gzip.open(filename) as bytestream:
        bytestream.read(8)
        buf = bytestream.read(1 * num_images)
        labels = np.frombuffer(buf, dtype=np.uint8).astype(np.int64)
        return labels

def fc(enco):
    flat = Flatten()(enco)
    den = Dense(128, activation='relu')(flat)
    out = Dense(num_classes, activation='softmax')(den)
    return out



train_labels = extract_labels(train_labels_file,60000)
test_labels = extract_labels(test_labels_file,10000)

# Create dictionary of target classes
label_dict = {
 0: '0',
 1: '1',
 2: '2',
 3: '3',
 4: '4',
 5: '5',
 6: '6',
 7: '7',
 8: '8',
 9: '9',
}

#Reshape the data
train_data = train_data.reshape(-1, 28,28, 1)
test_data = test_data.reshape(-1, 28,28, 1)

#Rescale the training and testing data with the maximum pixel value of the training and testing data
train_data = train_data / np.max(train_data)
test_data = test_data / np.max(test_data)

#Split the data in test and training set
train_X,valid_X,train_ground,valid_ground = train_test_split(train_data,
                                                             train_data,
                                                             test_size=0.2,
                                                             random_state=13)

#Load the autoencoder model
autoencoder = load_model(autoencoder_file)

while True:
    batch_size = int(input("Enter batch size:"))
    epochs = int(input("Enter epochs number:"))
    inChannel = 1
    x, y = 28, 28
    input_img = Input(shape = (x, y, inChannel))
    num_classes = 10
    # Change the labels from categorical to one-hot encoding
    train_Y_one_hot = to_categorical(train_labels)
    test_Y_one_hot = to_categorical(test_labels)

    #Split the data again
    train_X,valid_X,train_label,valid_label = train_test_split(train_data,train_Y_one_hot,test_size=0.2,random_state=13)

    encode = encoder(input_img)
    full_model = Model(input_img,fc(encode))

    for l1,l2 in zip(full_model.layers[:19],autoencoder.layers[0:19]):
        l1.set_weights(l2.get_weights())

    #autoencoder.get_weights()[0][1]

    #full_model.get_weights()[0][1]

    for layer in full_model.layers[0:19]:
        layer.trainable = False


    full_model.compile(loss=keras.losses.categorical_crossentropy, optimizer=keras.optimizers.Adam(),metrics=['accuracy'])


    #full_model.summary()

    #Train the model
    classify_train = full_model.fit(train_X, train_label, batch_size=512,epochs=100,verbose=1,validation_data=(valid_X, valid_label))

    """Save the classification model"""

    full_model.save_weights('autoencoder_classification.h5')

    """ Re-train the model by making the first nineteen layers trainable as True instead of keeping them False"""

    for layer in full_model.layers[0:19]:
        layer.trainable = True

        full_model.compile(loss=keras.losses.categorical_crossentropy, optimizer=keras.optimizers.Adam(),metrics=['accuracy'])

    """Train the entire model for one last time"""

    classify_train = full_model.fit(train_X, train_label, batch_size=512,epochs=100,verbose=1,validation_data=(valid_X, valid_label))

    """Save the model"""

    full_model.save_weights('classification_complete.h5')

    flag2 = False
    flag3 = False
    print("1)Repeat with different hyperparameters")
    print("2)Show the plots with accuracy and erros")
    print("3)Continue with classification")
    user_choice = int(input("Enter a choice(1-3):"))
    if user_choice == 1:
        continue
    if user_choice == 2:
        flag2 = True
    if user_choice == 3:
        flag3 = True

    if flag2:
        """Plot the accuracy and loss plots between training and validation data:"""
        accuracy = classify_train.history['accuracy']
        val_accuracy = classify_train.history['val_accuracy']
        loss = classify_train.history['loss']
        val_loss = classify_train.history['val_loss']
        epochs = range(len(accuracy))
        plt.plot(epochs, accuracy, 'bo', label='Training accuracy')
        plt.plot(epochs, val_accuracy, 'b', label='Validation accuracy')
        plt.title('Training and validation accuracy')
        plt.legend()
        plt.figure()
        plt.plot(epochs, loss, 'bo', label='Training loss')
        plt.plot(epochs, val_loss, 'b', label='Validation loss')
        plt.title('Training and validation loss')
        plt.legend()
        plt.show()

        """Model Evaluation on the Test Set"""
        test_eval = full_model.evaluate(test_data, test_Y_one_hot, verbose=0)
        print('Test loss:', test_eval[0])
        print('Test accuracy:', test_eval[1])

    if flag3 :
        """Predict Labels"""
        predicted_classes = full_model.predict(test_data)

        predicted_classes = np.argmax(np.round(predicted_classes),axis=1)
        predicted_classes.shape, test_labels.shape

        correct = np.where(predicted_classes==test_labels)[0]
        print ("Found %d correct labels" % len(correct))
        for i, correct in enumerate(correct[:9]):
            plt.subplot(3,3,i+1)
            plt.imshow(test_data[correct].reshape(28,28), cmap='gray', interpolation='none')
            plt.title("Predicted {}, Class {}".format(predicted_classes[correct], test_labels[correct]))
            plt.tight_layout()

        incorrect = np.where(predicted_classes!=test_labels)[0]
        print ("Found %d incorrect labels" % len(incorrect))
        for i, incorrect in enumerate(incorrect[:9]):
            plt.subplot(3,3,i+1)
            plt.imshow(test_data[incorrect].reshape(28,28), cmap='gray', interpolation='none')
            plt.title("Predicted {}, Class {}".format(predicted_classes[incorrect], test_labels[incorrect]))
            plt.tight_layout()

        """Classification Report"""
        target_names = ["Class {}".format(i) for i in range(num_classes)]
        print(classification_report(test_labels, predicted_classes, target_names=target_names))