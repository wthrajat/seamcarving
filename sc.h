#ifndef SEAMCARVINGCOMP665156
#define SEAMCARVINGCOMP665156

#include <opencv2/opencv.hpp>
 
// the function you need to implement - by defaiult it calls seam_carving_trivial
bool validation(int rows, int cols, int newWidth,int newHeight);

bool seam_carving_trivial(cv::Mat& inImage,cv::Mat& outImage, int newWidth, int newHeight);

double findPixelEnergy(cv::Mat& inImage, int i,int j);

//int* findHorizontalSeam(cv::Mat& inImage);

bool reduce_horizontal_seam_trivial(cv::Mat& inImage, cv::Mat& outImage);

//int* findVerticalSeam(cv::Mat& inImage);

bool reduce_vertical_seam_trivial(cv::Mat& in_image, cv::Mat& out_image);


#endif