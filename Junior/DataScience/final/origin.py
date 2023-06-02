import talib
import pandas as pd
import numpy as np
import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import seaborn as sns
from math import sqrt
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
from sklearn.metrics import accuracy_score
from sklearn.metrics import f1_score, confusion_matrix, accuracy_score, recall_score, precision_score
from sklearn.preprocessing import PolynomialFeatures
from sklearn.metrics import mean_squared_error
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import RandomForestRegressor
from sklearn import linear_model

import include.utils as util

FORECAST_PERIOD = 8

data = pd.read_csv('data.csv')
data['Time'] = pd.to_datetime(data['Time'])
data.set_index('Time', inplace=True)


# mark data with up_down 1 for up, 0 for down
data['up_down'] = util.data_marking(data)

# use follow indicator as features
ta_list = ['MACD','RSI','MOM','STOCH', 'CCI', 'ROC', 'OBV', 'ATR', 'ADX']
indicator_data = util.generate_indicate(data, ta_list)

# concat data and indicator_data and drop NaN
data = pd.concat([data, indicator_data], axis=1)
data = data.dropna()

# # Calculate and up_down correlations
# correlations = data.corr()['up_down'].sort_values(ascending=False)
# correlations.plot(kind='bar')
# plt.savefig('./image/correlations.png')
# plt.close()

# # OBV ROC MOM MACD RSI CCI STOCH  
# print(correlations)
# print(abs(correlations) > 0.1)

#We separe our features from our target feature (quality) and we split data intro training and test
X = data.loc[:,['RSI','CCI','MACD_HIST','ROC', 'MOM', 'MACD', 'STOCH_SLOWK', 'OBV']]
Y = data.iloc[:,5]

X_train, X_test, y_train, y_test = train_test_split(X, Y, test_size=0.3, random_state=42)

# Linear Regression
regressor = LinearRegression()
regressor.fit(X_train, y_train)
y_prediction_lr = regressor.predict(X_test)
y_prediction_lr = np.round(y_prediction_lr)

RMSE_rf = sqrt(mean_squared_error(y_test, y_prediction_lr))
print("Linear Regression RMSE:", RMSE_rf)
print("Linear Regression Accuracy:", accuracy_score(y_test, np.round(y_prediction_lr)))

#Decision Tree
regressor = DecisionTreeRegressor(random_state=0)
regressor.fit(X_train, y_train)
y_prediction_dt = regressor.predict(X_test)
y_prediction_dt = np.round(y_prediction_dt)

RMSE_rf = sqrt(mean_squared_error(y_test, y_prediction_dt))
print("Decision Tree RMSE:", RMSE_rf)
print("Decision Tree Accuracy:", accuracy_score(y_test, np.round(y_prediction_dt)))

# Random Forest
regressor = RandomForestRegressor(n_estimators=100, random_state=0)
regressor.fit(X_train, y_train)
y_prediction_rf = regressor.predict(X_test)

RMSE_rf = sqrt(mean_squared_error(y_test, y_prediction_rf))
print("Random Forest RMSE:", RMSE_rf)
print("Random Forest Accuracy:", accuracy_score(y_test, np.round(y_prediction_rf)))