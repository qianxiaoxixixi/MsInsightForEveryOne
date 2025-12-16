import torch

m = torch.nn.Softmax(dim=0)
input = torch.randn(2, 3, 4)
output = m(input)
print(f"input:{input}")
print(f"output:{output}]")
