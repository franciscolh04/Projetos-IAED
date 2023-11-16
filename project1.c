/* iaed-23 - ist1106970 - project1 */

/*
* Ficheiro; project1.c
* Autor: Francisco Lourenço Heleno - 106970
* Descrição: A minha resolução do projeto 1 da UC de IAED
*/

/* Bibliotecas utilizadas no desenvolvimento do projeto */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO 0 /* Constante que simboliza o estado NO */
#define YES 1 /* Constante que simboliza o estado YES */
#define MAX_CARREIRAS 200 /* Número máximo de carreiras no sistema */
#define MAX_PARAGENS 10000 /* Número máximo de paragens no sistema */
#define MAX_LIGACOES 30000 /* Número máximo de ligações no sistema */
#define MAX_NOME_CARREIRA 21 /* Comprimento máximo do nome de uma carreira */
#define MAX_NOME_PARAGEM 51 /* Comprimento máximo do nome de uma paragem */
#define MAX_INVERSO 8 /* Comprimento máximo da opção de ordenação do inverso */
#define __BUFSIZ__ 8192 /* Comprimento máximo de uma linha de comandos */

/* Definição da estrutura Paragem */
typedef struct paragem {
    char nome[MAX_NOME_PARAGEM];
    double lat;
    double lon;
    int num_carreiras;
} Paragem;

/* Definição da estrutura Carreira */
typedef struct carreira {
    char nome[MAX_NOME_CARREIRA];
    Paragem origem;
    Paragem destino;
    int num_paragens;
    int num_ligacoes;
    int ciclo;
    double custo_total;
    double duracao_total;
} Carreira;

/* Definição da estrutura Ligação */
typedef struct ligacao {
    Carreira carreira;
    Paragem origem;
    Paragem destino;
    double custo;
    double duracao;
    int num_carreira;
} Ligacao;

/* Variáveis globais */

/* Contadores de elementos dos vetores de estruturas */
int cont_paragens = 0, cont_carreiras = 0, cont_ligacoes = 0;

char c, comando; /* Carater atual e comando */
Paragem paragens[MAX_PARAGENS]; /* Vetor de paragens */
Carreira carreiras[MAX_CARREIRAS]; /* Vetor de carreiras */
Ligacao ligacoes[MAX_LIGACOES]; /* Vetor de ligações */


/* Função auxiliar que lê uma palavra para um
vetor que lhe é passado como argumento */
void lePalavra(char palavra[], int tamanho) {
    int i;
    char c = getchar();
    /* Caso a palavra esteja entre aspas */
    if (c == '\"') {
        c = getchar();
        for(i = 0; c != '\"' && i < tamanho - 1; i++) {
            palavra[i] = c;
            c = getchar();
        }
        palavra[i] = '\0';
    }
    /* Caso a palavra não esteja entre aspas */
    else {
        for (i = 0; c != EOF && c != ' ' && c != '\n'
            && c != '\t' && i < tamanho - 1; i++) {
                palavra[i] = c;
                c = getchar();
        }
        palavra[i] = '\0';
        ungetc(c, stdin);
    }
}

/* Função que cria uma nova carreira com um dado
nome e a adiciona ao vetor global de carreiras */
void adicionaCarreira(char nome[]) {
    Carreira nova;

    strcpy(nova.nome, nome);
    nova.num_paragens = 0;
    nova.custo_total = 0;
    nova.duracao_total = 0;
    nova.num_ligacoes = 0;
    nova.ciclo = NO;
    carreiras[cont_carreiras] = nova;
}

/* Função que imprime no stdout as paragens de uma
dada carreira no sentido Origem -> Destino */
int mostraInfoCarreira(char nome[]) {
    int i, pos_carreira = 0, numero_paragens, estado = NO;
    Paragem atual, destino;

    /* Encontra posição da carreira no vetor de carreiras */
    for(i = 0; i < cont_carreiras; i++)
        if (strcmp(carreiras[i].nome, nome) == 0)
            pos_carreira = i;
    
    /* Caso a carreira não tenha paragens */
    if (carreiras[pos_carreira].num_paragens == 0) {
        return 0;
    }
    
    /* Caso a carreira apenas tenha duas paragens */
    if (carreiras[pos_carreira].num_paragens == 2) {
        printf("%s, %s\n", carreiras[pos_carreira].origem.nome,
        carreiras[pos_carreira].destino.nome);
        return 0;
    }
    
    /* Define as primeira e última paragens da
    carreira e imprime a primeira paragem */
    atual = carreiras[pos_carreira].origem;
    destino = carreiras[pos_carreira].destino;
    printf("%s", atual.nome);

    /* Caso seja um ciclo, altera o destino
    para a última paragem antes do ciclo */
    if (strcmp(atual.nome, destino.nome) == 0) {
        for(i = 0; i < cont_ligacoes; i++) {
            if (strcmp(ligacoes[i].destino.nome, destino.nome) == 0) {
                destino = ligacoes[i].origem;
            }
        }
    }

    /* Encontra a paragem de destino da primeira ligação da carreira */
    for(i = 0; i < cont_ligacoes && estado == NO; i++)
        if (strcmp(ligacoes[i].origem.nome, atual.nome) == 0 &&
            strcmp(ligacoes[i].carreira.nome,
            carreiras[pos_carreira].nome) == 0) {
                estado = YES;
                atual = ligacoes[i].destino;
        }

    /* Imprime nomes das restantes paragens */
    numero_paragens = carreiras[pos_carreira].num_paragens;
    while(numero_paragens > 1) {
        printf(", %s", atual.nome);
        /* Percorre as ligações para achar as seguintes paragens */
        for(i = 0; i < cont_ligacoes; i++) {
            if (strcmp(ligacoes[i].origem.nome, atual.nome) == 0 &&
                strcmp(ligacoes[i].carreira.nome,
                carreiras[pos_carreira].nome) == 0) {
                    atual = ligacoes[i].destino;
                    break;
            }
        }
        numero_paragens--;
    }
    printf("\n");

    return 0;
}

/* Função que imprime no stdout as paragens de uma
dada carreira no sentido Destino -> Origem */
int inverso(char nome[]) {
    int i, pos_carreira = 0, numero_paragens, estado = NO;
    Paragem atual, origem;

    /* Encontra posição da carreira no vetor de carreiras */
    for(i = 0; i < cont_carreiras; i++)
        if (strcmp(carreiras[i].nome, nome) == 0)
            pos_carreira = i;
    
    /* Caso a carreira não tenha paragens */
    if (carreiras[pos_carreira].num_paragens == 0) {
        return 0;
    }
    
    /* Caso a carreira apenas tenha duas paragens */
    if (carreiras[pos_carreira].num_paragens == 2) {
        printf("%s, %s\n", carreiras[pos_carreira].destino.nome,
        carreiras[pos_carreira].origem.nome);
        return 0;
    }
    
    /* Define as primeira e última paragens
    da carreira e imprime a última paragem */
    atual = carreiras[pos_carreira].destino;
    origem = carreiras[pos_carreira].origem;
    printf("%s", atual.nome);

    /* Caso seja um ciclo, altera a origem para
    a última paragem da primeira ligação */ 
    if (strcmp(atual.nome, origem.nome) == 0) {
        for(i = 0; i < cont_ligacoes; i++) {
            if (strcmp(ligacoes[i].origem.nome, origem.nome) == 0) {
                origem = ligacoes[i].destino;
            }
        }
    }

    /* Encontra a paragem de origem da última ligação da carreira */
    for(i = 0; i < cont_ligacoes && estado == NO; i++)
        if (strcmp(ligacoes[i].destino.nome, atual.nome) == 0 &&
            strcmp(ligacoes[i].carreira.nome,
            carreiras[pos_carreira].nome) == 0) {
                estado = YES;
                atual = ligacoes[i].origem;
        }

    /* Imprime nomes das restantes paragens */
    numero_paragens = carreiras[pos_carreira].num_paragens;
    while(numero_paragens - 1 > 0) {
        printf(", %s", atual.nome);
        /* Percorre as ligações para achar as seguintes paragens */
        for(i = 0; i < cont_ligacoes; i++) {
            if (strcmp(ligacoes[i].destino.nome, atual.nome) == 0 &&
                strcmp(ligacoes[i].carreira.nome,
                carreiras[pos_carreira].nome) == 0) {
                    atual = ligacoes[i].origem;
                    break;
            }
        }
        numero_paragens--;
    }
    printf("\n");

    return 0;
}

/* Função que imprime no stdout as informações
gerais de todas as carreiras no sistema */
void mostraCarreiras() {
    int i;
    for(i = 0; i < cont_carreiras; i++) {
        if (carreiras[i].num_ligacoes == 0)
            printf("%s %d %.2f %.2f\n", carreiras[i].nome,
            carreiras[i].num_paragens, carreiras[i].custo_total,
            carreiras[i].duracao_total);
        else
            printf("%s %s %s %d %.2f %.2f\n", carreiras[i].nome,
            carreiras[i].origem.nome, carreiras[i].destino.nome,
            carreiras[i].num_paragens, carreiras[i].custo_total,
            carreiras[i].duracao_total);
    }
}

/* Função que verifica se uma dada carreira existe
no sistema e devolve o estado correspondente */
int carreiraExiste(int estado, char nome[]) {
    int i;

    for(i = 0; i < cont_carreiras; i++) {
        if (strcmp(carreiras[i].nome, nome) == 0)
            estado = YES;
    }
    return estado;
}

/* Função que cria uma nova paragem com um dado
nome e a adiciona ao vetor global de paragens */
void adicionaParagem(char nome[]) {
    double lat, lon;
    Paragem estacao;

    /* Recolha das coordenadas da paragem a adicionar */
    scanf("%lf %lf", &lat, &lon);

    /* Associa as informações a uma paragem */
    strcpy(estacao.nome, nome);
    estacao.lat = lat;
    estacao.lon = lon;
    estacao.num_carreiras = 0;
    paragens[cont_paragens] = estacao;
}

/* Função que imprime no stdout as informações
de uma paragem com um dado nome */
void mostraInfoParagem(char nome[]) {
    int i;
    for(i = 0; i < cont_paragens; i++) {
        if (strcmp(paragens[i].nome, nome) == 0)
            printf("%16.12f %16.12f\n", paragens[i].lat, paragens[i].lon);
    }
}

/* Função que conta o número de carreiras que param na
paragem cuja posição no vetor de paragens é pos_paragem */
int contaCarreirasParagem(int pos_paragem) {
    int i, n = 0, num_carreiras = 0;
    Carreira listaCarreirasTemp[MAX_CARREIRAS];

    for(i = 0; i < cont_ligacoes; i++) {
        /* Caso encontre a paragem numa das ligações, verifica se a
        respetiva carreira já foi adicionada ao vetor temporário */
        if (strcmp(paragens[pos_paragem].nome, ligacoes[i].origem.nome) == 0 ||
            strcmp(paragens[pos_paragem].nome, ligacoes[i].destino.nome) == 0){
                n = 0;
                while (n < num_carreiras && strcmp(ligacoes[i].carreira.nome,
                    listaCarreirasTemp[n].nome) != 0) {
                        n++;
                }
                /* Caso não a carreira não pertença ao vetor, adiciona-a
                e incrementa o número de carreiras que param na paragem */
                if (n == num_carreiras) {
                    listaCarreirasTemp[num_carreiras] = ligacoes[i].carreira;
                    num_carreiras++;
                }
        }
    }
    return num_carreiras;
}

/* Função que imprime no stdout as informações
de todas as paragens no sistema */
void mostraParagens() {
    int i;

    /* Imprime informação atualizada */
    for(i = 0; i < cont_paragens; i++) {
        paragens[i].num_carreiras = contaCarreirasParagem(i);
        printf("%s: %16.12f %16.12f %d\n", paragens[i].nome, paragens[i].lat,
        paragens[i].lon, paragens[i].num_carreiras);
    }
}

/* Função que verifica se uma dada paragem existe
no sistema e devolve o estado correspondente */
int paragemExiste(int estado, char nome[]) {
    int i;

    for(i = 0; i < cont_paragens; i++) {
        if (strcmp(paragens[i].nome, nome) == 0)
            estado = YES;
    }
    return estado;
}

/* Função que recolhe os nomes da carreira e
paragens de origem e destino de uma ligação */
void recolheDadosLigacao(char carreira[], char origem[], char destino[]) {
    getchar();
    lePalavra(carreira, MAX_NOME_CARREIRA);
    getchar();
    lePalavra(origem, MAX_NOME_PARAGEM);
    getchar();
    lePalavra(destino, MAX_NOME_PARAGEM);
}

/* Função que ordena alfabeticamente uma lista de nomes
de carreiras que lhe é passada como argumento */
void ordenaListaCarreiras(char lista[][MAX_NOME_CARREIRA], int cont_car_temp) {
    int i, j;
    char temporario[MAX_NOME_CARREIRA];

    /* Utilizando o algoritmo de ordenação bubblesort, ordena a lista */
    for(i = 0; i < cont_car_temp - 1; i++) {
        for(j = 0; j < cont_car_temp - 1 - i; j++) {
            if (strcmp(lista[j], lista[j + 1]) > 0) {
                strcpy(temporario, lista[j]);
                strcpy(lista[j], lista[j + 1]);
                strcpy(lista[j + 1], temporario);
            }
        }
    }
} 

/* Função que engloba outras funções de modo a fazer funcionar o comando c */
void comando_c() {
    c = getchar();
    /* Caso sejam fornecidos argumentos */
    if (c != '\n') {
        char nome[MAX_NOME_CARREIRA];
        int estado = NO;

        lePalavra(nome, MAX_NOME_CARREIRA);
        c = getchar();

        /* Verifica se a carreira já existe */
        estado = carreiraExiste(estado, nome);

        /* Caso seja fornecido mais que um argumento: inverso */
        if (c != '\n') {
            char sort_opt[MAX_INVERSO];
            lePalavra(sort_opt, __BUFSIZ__);

            if (strncmp(sort_opt, "inv", MAX_INVERSO) == 0 ||
                strncmp(sort_opt, "inve", MAX_INVERSO) == 0 ||
                strncmp(sort_opt, "inver", MAX_INVERSO) == 0 ||
                strncmp(sort_opt, "invers", MAX_INVERSO) == 0 ||
                strncmp(sort_opt, "inverso", MAX_INVERSO) == 0) {
                    inverso(nome);
            }
            else
                printf("incorrect sort option.\n");

        }
        /* Caso seja fornecido apenas um argumento */
        else if (c == '\n') {
            ungetc(c, stdin);
            /* Caso a carreira já exista, mostra as suas paragens */
            if (estado == YES)
                mostraInfoCarreira(nome);
            /* Caso contrário, adiciona uma nova carreira */
            else if (estado == NO && cont_carreiras < MAX_CARREIRAS) {
                adicionaCarreira(nome);
                cont_carreiras++;
            }
        }
    }
    /* Caso não sejam fornecidos argumentos */
    else if (c == '\n')
        mostraCarreiras();
}

/* Função que engloba outras funções de modo a fazer funcionar o comando p */
void comando_p() {
c = getchar();
    /* Caso sejam fornecidos argumentos */
    if (c != '\n') {
        char nome[MAX_NOME_PARAGEM];
        int estado = NO;

        lePalavra(nome, MAX_NOME_PARAGEM);
        c = getchar();

        /* Verifica se a paragem já existe */
        estado = paragemExiste(estado, nome);
        
        if (c != '\n' && cont_paragens < MAX_PARAGENS) {
            ungetc(c, stdin);
            /* Se a paragem não existir, adiciona uma nova */
            if (estado == NO) {
            adicionaParagem(nome);
            cont_paragens++;
            }
            /* Caso a paragem já exista: ERRO */
            else
                printf("%s: stop already exists.\n", nome);
        }
        else if (c == '\n') {
            /* Se a paragem existir, mostra as respetivas informações */
            if (estado == YES)
                mostraInfoParagem(nome);
            /* Caso contrário: ERRO */
            else
                printf("%s: no such stop.\n", nome);
        }
    }
    /* Caso não sejam fornecidos argumentos */
    else
        mostraParagens();
}

/* Função que engloba outras funções de modo a fazer funcionar o comando l */
int comando_l() {
    int i, estado_carreira = NO, estado_origem = NO, estado_destino = NO;
    char carreira[MAX_NOME_CARREIRA], origem[MAX_NOME_PARAGEM],
    destino[MAX_NOME_PARAGEM];
    double custo, duracao;
    Ligacao nova;

    /* Recolha de dados */
    recolheDadosLigacao(carreira, origem, destino);
    scanf("%lf %lf", &custo, &duracao);

    /* Verificação de erros */
    estado_carreira = carreiraExiste(estado_carreira, carreira);
    estado_origem = paragemExiste(estado_origem, origem);
    estado_destino = paragemExiste(estado_destino, destino);

    if (estado_carreira == NO) {
        printf("%s: no such line.\n", carreira);
        return -1;
    }
    else if (estado_origem == NO) {
        printf("%s: no such stop.\n", origem);
        return -1;
    }
    else if (estado_destino == NO) {
        printf("%s: no such stop.\n", destino);
        return -1;
    }
    else if (custo < 0 || duracao < 0) {
        printf("negative cost or duration.\n");
        return -1;
    }

    for(i = 0; i < cont_carreiras; i++) {
        if (strcmp(carreira, carreiras[i].nome) == 0 &&
            carreiras[i].num_ligacoes != 0) {
            /* Caso nenhum dos nomes das paragens
            coincida com os extremos da carreira */
            if ((strcmp(carreiras[i].origem.nome, origem) != 0 &&
                strcmp(carreiras[i].origem.nome, destino) != 0 &&
                strcmp(carreiras[i].destino.nome, origem) != 0 &&
                strcmp(carreiras[i].destino.nome, destino) != 0) ||
                /* Caso a inserção das paragens seja no meio da carreira */
                (strcmp(origem, carreiras[i].origem.nome) == 0 ||
                strcmp(destino, carreiras[i].destino.nome) == 0)) {
                    if (carreiras[i].ciclo == NO) {
                        printf("link cannot be associated with bus line.\n");
                        return -1;
                    }
            }
            break;
        }

    }

    if(estado_carreira == YES && estado_origem== YES && estado_destino == YES){
        /* Associa informação à nova ligação */
        /* Carreira */
        for(i = 0; i < cont_carreiras; i++) {
            if (strcmp(carreira, carreiras[i].nome) == 0) {
                nova.carreira = carreiras[i];
                nova.num_carreira = i;
                carreiras[i].num_ligacoes++;
            }
        }
        /* Paragem de Origem */
        for(i = 0; i < cont_paragens; i++) {
            if (strcmp(origem, paragens[i].nome) == 0) {
                nova.origem = paragens[i];
            }
        }
        /* Paragem de Destino */
        for(i = 0; i < cont_paragens; i++) {
            if (strcmp(destino, paragens[i].nome) == 0) {
                nova.destino = paragens[i];
            }
        }
        nova.custo = custo;
        nova.duracao = duracao;
        ligacoes[cont_ligacoes] = nova;
    }

    /* Altera as paragens de origem e de destino da carreira  */
    if (nova.carreira.num_ligacoes == 0) {
        carreiras[nova.num_carreira].origem = nova.origem;
        carreiras[nova.num_carreira].destino = nova.destino;
        carreiras[nova.num_carreira].num_paragens += 2;
        carreiras[nova.num_carreira].custo_total += nova.custo;
        carreiras[nova.num_carreira].duracao_total += nova.duracao;
        nova.carreira.num_ligacoes++;
    }
    else {
        /* Ciclo na carreira */
        if (strcmp(nova.origem.nome,
            carreiras[nova.num_carreira].destino.nome) == 0 &&
            strcmp(nova.destino.nome,
            carreiras[nova.num_carreira].origem.nome) == 0) {
                carreiras[nova.num_carreira].destino = nova.destino;
                carreiras[nova.num_carreira].num_paragens++;
                carreiras[nova.num_carreira].ciclo = YES;
            }

        /* Inserção da ligação no final da carreira */
        else if (strcmp(nova.origem.nome,
                carreiras[nova.num_carreira].destino.nome) == 0) {
                    carreiras[nova.num_carreira].destino = nova.destino;
                    carreiras[nova.num_carreira].num_paragens++;
        }
        /* Inserção da ligação no início da carreira */   
        else if (strcmp(nova.destino.nome,
                carreiras[nova.num_carreira].origem.nome) == 0) {
                    carreiras[nova.num_carreira].origem = nova.origem;
                    carreiras[nova.num_carreira].num_paragens++;
        }
        carreiras[nova.num_carreira].custo_total += nova.custo;
        carreiras[nova.num_carreira].duracao_total += nova.duracao;
    }
    cont_ligacoes++;
    
    return 0;
}

/* Função que engloba outras funções de modo a fazer funcionar o comando i */
void comando_i() {
    int i, j, n = 0, cont_car_temp = 0;
    char listaCarreirasTemp[MAX_CARREIRAS][MAX_NOME_CARREIRA];

    /* Percorre o vetor de paragens e verifica
    se há interseções entre carreiras */
    for(i = 0; i < cont_paragens; i++) {
        if (contaCarreirasParagem(i) > 1) {
            for(j = 0; j < cont_ligacoes; j++) {
                if (strcmp(paragens[i].nome, ligacoes[j].origem.nome) == 0 ||
                    strcmp(paragens[i].nome, ligacoes[j].destino.nome) == 0) {
                        n = 0;
                        while (n < cont_car_temp &&
                            strcmp(ligacoes[j].carreira.nome,
                            listaCarreirasTemp[n]) != 0) {
                                n++;
                        }
                        if (n == cont_car_temp) {
                            strcpy(listaCarreirasTemp[cont_car_temp],
                            ligacoes[j].carreira.nome);
                                cont_car_temp++;
                        }
                }
            }

            /* Ordena alfabeticamente a lista temporária
            com os nomes das carreiras */
            ordenaListaCarreiras(listaCarreirasTemp, cont_car_temp);

            /* Imprime todas as carreiras pertencentes à lista */
            printf("%s %d:", paragens[i].nome, contaCarreirasParagem(i));
            for(j = 0; j < cont_car_temp; j++) {
                printf(" %s", listaCarreirasTemp[j]);
            }
            printf("\n");

            cont_car_temp = 0;
        }
    }
}

/* Função principal do programa que engloba todos os comandos*/
int main() {
    while ((comando = getchar()) != EOF) {
        switch (comando) {
            case 'c':
                comando_c();
                break;
            case 'p':
                comando_p();
                break;
            case 'l':
                comando_l();
                break;
            case 'i':
                comando_i();
                break;
            case 'q':
                return 0;
        }
    }
    return 0;
}
