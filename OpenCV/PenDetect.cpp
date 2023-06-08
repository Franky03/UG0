#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <SerialStream.h>
#include <random>
#include "PenDetect.h"

using namespace cv;
using namespace std;
using namespace LibSerial;

Mat img;
vector<vector<int>> newPoints;
vector<Scalar> myColorValues = {{10, 243, 59}, 
                                {10, 20, 59},
                                {0, 0, 255}, //vermelho
                                {255, 0, 0}, //azul
                                {0, 255, 0}, //verde
                                {0, 255, 255}, //amarelo
                                {255, 255, 0}, //ciano
                                {255, 0, 255}, //magenta
                                {255, 255, 255} //branco
}; // rgb 

int color = 0;  

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

string getRandomWord(const char* filename){
    ifstream file(filename);

     if (!file.is_open()) {
        std::cout << "Erro ao abrir o arquivo.\n";
        return "";
    }

    std::vector<std::string> words;
    std::string word;
    
    while (file >> word) {
        words.push_back(word);
    }

    if (words.empty()) {
        std::cout << "O arquivo está vazio.\n";
        return "";
    }
    cout << "Tamanho do vetor de palavras: " << words.size() << endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, words.size() - 1);

    std::string random_word = words[dis(gen)];
    

    return random_word;
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

    //pegar uma palavra aleatória da lista de palavras em ./src/palavras.txt
    string word = getRandomWord("./src/palavras.txt");    

    while(true){
        cap.read(img);
        flip(img, img, 1);  

        cv::putText(img, word , cv::Point((img.rows / 2)+12, 35), cv::FONT_HERSHEY_DUPLEX, 1.2, cv::Scalar(0, 0, 0), 4, false);

        // Desenha um retângulo azul no canto superior esquerdo
        //cv::rectangle(img, cv::Point(0, 0), cv::Point(50, 50), corAzul, -1);

        float coord_w = my_pen.getWidth();
        float distance_cam = Distance_Finder(Real_pen_width, focal_length, coord_w);
        cout << distance_cam << endl;

        newPoints = my_pen.findColor(img, distance_cam);
        
        my_pen.drawnOnCanvas(newPoints, myColorValues, color); 

        char c = (char)waitKey(10);
        
        if (c=='A' || c=='a'){ 
          my_pen.clearOldPoints();
          //my_pen.sendCoord("111");
        }
        if(c=='B' || c=='b'){
            if(color <= myColorValues.size() - 1)
                color++;
            else
                color = 0;
        }

            
        string ugo_coord = my_pen.getUgoCoord();  
 
        if(distance_cam >= 30){
            ugo_coord += "1\n";
        } else {
            ugo_coord += "0\n";
        }

        cout << ugo_coord;
        
        try{ 
            my_pen.sendCoord(ugo_coord); 
        } catch(const exception& e){
            cout << "";
        }
        
        imshow("img", img);
        waitKey(1); 
        
    }

    return 0;
}


