#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

cv::Mat getImageOfHistogram(const cv::Mat& image);
void colorReduce(const cv::Mat& inputImage, cv::Mat& outputImage, int div = 64);

#endif // MAINWINDOW_H

