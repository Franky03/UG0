#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <exception>
#include <unistd.h>
#include <math.h>
#include <SerialStream.h>

using namespace std;
using namespace cv;
using namespace LibSerial;

class PenDetect
{
    private:
        Mat *img;
        //SerialStream serial =  SerialStream("/dev/ttyUSB0");
        
        vector<vector<int>> newPoints;
        vector<vector<int>> myColors = {{61,95, 138, 255, 101, 255}}; // verde
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        float bound_rect_width=0;
        string ugo_coord;
        int last_x = 0, last_y = 0;
        float last_width = 0;

    public:

        PenDetect(Mat *img){ this->img = img; };

        void setColors(const vector<vector<int>>& colors){ this->myColors = colors; };

        Point getContours(Mat imgDil){
        
            findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); 
            drawContours(*img, contours, -1, Scalar(255,0,255), 2);

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

                    if(contours.size()>0){
                        
                        if(boundRect[i].width > boundRect[i].height){
                            this->bound_rect_width = boundRect[i].height;
                            if(bound_rect_width > 70){ bound_rect_width = last_width;}
                            else last_width = bound_rect_width;
                        }
                        else{
                            this->bound_rect_width = boundRect[i].width;
                            if(bound_rect_width > 70){ bound_rect_width = last_width;}
                            else last_width = bound_rect_width;

                        }
                        
                    }
                        
                    drawContours(*img, conPoly, i, Scalar(255, 0, 255), 2);
                    int newCoordinates[2] = {45, 24};
                    int ugoCoordX = (((float)conPoly[i][0].x / 640) * newCoordinates[0]);
                    int ugoCoordY = newCoordinates[1] - (((float)conPoly[i][0].y / 480) * newCoordinates[1]);
                    
                    if(ugoCoordX < 10 && ugoCoordY < 10){
                        ugo_coord = "0" + to_string(ugoCoordX) + "0" + to_string(ugoCoordY);
                    } else if(ugoCoordX < 10){
                        ugo_coord = "0" + to_string(ugoCoordX) + to_string(ugoCoordY);
                    } else if(ugoCoordY < 10){
                        ugo_coord = to_string(ugoCoordX) + "0" + to_string(ugoCoordY);
                    } else ugo_coord = to_string(ugoCoordX) + to_string(ugoCoordY);
                    
                
                }
            }
            return myPoint;
        };

        string getUgoCoord() { return this->ugo_coord; };
        void resetUgoCoord(){ this->ugo_coord = ""; };
        float getWidth(){ return this->bound_rect_width; };
        
        vector<vector<int>> findColor(Mat img, float dist=0){
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
                if(myPoint.x != 0 && dist < 30){
                    newPoints.push_back({myPoint.x, myPoint.y, i});
                }
            }
            return newPoints;
        };

        void drawnOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorVals, int color){
            for(auto i=0; i< newPoints.size(); i++){
                circle(*img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorVals[color], FILLED); //[newPoints[i][2]]
            }
        };

        void clearOldPoints(){ newPoints.clear(); };

        vector<vector<int>> getNewPoints(){ return newPoints; };

        // void sendCoord(string coord){

        //     if (!serial.good())
        //     {
        //         cerr << "Erro ao abrir a porta serial." << endl;
        //         exit(1);
        //     }

        //     // Exemplo de envio de dado para o Arduino
        //     serial.SetCharacterSize( CharacterSize::CHAR_SIZE_8 ) ;
        //     serial.SetBaudRate( BaudRate::BAUD_9600 ) ;
        //     serial.SetParity( Parity::PARITY_NONE ) ;
        //     serial.SetStopBits( StopBits::STOP_BITS_1 ) ;
        //     // colocar um threshold para que se a coordenada for no minimo 10 pixels diferente da ultima enviada, enviar
            
        //     int x = stoi(coord.substr(0,2));
        //     int y = stoi(coord.substr(2,2));
        //     int threshold = 1;
        //     if(x-threshold > last_x || x+threshold < last_x || y-threshold > last_y || y+threshold < last_y){
        //         last_x = x;
        //         last_y = y;
        //         serial << coord;
        //     }
            
            

        // }
};