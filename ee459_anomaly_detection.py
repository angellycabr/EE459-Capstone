import pandas as pd
from datetime import datetime
from sklearn.model_selection import train_test_split
from keras.models import Sequential
from keras.layers import Dense, Input
from sklearn.metrics import r2_score
import matplotlib.pyplot as plt

# Read the data file
data = pd.read_csv("ee459_sensordata.csv")
data.head()

# Convert the Year column into datetime (from pandas)
data['Date'] = pd.to_datetime(data['DateTime'], format='%m/%d/%Y %H:%M')
data.head()

# Plot the temperature vs year.
plt.plot(data["Date"], data["Temperature (F)"])
plt.title("Temp vs. Month")
plt.ylabel("Temperature (F)")
plt.xlabel("Date")
plt.xticks(rotation=45)
plt.show()

# Plot the moisture  vs year.
plt.plot(data["Date"], data["Moisture (%)"])
plt.title("Moisture vs. Month")
plt.ylabel("Moisture (%)")
plt.xlabel("Date")
plt.xticks(rotation=45)
plt.show()

from sklearn.preprocessing import MinMaxScaler
import numpy as np

temperature_data = data['Temperature (F)'].values.astype("float32").reshape(-1, 1)
scaler = MinMaxScaler()
scaled_temperature = scaler.fit_transform(temperature_data)

def create_sequences(data, sequence_length):
  X = [] # Call this array X.
  y = [] # y is the next month’s temperature (1D array)
  for i in range(len(data) - sequence_length):
      X.append(data[i:i + sequence_length])
      y.append(data[i + sequence_length])
  return np.array(X), np.array(y)

# Setting the sequence length
sequence_length = 24
X, y = create_sequences(scaled_temperature, sequence_length)

# Reshaping
X = X.reshape(X.shape[0], X.shape[1])
y = y.reshape(y.shape[0])
print(X.shape, y.shape)

model = Sequential()
model.add(Input(shape=(24)))
model.add(Dense(128, activation="relu"))
model.add(Dense(64, activation="relu"))
model.add(Dense(1)) # activation='linear'

# Use mean squared error as the loss function.
model.compile(optimizer="adam", loss="mean_squared_error",)
model.summary()

# Train the network.
model.fit(X, y, verbose=1, epochs=50)

from sklearn.metrics import r2_score
from sklearn.metrics import mean_squared_error
import math

# Predict the next month’s temperature for all sequences in X. Inverse scale the temperature.
y_pred = model.predict(X)
inverse_y_pred = scaler.inverse_transform(y_pred)

# Score the model
RMSE = math.sqrt(mean_squared_error(scaler.inverse_transform(y.reshape(-1, 1)), inverse_y_pred[:, 0]))
print('Model RMSE:', RMSE)

plt.plot(data['Date'], scaler.inverse_transform(scaled_temperature))
plt.plot(data['Date'][-len(inverse_y_pred):], inverse_y_pred, color='red')
plt.title('Neural Network Temperature Prediction')
plt.xlabel('Year')
plt.xticks(rotation=45)
plt.ylabel('Temperature (F)')
plt.legend(['Original', 'Predicted'])
plt.show()

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Start with the last available sequence from the input data X
last_sequence = X[-1].copy()
future_predictions = []

num_predictions = 24
for _ in range(num_predictions):
    current_sequence_reshaped = last_sequence.reshape(1, -1)
    temperature = model.predict(current_sequence_reshaped)

    # Update the sequence with the new prediction
    last_sequence = np.roll(last_sequence, -1)
    last_sequence[-1] = temperature
    future_predictions.append(temperature[0, 0])

# Convert scaled predictions back to the temperature
inverse_future_predictions = scaler.inverse_transform(np.array(future_predictions).reshape(-1, 1))
prediction_dates = pd.date_range(start=data['Date'].iloc[-1], periods=num_predictions + 1, freq='H')[1:]

# Visualize original data, past predictions, and future predictions
plt.plot(data['Date'], scaler.inverse_transform(scaled_temperature), label='Original')
plt.plot(data['Date'][-len(inverse_y_pred):], inverse_y_pred, color='red', label='Past Predicted')
plt.plot(prediction_dates, inverse_future_predictions, color='green', label='Future Predicted')
plt.title('Neural Network Future Temperature Prediction')
plt.xlabel('Date')
plt.ylabel('Temperature (F)')
plt.xticks(rotation=45)
plt.legend()
plt.show()

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Start with the last available sequence from the input data X
last_sequence = X[-1].copy()
future_predictions = []

num_predictions = 720
for _ in range(num_predictions):
    current_sequence_reshaped = last_sequence.reshape(1, -1)
    temperature = model.predict(current_sequence_reshaped)

    # Update the sequence with the new prediction
    last_sequence = np.roll(last_sequence, -1)
    last_sequence[-1] = temperature
    future_predictions.append(temperature[0, 0])

# Convert scaled predictions back to the temperature
inverse_future_predictions = scaler.inverse_transform(np.array(future_predictions).reshape(-1, 1))
prediction_dates = pd.date_range(start=data['Date'].iloc[-1], periods=num_predictions + 1, freq='H')[1:]

# Visualize original data, past predictions, and future predictions
plt.plot(data['Date'], scaler.inverse_transform(scaled_temperature), label='Original')
plt.plot(data['Date'][-len(inverse_y_pred):], inverse_y_pred, color='red', label='Past Predicted')
plt.plot(prediction_dates, inverse_future_predictions, color='green', label='Future Predicted')
plt.title('Neural Network Future Temperature Prediction')
plt.xlabel('Date')
plt.ylabel('Temperature (F)')
plt.xticks(rotation=45)
plt.legend()
plt.show()

