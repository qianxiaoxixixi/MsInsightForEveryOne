import torch
import torch.nn as nn
import torch.optim as optim
import torch_npu
from msprobe.pytorch import PrecisionDebugger, seed_all
seed_all()

class ModelL(nn.Module):
    def __init__(self):
        super(ModelL, self).__init__()
        self.layer1 = nn.Linear(10, 20)
        self.layer2 = nn.Linear(20, 30)
        self.layer3 = nn.Linear(30, 1)

    def forward(self, x):
        x = torch.relu(self.layer1(x))
        x = torch.relu(self.layer2(x))
        x = self.layer3(x)
        return x

def generate_random_data(num_samples, num_features):
    X = torch.randn(num_samples, num_features)
    y = torch.randn(num_samples, 1)
    return X, y

X, y = generate_random_data(100, 10)

model = ModelL().npu()
debugger = PrecisionDebugger(model=model, task="statistics", dump_path='./train1', level='mix', step=[0])


def train_model(model, X, y, epochs=2, learning_rate=0.01):
    criterion = nn.MSELoss()
    optimizer = optim.Adam(model.parameters(), lr=learning_rate)
    
    for epoch in range(epochs):
        model.train()
        optimizer.zero_grad()
        debugger.start()
        outputs = model(X)
        loss = criterion(outputs, y)
        loss.backward()
        optimizer.step()
        debugger.stop()
        debugger.step()
        
        print(f'Epoch [{epoch+1}/{epochs}], Loss: {loss.item():.4f}')

train_model(model, X.npu(), y.npu())
