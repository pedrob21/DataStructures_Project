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
    FILE *fp = fopen("registro.txt", "a+b");
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

void criarRegistro() {
    Manga m;
    char buffer[100];

    printf("ISBN: ");
    fgets(m.isbn, sizeof(m.isbn), stdin);
    m.isbn[strcspn(m.isbn, "\n")] = 0;

    printf("Titulo: ");
    fgets(m.titulo, sizeof(m.titulo), stdin);
    m.titulo[strcspn(m.titulo, "\n")] = 0;

    printf("Autores: ");
    fgets(m.autores, sizeof(m.autores), stdin);
    m.autores[strcspn(m.autores, "\n")] = 0;

    printf("Ano de inicio: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &m.anoInicio);

    printf("Ano de fim (- se em andamento): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strcmp(buffer, "-") == 0)
        m.anoFim = -1;
    else
        sscanf(buffer, "%d", &m.anoFim);

    printf("Genero: ");
    fgets(m.genero, sizeof(m.genero), stdin);
    m.genero[strcspn(m.genero, "\n")] = 0;

    printf("Revista: ");
    fgets(m.revista, sizeof(m.revista), stdin);
    m.revista[strcspn(m.revista, "\n")] = 0;

    printf("Editora: ");
    fgets(m.editora, sizeof(m.editora), stdin);
    m.editora[strcspn(m.editora, "\n")] = 0;

    printf("Ano da edicao: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &m.anoEdicao);

    printf("Qtd. volumes: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &m.qtdVolumes);

    printf("Qtd. volumes adquiridos: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &m.qtdAdquiridos);

    printf("Volumes adquiridos: ");
    for (int i = 0; i < m.qtdAdquiridos; i++) {
        scanf("%d", &m.volumes[i]);
    }
    getchar(); 

    salvarRegistro(&m);
}


void lerRegistro() {
    char busca[20];
    printf("Buscar por ISBN: ");
    fgets(busca, 20, stdin); strtok(busca, "\n");
    for (int i = 0; i < totalIndices; i++) {
        if (strcmp(idxPrimarios[i].isbn, busca) == 0) {
            FILE *fp = fopen("registro.txt", "rb");
            fseek(fp, idxPrimarios[i].posicao, SEEK_SET);
            Manga m;
            fread(&m, sizeof(Manga), 1, fp);
            fclose(fp);
            printf("---\nTitulo: %s\nISBN: %s\nAutor(es): %s\nVolumes adquiridos: ", m.titulo, m.isbn, m.autores);
            for (int k = 0; k < m.qtdAdquiridos; k++) printf("%d ", m.volumes[k]);
            printf("\n---\n");
            return;
        }
    }
    printf("Registro nao encontrado.\n");
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
    printf("Insira os dados do manga (separados por ';', volumes entre colchetes):\n");
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

