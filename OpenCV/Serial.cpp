#define STRICT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Tserial.h"

/* -------------------------------------------------------------------- */
/* -------------------------    Tserial   ----------------------------- */
/* -------------------------------------------------------------------- */
Tserial::Tserial()
{
    parityMode       = spNONE;
    port[0]          = 0;
    rate             = 0;
    serial_handle    = INVALID_HANDLE_VALUE;
}

/* -------------------------------------------------------------------- */
/* --------------------------    ~Tserial     ------------------------- */
/* -------------------------------------------------------------------- */
Tserial::~Tserial()
{
    if (serial_handle!=INVALID_HANDLE_VALUE)
        CloseHandle(serial_handle);
    serial_handle = INVALID_HANDLE_VALUE;
}
/* -------------------------------------------------------------------- */
/* --------------------------    disconnect   ------------------------- */
/* -------------------------------------------------------------------- */
void Tserial::disconnect(void)
{
    if (serial_handle!=INVALID_HANDLE_VALUE)
        CloseHandle(serial_handle);
    serial_handle = INVALID_HANDLE_VALUE;
}
/* -------------------------------------------------------------------- */
/* --------------------------    connect      ------------------------- */
/* -------------------------------------------------------------------- */
int  Tserial::connect          (char *port_arg, int rate_arg, serial_parity parity_arg)
{
    int fd;
    struct termios tty;

    fd = open(port_arg, O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("Erro ao abrir a porta serial");
        return -1;
    }

    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0) {
        perror("Erro ao obter os atributos da porta serial");
        close(fd);
        return -1;
    }

    switch (parity_arg) {
        case 0:
            tty.c_cflag &= ~PARENB; // Desativa paridade
            tty.c_cflag &= ~CSTOPB; // 1 stop bit
            break;
        case 1:
            tty.c_cflag |= PARENB;  // Ativa paridade
            tty.c_cflag &= ~PARODD; // Paridade par
            tty.c_cflag &= ~CSTOPB; // 1 stop bit
            break;
        case 2:
            tty.c_cflag |= PARENB;  // Ativa paridade
            tty.c_cflag |= PARODD;  // Paridade ímpar
            tty.c_cflag &= ~CSTOPB; // 1 stop bit
            break;
    }

    tty.c_cflag |= CLOCAL;   // Ignora controle de modem
    tty.c_cflag |= CREAD;    // Permite recepção de caracteres
    tty.c_cflag |= CS8;      // 8 bits por caractere

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Modo não-canônico
    tty.c_oflag &= ~OPOST;                           // Desativa processamento de saída

    tty.c_cc[VTIME] = 0;   // Tempo de espera entre caracteres
    tty.c_cc[VMIN] = 1;    // Número mínimo de caracteres a serem lidos

    cfsetispeed(&tty, rate_arg);
    cfsetospeed(&tty, rate_arg);

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Erro ao configurar a porta serial");
        close(fd);
        return -1;
    }

    return fd;
}


/* -------------------------------------------------------------------- */
/* --------------------------    sendChar     ------------------------- */
/* -------------------------------------------------------------------- */
void Tserial::sendChar(char data)
{
    sendArray(&data, 1);
}

/* -------------------------------------------------------------------- */
/* --------------------------    sendArray    ------------------------- */
/* -------------------------------------------------------------------- */
void Tserial::sendArray(char *buffer, int len)
{
    unsigned long result;

    if (serial_handle!=INVALID_HANDLE_VALUE)
        WriteFile(serial_handle, buffer, len, &result, NULL);
}

/* -------------------------------------------------------------------- */
/* --------------------------    getChar      ------------------------- */
/* -------------------------------------------------------------------- */
char Tserial::getChar(void)
{
    char c;
    getArray(&c, 1);
    return(c);
}

/* -------------------------------------------------------------------- */
/* --------------------------    getArray     ------------------------- */
/* -------------------------------------------------------------------- */
int  Tserial::getArray         (char *buffer, int len)
{
    unsigned long read_nbr;

    read_nbr = 0;
    if (serial_handle!=INVALID_HANDLE_VALUE)
    {
        ReadFile(serial_handle, buffer, len, &read_nbr, NULL);
    }
    return((int) read_nbr);
}
/* -------------------------------------------------------------------- */
/* --------------------------    getNbrOfBytes ------------------------ */
/* -------------------------------------------------------------------- */
int Tserial::getNbrOfBytes    (void)
{
    struct _COMSTAT status;
    int             n;
    unsigned long   etat;

    n = 0;

    if (serial_handle!=INVALID_HANDLE_VALUE)
    {
        ClearCommError(serial_handle, &etat, &status);
        n = status.cbInQue;
    }


    return(n);
}
