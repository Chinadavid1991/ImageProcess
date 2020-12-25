#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <string>
using namespace cv;

int main(int argc, char** argv) {
    Mat src, dst;
    String path = argc >= 2 ? argv[argc-1] : "../../img/0.jpg"; 
    src = imread(path.c_str());
    if (!src.data) {
        printf("could not load image...\n");
        return -1;
    }
    namedWindow("input image", CV_WINDOW_NORMAL);
    imshow("input image", src);
    
    double t = getTickCount();
    Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    filter2D(src, dst, src.depth(), kernel);
    double timeconsume = (getTickCount() - t) / getTickFrequency();
    printf("tim consume %.2f\n", timeconsume);

    namedWindow("contrast image demo", CV_WINDOW_AUTOSIZE);
    imshow("contrast image demo", dst);

    waitKey(0);
    return 0;
}
