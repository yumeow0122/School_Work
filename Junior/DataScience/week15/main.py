#From: https://datauab.github.io/red_wine_quality/?fbclid=IwAR2fFSZMX1ity141TgEmlKfsP76PZCuSFr39rq4PPCfGFwqEvn8mZgnFggI

#Libraries we will use
import numpy as np
import warnings
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
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
from math import sqrt
from prettytable import PrettyTable

import include.util as util

df = pd.read_csv('./winequality-red.csv')
df.columns = df.columns.str.replace(' ', '_')

# check if have any null data
# print(df.isnull().sum())

#Calculate and order correlations
correlations = df.corr()['quality'].sort_values(ascending=False)
# print(correlations)
correlations.plot(kind='bar')
plt.savefig('./image/correlations.png')
plt.close()

# Heatmap to plot all correlations between features
plt.figure(figsize=(10, 6))
sns.heatmap(df.corr(), annot=True, fmt='.0%')
plt.title('Correlation Heatmap')
plt.savefig('./image/heatmap.png')
plt.close()

# Choose some high correlations features
correlations = df.corr()['quality'].abs()
high_corr_features = correlations[correlations > 0.2].index.tolist()
# print("High Correlation Features:", high_corr_features)

# Boxplot of alcohol percent in different quality wines
plt.figure(figsize=(10, 6))
bp = sns.boxplot(x='quality', y='alcohol', data=df)
bp.set(title="Alcohol Percent in Different Quality Wines")
plt.savefig('./image/alcohol.png')
plt.close()

# Boxplot of volatile acidity in medium quality wines
df_quality_five_six = df.loc[(df['quality'] >= 5) & (df['quality'] <= 6)]
correlations_subset = df_quality_five_six.corr()['quality'].sort_values(ascending=False)
bp = sns.boxplot(x='quality',y='sulphates', data=df)
bp.set(title="Sulphates in Different Quality Wines")
plt.savefig('./image/sulphates.png')
plt.close()

bp = sns.boxplot(x='quality',y='citric_acid', data=df)
bp.set(title="Citric Acid in Different Quality Wines")
plt.savefig('./image/citric_acid.png')
plt.close()

bp = sns.boxplot(x='quality',y='volatile_acidity', data=df)
bp.set(title="Acetic Acid Presence in Different Quality Wines")
plt.savefig('./image/volatile_acidity.png')
plt.close()

#We make a copy of our dataframe and group quality in differnt groups
# 0: Low quality, 1: Medium quality, 2: High quality
df_aux = df.copy()
df_aux['quality'].replace([3, 4], 0, inplace=True)
df_aux['quality'].replace([5, 6], 1, inplace=True)
df_aux['quality'].replace([7, 8], 2, inplace=True)
sns.countplot(data=df_aux, x='quality')
plt.savefig('./image/replace_countplot.png')
plt.close()

#We pot some histograms that show the values of features selected
flistt = ['alcohol','sulphates','citric_acid','volatile_acidity']
low = df_aux[df_aux['quality'] == 0]
medium = df_aux[df_aux['quality'] == 1]
high = df_aux[df_aux['quality'] == 2]
plt.rcParams.update({'font.size': 8})
plot, graphs = plt.subplots(nrows= 2, ncols= 2, figsize=(12,6))
graphs = graphs.flatten()
for i, graph in enumerate(graphs):
    graph.figure
    binwidth= (max(df_aux[flistt[i]]) - min(df_aux[flistt[i]]))/30
    bins = np.arange(min(df[flistt[i]]), max(df_aux[flistt[i]]) + binwidth, binwidth)
    graph.hist([low[flistt[i]],medium[flistt[i]],high[flistt[i]]], bins=bins, alpha=0.6, density=True, label=['Low','Medium','High'], color=['red','green','blue'])
    graph.legend(loc='upper right')
    graph.set_title(flistt[i])
plt.tight_layout()
plt.savefig('./image/histogram.png')
plt.close()

# choose features with high correlation
fix_correlations = correlations[abs(correlations) > 0.2]

#We separe our features from our target feature (quality) and we split data intro training and test
X = df.loc[:,['alcohol','sulphates','citric_acid','volatile_acidity']]
Y = df.iloc[:,11]

X_train, X_test, y_train, y_test = train_test_split(X, Y, test_size=0.3, random_state=42)

#Fit the model and make prediction
regressor = LinearRegression()
regressor.fit(X_train, y_train)
y_prediction_lr = regressor.predict(X_test)
y_prediction_lr = np.round(y_prediction_lr)

plt.scatter(y_test,y_prediction_lr)
plt.title("Prediction Using Linear Regression")
plt.xlabel("Real Quality")
plt.ylabel("Predicted")
plt.savefig('./image/prediction_lr.png')
plt.close()

#Confussion Matrix for the Linear Regression Model
label_aux = plt.subplot()
cm_linear_regression = confusion_matrix(y_test,y_prediction_lr)
cm_lr = pd.DataFrame(cm_linear_regression,
                     index = ['3','4','5','6','7','8'], 
                     columns = ['3','4','5','6','7','8'])
sns.heatmap(cm_lr,annot=True,fmt="d")
label_aux.set_xlabel('Predicted Quality');label_aux.set_ylabel('True Quality')
plt.savefig('./image/cm_lr1.png')
plt.close()

RMSE = sqrt(mean_squared_error(y_test, y_prediction_lr))
print("RMSE for origin Linear Regression is: " + str(RMSE))
print("Precision for origin Linear Regression is: " + str(precision_score(y_test, y_prediction_lr, average='weighted', zero_division=0)))

#Confussion Matrix for the Linear Regression Model
util.one_accuracy(y_prediction_lr, y_test)

label_aux = plt.subplot()
cm_linear_regression = confusion_matrix(y_test,y_prediction_lr)
cm_lr = pd.DataFrame(cm_linear_regression,
                     index = ['3','4','5','6','7','8'], 
                     columns = ['3','4','5','6','7','8'])
sns.heatmap(cm_lr,annot=True,fmt="d")
label_aux.set_xlabel('Predicted Quality');label_aux.set_ylabel('True Quality')
plt.savefig('./image/cm_lr2.png')
plt.close()

RMSE_lr = sqrt(mean_squared_error(y_test, y_prediction_lr))
print("RMSE for new improved Linear Regression is: " + str(RMSE_lr))
print("Precision for new improved Linear Regression is: " + str(precision_score(y_test, y_prediction_lr, average='weighted', zero_division=0)))
