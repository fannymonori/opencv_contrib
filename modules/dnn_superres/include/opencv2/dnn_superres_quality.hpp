// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CONTRIB_DNN_SUPERRES_QUALITY_HPP
#define OPENCV_CONTRIB_DNN_SUPERRES_QUALITY_HPP

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
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
                    static double psnr(Mat img, Mat orig);

                    static double ssim(Mat img, Mat orig);
            };
        }
    }
}
#endif //OPENCV_CONTRIB_DNN_SUPERRES_QUALITY_HPP
