#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <unistd.h>
#include <SerialStream.h>
#include "PenDetect.h"

using namespace cv;
using namespace std;
using namespace LibSerial;

Mat img;
vector<vector<int>> newPoints;
vector<Scalar> myColorValues = {{10, 243, 59}}; // rgb 

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

void createNewCanvas(cv::Mat* img) {
    // Define o tamanho da nova imagem
    int canvasWidth = 800;
    int canvasHeight = 600;
    
    // Cria uma nova imagem vazia
    *img = Mat(canvasHeight, canvasWidth, CV_8UC3, cv::Scalar(255, 255, 255));
    
    // Exibe a nova janela
    cv::imshow("Janela", *img);
}

int main(void){ 
    VideoCapture cap(0); 
    PenDetect my_pen = PenDetect(&img);
    char tecla;
 
    float original_distance = 30.0;
    float Real_pen_width = 1.0; 
    

    // ler a imagem de referência primeiro e pegar a focal_length
    Mat reference_img = imread("./src/referencia.jpeg");
    my_pen.findColor(reference_img);
    my_pen.clearOldPoints();

    float pen_width_in_image = my_pen.getWidth();

    float focal_length = FocalLengthFinder(original_distance, Real_pen_width, pen_width_in_image);
    my_pen.resetUgoCoord();

    Mat img_aux;
    bool first = false;

    while(true){
        cap.read(img);
        flip(img, img, 1);  

        cv::Scalar corAzul(255, 0, 0, 200);
        // Desenha um retângulo azul no canto superior esquerdo
        //cv::rectangle(img, cv::Point(0, 0), cv::Point(50, 50), corAzul, -1);

        newPoints = my_pen.findColor(img);

        float coord_w = my_pen.getWidth();
        
        float distance_cam = Distance_Finder(Real_pen_width, focal_length, coord_w);
        
        // if(distance_cam < 30.0) cout << "ABAIXA" << endl;
        // else cout << "LEVANTA" << endl;
        
        my_pen.drawnOnCanvas(newPoints, myColorValues); 

        char c = (char)waitKey(10);
        
        if (c=='A' || c=='a'){
          my_pen.clearOldPoints();
          //my_pen.sendCoord("111");
        }
            
        string ugo_coord = my_pen.getUgoCoord(); 
        
        // try{
        //     my_pen.sendCoord(ugo_coord); 
        // } catch(const exception& e){
        //     cout << "";
        // }
        
        imshow("img", img);
        waitKey(1); 
        
    }

    return 0;
}


