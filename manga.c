#include "manga.h"

IndicePrimario idxPrimarios[MAX_MANGAS];
IndiceSecundario idxSecundarios[MAX_MANGAS];
int totalIndices = 0;

void carregarIndices() {
    FILE *fp = fopen("indice_primario.idx", "rb");
    if (fp) {
        fread(&totalIndices, sizeof(int), 1, fp);
        fread(idxPrimarios, sizeof(IndicePrimario), totalIndices, fp);
        fclose(fp);
    }
    fp = fopen("indice_secundario.idx", "rb");
    if (fp) {
        fread(idxSecundarios, sizeof(IndiceSecundario), totalIndices, fp);
        fclose(fp);
    }
}

void salvarIndices() {
    FILE *fp = fopen("indice_primario.idx", "wb");
    fwrite(&totalIndices, sizeof(int), 1, fp);
    fwrite(idxPrimarios, sizeof(IndicePrimario), totalIndices, fp);
    fclose(fp);

    fp = fopen("indice_secundario.idx", "wb");
    fwrite(idxSecundarios, sizeof(IndiceSecundario), totalIndices, fp);
    fclose(fp);
}

void salvarRegistro(Manga *m) {
    FILE *fp = fopen("registro.txt", "ab"); 
    fseek(fp, 0, SEEK_END);
    long pos = ftell(fp);
    fwrite(m, sizeof(Manga), 1, fp);
    fclose(fp);

    strcpy(idxPrimarios[totalIndices].isbn, m->isbn);
    idxPrimarios[totalIndices].posicao = pos;

    strcpy(idxSecundarios[totalIndices].titulo, m->titulo);
    strcpy(idxSecundarios[totalIndices].isbn, m->isbn);

    totalIndices++;
    salvarIndices();
}



void lerRegistro() {
    char busca[20];
    printf("Buscar por ISBN: ");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = 0; // remove o \n

    for (int i = 0; i < totalIndices; i++) {
        if (strcmp(idxPrimarios[i].isbn, busca) == 0) {
            FILE *fp = fopen("registro.txt", "rb");
            if (!fp) {
                printf("Erro ao abrir o arquivo de registros.\n");
                return;
            }

            fseek(fp, idxPrimarios[i].posicao, SEEK_SET);

            Manga m;
            fread(&m, sizeof(Manga), 1, fp);
            fclose(fp);

            // Verifica se o registro está vazio (apagado)
            if (m.isbn[0] == '\0') {
                printf("Este registro foi apagado.\n");
                return;
            }

            printf("\n---\n");
            printf("Título: %s\n", m.titulo);
            printf("ISBN: %s\n", m.isbn);
            printf("Autor(es): %s\n", m.autores);
            printf("Ano de início: %d\n", m.anoInicio);
            if (m.anoFim == -1)
                printf("Ano de fim: Em andamento\n");
            else
                printf("Ano de fim: %d\n", m.anoFim);
            printf("Gênero: %s\n", m.genero);
            printf("Revista: %s\n", m.revista);
            printf("Editora: %s\n", m.editora);
            printf("Ano da edição: %d\n", m.anoEdicao);
            printf("Quantidade de volumes: %d\n", m.qtdVolumes);
            printf("Volumes adquiridos (%d): ", m.qtdAdquiridos);
            for (int k = 0; k < m.qtdAdquiridos; k++) {
                printf("%d ", m.volumes[k]);
            }
            printf("\n---\n");
            return;
        }
    }
    printf("Registro não encontrado.\n");
}



void apagarRegistro() {
    char isbn[20];
    printf("Digite o ISBN a apagar: ");
    fgets(isbn, 20, stdin); strtok(isbn, "\n");

    for (int i = 0; i < totalIndices; i++) {
        if (strcmp(idxPrimarios[i].isbn, isbn) == 0) {
            printf("Deseja realmente apagar o registro? (s/n): ");
            char op = getchar(); getchar();
            if (op == 's' || op == 'S') {
                FILE *fp = fopen("registro.txt", "r+b");
                fseek(fp, idxPrimarios[i].posicao, SEEK_SET);
                Manga vazio = {0};
                fwrite(&vazio, sizeof(Manga), 1, fp);
                fclose(fp);

                for (int j = i; j < totalIndices - 1; j++) {
                    idxPrimarios[j] = idxPrimarios[j + 1];
                    idxSecundarios[j] = idxSecundarios[j + 1];
                }
                totalIndices--;
                salvarIndices();
                printf("Registro apagado.\n");
            }
            return;
        }
    }
    printf("Registro nao encontrado.\n");
}

void processarLinha(Manga *m, const char *linhaOriginal) {
    char linha[512];
    strcpy(linha, linhaOriginal);

    char *token = strtok(linha, ";");
    if (!token) return;
    strcpy(m->isbn, token);

    token = strtok(NULL, ";");
    if (!token) return;
    strcpy(m->titulo, token);

    token = strtok(NULL, ";");
    if (!token) return;
    strcpy(m->autores, token);

    token = strtok(NULL, ";");
    if (!token) return;
    m->anoInicio = atoi(token);

    token = strtok(NULL, ";");
    if (!token) return;
    m->anoFim = (strcmp(token, "-") == 0) ? -1 : atoi(token);

    token = strtok(NULL, ";");
    if (!token) return;
    strcpy(m->genero, token);

    token = strtok(NULL, ";");
    if (!token) return;
    strcpy(m->revista, token);

    token = strtok(NULL, ";");
    if (!token) return;
    strcpy(m->editora, token);

    token = strtok(NULL, ";");
    if (!token) return;
    m->anoEdicao = atoi(token);

    token = strtok(NULL, ";");
    if (!token) return;
    m->qtdVolumes = atoi(token);

    token = strtok(NULL, ";");
    if (!token) return;
    m->qtdAdquiridos = atoi(token);

    token = strtok(NULL, ";");
    if (!token) return;

    int i = 0;
    char *volToken = strtok(token, "[, ]");
    while (volToken && i < m->qtdAdquiridos) {
        m->volumes[i++] = atoi(volToken);
        volToken = strtok(NULL, "[, ]");
    }
}

void criarRegistroPorLinha() {
    char linha[512];
    printf("Insira os dados do manga (separados por ';'e com volumes entre colchetes):\n");
    fgets(linha, sizeof(linha), stdin); strtok(linha, "\n");

    Manga m;
    processarLinha(&m, linha);
    salvarRegistro(&m);
}

void atualizarRegistro() {
    char isbn[20];
    printf("Digite o ISBN do registro a ser atualizado: ");
    fgets(isbn, 20, stdin); strtok(isbn, "\n");

    for (int i = 0; i < totalIndices; i++) {
        if (strcmp(idxPrimarios[i].isbn, isbn) == 0) {
            printf("Insira a nova linha no mesmo formato do input unico:\n");
            char linha[512];
            fgets(linha, sizeof(linha), stdin); strtok(linha, "\n");

            Manga m;
            processarLinha(&m, linha);

            FILE *fp = fopen("registro.txt", "r+b");
            fseek(fp, idxPrimarios[i].posicao, SEEK_SET);
            fwrite(&m, sizeof(Manga), 1, fp);
            fclose(fp);

            strcpy(idxPrimarios[i].isbn, m.isbn);
            strcpy(idxSecundarios[i].isbn, m.isbn);
            strcpy(idxSecundarios[i].titulo, m.titulo);
            salvarIndices();

            printf("Registro atualizado.\n");
            return;
        }
    }
    printf("Registro nao encontrado.\n");
}

void criarRegistrosDeArquivo() {
    char nomeArquivo[100];
    printf("Insira o nome do arquivo .txt: ");
    fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
    nomeArquivo[strcspn(nomeArquivo, "\n")] = 0;

    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[512];
    int count = 0;
    while (fgets(linha, sizeof(linha), fp)) {
        linha[strcspn(linha, "\n")] = 0;  
        if (linha[0] == '\0') continue;  

        Manga m;
        processarLinha(&m, linha);
        salvarRegistro(&m);
        count++;
    }

    fclose(fp);
    printf("%d registro(s) importado(s) do arquivo.\n", count);
}

