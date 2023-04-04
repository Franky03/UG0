#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;

/*
 * Function that gets the webcam frame and finds the HSV threshold image for it.
 * @param:  webcamFrame -> greyscale distance transformed image.
            maskImage -> image to use for bitwise_and.
 * @return: Image containing the HSV threshold image.
 */
Mat createHSVThresholdImage(Mat webcamFrame, Mat maskImage);

/*
 * Function that creates a binary threshold image of inputImage.
 * @param:  inputImage -> ROI of webcam frame.
 * @return: binary image of the webcam frame.
 */
Mat createBinaryThresholdImage(Mat inputImage);

/*
 * Function that creates the distance transformed image.
 * @param:  binaryImage -> binary image of the webcam frame.
 * @return: Distance transform image of the webcam frame.
 */
Mat createDistanceTransformImage(Mat binaryImage);