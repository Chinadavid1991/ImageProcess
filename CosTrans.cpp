//
// Created by feng on 2020/12/25.
//

#include "CosTrans.h"
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;


Mat CosTransform::dctTransform(){
    Mat& I = image;
    I.convertTo(I,CV_64FC1);
    Mat O;
    dct(I,O);
    return O;
}

Mat& CosTransform::compress(Mat& I,int stepx,int stepy){
    //将image分解成8X8的块
    image.convertTo(I,CV_64FC1);
    int dx = I.cols/stepx;
    int dy = I.rows/stepy;
    Rect2d windows;
    windows.height  = dy;
    windows.width   = dx;
    for(size_t ix = 0;ix < stepx;++ix)
    {
        for(size_t iy = 0;iy < stepy;++iy)
        {
            windows.x = ix*dx;
            windows.y = iy*dy;
            dct(I(windows),I(windows));
        }
    }
    return I;

}

Mat& CosTransform::uncompress(Mat& I,int stepx,int stepy){
    int dx = I.cols/stepx;
    int dy = I.rows/stepy;
    Rect2d windows;
    windows.height  = dy;
    windows.width   = dx;
    for(size_t ix = 0;ix < stepx;++ix)
    {
        for(size_t iy = 0;iy < stepy;++iy)
        {
            windows.x = ix*dx;
            windows.y = iy*dy;
            dct(I(windows),I(windows),DCT_INVERSE);
        }
    }
    return I;

}


void testDct(){
    Mat image = imread("/home/feng/disk/learn/learn-opencv/a.jpg",IMREAD_GRAYSCALE);
    resize(image,image,Size(640,480));
    CosTransform f(image);
    Mat m;

    m = f.compress(m,10,10);
    imshow("compress",m);
    m = f.uncompress(m,10,10);
    normalize(m,m,0,255,CV_MINMAX);
    m.convertTo(m,CV_8UC1);
    imshow("uncompress",m);
    waitKey(0);
}