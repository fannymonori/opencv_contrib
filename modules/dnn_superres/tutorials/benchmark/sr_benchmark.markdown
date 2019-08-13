Super-resolution benchmarking {#tutorial_dnn_superres_benchmark}
===========================

###General100

4x scaling: monarch.png

|               | Inference time (CPU)| PSNR | SSIM |
| ------------- |:-------------------:| ---------:|--------:|
| ESPCN            |0.0115944   | 26.5471 | 0.881162 |
| EDSR             |3.26758     |29.2404  |0.921126  |
| FSRCNN           | 0.012981   | 26.5646 | 0.880645 |
| LapSRN           |0.282575    |26.733   |0.886227  |
| Bicubic          |0.000311179 |26.0635  |0.875377  |
| Nearest neighbor |0.000148024 |23.5628  |0.817414  |
| Lanczos          |0.00101153  |25.9115  |0.870574  |