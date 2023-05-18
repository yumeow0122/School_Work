import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import LabelEncoder, OneHotEncoder
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import accuracy_score
from sklearn.impute import SimpleImputer


df = pd.read_csv('./Junior/DataScience/week14/titanic.csv')


# 填補缺失值
imputer = SimpleImputer(strategy="median")
df["Age"] = imputer.fit_transform(df[["Age"]])

# 移除包含缺失值的列
df.dropna(inplace=True)

# 進行獨熱編碼
df_encoded = pd.get_dummies(df, columns=["PClass", "Sex"])

# 選擇特徵和目標變量
features = ["Age", "PClass_1st", "PClass_2nd", "PClass_3rd", "Sex_female", "Sex_male"]
target = "Survived"

# 分割資料集
X = df_encoded[features]
y = df_encoded[target]
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

optimal_k = 0
optimal_accuracy = 0
for cur_k in range(1, 100):
    print("K值：", cur_k)
    # 建立KNN分類器模型
    knn = KNeighborsClassifier(n_neighbors=cur_k)  # 使用K=5
    knn.fit(X_train, y_train)

    # 在測試集上進行預測
    y_pred = knn.predict(X_test)

    # 計算預測正確性
    accuracy = accuracy_score(y_test, y_pred)
    if optimal_accuracy < accuracy:
        optimal_accuracy = accuracy
        optimal_k = cur_k

print("最佳K值：", optimal_k)
print("最佳正確性：", optimal_accuracy)