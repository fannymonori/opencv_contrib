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

                Mat sigma_img, sigma_orig;
                GaussianBlur( mu_img, sigma_img, Size( 11, 11 ), 0, 0 );
                GaussianBlur( mu_orig, sigma_orig, Size( 11, 11 ), 0, 0 );

                double c1 = (0.01 * 255) * (0.01 * 255);
                double c2 = (0.03 * 255) * (0.03 * 255);

                Mat nom1 = (2 * mu_img.mul(mu_orig) + c1);
                Mat nom2 = (2 * sigma_img.mul(sigma_orig) + c2);

                Mat nominator = nom1.mul(nom2);

                Mat denom1 = mu_img.mul(mu_img) + mu_orig.mul(mu_orig) + c1;
                Mat denom2 = sigma_img.mul(sigma_img) + sigma_orig.mul(sigma_orig) + c2;

                Mat denominator = denom1.mul(denom2);

                Mat ssim = nominator / denominator;

                Scalar mssim = mean( ssim );

                return mssim[0];
            }
        }
    }
}