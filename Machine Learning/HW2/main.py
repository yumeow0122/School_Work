# import numpy as np

# print('Loading data ...')

# data_root='./timit_11/'
# train = np.load(data_root + 'train_11.npy')
# train_label = np.load(data_root + 'train_label_11.npy')
# test = np.load(data_root + 'test_11.npy')

# print('Size of training data: {}'.format(train.shape))
# print('Size of testing data: {}'.format(test.shape))

# PyTorch
import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader

# For data preprocess
import numpy as np
import csv
import os
import gc

import matplotlib.pyplot as plt
from matplotlib.pyplot import figure

# self modules
from modules import model, dataset, plot
from modules.dataset import TIMITDataset

# Set random 
model.set_seed(1203)

print('Loading data ...')
train_path = "./data/train_11.npy"
train_label_path = "./data/train_label_11.npy"
test_path = "./data/test_11.npy"
train = np.load(train_path)
train_label = np.load(train_label_path)
test = np.load(test_path)

print('Size of training data: {}'.format(train.shape))
print('Size of testing data: {}'.format(test.shape))
print(train_label)

print('Split data ...')
VAL_RATIO = 0.2
train_x, train_y = dataset.train_valid_split(train, VAL_RATIO, seed=1203)
label_x, label_y = dataset.train_valid_split(train_label, VAL_RATIO, seed=1203)
print('Size of training set: {}'.format(train_x.shape))
print('Size of validation set: {}'.format(label_x.shape))

BATCH_SIZE = 64
train_set = TIMITDataset(train_x, train_y)
val_set = TIMITDataset(label_x, label_y)
train_loader = DataLoader(train_set, batch_size=BATCH_SIZE, shuffle=True) #only shuffle the training data
val_loader = DataLoader(val_set, batch_size=BATCH_SIZE, shuffle=False)

del train, train_label, train_x, train_y, label_x, label_y
gc.collect()