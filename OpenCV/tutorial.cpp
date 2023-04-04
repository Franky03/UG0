#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Basic functions


void basicFunctions(void){
    string path = "../images/Ugin.jpg";
    //VideoCapture cap(-1);
    Mat img = imread(path);
    Mat imgGray, imgBlur, imgCanny, imgDia, imgErode;

    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(img, imgBlur, Size(7,7), 5,0);
    Canny(imgBlur, imgCanny, 25, 75);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
    dilate(imgCanny, imgDia, kernel);
    erode(imgDia, imgErode, kernel);

    imshow("Image", img);
    imshow("ImageBlur", imgBlur);
    imshow("ImageCanny", imgCanny);
    imshow("ImageDilate", imgDia);
    imshow("ImageErode", imgErode);


    // while(true){
    //     cap.read(img);
    //     imshow("Image", img);
    //     waitKey(1);
    // }

    waitKey(0);
}

// Resize and crop

void resizeAndCrop(void){
    string path = "../images/Ugin.jpg";
    Mat img = imread(path);
    Mat imgResize, imgCrop;
    
    //cout << img.size() << endl;

    resize(img, imgResize, Size(), 0.5, 0.5);

    Rect roi(700, 300, 300, 250);
    
    imgCrop = img(roi);
    
    imshow("Image", img);
    imshow("ImageRsz", imgResize);
    imshow("ImageCrop", imgCrop);

    // while(true){
    //     cap.read(img);
    //     imshow("Image", img);
    //     waitKey(1);
    // }

    waitKey(0);

}

// Drawing shapes 

int drawInImage(){
    // Blank Image
    Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));
    circle(img, Point(256,256), 155, Scalar(0,0,255), FILLED);
    rectangle(img, Point(130, 226), Point(382, 286), Scalar(255,255,255), FILLED);
    line(img, Point(130, 296), Point(382, 296), Scalar(255,255,255), 2);
    putText(img, "HELLO CV2", Point(137, 262), FONT_HERSHEY_DUPLEX,0.75, Scalar(0,69,255), 2);
    imshow("Image", img);
    

    waitKey(0);
    
    return 0;
}

// Warp perspective
int warp(void){
    float w = 250, h=350;
    Mat matrix, imgWarp;

    string path = "./src/cards.jpg";
    Mat img = imread(path);
    Point2f src[4] = {{529,142}, {771,190}, {405,395}, {674,457}}; // source
    Point2f dst[4] = {{0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w,h}}; // destination

    matrix = getPerspectiveTransform(src , dst);
    warpPerspective(img, imgWarp, matrix, Point(w,h));

    for(int i = 0; i<4; i++){
        circle(img, src[i], 10, Scalar(0,0,255), FILLED);
    }

    imshow("Image", img);
    imshow("ImageWarp", imgWarp);

    waitKey(0);

    return 0;
}
//Color detection
int main(void){
    string path = "./src/shapes.png";
    VideoCapture cap(-1);
    Mat img;
    
    Mat imgHSV, mask; // HSV - Hue Saturation Value  Hue: 0-179 Saturation: 0-255 Value: 0-255
    

    int hmin = 0, smin =  0, vmin = 0;
    int hmax = 255, smax = 255, vmax = 255;

    namedWindow("Trackbars", (640, 200));
    createTrackbar("Hue Min", "Trackbars", &hmin, 179);
    createTrackbar("Hue Max", "Trackbars", &hmax, 179);
    createTrackbar("Sat Min", "Trackbars", &smin, 255);
    createTrackbar("Sat Max", "Trackbars", &smax, 255);
    createTrackbar("Val Min", "Trackbars", &vmin, 255);
    createTrackbar("Val Max", "Trackbars", &vmax, 255);



    while (true)
    {
        cap.read(img);
        cvtColor(img, imgHSV, COLOR_BGR2HSV);
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);

        inRange(imgHSV, lower, upper, mask);
        
        imshow("Image", img);
        imshow("ImageHSV", imgHSV);
        imshow("ImageMask", mask);
        cout << hmin << " " << hmax << " " << smin << " " << smax << " " << vmin << " " << vmax << endl;    
        waitKey(1);
    }
    
    return 0;
}

void getContours(Mat imgDil, Mat img){
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); 
    //drawContours(img, contours, -1, Scalar(255,0,255), 2);

    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    
    // ...
    //adicionando um filtro para as áreas, para eliminar os ruídos
    for(int i = 0; i< contours.size(); i++){
        int area = contourArea(contours[i]);
        cout << area << endl;
        string objectType;

        if(area > 1000)
        {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            cout << conPoly[i].size() << endl;

            boundRect[i] = boundingRect(conPoly[i]);

            int objCor = (int)conPoly[i].size();
            if(objCor == 3){ objectType = "Tri";}
            else if(objCor == 4) {
                float aspRatio  = (float)boundRect[i].width / (float)boundRect[i].height;
                cout << aspRatio << endl;
                if(aspRatio > 0.95 && aspRatio < 1.05) {objectType = "Square";}
                else { objectType = "Rect";}
            }
            else if(objCor > 4) { objectType = "Circle";}

            drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0,255,0), 5);
            putText(img, objectType, {boundRect[i].x, boundRect[i].y - 5}, FONT_HERSHEY_PLAIN, 1, Scalar(0,69,255), 2);;


        }
    }

}

int shapeDetec(void){
    
    Mat img;
    VideoCapture cap(-1);

    Mat imgGray, imgBlur, imgCanny, imgDiL, imgErode;
    // Preprocessing
    
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3));
      

    // Contours
    while(true){
        cap.read(img);
        cvtColor(img, imgGray, COLOR_BGR2GRAY);
        GaussianBlur(imgGray, imgBlur, Size(7,7), 5,0);
        Canny(imgBlur, imgCanny, 25, 75);
        dilate(imgCanny, imgDiL, kernel); 

        getContours(imgCanny, img);

        imshow("Image", img);
        waitKey(1);
    }

    //getContours(imgDiL, img);
    //imshow("Image", img);
    // imshow("ImageG", imgGray);
    // imshow("ImageB", imgBlur);
    // imshow("ImageC", imgCanny);
    // imshow("ImageD", imgDiL);

    //waitKey(0);
    return 0;
}

// Face detection

int faceDetec(void){
    string path = "./src/reference_image.png";
    //VideoCapture cap(-1);
    Mat img = imread(path);
    CascadeClassifier faceCascade;
    faceCascade.load("./src/haarcascade_frontalface_default.xml");
    if(faceCascade.empty()) {cout << "XML file not loaded" << endl; return -1;}

    vector<Rect> faces;
    faceCascade.detectMultiScale(img, faces, 1.1, 10);
    
    for(int i=0; i<faces.size(); i++){
        rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255),3);
    }

    
    imshow("Image", img);


    // while(true){
    //     cap.read(img);
    //     imshow("Image", img);
    //     waitKey(1);
    // }

    waitKey(0);

    return 0;
}