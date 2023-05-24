// 158 170 65 255 166 255
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <unistd.h>
#include <SerialStream.h>

using namespace cv;
using namespace std;
using namespace LibSerial;
                    //        hmin, hmax, smin, smax, vmin, vmax
                    // verde : {52,73, 57,228, 31,240}
vector<vector<int>> myColors = {{158, 170, 65, 255, 166, 255} }; // pink
vector<Scalar> myColorValues = {{201, 107, 143}}; // rgb

Mat img;
vector<vector<int>> newPoints;

float orginal_distnace = 30.1;
float Real_pen_width = 1.0; 
float pen_width_in_image = 0.0;

Mat reference_img = imread("./src/referencia.jpeg");

// Função para calcular a distância focal
float FocalLengthFinder(float Measured_Distance, float Real_Width, float Width_In_Image) {

    float Focal_length = (Measured_Distance * Width_In_Image) / Real_Width;
    return Focal_length;
}

// Função para calcular a distância de um contorno em relação à câmera
float Distance_Finder(float  Real_width, float Focal_Length, float Width_in_Image) {
    float distance = (Real_width * Focal_Length) / Width_in_Image;
    return distance;
}

void sendCoord(string coord){
    SerialStream serial("/dev/ttyUSB0");

    if (!serial.good())
    {
        cerr << "Erro ao abrir a porta serial." << endl;
        exit(1);
    }

    // Exemplo de envio de dado para o Arduino
    serial.SetCharacterSize( CharacterSize::CHAR_SIZE_8 ) ;
    serial.SetBaudRate( BaudRate::BAUD_9600 ) ;
    serial.SetParity( Parity::PARITY_NONE ) ;
    serial.SetStopBits( StopBits::STOP_BITS_1 ) ;

    serial << coord;

}

Point getContours(Mat imgDil){
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    //findContours(reference_img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    

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
            int ugoCoordY = newCoordinates[1] - (((float)conPoly[i][0].y / 480) * newCoordinates[1]);
            
            float imageWidth = conPoly[i][0].width;
            
            //cout << distance << endl;
            string coord = to_string(ugoCoordX) + " " + to_string(ugoCoordY) + "\n";
            try{
                sendCoord(coord);
            }
            catch(const exception& e){
                cout << e.what() << endl;
            }
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
    VideoCapture cap(0);

    while(true){
        cap.read(img);
        flip(img, img, 1);
        newPoints = findColor(img);
        //drawnOnCanvas(newPoints, myColorValues);

        imshow("img", img);
        waitKey(1);
        
    }

    return 0;
}

