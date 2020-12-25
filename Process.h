//
// Created by feng on 2020/12/25.
//

#ifndef IMAGEPROCESSING_PROCESS_H
#define IMAGEPROCESSING_PROCESS_H


#include <opencv2/opencv.hpp>

class Process{
    using Mat = cv::Mat;
public:
    explicit Process(Mat& m):image(m){}
    Mat edgeFilter(const Mat& kernel) const;
    Mat gaussianBlur(const cv::Size& s,double sigmaXY) const;

    void addSaltNoise(int n);
    void addGaussianNoise(double mu = 2, double sigma = 1);
    const Mat& getImage() const{
        return image;
    }
private:
    static double generateGaussianNoise(double mu, double sigma);
private:
    Mat image;

};
void testFilterEdge();
void testBlurEdge();
void testRoberts();
void testLaplace();
void testLogDog();


#endif //IMAGEPROCESSING_PROCESS_H
