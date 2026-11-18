import tensorflow as tf
import pandas as pd
import numpy as np
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from sklearn.metrics import confusion_matrix

print(tf.__version__)


conjunto_datos = pd.read_csv('entrenar.txt', header=None, sep=' ')
datos_entrada = conjunto_datos.iloc[:, 0:3].values
datos_salida = conjunto_datos.iloc[:, 3:4].values

tf.random.set_seed(42)

modelo = Sequential()
modelo.add(Dense(2, input_dim=3, activation='relu', kernel_initializer='he_uniform'))
modelo.add(Dense(1, activation='sigmoid'))


modelo.compile(
    loss='binary_crossentropy',
    optimizer=tf.keras.optimizers.Adam(learning_rate=0.01),
    metrics=['binary_accuracy']
)

modelo.fit(datos_entrada, datos_salida, epochs=400)

p, b = modelo.layers[0].get_weights()
p2, b2 = modelo.layers[1].get_weights()

print("\n--- PESOS CAPA OCULTA (p y b) ---")
print(p)
print(b)

print("\n--- PESOS CAPA DE SALIDA (p2 y b2) ---")
print(p2)
print(b2)

x = [0.6301, 0.3288, 0.2511]
h1 = max(tf.keras.activations.relu(x[0]*p[0][0] + x[1]*p[1][0] + x[2]*p[2][0] + b[0]), 0).numpy()
h2 = max(tf.keras.activations.relu(x[0]*p[0][1] + x[1]*p[1][1] + x[2]*p[2][1] + b[1]), 0).numpy()
y = tf.keras.activations.sigmoid(h1*p2[0][0] + h2*p2[1][0] + b2[0]).numpy()


datos = modelo.evaluate(datos_entrada, datos_salida)
print("\n%s: %.2f%%" % (modelo.metrics[1], datos[1]*100))

y_pred = modelo.predict(datos_entrada)
y_pred = (y_pred >= 0.5)
cm = confusion_matrix(datos_salida, y_pred)
print("\nMatriz de Confusión:")
print(cm)