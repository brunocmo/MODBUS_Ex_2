#ifndef COMMS_HPP
#define COMMS_HPP

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <termios.h>
#include <bitset>
#include <iostream>


class Comms {

    private:
        int uart0_filestream;
        std::string palavra;
        struct termios options;
        char matricula[4];

    public:

        Comms();
        ~Comms();

        void init();

        void pedidoInteiro();
        void pedidoReal();
        void pedidoString();

        void enviarInteiro(int inteiroEnviado);
        void enviarReal(float floatEnviado);
        void enviarString(std::string stringEnviado);

        void solicitar(std::string solicitacao);
        void enviar(std::string solicitacao);
        void receber(int flag);

        int get_uart0_filestream();
        void set_uart0_filestream( int uart0_filestream );



};




#endif