// Marcel Henrique Rodrigues Batista -15474421
// Pedro Bernardo Rodrigues Pinto - 15590042

#include "manga.h"

int main() {
    int op;
    do {
        carregarIndices();
        printf("\n\t=====Menu=====\n");
        printf("================================\n");
        printf("\nEscolha uma opcao:\n");
        printf("1. Criar registro por linha unica\n");
        printf("2. Criar registro lendo arquivo .txt\n");
        printf("3. Ler registro pelo ISBN\n");
        printf("4. Atualizar registro\n");
        printf("5. Apagar registro\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &op); 
        getchar();
        printf("\n================================\n\n");


        switch (op) {
            case 1: 
                criarRegistroPorLinha(); 
                break;
            case 2:
                criarRegistrosDeArquivo(); 
                break;
            case 3: 
                lerRegistro(); 
                break;
            case 4: 
                atualizarRegistro(); 
                break;
            case 5: 
                apagarRegistro(); 
                break;
        }
    } while (op != 0);
}

