import numpy as np

def get_group_data():
    a = float(input())
    if(a == -1):
        return a, -1, -1
    b = float(input())
    c = float(input())
    return a, b ,c

def get_np_data():
    data = np.zeros([0,3])

    idx = 0 
    while True:
        a, b, c = get_group_data()
        if a == -1:
            break
        data = np.insert(data, idx, [a,b,c], 0)
        idx = idx + 1
    return data

first = get_np_data()
print(first)
print()

second = get_np_data()
print(second)
print()

final = (first + second) / 2
for idx in range (0, int(final.size/3)):
    final[idx][0] = final[idx][0] + 10
    
    for student in range(0,3):
        if final[idx][student] < 60:
            final[idx][student] = -1
print(final)