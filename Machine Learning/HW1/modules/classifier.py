import torch.nn as nn

class CovidClassifier(nn.Module):
    def __init__(self, input_dim):
        super(CovidClassifier, self).__init__()
        # TODO: modify model's structure, be aware of dimensions. 
        self.layers = nn.Sequential(
            nn.Linear(input_dim, 64),
            nn.LeakyReLU(0.1)
            nn.Linear(64, 16),
            nn.LeakyReLU(0.1)
            nn.Linear(16, 1)
        )

    def forward(self, x):
        x = self.layers(x)
        x = x.squeeze(1) # (B, 1) -> (B)
        return x