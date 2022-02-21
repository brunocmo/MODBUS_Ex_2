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

        printf("%d --- %X\n", i, teste[i]);
    }

    int count = write( get_uart0_filestream(), &teste, solicitacao.length());

    printf("Numero de bytes enviados: %d\n", count);

    if(count < 0) {
        printf("Erro no envio de dados - TX\n");
    }

    sleep(1);

    if (teste[2] == 0xA1 || teste[2] == 0xB1 ) {
        receber(1);
    } else if (teste[2] == 0xA2 || teste[2] == 0xB2 ) {
        receber(2);
    } else {
        receber(3);
    }
}

void Comms::pedidoInteiro() {

    escolherFuncao(0x23);
    char SolicitacaoInteiro;
    SolicitacaoInteiro = 0xA1;
    unsigned char crcPreVerificador[3] { 
        (unsigned char)enderecoDispositivo, 
        (unsigned char)codigoFuncao, 
        (unsigned char)SolicitacaoInteiro 
        };
    short crc = calcula_CRC(&crcPreVerificador[0], 3);
    char crcConvert[2];

    std::memcpy(crcConvert, &crc, sizeof(short));

    std::string stringTemp{""};
    stringTemp.push_back(enderecoDispositivo);
    stringTemp.push_back(codigoFuncao);
    stringTemp.push_back(SolicitacaoInteiro);
    stringTemp.push_back(crcConvert[0]);
    stringTemp.push_back(crcConvert[1]);


    solicitar(stringTemp);
}

void Comms::pedidoReal() {

    escolherFuncao(0x23);
    char SolicitacaoReal;
    SolicitacaoReal = 0xA2;
    unsigned char crcPreVerificador[3] { 
        (unsigned char)enderecoDispositivo, 
        (unsigned char)codigoFuncao, 
        (unsigned char)SolicitacaoReal };
    short crc = calcula_CRC(&crcPreVerificador[0], 3);
    char crcConvert[2];

    std::memcpy(crcConvert, &crc, sizeof(short));

    std::string stringTemp{""};
    stringTemp.push_back(enderecoDispositivo);
    stringTemp.push_back(codigoFuncao);
    stringTemp.push_back(SolicitacaoReal);
    stringTemp.push_back(crcConvert[0]);
    stringTemp.push_back(crcConvert[1]);

    solicitar(stringTemp);


}

void Comms::pedidoString() {

    escolherFuncao(0x23);
    char SolicitacaoString;
    SolicitacaoString = 0xA3;
    unsigned char crcPreVerificador[3] { 
        (unsigned char)enderecoDispositivo, 
        (unsigned char)codigoFuncao, 
        (unsigned char)SolicitacaoString };
    short crc = calcula_CRC(&crcPreVerificador[0], 3);
    char crcConvert[2];

    std::memcpy(crcConvert, &crc, sizeof(short));

    std::string stringTemp{""};
    stringTemp.push_back(enderecoDispositivo);
    stringTemp.push_back(codigoFuncao);
    stringTemp.push_back(SolicitacaoString);
    stringTemp.push_back(crcConvert[0]);
    stringTemp.push_back(crcConvert[1]);


    solicitar(stringTemp);


}

void Comms::escolherDispositivo(char numeroDispositivo) {
    this->enderecoDispositivo = numeroDispositivo;
}

void Comms::escolherFuncao(char codigoFuncao){
    this->codigoFuncao = codigoFuncao;
}

void Comms::enviarInteiro(int inteiroEnviado) {

    escolherFuncao(0x16);
    char SolicitacaoInteiro = 0xB1;
    char InteiroBytes[4];

    std::memcpy(InteiroBytes, &inteiroEnviado, sizeof(int));

    unsigned char crcPreVerificador[7] { 
        (unsigned char)enderecoDispositivo, 
        (unsigned char)codigoFuncao, 
        (unsigned char)SolicitacaoInteiro, 
        (unsigned char)InteiroBytes[0],
        (unsigned char)InteiroBytes[1],
        (unsigned char)InteiroBytes[2],
        (unsigned char)InteiroBytes[3],
        };
    short crc = calcula_CRC(&crcPreVerificador[0], 7);
    char crcConvert[2];

    std::memcpy(crcConvert, &crc, sizeof(short));

    std::string stringTemp{""};
    stringTemp.push_back(enderecoDispositivo);
    stringTemp.push_back(codigoFuncao);
    stringTemp.push_back(SolicitacaoInteiro);
    stringTemp.push_back(InteiroBytes[0]);
    stringTemp.push_back(InteiroBytes[1]);
    stringTemp.push_back(InteiroBytes[2]);
    stringTemp.push_back(InteiroBytes[3]);
    stringTemp.push_back(crcConvert[0]);
    stringTemp.push_back(crcConvert[1]);

    solicitar(stringTemp);
}

void Comms::enviarReal(float floatEnviado) {
    escolherFuncao(0x16);
    char SolicitacaoFloat = 0xB2;
    char RealBytes[4];

    std::memcpy(RealBytes, &floatEnviado, sizeof(int));

    unsigned char crcPreVerificador[7] { 
        (unsigned char)enderecoDispositivo, 
        (unsigned char)codigoFuncao, 
        (unsigned char)SolicitacaoFloat, 
        (unsigned char)RealBytes[0],
        (unsigned char)RealBytes[1],
        (unsigned char)RealBytes[2],
        (unsigned char)RealBytes[3],
        };
    short crc = calcula_CRC(&crcPreVerificador[0], 7);
    char crcConvert[2];

    std::memcpy(crcConvert, &crc, sizeof(short));

    std::string stringTemp{""};
    stringTemp.push_back(enderecoDispositivo);
    stringTemp.push_back(codigoFuncao);
    stringTemp.push_back(SolicitacaoFloat);
    stringTemp.push_back(RealBytes[0]);
    stringTemp.push_back(RealBytes[1]);
    stringTemp.push_back(RealBytes[2]);
    stringTemp.push_back(RealBytes[3]);
    stringTemp.push_back(crcConvert[0]);
    stringTemp.push_back(crcConvert[1]);

    solicitar(stringTemp);
}

void Comms::enviarString(std::string stringEnviado) {

    escolherFuncao(0x16);
    char SolicitacaoString = 0xB3;
    int quantidadePalavras = (int)(stringEnviado.length());

    quantidadePalavras += 3;

    unsigned char crcPreVerificador[255];

    crcPreVerificador[0] = (unsigned char)enderecoDispositivo;
    crcPreVerificador[1] = (unsigned char)codigoFuncao;
    crcPreVerificador[2] = (unsigned char)SolicitacaoString;

    for(int i{3}; i<(int)stringEnviado.length(); i++) {
        crcPreVerificador[i] = (unsigned char)stringEnviado.at(i);
    }

    short crc = calcula_CRC(&crcPreVerificador[0], quantidadePalavras);
    char crcConvert[2];

    std::memcpy(crcConvert, &crc, sizeof(short));

    std::string stringTemp{""};
    stringTemp.push_back(enderecoDispositivo);
    stringTemp.push_back(codigoFuncao);
    stringTemp.push_back(SolicitacaoString);
    stringTemp = stringTemp + stringEnviado;
    stringTemp.push_back(crcConvert[0]);
    stringTemp.push_back(crcConvert[1]);

    solicitar(stringTemp);
}

void Comms::receber(int flag) {
    
    int valorInteiro{0};
    float valorPontoFlut{0.0f};
    short crc;
    unsigned char crcPreVerificador[7];

    if(get_uart0_filestream() != -1) {

        unsigned char verificador[2];
        short crcRecebido;

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

                verificador[0] = rx_buffer[7];
                verificador[1] = rx_buffer[8];

                std::memcpy(&crcRecebido, &verificador[0], sizeof(short));

                for(int k{0}; k<7; k++) {
                    crcPreVerificador[k] = (unsigned char)rx_buffer[k];
                }

                crc = calcula_CRC(&crcPreVerificador[0], 7);
                        
                if( crcRecebido == crc ) {
                    printf("CRC verificado com sucesso!!! \n");
                } else {
                    printf("CRC falhou na verificao");
                }

                std::memcpy(&valorInteiro, rx_buffer, sizeof(int));

                printf("Mensagem de comprimento %d: %d\n", rx_length, valorInteiro);
                break;

            case 2 : 
                std::memcpy(&valorPontoFlut, rx_buffer, 4);
                printf("Mensagem de comprimento %d: %f\n", rx_length, valorPontoFlut);
                break;

            case 3 : 
                printf("Mensagem de comprimento %d: %s\n", rx_length, (rx_buffer+1) );
                break;

            default:
                break;
            }
        }
    }

    close(get_uart0_filestream());
}

int Comms::get_uart0_filestream() {
    return uart0_filestream;
}

void Comms::set_uart0_filestream( int uart0_filestream ) {
    this->uart0_filestream = uart0_filestream;
}