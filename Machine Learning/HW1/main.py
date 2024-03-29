# Numerical Operations
import math
import numpy as np

# Reading/Writing Data
import pandas as pd
import os
import csv

# Pytorch
import torch 
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader, random_split

# self defined modules
from modules import model, dataset, plot
from modules.dataset import COVID19Dataset
from modules.model import MyModel

train_path = "./data/covid.train.csv"
test_path = "./data/covid.test.csv"

# features = dataset.get_features(train_path)
# print(features.nlargest(20,'score'))

device = model.get_device()
os.makedirs('models', exist_ok=True) 
target_only = True                   # 使用前面设置的特征

# TODO: How to tune these hyper-parameters to improve your model's performance?
config = {
    'n_epochs': 5000,                # 因为有early_stop，所以大一点没有影响
    'batch_size': 100,               # 微调batchsize
    'optimizer': 'Adam',              # 使用Adam优化器
    'optim_hparas': {                # 完全使用默认参数
        'lr': 0.0001,                 
        'weight_decay': 1e-6,
    },
    'early_stop': 500,               # 由于最后训练使用了所有数据，大一点影响不大
    'save_path': 'models/model.pth'  
}

tr_set = dataset.prep_dataloader(train_path, 'train', config['batch_size'], target_only=target_only)
dv_set = dataset.prep_dataloader(train_path, 'dev', config['batch_size'], target_only=target_only)
tt_set = dataset.prep_dataloader(test_path, 'test', config['batch_size'], target_only=target_only)

model_ = MyModel(tr_set.dataset.dim).to(device)  # Construct model and move to device
model_loss, model_loss_record = model.train(tr_set, dv_set, model_, config, device)

plot.learning_curve(model_loss_record, title='deep model')
ckpt = torch.load(config['save_path'], map_location='cpu')  # Load your best model
model_.load_state_dict(ckpt)
plot.pred(dv_set, model_, device)  # Show prediction on the validation set
