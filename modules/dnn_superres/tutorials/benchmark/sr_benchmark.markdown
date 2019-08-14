Super-resolution benchmarking {#tutorial_dnn_superres_benchmark}
===========================

Here we collected a few benchmarking results of the dnn based super resolution module.
The benchmarking was performed on an Intel i7-9700K CPU.

Dataset benchmarking
----

###General100 dataset

#####2x scaling factor

|               | Inference time (CPU)| PSNR | SSIM |
| ------------- |:-------------------:| ---------:|--------:|
| ESPCN            | **0.00879461** | 32.7059 | 0.927588 |
| EDSR             | 5.92345 | **34.13** | **0.944665** |
| FSRCNN           | 0.0217408 | 32.8886 | 0.930145 |
| LapSRN           | 0.114812 | 32.2681 | 0.924783 |
| Bicubic          | 0.000207868 | 32.1638 | 0.930486 |
| Nearest neighbor | 0.000113877 | 29.1665 | 0.904894 |
| Lanczos          | 0.00109412 | 32.4687 | 0.932745 |

#####3x scaling factor

|               | Inference time (CPU)| PSNR | SSIM |
| ------------- |:-------------------:| ---------:|--------:|
| ESPCN            | **0.00549483**  | 28.4229 | 0.847427 |
| EDSR             |2.45551    | **29.9828**  | **0.880082** |
| FSRCNN           | 0.00880706   | 28.3068 | 0.842944 |
| LapSRN           |0.282575    |26.733   |0.886227  |
| Bicubic          |0.000311179 |26.0635  |0.875377  |
| Nearest neighbor |0.000148024 |23.5628  |0.817414  |
| Lanczos          |0.00101153  |25.9115  |0.870574  |


#####4x scaling factor

|               | Inference time (CPU)| PSNR | SSIM |
| ------------- |:-------------------:| ---------:|--------:|
| ESPCN            | **0.0043111** | 26.687 | 0.789056 |
| EDSR             | 1.60757    | **28.1552** | **0.831722**  |
| FSRCNN           | 0.0053019  | 26.6088 | 0.786267 |
| LapSRN           |0.121229    |26.7383   |0.789593  |
| Bicubic          |0.000311179 |26.0635  |0.875377  |
| Nearest neighbor |0.000148024 |23.5628  |0.817414  |
| Lanczos          |0.00101153  |25.9115  |0.870574  |


Images
----

####2x scaling factor

|Set5: butterfly.png | | ||
|:-------------:|:-------------------:|:-------------:|:----:|
|![Original](images/orig_butterfly.jpg)|![Bicubic interpolation](images/bicubic_butterfly.jpg)|![Nearest neighbor interpolation](images/nearest_butterfly.jpg)|![Lanczos interpolation](images/lanczos_butterfly.jpg) |
|PSRN / SSIM / Speed (CPU)|26.6645 / 0.904798 / 0.000200497 |23.6854 / 0.869778 / **0.0000752** |**26.9476** / **0.907497** / 0.00103943|
![ESPCN](images/espcn_butterfly.jpg)| ![FSRCNN](images/fsrcnn_butterfly.jpg) | ![LapSRN](images/lapsrn_butterfly.jpg) | ![EDSR](images/edsr_butterfly.jpg)
|29.0341 / 0.935363 / **0.0041565**| 29.0077 / 0.934455 / 0.0063251 | 27.8212 / 0.922938 / 0.0379372 | **30.0347** / **0.945346** / 2.07728 |

####3x scaling factor

|Urban100: img_001.png | | ||
|:-------------:|:-------------------:|:-------------:|:----:|
|![Original](images/orig_urban.jpg)|![Bicubic interpolation](images/bicubic_urban.jpg)|![Nearest neighbor interpolation](images/nearest_urban.jpg)|![Lanczos interpolation](images/lanczos_urban.jpg) |
|PSRN / SSIM / Speed (CPU)| 27.0474 / **0.848351** / 0.0.000391067 | 26.0842 / 0.835317 / **0.000236118** | **27.0704** / 0.848252 / 0.00223365|
|![ESPCN](images/espcn_urban.jpg)| ![FSRCNN](images/fsrcnn_urban.jpg) |  | ![EDSR](images/edsr_urban.jpg)
|28.0118 / 0.85882 / **0.0307482**| 28.0184 / 0.859662 / 0.0941731 | LapSRN is trained for x2, x4, and x8 because of its architecture  | **30.5671** / **0.90193** / 21.4095 |


####4x scaling factor

|Set14: comic.png | | ||
|:-------------:|:-------------------:|:-------------:|:----:|
|![Original](images/orig_comic.jpg)|![Bicubic interpolation](images/bicubic_comic.jpg)|![Nearest neighbor interpolation](images/nearest_comic.jpg)|![Lanczos interpolation](images/lanczos_comic.jpg) |
|PSRN / SSIM / Speed (CPU)| **19.6766** / **0.64134** / 0.0002619|18.5106 / 0.5879 / **0.0000847** |19.4948 / 0.6317 / 0.001098|
|![ESPCN](images/espcn_comic.jpg)| ![FSRCNN](images/fsrcnn_comic.jpg) | ![LapSRN](images/lapsrn_comic.jpg) | ![EDSR](images/edsr_comic.jpg)
|20.0417 / 0.6302 / **0.001894**| 20.0885 / 0.63835 / 0.0021026 | 20.0676 / 0.633933 / 0.06164 | **20.5233** / **0.690117** / 0.665876 |

####8x scaling factor

|Div2K: 0006.png | | |
|:-------------:|:-------------------:|:-------------:|
|![Original](images/orig_div2k.jpg)|![Bicubic interpolation](images/bicubic_div2k.jpg)|![Nearest neighbor interpolation](images/nearest_div2k.jpg)|
|PSRN / SSIM / Speed (CPU)| 26.3139 / **0.803324** / 0.0011072| 23.8291 / 0.73403 / **0.000610496** |
|![Lanczos interpolation](images/lanczos_div2k.jpg)| ![LapSRN](images/lapsrn_div2k.jpg) | |
|26.1565 / 0.796231 / 0.00478167| **26.7046** / 0.798661 / 2.27429 | |
