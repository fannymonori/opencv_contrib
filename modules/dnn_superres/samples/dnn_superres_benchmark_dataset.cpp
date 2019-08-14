// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include <iostream>

#include <opencv2/dnn_superres.hpp>
#include <opencv2/dnn_superres_quality.hpp>
#include <opencv2/datasets/sr_general100.hpp>

using namespace std;
using namespace cv;
using namespace dnn_superres;
using namespace cv::datasets;

int main(int argc, char *argv[])
{
    // Check for valid command line arguments, print usage
    // if insufficient arguments were given.
    if (argc < 4) {
        cout << "usage:   Arg 1: dataset path     | Path to dataset" << endl;
        cout << "\t Arg 2: algorithm 1 | edsr, espcn, fsrcnn or lapsrn" << endl;
        cout << "\t Arg 3: path to model file \n";
        cout << "\t Arg 4: scale     | 2, 3, 4 or 8 \n";
        return -1;
    }

    string dataset_path = string(argv[1]);
    string algorithm = string(argv[2]);
    string path = string(argv[3]);
    int scale = atoi(argv[4]);

    Ptr<SR_general100> dataset = SR_general100::create();
    dataset->load(dataset_path);

    int size = dataset->getTrain().size();

    std::vector<double> psnrs;
    std::vector<double> ssims;
    std::vector<double> perfs;

    std::vector<double> psnr_sums{0.0, 0.0, 0.0, 0.0};
    std::vector<double> ssim_sums{0.0, 0.0, 0.0, 0.0};
    std::vector<double> perf_sums{0.0, 0.0, 0.0, 0.0};

    //Make dnn super resolution instance
    DnnSuperResImpl sr;
    sr.readModel(path);
    sr.setModel(algorithm, scale);

    for(int i = 0; i < size; i++)
    {
        SR_general100Obj *dataset_object = static_cast<SR_general100Obj *>(dataset->getTrain()[i].get());
        string fullPath = dataset_path + "/" + dataset_object->imageName.c_str();


        std::cout << dataset_object->imageName.c_str() << std::endl;

        Mat img = cv::imread(fullPath);
        if (img.empty())
        {
            std::cerr << "Couldn't load image: " << img << "\n";
            return -2;
        }

        int width = img.cols - (img.cols % scale);
        int height = img.rows - (img.rows % scale);
        Mat cropped = img(Rect(0, 0, width, height));

        Mat img_downscaled;
        cv::resize(cropped, img_downscaled, cv::Size(), 1.0 / scale, 1.0 / scale);

        //Run benchmark
        //Mat img_new;
        //sr.upsample(img_downscaled, img_new);
        DnnSuperResQuality::benchmark(sr, cropped, psnrs, ssims, perfs, 0, 0);

        std::transform (psnr_sums.begin(), psnr_sums.end(), psnrs.begin(), psnr_sums.begin(), std::plus<double>());
        std::transform (ssim_sums.begin(), ssim_sums.end(), ssims.begin(), ssim_sums.begin(), std::plus<double>());
        std::transform (perf_sums.begin(), perf_sums.end(), perfs.begin(), perf_sums.begin(), std::plus<double>());
    }

    std::vector<String> names{algorithm, "bicubic", "nearest", "lanczos"};

    double psnr_avg, ssim_avg, perf_avg;
    for(unsigned int i = 0; i < psnr_sums.size(); i++)
    {
        psnr_avg = psnr_sums[i] / (double) size;
        std::cout << names[i] << " - PSNR - " << psnr_avg << std::endl;
        ssim_avg = ssim_sums[i] / (double) size;
        std::cout << names[i] << " - SSIM - " << ssim_avg << std::endl;
        perf_avg = perf_sums[i] / (double) size;
        std::cout << names[i] << " - Speed - " << perf_avg << std::endl;
    }

    return 0;
}