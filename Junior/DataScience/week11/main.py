import pandas as pd

srcData = pd.read_csv('C:/Users/yumeow/Desktop/PROJECT/School_Work/Junior/DataScience/week11/data.csv')

resData = srcData.groupby(['High school system', 'Gender'], as_index=False).sum()

resData = resData.drop(['Num of enroll in National university', 'Num of enroll in private university', 'Num of enroll in other school'], axis=1)

resData['military school average'] = resData.groupby(['High school system', 'Gender'])['Num of enroll in military school'].transform(lambda x: x.sum() / x.count())

# Police university rate
resData['Police university rate'] = resData['Num of enroll in police university'] / resData['Num of graduates']
resData['Police university rate'] = resData['Police university rate'].round(4)

# Police college rate
resData['Police college rate'] = resData['Num of enroll in police college'] / resData['Num of graduates']
resData['Police college rate'] = resData['Police college rate'].round(4)

# Military school enrollment rate
resData['Military school enrollment rate'] = resData['Num of enroll in military school'] / resData['Num of graduates']
resData['Military school enrollment rate'] = resData['Military school enrollment rate'].round(4)

print(resData)
