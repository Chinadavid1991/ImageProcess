//
// Created by feng on 2020/12/25.
//

#include "BlockMatch.h"
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;


long BlockMatch::match_error(const Mat& refImage,const CvPoint& center){

    Mat refRoi = refImage(cv::Rect2i(center.x-blockSize.width/2,center.y-blockSize.height/2,blockSize.width,blockSize.height));
    Mat df;
    cv::absdiff(refRoi,matchRoi,df);
    long sum = 0;
    for(int ix = 0;ix < df.cols;++ix)
    {
        for(int iy = 0;iy < df.rows;++iy)
        {
            for(int c : {0,1,2}){
                sum += df.at<cv::Vec3b>(iy,ix)[c];
            }
        }
    }
    return sum;

}
CvPoint BlockMatch::fullSearch(const Mat& refImage){
    long err = match_error(refImage);
    CvPoint ct = center;
    for(int ix = center.x-searchSize.width;abs(ix - center.x) <= searchSize.width;++ix)
    {
        for(int iy = center.y-searchSize.height;abs(iy - center.y) <= searchSize.height;++iy)
        {
            CvPoint c = CvPoint(ix,iy);
            long e = match_error(refImage,c);
            if(e < err){
                err = e;
                ct = c;
            }
        }
    }
    return ct;
}

CvPoint BlockMatch::tssSearch(const Mat& refImage,CvPoint center,CvSize searchSize,long minerror){
    if(searchSize.height <= 1 || searchSize.width <= 1){
        return center;
    }
    for(int ix : {-1,0,1})
    {
        for(int iy : {-1,0,1})
        {
            if(ix == 0 && iy == 0)
            {
                continue;
            }
            CvPoint c = CvPoint(center.x+searchSize.width*ix,center.y+searchSize.height*iy);
            long e = match_error(refImage,c);
            if(e < minerror){
                minerror = e;
                center = c;
            }
        }
    }

    searchSize.height /= 2;
    searchSize.width /= 2;
    return tssSearch(refImage,center,searchSize,minerror);
}

void testBlockMatch()
{
    Mat src = imread("/home/feng/disk/learn/learn-opencv/data/a.jpg");
    Mat ref = src;
    resize(src,src,Size(960,1280));
    resize(ref,ref,Size(960,1280));
    Rect2i r_ori,r_ref;
    r_ori.x = 100;
    r_ori.y = 100;
    r_ori.width = 800;
    r_ori.height = 800;
    r_ref.x = r_ori.x+15;
    r_ref.y = r_ori.y+15;
    r_ref.width = r_ori.width;
    r_ref.height = r_ori.height;

    src = src(r_ori);
    ref = ref(r_ref);
    BlockMatch b(src,CvSize(10,10),CvSize(9,9),CvPoint(420,150));
    CvPoint mv = b.tssSearch(ref);

    cout << mv.x << "," << mv.y << endl;
}