---
layout: post
title: Installing Pytorch with AMD ROCm on GNU/Linux
category: graphics
---

Quickly sharing my notes on how to install drivers for ROCm and Pytorch for machine learning on AMD GPUs:

First [install ROCm and the AMD GPU driver][1].

Then as shown in this [Reddit post][2] install triton, torch, torchvision, and torchaudio.
Make sure to install the packages matching your ROCm version and Python version (e.g. Python 3.12 requires cp312 builds).
You can browse the available versions at [https://repo.radeon.com/rocm/manylinux/][3].
```Shell
pip install https://repo.radeon.com/rocm/manylinux/rocm-rel-7.0/pytorch_triton_rocm-3.4.0%2Brocm7.0.0.gitf9e5bf54-cp312-cp312
pip install https://repo.radeon.com/rocm/manylinux/rocm-rel-7.0/torch-2.8.0%2Brocm7.0.0.lw.git64359f59-cp312-cp312-linux_x86_64.whl
pip install https://repo.radeon.com/rocm/manylinux/rocm-rel-7.0/torchvision-0.24.0%2Brocm7.0.0.gitf52c4f1a-cp312-cp312-linux_x86_64.whl
pip install https://repo.radeon.com/rocm/manylinux/rocm-rel-7.0/torchaudio-2.8.0%2Brocm7.0.0.git6e1c7fe9-cp312-cp312-linux_x86_64.whl
```

I am trying the following program to train a neural network to imitate an XOR gate.

```Python
import torch
import torch.nn as nn
import torch.optim as optim

# Check if GPU is available
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# XOR data
X = torch.tensor([[0, 0], [0, 1], [1, 0], [1, 1]], dtype=torch.float32).to(device)
Y = torch.tensor([[0], [1], [1], [0]], dtype=torch.float32).to(device)

# Define the neural network
class XORNet(nn.Module):
    def __init__(self):
        super(XORNet, self).__init__()
        self.fc1 = nn.Linear(2, 2)
        self.fc2 = nn.Linear(2, 1)
        self.sigmoid = nn.Sigmoid()

    def forward(self, x):
        x = torch.relu(self.fc1(x))
        x = self.sigmoid(self.fc2(x))
        return x

# Initialize the network, loss function and optimizer
model = XORNet().to(device)
criterion = nn.BCELoss()
optimizer = optim.SGD(model.parameters(), lr=0.1)

# Training loop
for epoch in range(10000):
    model.train()
    optimizer.zero_grad()
    outputs = model(X)
    loss = criterion(outputs, Y)
    loss.backward()
    optimizer.step()

    if (epoch+1) % 1000 == 0:
        print(f'Epoch [{epoch+1}/10000], Loss: {loss.item():.4f}')

# Test the model
model.eval()
with torch.no_grad():
    predictions = model(X)
    print("Predictions:", predictions.round())
```

Currently I am getting the following error using Torch 2.9.1 and ROCm 7.2.0.

```Shell
RuntimeError: CUDA error: HIPBLAS_STATUS_INVALID_VALUE when calling `hipblasLtMatmulAlgoGetHeuristic( ltHandle, computeDesc.descriptor(), Adesc.descriptor(), Bdesc.descriptor(), Cdesc.descriptor(), Cdesc.descriptor(), preference.descriptor(), 1, &heuristicResult, &returnedResult)`
```

[1]: https://rocm.docs.amd.com/projects/install-on-linux/en/latest/install/quick-start.html
[2]: https://www.reddit.com/r/comfyui/comments/1njjp79/complete_rocm_70_pytorch_280_installation_guide/
[3]: https://repo.radeon.com/rocm/manylinux/
