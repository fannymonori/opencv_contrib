// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CONTRIB_DNN_SUPERRES_QUALITY_HPP
#define OPENCV_CONTRIB_DNN_SUPERRES_QUALITY_HPP

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdarg>
#include <sstream>
#include <ctime>
#include <opencv2/dnn_superres.hpp>
#include "opencv2/dnn.hpp"

namespace cv
{
    namespace dnn
    {
        namespace dnn_superres
        {
            class CV_EXPORTS DnnSuperResQuality
            {
                public:

                    /** @brief Returns the PSNR of two given image:
                    @param img Upscaled image
                    @param orig Original image
                    @return PSNR value.
                    */
                    static double psnr(Mat img, Mat orig);

                    /** @brief Returns the SSIM of two given image:
                    @param img Upscaled image
                    @param orig Original image
                    @return SSIM value.
                    */
                    static double ssim(Mat img, Mat orig);

                    /** @brief Gives a benchmark for the given super resolution algorithm. It compares it to
                     * bicubic, nearest neighbor, and lanczos interpolation methods.
                    @param sr DnnSuperRes object
                    @param img Image to upscale
                    @param showImg Displays the images if set to true
                    */
                    static void benchmark(DnnSuperResImpl sr, Mat img, bool showImg = false);

                    /** @brief Displays benchmarking for given images
                    @param orig Original image
                    @param imgages Upscaled images
                    @param imgages Title to window
                    @param imageSize Display size of images
                    */
                    static void showBenchmark(Mat orig, std::vector<Mat> images, std::string title, Size imageSize);
            };
        }
    }
}
#endif //OPENCV_CONTRIB_DNN_SUPERRES_QUALITY_HPP
