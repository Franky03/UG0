#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "thresholdHelpers.h"

/*
 * Function that gets the webcam frame and finds the HSV threshold image for it.
 * @param:  webcamFrame -> greyscale distance transformed image.
						maskImage -> image to use for bitwise_and.
 * @return: Image containing the HSV threshold image.
 */
Mat createHSVThresholdImage(Mat webcamFrame, Mat maskImage) {

	Mat hsvReturn;
	cvtColor(webcamFrame, maskImage, COLOR_BGR2HSV);

	// outlier, red color
	std::vector<int> upperBound = { 30, 255, 255 };
	std::vector<int> lowerBound = { 0, 58, 50 };

	// add hsv mask to threshold image
	inRange(maskImage, lowerBound, upperBound, maskImage);
	bitwise_and(webcamFrame, webcamFrame, hsvReturn, maskImage);

  return hsvReturn;
}


/*
 * Function that creates a binary threshold image of inputImage.
 * @param:  inputImage -> ROI of webcam frame.
 * @return: binary image of the webcam frame.
 */
Mat createBinaryThresholdImage(Mat inputImage) {
	
	Mat binaryImage;
	
	// convert to greyscale 
	cvtColor(inputImage, binaryImage, COLOR_RGB2GRAY);

	//blur the image
	GaussianBlur(binaryImage, binaryImage, Size(19, 19), 0.0, 0);
	
	// threshold the value of the greyscale image
	threshold(binaryImage, binaryImage, 0, 255, THRESH_BINARY + THRESH_OTSU);
	
	return binaryImage;
}

/*
 * Function that creates the distance transformed image.
 * @param:  binaryImage -> binary image of the webcam frame.
 * @return: Distance transform image of the webcam frame.
 */
Mat createDistanceTransformImage(Mat binaryImage) {
	Mat distanceTransformImage;

	distanceTransform(binaryImage, distanceTransformImage, DIST_L2, 5);
	normalize(distanceTransformImage, distanceTransformImage, 0, 1.0, NORM_MINMAX);
	
	return distanceTransformImage;
}