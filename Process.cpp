//
// Created by feng on 2020/12/25.
//

#include "Process.h"
#include <opencv2/opencv.hpp>
#include <limits>
using namespace cv;
using namespace std;
Mat Process::edgeFilter(const Mat& kernel) const{
    Mat edge;
    filter2D(image,edge,CV_64F,kernel);
    return edge;
}

Mat Process::gaussianBlur(const Size& s,double sigmaXY) const{
    Mat blur;
    GaussianBlur(image,blur,s,sigmaXY,sigmaXY);
    return blur;
}


void Process::addSaltNoise(int n)
{
    for (int k = 0; k < n; k++)
    {
        //随机取值行列
        int i = (int)random() % image.rows;
        int j = (int)random() % image.cols;
        //图像通道判定
        if (image.channels() == 1)
        {
            image.at<uchar>(i, j) = 255;
        }
        else
        {
            image.at<Vec3b>(i, j)[0] = 255;
            image.at<Vec3b>(i, j)[1] = 255;
            image.at<Vec3b>(i, j)[2] = 255;
        }
    }
    for (int k = 0; k < n; k++)
    {
        //随机取值行列
        int i = (int)random() % image.rows;
        int j = (int)random() % image.cols;
        //图像通道判定
        if (image.channels() == 1)
        {
            image.at<uchar>(i, j) = 0;		//椒噪声
        }
        else
        {
            image.at<Vec3b>(i, j)[0] = 0;
            image.at<Vec3b>(i, j)[1] = 0;
            image.at<Vec3b>(i, j)[2] = 0;
        }
    }
}
double Process::generateGaussianNoise(double mu, double sigma)
{
    //定义小值
    const double epsilon = numeric_limits<double>::min();
    double z;
    double u1, u2;
    //构造随机变量
    do
    {
        u1 = (int)random() * (1.0 / RAND_MAX);
        u2 = (int)random() * (1.0 / RAND_MAX);
    } while (u1 <= epsilon);

    z = sqrt(-2.0*log(u1))*cos(2 * CV_PI*u2)*32;
    return z*sigma + mu;
}


void Process::addGaussianNoise(double mu, double sigma)
{
    int channels = image.channels();
    int rowsNumber = image.rows;
    int colsNumber = image.cols*channels;
    //判断图像的连续性
    if (image.isContinuous())
    {
        colsNumber *= rowsNumber;
        rowsNumber = 1;
    }
    for (int i = 0; i < rowsNumber; i++)
    {
        for (int j = 0; j < colsNumber; j++)
        {
            //添加高斯噪声
            int val = image.ptr<uchar>(i)[j] +
                      generateGaussianNoise(mu, sigma);
            if (val < 0)
                val = 0;
            if (val>255)
                val = 255;
            image.ptr<uchar>(i)[j] = (uchar)val;
        }
    }
}


void testFilterEdge(){
    Mat image =  imread("/home/feng/disk/learn/learn-opencv/data/a.jpg");
    Mat edge;
    resize(image,image,Size(640,480));
    Process p(image);
    Mat kernel = (Mat_<int>(3,3) << -1,0,1,-2,0,2,-1,0,1);
    edge = p.edgeFilter(kernel)*0.2;
    convertScaleAbs(edge,edge);
    edge.convertTo(edge,CV_8UC3);
    edge = edge + image;
    //GaussianBlur(edge,edge,Size(3,3),1,1);

    imshow("edge",edge);
    imshow("image",image);
    waitKey(0);
}
void testBlurEdge(){
    Mat image =  imread("/home/feng/disk/learn/learn-opencv/data/a.jpg");
    Mat blur;
    resize(image,image,Size(640,480));
    Process p(image);
    blur = p.gaussianBlur(Size(15,15),1);
    convertScaleAbs(2*(image - blur),blur);
    blur.convertTo(blur,CV_8UC3);
    imshow("blur",blur);
    imshow("image",image);
    waitKey(0);
}

void testRoberts(){
    Mat image =  imread("/home/feng/disk/learn/learn-opencv/data/a.jpg");
    Mat e1,e2,edge;
    resize(image,image,Size(640,480));
    Process p(image);
    //p.addGaussianNoise(0,0.5);

    Mat k1 = (Mat_<int>(2,2) << 1,0,0,-1);
    Mat k2 = (Mat_<int>(2,2) << 0,1,-1,0);
    convertScaleAbs(p.edgeFilter(k1),e1);
    convertScaleAbs(p.edgeFilter(k2),e2);
    edge = e1+e2;
    edge.convertTo(edge,CV_8UC3);
    imshow("Roberts",edge);
    waitKey(0);
}

void testLaplace(){

    Mat image =  imread("/home/feng/disk/learn/learn-opencv/data/a.jpg");
    resize(image,image,Size(640,480));

    Process pss(image);
    pss.addGaussianNoise(0,0.5);
    Mat k = (Mat_<int>(3,3) << 0,1,0,1,-4,1,0,1,0);
    Mat laplace;
    convertScaleAbs(pss.edgeFilter(k),laplace);
    laplace.convertTo(laplace,CV_8UC3);
    imshow("laplace",laplace);

    //调用opencv版本
    Laplacian(image,image,CV_8UC3);
    convertScaleAbs(image,image);
    normalize(image,image,0,255,NORM_MINMAX);
    image.convertTo(image,CV_8UC3);
    imshow("image",image);
    waitKey(0);
}
void testLogDog(){
    Mat image =  imread("/home/feng/disk/learn/learn-opencv/data/a.jpg",IMREAD_GRAYSCALE);
    resize(image,image,Size(640,480));
    Process pss(image);
    pss.addGaussianNoise(0,0.2);
    Mat G1,G2;
    GaussianBlur(image,G1,Size(5,5),4);
    GaussianBlur(image,G2,Size(5,5),0.5);
    Mat k = (Mat_<int>(5,5) << 0,0,-1,0,0,0,-1,-2,-1,0,-1,-2,16,-2,-1,0,-1,-2,-1,0,0,0,-1,0,0);
    Mat e1,e2;
    convertScaleAbs(pss.edgeFilter(k),e1);
    convertScaleAbs(G2-G1,e2);
    normalize(e1, e1, 0, 255, NORM_MINMAX);
    normalize(e2, e2, 0, 255, NORM_MINMAX);
    e1.convertTo(e1,CV_8UC1);
    e2.convertTo(e2,CV_8UC1);
    imshow("edge1",e1);
    imshow("edge2",e2);
    waitKey(0);
}