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

void exibeMenu(){
    cout << "Pressione 'A' para começar a desenhar" << endl;
    cout << "Pressione 'N' para uma nova palavra" << endl;
    cout << "Pressione 'B' para mudar a cor" << endl;
    cout << "Pressione 'W' para mover o braço para cima" << endl;
    cout << "Pressione 'S' para mover o braço para baixo" << endl;
    cout << "Pressione 'ESC' para sair" << endl;
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
    string caminho = "";

    double startTime = cv::getTickCount(); // Inicia o cronômetro

    exibeMenu();


    while(true){
        cap.read(img);
        flip(img, img, 1);  

        cv::rectangle(img, cv::Point(150, 5), cv::Point(500, 50), Scalar(255,255,255), -1);
        cv::putText(img, word , cv::Point((img.rows / 2)+12, 40), cv::FONT_HERSHEY_DUPLEX, 1.2, cv::Scalar(0, 0, 0), 4, false);

        // Desenha um retângulo azul no canto superior esquerdo
        double tickFrequency = cv::getTickFrequency(); // Obter a frequência dos ticks
        
        double currentTime = ((double)cv::getTickCount() - startTime) / tickFrequency; // Calcular o tempo atual em segundos
        int minutes = static_cast<int>(currentTime / 60);
        int seconds = static_cast<int>(currentTime) % 60;
        int centiseconds = static_cast<int>((currentTime - static_cast<int>(currentTime)) * 100);

        // Cria uma string com o tempo formatado
        std::string timeString = std::to_string(minutes) + ":" +
                                 std::to_string(seconds) + ":" +
                                 std::to_string(centiseconds);

        // Escreve o tempo no canto inferior direito
        // coloca um retângulo embaixo do texto para que ele fique mais legível
        cv::rectangle(img, cv::Point(10, img.rows - 50), cv::Point(150, img.rows - 5), Scalar(255,255,255), -1);
        cv::putText(img, timeString, cv::Point(10, img.rows - 10), cv::FONT_HERSHEY_DUPLEX, 1.2, cv::Scalar(0, 0, 0), 4, false);

        // quando o tempo chegar em 1 minuto, o tempo para de contar 

        char c = (char)waitKey(10);

        if(minutes==1){
            system("mplayer /home/kaiky/Downloads/timeout.mp3 > /dev/null 2>&1 &"); 

            system("clear");
            cout << "Tempo esgotado!" << endl;
            cout << "A palavra era: " << word << endl;
            cout << "Pressione 'N' para uma nova palavra" << endl;

            while(waitKey(1) != 'n'){
                sleep(1);
            }
            c = 'n';
            my_pen.clearOldPoints();
            //reinicia o cronômetro
            startTime = cv::getTickCount();

        }

        float coord_w = my_pen.getWidth();
        float distance_cam = Distance_Finder(Real_pen_width, focal_length, coord_w);

        newPoints = my_pen.findColor(img, distance_cam);
        
        my_pen.drawnOnCanvas(newPoints, myColorValues, color); 
        
        if (c=='A' || c=='a'){ 
            my_pen.clearOldPoints();
            //my_pen.sendCoord("111");
        }
        if (c == 'N' || c=='n'){

            string new_word = getRandomWord("./src/palavras.txt");
            while(new_word == word){
                new_word = getRandomWord("./src/palavras.txt");
            }
            word = new_word;
            rectangle(img, cv::Point(150, 5), cv::Point(500, 50), Scalar(255,255,255), -1);
            putText(img, word , cv::Point((img.rows / 2)+12, 40), cv::FONT_HERSHEY_DUPLEX, 1.2, cv::Scalar(0, 0, 0), 4, false);
        }
        if(c=='B' || c=='b'){
            if(color <= myColorValues.size() - 1)
                color++;
            else
                color = 0;
        }

            
        string ugo_coord = my_pen.getUgoCoord();  

        //cout << ugo_coord;
        if(c=='W' || c=='w'){
            ugo_coord += "1\n"; 
        }
        if(c=='S' || c=='s'){ 
            ugo_coord += "0\n"; 
        }
        //verificar se o ultimo digito da string é um \n e se não for, adicionar
        if(ugo_coord[ugo_coord.size() - 1] != '\n'){
            ugo_coord += '\n';
        }
        // try{
        //     my_pen.sendCoord(ugo_coord);  
        // } catch(const exception& e){
        //     cout << "";
        // }
        
        imshow("img", img);
        // se ele apertar esc, sai do programa
        if(c==27){
            break;
        }
        
    }

    return 0;
}

 
