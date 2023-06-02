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
        #'SUB', 'SMA', 'EMA', 'WMA', 'DEMA', 'TEMA', 'TRIMA', 'KAMA', 'MAMA', 'T3'
        elif ta == 'SUB':
            sub = talib.SUB(data['Close'], data['Open'])
            indicator_data['SUB'] = sub
        elif ta == 'SMA':
            sma = talib.SMA(data['Close'])
            indicator_data['SMA'] = sma
        elif ta == 'EMA':
            ema = talib.EMA(data['Close'])
            indicator_data['EMA'] = ema
        elif ta == 'WMA':
            wma = talib.WMA(data['Close'])
            indicator_data['WMA'] = wma
        elif ta == 'DEMA':
            dema = talib.DEMA(data['Close'])
            indicator_data['DEMA'] = dema
        elif ta == 'TEMA':
            tema = talib.TEMA(data['Close'])
            indicator_data['TEMA'] = tema
        elif ta == 'TRIMA':
            trima = talib.TRIMA(data['Close'])
            indicator_data['TRIMA'] = trima
        elif ta == 'KAMA':
            kama = talib.KAMA(data['Close'])
            indicator_data['KAMA'] = kama
        elif ta == 'MAMA':
            mama, fama = talib.MAMA(data['Close'])
            indicator_data['MAMA'] = mama
            indicator_data['FAMA'] = fama
        elif ta == 'T3':
            ttt = talib.T3(data['Close'])
            indicator_data['T3'] = ttt
        elif ta == 'DX':
            dx = talib.DX(data['High'], data['Low'], data['Close'])
            indicator_data['DX'] = dx
        elif ta == 'TRIX':
            trix = talib.TRIX(data['Close'])
            indicator_data['TRIX'] = trix
        elif ta == 'TSF':
            tsf = talib.TSF(data['Close'])
            indicator_data['TSF'] = tsf
        elif ta == 'STDDEV':
            stddev = talib.STDDEV(data['Close'])
            indicator_data['STDDEV'] = stddev
        elif ta == 'MIDPOINT':
            midpoint = talib.MIDPOINT(data['Close'])
            indicator_data['MIDPOINT'] = midpoint
        elif ta == 'MIDPRICE':
            midprice = talib.MIDPRICE(data['High'], data['Low'])
            indicator_data['MIDPRICE'] = midprice
        elif ta == 'SAR':
            sar = talib.SAR(data['High'], data['Low'])
            indicator_data['SAR'] = sar
        elif ta == 'BETA':
            beta = talib.BETA(data['High'], data['Low'])
            indicator_data['BETA'] = beta
        elif ta == 'LINEARREG':
            linearreg = talib.LINEARREG(data['Close'])
            indicator_data['LINEARREG'] = linearreg
        elif ta == 'LINEARREG_ANGLE':
            linearreg_angle = talib.LINEARREG_ANGLE(data['Close'])
            indicator_data['LINEARREG_ANGLE'] = linearreg_angle
           
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

    visualize_up_down(data)
    return data['up_down']