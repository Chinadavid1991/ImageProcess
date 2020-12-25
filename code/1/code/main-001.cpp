#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
using namespace cv;
int main(int argc, char** argv)
{
    // read image
    Mat image = imread("../../img/0.jpg");    
    Mat invertImage;
    image.copyTo(invertImage);
   
    int channels = image.channels();
    int rows = image.rows;
    int cols = image.cols * channels;
    if (image.isContinuous()) {
        cols *= rows;         
        rows = 1;
    }
    // 图像每一行是连续的
    uchar* p1;
    uchar* p2;
    ror (int row = 0; row < rows; row++) {
        p1 = image.ptr<uchar>(row);
        p2 = invertImage.ptr<uchar>(row);
        for (int col = 0; col < cols; col++) {
            *p2 = 255 - *p1; 
            p2++;
            p1++;
        }
    }
    // create windows
    namedWindow("My Test", CV_WINDOW_NORMAL);
    namedWindow("My Invert Image", CV_WINDOW_NORMAL);
    
    // display image
    imshow("My Test", image);
    imshow("My Invert Image", invertImage);
    
    waitKey(0);
    destroyWindow("My Test");
    destroyWindow("My Invert Image");
    return 0;
}
