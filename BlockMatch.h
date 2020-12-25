//
// Created by feng on 2020/12/25.
//

#ifndef IMAGEPROCESSING_BLOCKMATCH_H
#define IMAGEPROCESSING_BLOCKMATCH_H


#include <opencv2/opencv.hpp>
#include <iostream>
class BlockMatch{
    typedef cv::Mat FMat;
public:
    BlockMatch(FMat& m,CvSize b,CvSize s,CvPoint p):blockSize(b),searchSize(s),center(p){
        assert(m.rows >= b.width+s.width+p.x &&
               m.cols >= b.height+b.height+p.y &&
               p.x - b.width - s.width >= 0 &&
               p.y - b.width - s.height >= 0);
        matchRoi = m(cv::Rect2i(p.x - b.width/2,p.y - b.width/2,b.width,b.height));
    }
    //计算以center为中心，2×blockSize+1为长宽区域的像素偏差
    long match_error(const FMat& refImage){
        return match_error(refImage,center);

    }
    long match_error(const FMat& refImage,const CvPoint& center);

    //TSS三步法搜索最佳匹配点
    CvPoint tssSearch(const FMat& refImage){
        long err = match_error(refImage);
        return tssSearch(refImage,center,searchSize,err);
    }
    CvPoint fullSearch(const FMat& refImage);
    ~BlockMatch()= default;
private:
    CvPoint tssSearch(const FMat& refImage,CvPoint center,CvSize searchSize,long minerror);

private:
    FMat matchRoi;
    const CvSize blockSize;
    const CvSize searchSize;
    const CvPoint center;


};

void testBlockMatch();


#endif //IMAGEPROCESSING_BLOCKMATCH_H
