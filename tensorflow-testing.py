import tensorflow as tf
import numpy as np
from tensorflow import keras
from PIL import Image
import os
import cv2


train_directory = "Img"
test_directory = "Img-testing"

numFiles = len([name for name in os.listdir(train_directory) if os.path.isfile(os.path.join(train_directory, name))])

#print(f'numFiles: {numFiles}')

train_images = []
test_images = []

for filename in os.listdir(train_directory):
  f = os.path.join(train_directory, filename)
  if os.path.isfile(f):
    image = cv2.resize(cv2.imread(f), (0, 0), fx=(1/40), fy=(1/30))
    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    train_images.append(image)

train_images = np.array(train_images)
train_labels = np.genfromtxt('english-types.csv', dtype=np.uint)

for filename in os.listdir(test_directory):
  f = os.path.join(test_directory, filename)
  if os.path.isfile(f):
    image = cv2.imread(f)
    image = cv2.resize(image, (0, 0), fx=(30/image.shape[1]), fy=(30/image.shape[0]))
    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    test_images.append(image)

test_images = np.array(test_images)
test_labels = np.genfromtxt('english-test-types.csv', dtype=np.uint)

model = tf.keras.models.Sequential([
  tf.keras.layers.Conv2D(64, (3, 3), activation='relu', input_shape=(30, 30, 1)),
  tf.keras.layers.MaxPooling2D(2, 2),
  tf.keras.layers.Conv2D(64, (3, 3), activation='relu'),
  tf.keras.layers.MaxPooling2D(2, 2),
  tf.keras.layers.Flatten(),
  tf.keras.layers.Dense(128, activation='relu'),
  tf.keras.layers.Dense(62, activation='softmax')
])

model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
model.fit(train_images, train_labels, epochs=50, verbose=2)

test_loss, test_acc = model.evaluate(test_images,  test_labels, verbose=2)
print('\nTest accuracy:', test_acc)

probability_model = tf.keras.Sequential([model, 
                                         tf.keras.layers.Softmax()])
predictions = probability_model.predict(test_images)
print(np.argmax(predictions[0]))
print(np.argmax(predictions[1]))
print(np.argmax(predictions[2]))
print(np.argmax(predictions[3]))
print(np.argmax(predictions[4]))