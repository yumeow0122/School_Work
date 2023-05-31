import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import itertools

df = pd.read_csv('./winequality-red.csv')

# 計算共變數
cov_matrix = df.cov()
# 計算相關係數
corr_matrix = df.corr()

# 定義所有特徵的列表
all_features = df.columns[:-1]  # 不包括品質特徵
# 定義最佳解的初始值
best_features = None
best_rmse = float('inf')

# 迴圈遍歷所有可能的特徵組合
for r in range(1, len(all_features) + 1):  # 從單個特徵到全部特徵
    for combination in itertools.combinations(all_features, r):
        features = list(combination)
        
        # 提取選定特徵和目標變量
        X = df[features]
        y = df['quality']
        
        # 拆分數據集為訓練集和測試集
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
        
        # 建立線性回歸模型
        model = LinearRegression()
        model.fit(X_train, y_train)
        
        # 在測試集上進行預測
        y_pred = model.predict(X_test)
        
        # 計算均方根誤差
        rmse = mean_squared_error(y_test, y_pred, squared=False)
        
        # 檢查是否為目前最佳解
        if rmse < best_rmse:
            best_rmse = rmse
            best_features = features

# 輸出最佳解
print("最佳特徵組合:", best_features)
print("最佳解的均方根誤差 (RMSE):", best_rmse)