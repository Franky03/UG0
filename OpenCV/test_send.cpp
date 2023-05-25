#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <SerialStream.h>

using namespace LibSerial; 
using namespace std;

int main() {
    // Seu código OpenCV para capturar dados
    SerialStream serial("/dev/ttyUSB0");  // Substitua "/dev/ttyUSB0" pela porta serial correta
    
    if (!serial.good())
    {
        cerr << "Erro ao abrir a porta serial." << endl;
        return 1;
    }
    // Exemplo de envio de dado para o Arduino
    serial.SetCharacterSize( CharacterSize::CHAR_SIZE_8 ) ;
    serial.SetBaudRate( BaudRate::BAUD_9600 ) ;
    serial.SetParity( Parity::PARITY_NONE ) ;
    serial.SetStopBits( StopBits::STOP_BITS_1 ) ;

    serial << "0232" << endl;
   
    cout << "Hello World" << endl;

    return 0;
}