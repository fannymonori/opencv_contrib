#include <iostream>

#include <opencv2/dnn_superres.hpp>
#include <opencv2/dnn_superres_quality.hpp>
#include <opencv2/datasets/sr_div2k.hpp>

using namespace std;
using namespace cv;
using namespace dnn;
using namespace dnn_superres;
using namespace cv::datasets;

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
        cout << "\t Arg 6: scale     | 2, 3 or 4 \n";
        return -1;
    }

    //string img_path = string(argv[1]);
    string path = string(argv[1]);
    string algorithm1 = string(argv[2]);
    string algorithm2 = string(argv[3]);
    string path1 = string(argv[4]);
    string path2 = string(argv[5]);
    int scale = atoi(argv[6]);

    Ptr<SR_div2k> dataset = SR_div2k::create();
    dataset->load(path);

    int size = dataset->getTrain().size();

    for(int i = 0; i < 10 ; i++)
    {
        SR_div2kObj *dataset_object = static_cast<SR_div2kObj *>(dataset->getTrain()[i].get());
        string fullPath = path + "/" + dataset_object->imageName.c_str();

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

        std::cout << algorithm1 << ": " << ps1 << " " << algorithm2 << ": " << ps2 << std::endl;
        std::cout << algorithm1 << ": " << ssim1 << " " << algorithm2 << ": " << ssim2 << std::endl;

        // Display image
        cv::namedWindow("Initial Image 1", WINDOW_AUTOSIZE);
        cv::namedWindow("Initial Image 2", WINDOW_AUTOSIZE);
        cv::imshow("Initial Image 1", img_new1);
        cv::imshow("Initial Image 2", img_new2);
        cv::waitKey(0);
    }

    return 0;
}