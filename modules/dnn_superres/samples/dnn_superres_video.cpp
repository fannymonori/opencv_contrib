#include <iostream>

#include <opencv2/dnn_superres.hpp>

using namespace std;
using namespace cv;
using namespace dnn;
using namespace dnn_superres;

int main(int argc, char *argv[])
{
    // Check for valid command line arguments, print usage
    // if insufficient arguments were given.
    if (argc < 4) {
        cout << "usage:   Arg 1: input video path" << endl;
        cout << "\t Arg 2: output video path" << endl;
        cout << "\t Arg 3: algorithm | bilinear, bicubic, edsr, espcn, fsrcnn or lapsrn" << endl;
        cout << "\t Arg 4: scale     | 2, 3 or 4 \n";
        cout << "\t Arg 5: path to model file \n";
        return -1;
    }

    string input_path = string(argv[1]);
    string output_path = string(argv[2]);
    string algorithm = string(argv[3]);
    int scale = atoi(argv[4]);
    string path = string(argv[5]);

    //Make dnn super resolution instance
    DnnSuperResImpl sr;

    sr.readModel(path);
    sr.setModel(algorithm, scale);
    sr.upsampleVideo(input_path, output_path);


    cout << "Upsampling succeeded. \n";

    // Display image
    //cv::namedWindow("Initial Image", WINDOW_AUTOSIZE);
    //cv::imshow("Initial Image", img_new);
    //cv::imwrite("./saved.jpg", img_new);
    //cv::waitKey(0);

    return 0;
}