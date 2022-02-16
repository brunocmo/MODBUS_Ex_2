#include "../inc/Comms.hpp"

Comms::Comms() {
    this->uart0_filestream = -1;
    this->palavra = "";
    this->matricula[0] = 7;
    this->matricula[1] = 5;
    this->matricula[2] = 4;
    this->matricula[3] = 8;
}

Comms::~Comms() {
}

void Comms::init() {
    set_uart0_filestream( open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY) );

    if ( get_uart0_filestream() == -1 ) {
        printf("Erro na abertura da UART\n");
    }

    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    printf("Abertura e configuração realizadas\n");

}

void Comms::solicitar(std::string solicitacao){
    
    init();
    
    unsigned char teste[255];

    for(unsigned int i{0}; i<solicitacao.length() ; i++) {
        teste[i] = solicitacao.at(i);

        printf("%d --- %d\n", i, teste[i]);
    }

    int count = write( get_uart0_filestream(), &teste, solicitacao.length());

    printf("Numero de bytes enviados: %d\n", count);

    if(count < 0) {
        printf("Erro no envio de dados - TX\n");
    }

    sleep(1);

    if (teste[0] == 0xA1 || teste[0] == 0xB1 ) {
        receber(1);
    } else if (teste[0] == 0xA2 || teste[0] == 0xB2 ) {
        receber(2);
    } else {
        receber(3);
    }
}

void Comms::pedidoInteiro() {

    char SolicitacaoInteiro;
    SolicitacaoInteiro = 0xA1;

    std::string stringTemp{""};
    stringTemp.push_back(SolicitacaoInteiro);
    stringTemp.push_back(matricula[0]);
    stringTemp.push_back(matricula[1]);
    stringTemp.push_back(matricula[2]);
    stringTemp.push_back(matricula[3]);

    solicitar(stringTemp);
}

void Comms::pedidoReal() {

    char SolicitacaoReal;
    SolicitacaoReal = 0xA2;

    std::string stringTemp{""};
    stringTemp.push_back(SolicitacaoReal);
    stringTemp.push_back(matricula[0]);
    stringTemp.push_back(matricula[1]);
    stringTemp.push_back(matricula[2]);
    stringTemp.push_back(matricula[3]);

    solicitar(stringTemp);


}

void Comms::pedidoString() {

    char SolicitacaoString;
    SolicitacaoString = 0xA3;

    std::string stringTemp{""};
    stringTemp.push_back(SolicitacaoString);
    stringTemp.push_back(matricula[0]);
    stringTemp.push_back(matricula[1]);
    stringTemp.push_back(matricula[2]);
    stringTemp.push_back(matricula[3]);

    solicitar(stringTemp);


}


void Comms::enviarInteiro(int inteiroEnviado) {

    char SolicitacaoInteiro;
    SolicitacaoInteiro = 0xB1;

    char InteiroBytes[4];

    std::memcpy(InteiroBytes, &inteiroEnviado, sizeof(int));

    std::string stringTemp{""};
    stringTemp.push_back(SolicitacaoInteiro);
    stringTemp.push_back(InteiroBytes[0]);
    stringTemp.push_back(InteiroBytes[1]);
    stringTemp.push_back(InteiroBytes[2]);
    stringTemp.push_back(InteiroBytes[3]);
    stringTemp.push_back(matricula[0]);
    stringTemp.push_back(matricula[1]);
    stringTemp.push_back(matricula[2]);
    stringTemp.push_back(matricula[3]);


    solicitar(stringTemp);
}

void Comms::enviarReal(float floatEnviado) {

    char SolicitacaoFloat;
    SolicitacaoFloat = 0xB2;

    char RealBytes[4];

    std::memcpy(RealBytes, &floatEnviado, sizeof(int));

    std::string stringTemp{""};
    stringTemp.push_back(SolicitacaoFloat);
    stringTemp.push_back(RealBytes[0]);
    stringTemp.push_back(RealBytes[1]);
    stringTemp.push_back(RealBytes[2]);
    stringTemp.push_back(RealBytes[3]);
    stringTemp.push_back(matricula[0]);
    stringTemp.push_back(matricula[1]);
    stringTemp.push_back(matricula[2]);
    stringTemp.push_back(matricula[3]);

    solicitar(stringTemp);
}

void Comms::enviarString(std::string stringEnviado) {

    char SolicitacaoString;
    SolicitacaoString = 0xB3;

    std::string contador = stringEnviado;
    char quantidadePalavras = (int)(contador.length());

    std::string stringTemp{""};
    stringTemp.push_back(SolicitacaoString);
    stringTemp.push_back(quantidadePalavras);
    stringTemp = stringTemp + stringEnviado;
    stringTemp.push_back(matricula[0]);
    stringTemp.push_back(matricula[1]);
    stringTemp.push_back(matricula[2]);
    stringTemp.push_back(matricula[3]);

    solicitar(stringTemp);
}

// void Comms::enviar(std::string solicitacao){
    
//     init();
    
//     unsigned char teste[255];

//     teste[0] = solicitacao.at(0);
//     teste[1] = solicitacao.at(1);

//     for(unsigned int i{2}; i<solicitacao.length() ; i++) {
        
//         if( i < solicitacao.length()-4) {
//             teste[i] = (solicitacao.at(i));
//         }else {
//             teste[i] = (solicitacao.at(i)-48);
//         }
        
//         printf("%d --- %d\n", i, teste[i]);
//     }

//     int count = write( get_uart0_filestream(), &teste, solicitacao.length());

//     printf("Numero de bytes enviados: %d\n", count);

//     if(count < 0) {
//         printf("Erro no envio de dados - TX\n");
//     }

//     sleep(2);

//     receber(3);
// }



void Comms::receber(int flag) {
    
    int valorInteiro{0};
    float valorPontoFlut{0.0f};

    if(get_uart0_filestream() != -1) {

        unsigned char rx_buffer[100];
        int rx_length = read(get_uart0_filestream(), (void*)rx_buffer, 100);
        if(rx_length < 0){
            printf("Erro na leitura da UART - RX\n");
        }
        else if(rx_length == 0){
            printf("Nenhum dado disponível\n");
        } else {

            switch (flag)
            {
            case 1 : 

                std::memcpy(&valorInteiro, rx_buffer, sizeof(int));

                printf("Mensagem de comprimento %d: %d\n", rx_length, valorInteiro);
                break;

            case 2 : 
                std::memcpy(&valorPontoFlut, rx_buffer, 4);
                printf("Mensagem de comprimento %d: %f\n", rx_length, valorPontoFlut);
                break;

            case 3 : 
                printf("Mensagem de comprimento %d: %s\n", rx_length, rx_buffer);
                break;

            default:
                break;
            }
        }
    }

    close(get_uart0_filestream());
}



// int Comms::binToDecimal() {
    
// }


int Comms::get_uart0_filestream() {
    return uart0_filestream;
}

void Comms::set_uart0_filestream( int uart0_filestream ) {
    this->uart0_filestream = uart0_filestream;
}