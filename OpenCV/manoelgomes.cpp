// 158 170 65 255 166 255
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;
                    //        hmin, hmax, smin, smax, vmin, vmax
vector<vector<int>> myColors = {{158, 170, 65, 255, 166, 255}}; // pink
vector<Scalar> myColorValues = {{201, 107, 143}}; // rgb

Mat img;
vector<vector<int>> newPoints;

Point getContours(Mat imgDil){
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); 
    //drawContours(img, contours, -1, Scalar(255,0,255), 2);

    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    Point myPoint(0,0);
    
    // ...
    //adicionando um filtro para as áreas, para eliminar os ruídos
    for(int i = 0; i< contours.size(); i++){
        int area = contourArea(contours[i]);
        
        string objectType;

        if(area > 100)
        {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            boundRect[i] = boundingRect(conPoly[i]);
            myPoint.x = boundRect[i].x + boundRect[i].width / 2;
            myPoint.y = boundRect[i].y;

            drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
            int newCoordinates[2] = {50, 40};
            int ugoCoordX = (((float)conPoly[i][0].x / 640) * newCoordinates[0]);
            int ugoCoordY = (((float)conPoly[i][0].y / 480) * newCoordinates[1]);

            cout << ugoCoordX << " " << ugoCoordY << endl;
           
        }
    }
    return myPoint;

}

vector<vector<int>> findColor(Mat img){
    Mat imgHSV;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    for(auto i=0; i<myColors.size(); i++){
        Scalar lower(myColors[i][0], myColors[i][2], myColors[i][4]);
        Scalar upper(myColors[i][1], myColors[i][3], myColors[i][5]);

        Mat mask;
        Mat imgDil;
        Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
        inRange(imgHSV, lower, upper, mask);
        //erode(mask, imgDil, kernel);
        //imshow(to_string(i), mask);
        Point myPoint = getContours(mask);
        if(myPoint.x != 0){
            newPoints.push_back({myPoint.x, myPoint.y, i});
        }
    }
    return newPoints;
}

void drawnOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorVals){
    for(auto i=0; i< newPoints.size(); i++){
        circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorVals[newPoints[i][2]], FILLED);
    }
}


int main(void){
    VideoCapture cap(-1);

    while(true){
        cap.read(img);
        flip(img, img, 1);
        newPoints = findColor(img);
        drawnOnCanvas(newPoints, myColorValues);

        imshow("img", img);
        waitKey(1);
        
    }

    return 0;
}

