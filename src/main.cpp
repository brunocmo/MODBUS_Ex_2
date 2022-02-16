#include "Comms.hpp"

int main() {

    Comms ardu;

    ardu.escolherDispositivo(0x01);

    int selecaoMenu{0};
    int selecaoSubmenu{0};
    int inteiroEnviar{0};
    float realEnviar{0.00f};
    std::string stringEnviar{""};

    printf("Selecione uma opcao:\n");
    printf(" 1 - Solicitar Dados\n");
    printf(" 2 - Envia Dados\n");
    printf(" 0 - Sair\n");

    scanf("%d", &selecaoMenu);

    system("clear");

    switch (selecaoMenu)
    {
        case 1:
            
            printf(" 1 - Solicitar Inteiro\n");
            printf(" 2 - Solicitar Real\n");
            printf(" 3 - Solicitar String\n");
            printf(" 0 - Sair\n");

            scanf("%d", &selecaoSubmenu);

            system("clear");

            switch (selecaoSubmenu)
            {
                case 1:
                    ardu.pedidoInteiro();
                    break;
                case 2:
                    ardu.pedidoReal();
                    break;
                case 3:
                    ardu.pedidoString();
                    break;
                case 0:
                default:
                    break;
            }

            break;

        case 2:

            printf(" 1 - Enviar Inteiro\n");
            printf(" 2 - Enviar Real\n");
            printf(" 3 - Enviar String\n");
            printf(" 0 - Sair\n");

            scanf("%d", &selecaoSubmenu);

            system("clear");

            switch (selecaoSubmenu)
            {
                case 1:
                    printf("Digite um valor inteiro: ");
                    scanf("%d", &inteiroEnviar);
                    ardu.enviarInteiro(inteiroEnviar);
                    break;
                case 2:
                    printf("Digite um valor real: ");
                    scanf("%f", &realEnviar);
                    ardu.enviarReal(realEnviar);
                    break;
                case 3:
                    printf("Digite uma mensagem: ");
                    std::cin >> stringEnviar;
                    ardu.enviarString(stringEnviar);
                    break;
                case 0:
                default:
                    break;
            }
        default:
            break;
    }


    // ardu.pedidoInteiro();

    // ardu.pedidoReal();

    // ardu.pedidoString();

    // ardu.enviarInteiro(inteiroTeste);

    // ardu.enviarReal(realTeste);

    // ardu.enviarString(fraseTeste);

    return 0;
}