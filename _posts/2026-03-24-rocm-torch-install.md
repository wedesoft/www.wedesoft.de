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

[1]: https://rocm.docs.amd.com/projects/install-on-linux/en/latest/install/quick-start.html
[2]: https://www.reddit.com/r/comfyui/comments/1njjp79/complete_rocm_70_pytorch_280_installation_guide/
[3]: https://repo.radeon.com/rocm/manylinux/
