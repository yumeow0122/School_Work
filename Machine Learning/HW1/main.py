# PyTorch
import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader

from sklearn import preprocessing
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import f_regression

# For data preprocess
import pandas as pd
import numpy as np
import csv
import os

# For plotting
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure

# self defined module
from module import pytorch
from module import plot
from module import sk
# Set random seed for reproducibility
pytorch.setting()

# Choose the best features
# feature = sk.select_features()
# print(feature.nlargest(15,'Score'))

device = pytorch.get_device()
os.makedirs('models', exist_ok=True) 
target_only = True                   # 使用前面设置的特征

# TODO: How to tune these hyper-parameters to improve your model's performance?
config = {
    'n_epochs': 10000,                # 因为有early_stop，所以大一点没有影响
    'batch_size': 200,               # 微调batchsize
    'optimizer': 'Adam',              # 使用Adam优化器
    'optim_hparas': {                # 完全使用默认参数
        #'lr': 0.0001,                 
        #'momentum': 0.9,             
        #'weight_decay': 5e-4,
    },
    'early_stop': 500,               # 由于最后训练使用了所有数据，大一点影响不大
    'save_path': 'models/model.pth'  
}

tr_path = './data/covid.train.csv'  # path to training data
tt_path = './data/covid.test.csv'   # path to testing data
tr_set = pytorch.prep_dataloader(tr_path, 'train', config['batch_size'], target_only=target_only)
dv_set = pytorch.prep_dataloader(tr_path, 'dev', config['batch_size'], target_only=target_only)
tt_set = pytorch.prep_dataloader(tt_path, 'test', config['batch_size'], target_only=target_only)


model = pytorch.NeuralNet(tr_set.dataset.dim).to(device)  # Construct model and move to device

model_loss, model_loss_record = pytorch.train(tr_set, dv_set, model, config, device)

plot.learning_curve(model_loss_record, title='deep model')
model = pytorch.NeuralNet(tr_set.dataset.dim).to(device)
ckpt = torch.load(config['save_path'], map_location='cpu')  # Load your best model
model.load_state_dict(ckpt)
plot.pred(dv_set, model, device)  # Show prediction on the validation set

preds = pytorch.test(tt_set, model, device)  # predict COVID-19 cases with your model
pytorch.save_pred(preds, './data/submit.csv')         # save prediction file to pred.csv