// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include <iostream>

#include <opencv2/dnn_superres.hpp>
#include <opencv2/dnn_superres_quality.hpp>

using namespace std;
using namespace cv;
using namespace dnn;
using namespace dnn_superres;

int main(int argc, char *argv[])
{
    // Check for valid command line arguments, print usage
    // if insufficient arguments were given.
    if (argc < 4) {
        cout << "usage:   Arg 1: dataset path     | Path to dataset" << endl;
        cout << "\t Arg 2: algorithm 1 | bilinear, bicubic, edsr, espcn, fsrcnn or lapsrn" << endl;
        cout << "\t Arg 3: algorithm 2 | bilinear, bicubic, edsr, espcn, fsrcnn or lapsrn" << endl;
        cout << "\t Arg 4: path to model file 1 \n";
        cout << "\t Arg 5: path to model file 2 \n";
        cout << "\t Arg 6: scale     | 2, 3, 4 or 8 \n";
        return -1;
    }

    string fullPath = string(argv[1]);
    string path = string(argv[1]);
    string algorithm1 = string(argv[2]);
    string algorithm2 = string(argv[3]);
    string path1 = string(argv[4]);
    string path2 = string(argv[5]);
    int scale = atoi(argv[6]);

    Mat img = cv::imread(fullPath);
    if (img.empty())
    {
        std::cerr << "Couldn't load image: " << img << "\n";
        return -2;
    }

    std::cout << "Image loaded." << std::endl;

    int width = img.cols - (img.cols % scale);
    int height = img.rows - (img.rows % scale);
    Mat cropped = img(Rect(0, 0, width, height));

    Mat img_downscaled;
    cv::resize(cropped, img_downscaled, cv::Size(), 1.0/scale, 1.0/scale);

    //Make dnn super resolution instance
    DnnSuperResImpl sr;

    std::vector<Mat> allImages;
    Mat img_new1;
    Mat img_new2;

    //alg1
    sr.readModel(path1);
    sr.setModel(algorithm1, scale);
    sr.upsample(img_downscaled, img_new1);

    //alg2
    sr.readModel(path2);
    sr.setModel(algorithm2, scale);
    sr.upsample(img_downscaled, img_new2);

    double ps1 = DnnSuperResQuality::psnr(img_new1, cropped);
    double ps2 = DnnSuperResQuality::psnr(img_new2, cropped);

    double ssim1 = DnnSuperResQuality::ssim(img_new1, cropped);
    double ssim2 = DnnSuperResQuality::ssim(img_new2, cropped);

    if ( img_new1.empty() || img_new2.empty() )
    {
        std::cerr << "Upsampling failed. \n";
        return -3;
    }
    cout << "Upsampling succeeded. \n";

    std::cout << algorithm1 << " | PSNR: " << ps1 << " SSIM: " << ssim1 << std::endl;
    std::cout << algorithm2 << " | PSNR: " << ps2 << " SSIM: " << ssim2 << std::endl;

    Mat bicubic;
    cv::resize(img_downscaled, bicubic, cv::Size(), scale, scale, cv::INTER_CUBIC );
    double ps_bicubic = DnnSuperResQuality::psnr(bicubic, cropped);
    double ssim_bicubic = DnnSuperResQuality::ssim(bicubic, cropped);
    std::cout << "Bicubic" << " | PSNR: " << ps_bicubic << " SSIM: " << ssim_bicubic << std::endl;

    Mat nearest;
    cv::resize(img_downscaled, nearest, cv::Size(), scale, scale, cv::INTER_NEAREST );
    double ps_nearest = DnnSuperResQuality::psnr(nearest, cropped);
    double ssim_nearest = DnnSuperResQuality::ssim(nearest, cropped);
    std::cout << "Nearest neighbor" << " | PSNR: " << ps_nearest << " SSIM: " << ssim_nearest << std::endl;

    Mat lanczos;
    cv::resize(img_downscaled, lanczos, cv::Size(), scale, scale, cv::INTER_LANCZOS4 );
    double ps_lanczos = DnnSuperResQuality::psnr(lanczos, cropped);
    double ssim_lanczos = DnnSuperResQuality::ssim(lanczos, cropped);
    std::cout << "Lanczos" << " | PSNR: " << ps_lanczos << " SSIM: " << ssim_lanczos << std::endl;

    allImages.push_back(img_new1);
    allImages.push_back(img_new2);
    allImages.push_back(bicubic);
    allImages.push_back(nearest);
    allImages.push_back(lanczos);

    DnnSuperResQuality::showBenchmark(cropped, allImages, "Image", cv::Size(bicubic.cols, bicubic.rows));

    imwrite( algorithm1 + ".jpg", img_new1 );
    imwrite( algorithm2 + ".jpg", img_new2 );
    imwrite( "bicubic.jpg", bicubic );
    imwrite( "nearest.jpg", nearest );
    imwrite( "lanczos.jpg", lanczos );

    cv::waitKey(0);

    return 0;
}