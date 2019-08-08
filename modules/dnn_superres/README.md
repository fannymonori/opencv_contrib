# Super Resolution using Convolutional Neural Networks

This module contains several learning-based algorithms for upscaling an image.

## Building this module

Run the following command to build this module:

```make
cmake -DOPENCV_EXTRA_MODULES_PATH=<opencv_contrib>/modules -Dopencv_dnn_superres=ON <opencv_source_dir>
```

## Models

There are four models which are trained.

#### EDSR

Trained models can be downloaded from [here](https://github.com/Saafke/EDSR_Tensorflow/tree/master/models).

- Size of the model: ~38.5MB. This is a quantized version, so that it can be uploaded to GitHub. (Original was 150MB.)
- This model was trained for 3 days with a batch size of 16
- Link to implementation code: https://github.com/Saafke/EDSR_Tensorflow
- x2, x3, x4 trained models available
- Advantage: Highly accurate
- Disadvantage: Slow and large filesize
- Speed: ~12.0sec for a 256x256 image | ~49.0sec for a 512x512 image
- Original paper: <https://arxiv.org/abs/1707.02921>

#### ESPCN

Trained models can be downloaded from [here](https://github.com/fannymonori/TF-ESPCN/tree/master/export).

- Size of the model: ~100kb
- This model was trained for 100 iterations with a batch size of 32
- Link to implementation code: https://github.com/fannymonori/TF-ESPCN
- x2, x3, x4 trained models available
- Advantage: It is tiny, and fast, and still performs well.
- Disadvantage: Perform worse visually than newer, more robust models.
- Speed:
- Original paper: <https://arxiv.org/abs/1609.05158>

#### FSRCNN

Trained models can be downloaded from [here](https://github.com/Saafke/FSRCNN_Tensorflow/tree/master/models).

- Size of the model: ~40KB (~9kb for FSRCNN-small)
- This model was trained for ~30 iterations with a batch size of 1
- Link to implementation code: https://github.com/Saafke/FSRCNN_Tensorflow
- Advantage: Fast, small and accurate
- Disadvantage: Not state-of-the-art accuracy
- Speed: ~0.06sec for a 256x256 image | ~0.2sec for a 512x512 image
- Notes: FSRCNN-small has fewer parameters, thus less accurate but faster.
- Original paper: <https://arxiv.org/abs/1608.00367>

#### LapSRN

Trained models can be downloaded from [here](https://github.com/fannymonori/TF-LapSRN/tree/master/export).

- Size of the model: between 1-5Mb
- This model was trained for ~50 iterations with a batch size of 32
- Link to implementation code: https://github.com/fannymonori/TF-LAPSRN
- x2, x4, x8 trained models available
- Advantage: The model can do multi-scale super-resolution with one forward pass. It can now support 2x, 4x, 8x, and [2x, 4x] and [2x, 4x, 8x] super-resolution.
- Disadvantage: It is a slower model.
- Speed:
- Original paper: <https://arxiv.org/abs/1710.01992>
