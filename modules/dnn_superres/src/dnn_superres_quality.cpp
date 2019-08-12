// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "precomp.hpp"

#include "opencv2/dnn_superres_quality.hpp"

namespace cv
{
    namespace dnn
    {
        namespace dnn_superres
        {
            double DnnSuperResQuality::psnr(Mat img, Mat orig)
            {
                CV_Assert(!img.empty());
                CV_Assert(!orig.empty());

                Mat img_float;
                img.convertTo(img_float, CV_32F);

                Mat orig_float;
                orig.convertTo(orig_float, CV_32F);

                Mat img_diff;
                absdiff(orig_float, img_float, img_diff);

                Mat img_mul = img_diff.mul(img_diff);

                Scalar sum = cv::sum(img_mul);

                double rgb_sum = 0.0;
                for(int i = 0; i < 4; i++){
                    rgb_sum += sum[i];
                }

                double mse = rgb_sum / (double) ( 3.0 * img_float.total() );

                double max = 255 * 255;
                double psnr = 10 * log10( max / (double) mse);

                return psnr;
            }

            double DnnSuperResQuality::ssim(Mat img, Mat orig)
            {
                CV_Assert(!img.empty());
                CV_Assert(!orig.empty());

                Mat ycrcb_img, ycrcb_orig;
                cvtColor(img, ycrcb_img, COLOR_BGR2YCrCb);
                cvtColor(orig, ycrcb_orig, COLOR_BGR2YCrCb);

                Mat img_channels[3], orig_channels[3];
                split(img, img_channels);
                split(orig, orig_channels);

                Mat img_float;
                img_channels[0].convertTo(img_float, CV_32F);

                Mat orig_float;
                orig_channels[0].convertTo(orig_float, CV_32F);

                Mat mu_img, mu_orig;
                GaussianBlur( img_float, mu_img, Size( 11, 11 ), 0, 0 );
                GaussianBlur( orig_float, mu_orig, Size( 11, 11 ), 0, 0 );
                Mat mu_mul = mu_img.mul(mu_orig);
                mu_img = mu_img.mul(mu_img);
                mu_orig = mu_orig.mul(mu_orig);

                Mat sigma_img, sigma_orig;
                GaussianBlur( img_float.mul(img_float), sigma_img, Size( 11, 11 ), 0, 0 );
                GaussianBlur( orig_float.mul(orig_float), sigma_orig, Size( 11, 11 ), 0, 0 );
                sigma_img = sigma_img - mu_img;
                sigma_orig = sigma_orig - mu_orig;

                Mat sigma_mul;
                GaussianBlur( img_float.mul(orig_float), sigma_mul, Size( 11, 11 ), 0, 0 );
                sigma_mul = sigma_mul - mu_mul;

                double c1 = (0.01 * 255) * (0.01 * 255);
                double c2 = (0.03 * 255) * (0.03 * 255);

                Mat nom1 = (2 * mu_mul + c1);
                Mat nom2 = (2 * sigma_mul + c2);

                Mat nominator = nom1.mul(nom2);

                Mat denom1 = mu_img + mu_orig + c1;
                Mat denom2 = sigma_img + sigma_orig + c2;

                Mat denominator = denom1.mul(denom2);

                Mat ssim = nominator / denominator;

                Scalar mssim = mean( ssim );

                return mssim[0];
            }

            void DnnSuperResQuality::benchmark(DnnSuperResImpl sr, Mat img, bool showImg)
            {
                CV_Assert(!img.empty());

                std::cout << "Start benchmarking" << std::endl;

                int scale = sr.getScale();

                int width = img.cols - (img.cols % scale);
                int height = img.rows - (img.rows % scale);
                Mat cropped = img(Rect(0, 0, width, height));
                Mat imgDownscaled;

                cv::resize(cropped, imgDownscaled, cv::Size(), 1.0/scale, 1.0/scale);

                timespec ts_beg, ts_end;

                Mat imgUpscaled;

                clock_gettime(CLOCK_REALTIME, &ts_beg);
                sr.upsample(imgDownscaled, imgUpscaled);
                clock_gettime(CLOCK_REALTIME, &ts_end);

                double elapsed = (ts_end.tv_sec - ts_beg.tv_sec) + (ts_end.tv_nsec - ts_beg.tv_nsec) / 1e9;

                std::cout << sr.getAlgorithm() << ":" << std::endl;
                std::cout << "Upsampling time: " << elapsed << std::endl;
                std::cout << "PSNR: " << psnr(imgUpscaled, cropped) << std::endl;
                std::cout << "SSIM: " << ssim(imgUpscaled, cropped) << std::endl;
                std::cout << "----------------------" << std::endl;

                //BICUBIC
                Mat bicubic;
                clock_gettime(CLOCK_REALTIME, &ts_beg);
                cv::resize(imgDownscaled, bicubic, cv::Size(), scale, scale, cv::INTER_CUBIC );
                clock_gettime(CLOCK_REALTIME, &ts_end);

                elapsed = (ts_end.tv_sec - ts_beg.tv_sec) + (ts_end.tv_nsec - ts_beg.tv_nsec) / 1e9;

                std::cout << "Bicubic \n" << "Upsampling time: " << elapsed << std::endl;
                std::cout << "PSNR: " << psnr(bicubic, cropped) << std::endl;
                std::cout << "SSIM: " << ssim(bicubic, cropped) << std::endl;
                std::cout << "----------------------" << std::endl;

                //NEAREST NEIGHBOR
                Mat nearest;
                clock_gettime(CLOCK_REALTIME, &ts_beg);
                cv::resize(imgDownscaled, nearest, cv::Size(), scale, scale, cv::INTER_NEAREST );
                clock_gettime(CLOCK_REALTIME, &ts_end);

                elapsed = (ts_end.tv_sec - ts_beg.tv_sec) + (ts_end.tv_nsec - ts_beg.tv_nsec) / 1e9;

                std::cout << "Nearest neighbor \n" << "Upsampling time: " << elapsed << std::endl;
                std::cout << "PSNR: " << psnr(nearest, cropped) << std::endl;
                std::cout << "SSIM: " << ssim(nearest, cropped) << std::endl;
                std::cout << "----------------------" << std::endl;

                //LANCZOS
                Mat lanczos;
                clock_gettime(CLOCK_REALTIME, &ts_beg);
                cv::resize(imgDownscaled, lanczos, cv::Size(), scale, scale, cv::INTER_LANCZOS4 );
                clock_gettime(CLOCK_REALTIME, &ts_end);

                elapsed = (ts_end.tv_sec - ts_beg.tv_sec) + (ts_end.tv_nsec - ts_beg.tv_nsec) / 1e9;

                std::cout << "Lanczos \n" << "Upsampling time: " << elapsed << std::endl;
                std::cout << "PSNR: " << psnr(lanczos, cropped) << std::endl;
                std::cout << "SSIM: " << ssim(lanczos, cropped) << std::endl;

                std::cout << "-----------------------------------------------" << std::endl;

                if(showImg)
                {
                    std::vector<Mat> imgs{ imgUpscaled, bicubic, nearest, lanczos };
                    showBenchmark(cropped, imgs, "Benchmarking", cv::Size(imgUpscaled.cols, imgUpscaled.rows));
                }

            }

            void DnnSuperResQuality::showBenchmark(Mat orig, std::vector<Mat> images, std::string title, Size imageSize)
            {
                CV_Assert(images.size() > 0);
                CV_Assert(!orig.empty());

                int cols, rows;

                int len = images.size();

                if (len <= 0)
                {
                    std::cout << "Function showBenchmark() supports up to 9 images" << std::endl;
                    return;
                }
                else if (len > 9)
                {
                    std::cout << "Function showBenchmark() supports up to 9 images" << std::endl;
                    return;
                }

                else if (len == 1)
                {
                    cols = 1;
                    rows = 1;
                }
                else if (len == 2)
                {
                    cols = 2;
                    rows = 1;
                }
                else if (len == 3 || len == 4)
                {
                    cols = 2;
                    rows = 2;
                }
                else if (len == 5 || len == 6)
                {
                    cols = 3;
                    rows = 2;
                }
                else if (len == 7 || len == 8 || len == 9)
                {
                    cols = 3;
                    rows = 3;
                }

                Mat fullImage = Mat::zeros(Size((cols * 10) + imageSize.width * cols, (rows * 10) + imageSize.height * rows), CV_8UC3);

                std::stringstream ss;
                int h_ = -1;
                for (int i = 0; i < len; i++)
                {
                    Mat img = images[i];

                    CV_Assert(!img.empty());

                    int w_ = i % cols;
                    if(i % cols == 0)
                        h_++;

                    Rect ROI((w_  * (10 + imageSize.width)), (h_  * (10 + imageSize.height)), imageSize.width, imageSize.height);
                    Mat tmp;
                    resize(img, tmp, Size(ROI.width, ROI.height));

                    ss << "PSNR: " << psnr(img, orig);
                    cv::putText(tmp,
                                ss.str(),
                                cv::Point(5,15),
                                cv::FONT_HERSHEY_COMPLEX_SMALL,
                                1.0,
                                cv::Scalar(255,255,255),
                                1,
                                16);

                    ss.str("");
                    ss << "SSIM: " << ssim(img, orig);
                    cv::putText(tmp,
                                ss.str(),
                                cv::Point(5,32),
                                cv::FONT_HERSHEY_COMPLEX_SMALL,
                                1.0,
                                cv::Scalar(255,255,255),
                                1,
                                16);

                    ss.str("");

                    tmp.copyTo(fullImage(ROI));
                }

                namedWindow( title, 1 );
                imshow( title, fullImage);
                waitKey();
            }
        }
    }
}