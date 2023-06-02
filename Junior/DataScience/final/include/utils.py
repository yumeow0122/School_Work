import talib
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def one_accuracy(predicted, true):
	i = 0
	for x,y in zip(predicted,true):
		if(abs(x-y)==1):
			predicted[i] = y
		i = i + 1
  
def generate_indicate(data, indicator_list):
    # 創建空的DataFrame來存儲指標數據
    indicator_data = pd.DataFrame(index=data.index)

    # 計算每個技術指標
    for ta in indicator_list:
        if ta == 'MACD':
            macd, macdsignal, macdhist = talib.MACD(data['Close'])
            indicator_data['MACD'] = macd
            indicator_data['MACD_SIGNAL'] = macdsignal
            indicator_data['MACD_HIST'] = macdhist
        elif ta == 'RSI':
            rsi = talib.RSI(data['Close'])
            indicator_data['RSI'] = rsi
        elif ta == 'MOM':
            mom = talib.MOM(data['Close'])
            indicator_data['MOM'] = mom
        elif ta == 'STOCH':
            slowk, slowd = talib.STOCH(data['High'], data['Low'], data['Close'], fastk_period=9, slowk_period=5, slowd_period=4)
            indicator_data['STOCH_SLOWK'] = slowk
            indicator_data['STOCH_SLOWD'] = slowd
        elif ta == 'CCI':
            cci = talib.CCI(data['High'], data['Low'], data['Close'])
            indicator_data['CCI'] = cci
        elif ta == 'ROC':
            roc = talib.ROC(data['Close'])
            indicator_data['ROC'] = roc
        elif ta == 'OBV':
            obv = talib.OBV(data['Close'], data['Volume'])
            indicator_data['OBV'] = obv
        elif ta == 'ATR':
            atr = talib.ATR(data['High'], data['Low'], data['Close'])
            indicator_data['ATR'] = atr
        elif ta == 'ADX':
            adx = talib.ADX(data['High'], data['Low'], data['Close'])
            indicator_data['ADX'] = adx
    return indicator_data

def visualize_up_down(data):
    counts = data['up_down'].value_counts()
    plt.bar(counts.index, counts.values)
    plt.xlabel('up_down')
    plt.ylabel('Count')
    plt.title('Count of up_down')
    plt.savefig('./image/up_down.png')
    plt.close()
    
# To count after how many days the price will less then the current price
def data_marking(data):
    data.reset_index(drop=True, inplace=True)
    data['up_down'] = 0
    for cur in range(len(data)-1):
        highest_price = data['Close'][cur]
        for i in range(1, 20):
            if cur+i < len(data) and data['Close'][cur+i] < highest_price:
                data.loc[cur, 'up_down'] = i//4
                highest_price = max(highest_price, data['Close'][cur+i])
            else:
                break
    
    # Group the data by up_down
    

    visualize_up_down(data)
    return data['up_down']