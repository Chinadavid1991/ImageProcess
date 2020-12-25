//
// Created by feng on 2020/12/25.
//

#ifndef IMAGEPROCESSING_FOURIER_H
#define IMAGEPROCESSING_FOURIER_H


#include <opencv2/opencv.hpp>
#include <vector>

class FourierTransform
{
private:
    typedef cv::Mat FMat;
public:
    FourierTransform(FMat& m):image(m){};
    void dftTransform();
    FMat udftTransform();
    FMat amplitudeSpectrum();
    FMat phaseSpectrum();


    void tocart(const FMat& angle,const FMat& mag);
    void topolar(FMat& angle,FMat& mag);
    void setReal(const FMat& r){
        re = r;
    }
    void setImag(const FMat& i){
        im = i;
    }
    FMat angle() const{
        FMat angI;
        cv::phase(this->re,this->im,angI);
        return angI;
    }
    FMat amplitude() const{
        FMat magI;
        cv::magnitude(this->re,this->im,magI);
        return magI;
    }

    ~FourierTransform() {};
private:
    FMat& crop(FMat& magI);
private:
    FMat image;
    FMat re;
    FMat im;

};

void testDft();

#endif //IMAGEPROCESSING_FOURIER_H
