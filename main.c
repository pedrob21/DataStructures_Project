#include "manga.h"

int main() {
    int op;
    do {
        printf("\n\t=====Menu=====\n");
        printf("================================\n");
        printf("\nEscolha uma opcao:\n");
        printf("1. Criar registro manual\n");
        printf("2. Criar registro por linha unica\n");
        printf("3. Criar registro lendo arquivo .txt\n");
        printf("4. Ler registro pelo ISBN\n");
        printf("5. Atualizar registro\n");
        printf("6. Apagar registro\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &op); 
        getchar();
        printf("\n================================\n\n");


        switch (op) {
            case 1: 
                criarRegistro(); 
                break;
            case 2: 
                criarRegistroPorLinha(); 
                break;
            case 3:
                criarRegistrosDeArquivo(); 
                break;
            case 4: 
                lerRegistro(); 
                break;
            case 5: 
                atualizarRegistro(); 
                break;
            case 6: 
                apagarRegistro(); 
                break;
        }
    } while (op != 0);
}

