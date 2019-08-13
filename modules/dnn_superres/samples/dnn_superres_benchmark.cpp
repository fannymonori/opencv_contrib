// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include <iostream>

#include <opencv2/dnn_superres.hpp>
#include <opencv2/dnn_superres_quality.hpp>
#include <opencv2/datasets/sr_div2k.hpp>

using namespace std;
using namespace cv;
using namespace dnn;
using namespace dnn_superres;

int main(int argc, char *argv[])
{
    // Check for valid command line arguments, print usage
    // if insufficient arguments were given.
    if (argc < 4) {
        cout << "usage:   Arg 1: image path | Path to image" << endl;
        cout << "\t Arg 2: algorithm 1 | bilinear, bicubic, edsr, espcn, fsrcnn or lapsrn" << endl;
        cout << "\t Arg 3: algorithm 2 | bilinear, bicubic, edsr, espcn, fsrcnn or lapsrn" << endl;
        cout << "\t Arg 4: path to model file 1 \n";
        cout << "\t Arg 5: path to model file 2 \n";
        cout << "\t Arg 6: scale     | 2, 3 ,4 or 8 \n";
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

    Mat img_new1;
    Mat img_new2;

    //alg1
    sr.readModel(path1);
    sr.setModel(algorithm1, scale);
    sr.upsample(img_downscaled, img_new1);

    std::vector<double> psnrs;
    std::vector<double> ssims;
    std::vector<double> perfs;

    DnnSuperResQuality::setFontColor(cv::Scalar(255,0,0));
    DnnSuperResQuality::setFontScale(1.0);
    DnnSuperResQuality::setFontFace(cv::FONT_HERSHEY_COMPLEX_SMALL);
    DnnSuperResQuality::benchmark(sr, cropped, psnrs, ssims, perfs, 1, 1);

    //alg2
    sr.readModel(path2);
    sr.setModel(algorithm2, scale);
    sr.upsample(img_downscaled, img_new2);

    DnnSuperResQuality::benchmark(sr, cropped, psnrs, ssims, perfs, 1, 1);

    return 0;
}