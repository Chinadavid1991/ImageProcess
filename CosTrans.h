//
// Created by feng on 2020/12/25.
//

#ifndef IMAGEPROCESSING_COSTRANS_H
#define IMAGEPROCESSING_COSTRANS_H


#include <opencv2/opencv.hpp>



class CosTransform{
    typedef cv::Mat FMat;
public:
    explicit CosTransform(FMat& m):image(m){};
    FMat dctTransform();
    FMat& compress(FMat& I,int stepx=8,int stepy=8);
    static FMat& uncompress(FMat& I,int stepx=8,int stepy=8);
private:
    FMat image;
};


void testDct();


#endif //IMAGEPROCESSING_COSTRANS_H
