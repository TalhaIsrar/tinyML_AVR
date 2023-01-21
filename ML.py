import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

dataset = pd.read_csv(r'D:\NUST\Semester4\MPS\week7\assignment\temp_value.csv')


x = dataset.iloc[:, :1]
y = dataset.iloc[:, :2]

from sklearn.model_selection import train_test_split
x_train, x_test, y_train, y_test = train_test_split(x, y, test_size = 0.2, random_state = 0)

from sklearn.linear_model import LinearRegression
regressor = LinearRegression()
regressor.fit(x_train, y_train)

print(regressor.intercept_)
print(regressor.coef_)

dataset.plot(x = 'Voltage', y = 'Temperature', style='o')
plt.title('Voltage vs Temperature')
plt.xlabel('Voltage(mV)')
plt.ylabel('Temperature(F)')
plt.show()
