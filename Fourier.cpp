//
// Created by feng on 2020/12/25.
//

#include "Fourier.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace cv;
using namespace std;

void FourierTransform::dftTransform(){

    Mat& I = image;
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
    dft(complexI, complexI);            // this way the result may fit in the source matrix

    split(complexI, planes);            // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    this->re = planes[0];
    this->im = planes[1];

}

Mat FourierTransform::udftTransform(){
    assert(this->re.data && this->im.data);
    vector<Mat> plane = {this->re,this->im};
    Mat imgComplex;
    merge(plane,imgComplex);
    idft(imgComplex,imgComplex);
    Mat m[] = {Mat::zeros(re.size(),CV_32F),Mat::zeros(im.size(),CV_32F)};
    split(imgComplex,m);
    Mat &img = m[0];
    magnitude(m[0],m[1],img);
    normalize(img, img, 0, 255, NORM_MINMAX);
    img.convertTo(img,CV_8UC1);

    return img;
}


Mat FourierTransform::amplitudeSpectrum(){
    if(this->re.data == nullptr || this->im.data == nullptr)
    {
        return Mat();
    }
    Mat magI;
    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    magnitude(this->re,this->im,magI);
    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    magI = crop(magI);
    normalize(magI, magI, 0, 255, CV_MINMAX); // Transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).
    magI.convertTo(magI,CV_8UC1);
    return magI;

}

Mat FourierTransform::phaseSpectrum(){
    if(this->re.data == nullptr || this->im.data == nullptr)
    {
        return Mat();
    }
    Mat angI;

    //arctan(Im/Re)
    phase(this->re,this->im,angI);
    angI = crop(angI);
    normalize(angI, angI, 0, 255, CV_MINMAX); // Transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).
    angI.convertTo(angI,CV_8UC1);
    return angI;

}

void FourierTransform::tocart(const Mat& angle,const Mat& mag){
    polarToCart(mag,angle,this->re,this->im);
}

void FourierTransform::topolar(Mat& angle,Mat& mag){
    cartToPolar(this->re,this->im,mag,angle);
}



//私有方法
Mat& FourierTransform::crop(Mat& magI)
{
    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
    return magI;
}


//测试交换两幅图像的相位和幅度值，分析相位和幅度谱对图像的影响
void testDft()
{
    Mat ia,ib;
    ia = imread("/home/feng/disk/learn/learn-opencv/a.jpg");
    ib = imread("/home/feng/disk/learn/learn-opencv/b.jpg");

    cv::resize(ia,ia,cv::Size(640,480));
    cv::resize(ib,ib,cv::Size(640,480));

    std::vector<Mat> cha,chb;
    cv::split(ia,cha);
    cv::split(ib,chb);

    FourierTransform fa(cha[0]);
    FourierTransform fb(chb[0]);
    fa.dftTransform();
    fb.dftTransform();

    Mat ag,bg,am,bm;
    fa.topolar(ag,am);
    fb.topolar(bg,bm);

    fa.tocart(ag,bm);
    fb.tocart(bg,am);
    imshow("oia",cha[0]);
    imshow("oib",chb[0]);
    ia = fa.udftTransform();
    ib = fb.udftTransform();

    imshow("ia",ia);
    imshow("ib",ib);

    waitKey(0);
}