#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;

/*
 * Function that finds and circles the center of the palm.
 * @param:  inputImage -> greyscale distance transformed image.
            imgROI -> pointer to the webcam frame but with only region of interest.
            palmPoint -> vector of pair that stores the center of palm.
 * @return: Image containing the circle at the center of the palm.
 */
Mat createPalmPoint(const Mat *inputImage, const Mat* imgROI, std::pair<double, double> &palmPoint);

/*
 * Function that finds the inner circle and draws it with center from the center of the palm.
 * @param:  binaryImage -> threshold image with only binary pixels.
            palmPointImage -> image containing the center of the palm.
            palmPoint -> vector of pair that stores the center of the palm.
 * @return: void
 */
void findInnerCircle(Mat* binaryImage, Mat *palmPointImage, const std::pair<double, double>& palmPoint);

/*
 * Function that finds the contours of the hand.
 * @param:  binaryImage -> threshold image with only binary pixels.
            contours -> vector that stores all the contours detected in the image.
            maxContourIndex -> index of the contour containing the maximum value.
 * @return: void
 */
void findAllContours(Mat* binaryImage, std::vector<std::vector<Point>> &contours, int& maxContourIndex);


/*
 * Function that finds the hulls and defects from the image.
 * @param:  image -> region of interest image from webcam frame.
            contours -> vector that stores all the contours detected in the image.
            contourSize -> size of the contour vector.
            hull -> vector that stores all the hull points.
            hullPixels -> vector that stores all the points for each hull.
            defects -> vector that stores all the defect points.
            defectPixels -> vector that stores all the points for each defect.
            maxContourIndex -> index of the contour containing the maximum value.
 * @return: int -> total number of fingers detected
 */
int findHullsAndDefects( Mat * image,
                          std::vector<std::vector<Point>> &contours, 
                          const int &contourSize, 
                          std::vector<std::vector<int>> &hull, 
                          std::vector<std::vector<Point>> &hullPixels, 
                          std::vector<std::vector<Vec4i>> &defects, 
                          std::vector<std::vector<Point>> &defectPixels,
                          const int &maxContourIndex);


/*
 * Function that gets the value of the fingers and displays message to the user.
 * @param:  fingers -> Total number of fingers counted.
 * @return: string -> message to be displayed
 */
std::string getDisplayMessage(const int& fingers);