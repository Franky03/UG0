#include <iostream>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int HueMin = 0, HueMax = 25, SatMin = 25, SatMax = 125, ValMin = 50, ValMax = 225;
cv::Mat frame;

float iAng(float pixela1, float pixelb1, float pixela2, float pixelb2, float centre1, float centre2)
{
 float DistanceA = std::sqrt(  (pixela1-centre1)*(pixela1-centre1) + (pixelb1-centre2)*(pixelb1-centre2) );
 float DistanceB = std::sqrt(  (pixela2-centre1)*(pixela2-centre1) + (pixelb2-centre2)*(pixelb2-centre2) );

 if(DistanceA < DistanceB)
 {
 float E1 = centre1 - pixela2;
 float E2 = centre2 - pixelb2;
 float F1 = pixela1 - pixela2;
 float F2 = pixelb1 - pixelb2;

 float A = std::acos( (F1*E1 + F2*E2) / ( std::sqrt(F1*F1+F2*F2) * std::sqrt(E1*E1+E2*E2) ) );

 A = A*180/CV_PI;

 return A;
 }
 else{
 float E1 = centre1 - pixela1;
 float E2 = centre2 - pixelb1;
 float F1 = pixela2 - pixela1;
 float F2 = pixelb2 - pixelb1;

 float A = std::acos( (F1*E1 + F2*E2) / ( std::sqrt(F1*F1+F2*F2) * std::sqrt(E1*E1+E2*E2) ) );

 A = A*180/CV_PI;

 return A;
 }
}

int main()
{
  cv::VideoCapture cap(0);
  const char* windowName = "Fingerprint";

  int MinIA = 200, MaxIA = 300, MinA = 180, MaxA = 359;

   while (1)
  {
      cap >> frame;
      cv::Mat hsv;
      cv::cvtColor(frame, hsv, COLOR_BGR2HSV);
      cv::inRange(hsv, cv::Scalar(HueMin, SatMin, ValMin), cv::Scalar(HueMax, SatMax, ValMax), hsv);
      if (cv::waitKey(30) >= 0) break;
      int blur = 5;
      int SizeOfElement = 5;
      cv::medianBlur(hsv, hsv, blur);
      cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * 5 + 1, 2 * 5 + 1), cv::Point(5, 5));
      cv::dilate(hsv, hsv, element);
      std::vector<std::vector<cv::Point> > contours;
      std::vector<cv::Vec4i> hierarchy;
      cv::findContours(hsv, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
      size_t ContourBig = 0;
      for (size_t i = 1; i < contours.size(); i++)
      {
          if (cv::contourArea(contours[i]) > cv::contourArea(contours[ContourBig]))
              ContourBig = i;
      }

      if (!contours.empty())
      {
          std::vector<std::vector<cv::Point> > hull(1);
          cv::convexHull(cv::Mat(contours[ContourBig]), hull[0], false);
          if (hull[0].size() > 2)
          {
              std::vector<int> hullIndexes;
              cv::convexHull(cv::Mat(contours[ContourBig]), hullIndexes, true);
              std::vector<cv::Vec4i> convexityDefects;
              cv::convexityDefects(cv::Mat(contours[ContourBig]), hullIndexes, convexityDefects);
              cv::Rect boundingBox = cv::boundingRect(hull[0]);
              cv::Point center = cv::Point(boundingBox.x + boundingBox.width / 2 , boundingBox.y  + boundingBox.height / 2);
              std::vector<cv::Point> validPoints;
              for (size_t i = 0; i < convexityDefects.size(); i++)
              {
                  cv::Point F1 = contours[ContourBig][convexityDefects[i][0]];
                  cv::Point F2 = contours[ContourBig][convexityDefects[i][1]];
                  cv::Point p3 = contours[ContourBig][convexityDefects[i][2]];
                  double angle = std::atan2(center.y - F1.y, center.x - F1.x) * 180 / CV_PI;
                  double inAngle = iAng(F1.x, F1.y, F2.x, F2.y, p3.x, p3.y);
                  if (angle > MinA - 180 && angle < MaxA - 180 && inAngle > MinIA - 180 && inAngle < MaxIA - 180)
                  {
                     validPoints.push_back(F1);
                  }
              }
              for (size_t i = 0; i < validPoints.size(); i++)
              {

                  cv::rectangle(frame, cv::Point(validPoints[i].x-18, 7 + validPoints[i].y-10),cv::Point(validPoints[i].x+18, 20+validPoints[i].y+30),cv::Scalar(0,255,0),2 );

              }
          }
      }
      cv::imshow(windowName, frame);
      if (cv::waitKey(30) >= 0) break;
  }
  return 0;
}