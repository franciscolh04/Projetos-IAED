/* iaed-23 - ist1106970 - project2 */

/*
* Ficheiro: project2.c
* Autor: Francisco Lourenço Heleno - 106970
* Descrição: O ficheiro principal da minha resolução do projeto 2 da UC de IAED
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constantes.h"
#include "prototipos.h"

/* Função para terminar o programa de forma
controlada caso a memória se esgote */
void verificaMemoria(void *ptr) {
    if (ptr == NULL) {
        printf("No memory.\n");
        exit(1);
    }
}

/* Funções para parsing do input */

/* Le espaços. Devolve 0 se chegou ao 
final de linha ou 1 caso contrário. */
int leEspacos() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
        return 0;
    ungetc(c, stdin);
    return 1;
}

/* Le um nome para a string 
que recebe como parametro. */
char* leNome() {
    char *s, buffer[MAX];
    int i = 0, c;

    buffer[0] = getchar();
    if (buffer[0] != '"') {
        i = 1;
        while ((c = getchar()) != ' ' && c != '\t' && c != '\n')
            buffer[i++] = c;
        ungetc(c, stdin);
    }
    else {
        while((c = getchar()) != '"')
            buffer[i++] = c;
    }
    buffer[i++] = '\0';

    s = (char *) malloc(sizeof(char) * (strlen(buffer) + 1));
    verificaMemoria(s);
    strcpy(s, buffer);

    return s;
}

/* Le todo o texto até ao final de linha. */
void leAteFinalLinha() {
    char c;
    while ((c = getchar()) != '\n');
}


/* Funções para tratar comandos */

/* Funções para tratar as carreiras */

/* Mostra no output a carreira cujo ponteiro é atual */
void mostraCarreira(Carreira *atual) {
    int numLigacoes = atual->numLigacoes;

    printf("%s ", atual->nome);
    if (numLigacoes > 0) {
        Paragem *origem, *destino;
        Ligacao *ligacaoAtual = atual->primeira_ligacao;

        origem = ligacaoAtual->origem;
        /* Encontra paragem de destino da carreira */
        while (ligacaoAtual->next != NULL) {
            ligacaoAtual = ligacaoAtual->next;
        }
        destino = ligacaoAtual->destino;

        /* Imprime informações */
        printf("%s %s ", origem->nome, destino->nome);
        printf("%d ", numLigacoes+1);
        printf("%.2f ", atual->custoTotal);
        printf("%.2f\n", atual->duracaoTotal);
    }
    else 
        printf("%d %.2f %.2f\n", 0, 0.0, 0.0);
}

/* Mostra as ligações da carreira com indice i. */
void mostraLigacoesCarreira(Carreira *carreira, int inverso) {
    int numLigacoes = carreira->numLigacoes;
    Ligacao *ligacaoAtual;

    if (numLigacoes == 0) 
        return;
    /* Ordem normal */
    if (inverso == FALSO) {
        for (ligacaoAtual = carreira->primeira_ligacao;
             ligacaoAtual != NULL; ligacaoAtual = ligacaoAtual->next) {
                printf("%s, ", ligacaoAtual->origem->nome);
                if (ligacaoAtual->next == NULL)
                    printf("%s\n", ligacaoAtual->destino->nome);
        }
    }
    /* Ordem inversa */
    else {
        for (ligacaoAtual = carreira->ultima_ligacao;
             ligacaoAtual != NULL; ligacaoAtual = ligacaoAtual->prev) {
                printf("%s, ", ligacaoAtual->destino->nome);
                if (ligacaoAtual->prev == NULL)
                    printf("%s\n", ligacaoAtual->origem->nome);
        }
    }
}

/* Mostra todas as carreiras. */
void listaCarreiras(Carreira **head) {
    Carreira *atual;

    for (atual = *head; atual != NULL; atual = atual->next)
        mostraCarreira(atual);
}

/* Procura uma carreira por nome.
   Devolve o ponteiro para a carreira ou NULL se não existe. */
Carreira* encontraCarreira(Carreira **head, char *nomeCarreira) {
    Carreira *atual = *head, *aux = NULL;

    for (atual = *head; atual != NULL; atual = atual->next)
        if (strcmp(nomeCarreira, atual->nome) == 0)
            return atual;
    return aux;
}

/* Procura se uma paragem existe numa carreira.
   Se existir, devolve VERDADE. Caso contrário, devolve NAO_ENCONTRADO. */
int encontraParagemCarreira(Carreira *carreira, Paragem *paragem) {
    Ligacao *ligacaoAtual;

    for(ligacaoAtual = carreira->primeira_ligacao;
        ligacaoAtual != NULL; ligacaoAtual = ligacaoAtual->next)
            if (strcmp(ligacaoAtual->origem->nome, paragem->nome) == 0 ||
                strcmp(ligacaoAtual->destino->nome, paragem->nome) == 0)
                    return VERDADE;

    return NAO_ENCONTRADO;
}

/* Cria uma carreira nova. */
void criaCarreira(Carreira **head, char *nomeCarreira) {
    /* Aloca espaço necessário */
    Carreira *novaCarreira = (Carreira *) malloc(sizeof(Carreira));
    verificaMemoria(novaCarreira);
    novaCarreira->nome = (char *) malloc(sizeof(char) * strlen(nomeCarreira)+1);
    verificaMemoria(novaCarreira->nome);
    
    /* Copia informação */
    strcpy(novaCarreira->nome, nomeCarreira);
    novaCarreira->custoTotal = 0.0;
    novaCarreira->duracaoTotal = 0.0;
    novaCarreira->numLigacoes = 0;
    novaCarreira->primeira_ligacao = NULL;
    novaCarreira->ultima_ligacao = NULL;
    novaCarreira->next = NULL;

    /* Muda lista ligada */
    if (*head == NULL) {
        *head = novaCarreira;
    }
    else {
        Carreira *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novaCarreira;
    }
}

/* Verifica se a string é um prefixo de tamanho pelo menos 3 da
   palavra inverso. */
int verificaInversoOk(char *s) {
    char inv[] = "inverso";
    int size = strlen(s), i;

    if (size < 3 || size > 7)
        return FALSO;
    for (i = 0; i < size; i++)
        if (inv[i] != s[i])
            return FALSO;
    return VERDADE;
}

/* Função para tratar do comando 'c'. */
void carreiras(Carreira **head) {
    char *s, *inverso;
    int fimLinha = leEspacos();
    Carreira *aux;
    
    if (!fimLinha) {
        listaCarreiras(head);
        return;
    }
    s = leNome();
    fimLinha = leEspacos(); 
    aux = encontraCarreira(head, s);
    if (!fimLinha) {
        if (aux == NULL)
            criaCarreira(head, s);
        else
            mostraLigacoesCarreira(aux, FALSO);
    }
    else {
        inverso = leNome();
        if (verificaInversoOk(inverso)) {
            mostraLigacoesCarreira(aux, VERDADE);
        }
        else
            printf("incorrect sort option.\n");
        leAteFinalLinha();
        free(inverso);
    }
    free(s);
}


/* Funções para tratar as paragens */

/* Mostra uma paragem */
void mostraParagem(Paragem *atual) {
    printf("%s: %16.12f %16.12f %d\n", atual->nome, atual->latitude,
        atual->longitude, atual->numCarreiras);
}

/* Mostra todas as paragens. */
void listaParagens(Paragem **head) {
    Paragem *atual;

    for(atual = *head; atual != NULL; atual = atual->next) {
        mostraParagem(atual);
    }
}

/* Verifica se existe uma paragem com um determinado nome.
   Se existir devolve o ponteiro para a paragem. Caso contrário, devolve NULL. */
Paragem* encontraParagem(Paragem **head, char *nomeParagem) {
    Paragem *atual = *head, *aux = NULL;

    for (atual = *head; atual != NULL; atual = atual->next)
        if (strcmp(nomeParagem, atual->nome) == 0)
            return atual;
    return aux;
}

/* Cria uma nova paragem. */
void criaParagem(Paragem **head, char *nomeParagem, 
                 double latitude, double longitude) {
    /* Aloca espaço necessário */
    Paragem *novaParagem = (Paragem *) malloc(sizeof(Paragem));
    verificaMemoria(novaParagem);
    novaParagem->nome = (char *) malloc(sizeof(char) * strlen(nomeParagem) + 1);
    verificaMemoria(novaParagem->nome);

    /* Copia informação */
    strcpy(novaParagem->nome, nomeParagem);
    novaParagem->latitude = latitude;
    novaParagem->longitude = longitude;
    novaParagem->numCarreiras = 0;
    novaParagem->primeira_carreira = NULL;
    novaParagem->next = NULL;

    /* Muda lista ligada */
    if (*head == NULL) {
        *head = novaParagem;
    }
    else {
        Paragem *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novaParagem;
    }
}

/* Função para tratar o comando 'p'. */
void paragens(Paragem **head) {
    char *s;
    int fimLinha = leEspacos();
    Paragem *aux;
    
    if (!fimLinha) {
        listaParagens(head);
        return;
    }
    s = leNome();
    fimLinha = leEspacos();
    if (!fimLinha) {
        if ((aux = encontraParagem(head, s)) == NULL)
            printf("%s: no such stop.\n", s);
        else   
            printf("%16.12f %16.12f\n", aux->latitude, aux->longitude);
    }
    else {
        double latitude, longitude;
        scanf("%lf%lf", &latitude, &longitude);
        if (encontraParagem(head, s) == NULL)
            criaParagem(head, s, latitude, longitude);
        else
            printf("%s: stop already exists.\n", s);
        leAteFinalLinha();
    }
    free(s);
}

/* Funções para tratar as Ligações */

/* Adiciona uma nova ligação. */
Ligacao* criaLigacao(Carreira *carreira, Paragem *origem,
                 Paragem *destino, double custo, double duracao) {
    Ligacao *novaLigacao = (Ligacao *) malloc(sizeof(Ligacao));
    verificaMemoria(novaLigacao);

    novaLigacao->carreira = carreira;
    novaLigacao->origem = origem;
    novaLigacao->destino = destino;
    novaLigacao->custo = custo;
    novaLigacao->duracao = duracao;
    novaLigacao->next = NULL;
    novaLigacao->prev = NULL;

    return novaLigacao;
}

/* Copia as informações de uma uma ligação para outra */
void copiaLigacao(Ligacao *nova, Ligacao *atual) {
    nova->carreira = atual->carreira;
    nova->origem = atual->origem;
    nova->destino = atual->destino;
    nova->custo = atual->custo;
    nova->duracao = atual->duracao;
    nova->prev = atual->prev;
    nova->next = atual->next;
}

/* Acrescenta uma nova ligação no fim de uma carreira. */
void acrescentaLigacaoFim(Carreira *carreira, Ligacao *ligacao) {
    int numLigacoesCarreira = carreira->numLigacoes;
    Ligacao *novaLigacao = (Ligacao *) malloc(sizeof(Ligacao));
    verificaMemoria(novaLigacao);
    copiaLigacao(novaLigacao, ligacao);

    /* Muda lista ligada */
    if (numLigacoesCarreira == 0) {
        carreira->primeira_ligacao = novaLigacao;
        carreira->primeira_ligacao->prev = NULL;
        carreira->primeira_ligacao->next = NULL;
    }
    else {
        Ligacao *temp = carreira->primeira_ligacao;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        novaLigacao->prev = temp;
        temp->next = novaLigacao;
    }
    carreira->ultima_ligacao = novaLigacao;
    carreira->custoTotal += ligacao->custo;
    carreira->duracaoTotal += ligacao->duracao;
    carreira->numLigacoes++;
    free(ligacao);
}

/* Acrescenta uma nova ligação no início de uma carreira. */
void acrescentaLigacaoInicio(Carreira *carreira, Ligacao *ligacao) {
    Ligacao *novaLigacao = (Ligacao *) malloc(sizeof(Ligacao));
    verificaMemoria(novaLigacao);
    copiaLigacao(novaLigacao, ligacao);

    /* Muda lista ligada */
    novaLigacao->prev = NULL;
    novaLigacao->next = carreira->primeira_ligacao;
    carreira->primeira_ligacao->prev = novaLigacao;
    carreira->primeira_ligacao = novaLigacao;
    carreira->custoTotal += ligacao->custo;
    carreira->duracaoTotal += ligacao->duracao;
    carreira->numLigacoes++;
    free(ligacao);
}

/* Adiciona que existe uma nova carreira associada a uma paragem. */
void adicionaCarreiraParagem(Paragem *paragem, Carreira *carreira) {
    int numCarreiras = paragem->numCarreiras;
    Carreira *copiaCarreira = (Carreira *) malloc(sizeof(Carreira));
    verificaMemoria(copiaCarreira);
    copiaCarreira->nome = (char *) malloc(sizeof(char) * (strlen(carreira->nome)+1));
    verificaMemoria(copiaCarreira->nome);
    strcpy(copiaCarreira->nome, carreira->nome);

    /* Muda lista de carreiras da paragem */
    if (numCarreiras == 0) {
        paragem->primeira_carreira = copiaCarreira;
        paragem->primeira_carreira->next = NULL;
    }
    else {
        Carreira *temp = paragem->primeira_carreira;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        copiaCarreira->next = NULL;
        temp->next = copiaCarreira;
    }
    paragem->numCarreiras++;
}


/* Adiciona primeira ligacao da carreira. */
void adicionaPrimeiraLigacao(Carreira *carreira, Paragem *origem,
            Paragem *destino, double custo, double duracao) {
    Ligacao *nova;
    
    adicionaCarreiraParagem(origem, carreira);
    if (strcmp(origem->nome, destino->nome) != 0)
        adicionaCarreiraParagem(destino, carreira);
    nova = criaLigacao(carreira, origem, destino, custo, duracao);
    acrescentaLigacaoFim(carreira, nova);  
}


/* Adiciona uma nova ligação a uma carreira. */
void adicionaLigacao(Carreira *carreira, Paragem *origem, Paragem *destino,
                     double custo, double duracao) {

    int numLigacoesCarreira = carreira->numLigacoes;
    
    if (numLigacoesCarreira == 0) 
        adicionaPrimeiraLigacao(carreira, origem, destino, custo, duracao);
    else {
        Ligacao *nova;

        Ligacao *PrimeiraLigacao = carreira->primeira_ligacao;
        Ligacao *UltimaLigacao = carreira->ultima_ligacao;

        if (strcmp(origem->nome, UltimaLigacao->destino->nome) == 0) {
            if (encontraParagemCarreira(carreira, destino) == NAO_ENCONTRADO) {
                adicionaCarreiraParagem(destino, carreira);
            }
            nova = criaLigacao(carreira, origem, destino, custo, duracao);
            acrescentaLigacaoFim(carreira, nova);
        }
        else if (strcmp(destino->nome, PrimeiraLigacao->origem->nome) == 0) {
            if (encontraParagemCarreira(carreira, origem) == NAO_ENCONTRADO)
                adicionaCarreiraParagem(origem, carreira);
            nova = criaLigacao(carreira, origem, destino, custo, duracao);
            acrescentaLigacaoInicio(carreira, nova);
        }
        else 
            printf("link cannot be associated with bus line.\n");
    }
}

/* Le nome de carreira e paragens de uma ligacao. */
void leNomesComando(char **nomeCarreira, char **nomeOrigem, char **nomeDestino){
    leEspacos();
    *nomeCarreira = leNome();
    leEspacos();
    *nomeOrigem = leNome();
    leEspacos();
    *nomeDestino = leNome();
}

/* Função para tratar o comando 'l'. */
void ligacoes(Carreira **headCarreiras, Paragem **headParagens) {
    char *nomeCarreira, *nomeOrigem, *nomeDestino;
    double custo, duracao;
    Carreira *carreira;

    leNomesComando(&nomeCarreira, &nomeOrigem, &nomeDestino);
    scanf("%lf%lf", &custo, &duracao);
    leAteFinalLinha();

    carreira = encontraCarreira(headCarreiras, nomeCarreira);
    if (carreira == NULL)
        printf("%s: no such line.\n", nomeCarreira);
    else {
        Paragem *origem = encontraParagem(headParagens, nomeOrigem);
        if (origem == NULL) 
            printf("%s: no such stop.\n", nomeOrigem);
        else {
            Paragem *destino = encontraParagem(headParagens, nomeDestino);
            if (destino == NULL)
                printf("%s: no such stop.\n", nomeDestino);
            else if (custo < 0.0 || duracao < 0.0)
                    printf("negative cost or duration.\n");
            else {
                adicionaLigacao(carreira, origem, destino, custo, duracao);
            }
	    }
    }
    free(nomeCarreira);
    free(nomeOrigem);
    free(nomeDestino);
}

/* Funções para tratar as interseções */

/* Aplica Bubble Sort a um vector de identificadores de carreiras. */
void ordenaCarreiras(Paragem *paragem) {
    Carreira *temp;
    int houveTroca = VERDADE;
    Carreira **atual;

    while (houveTroca) {
        houveTroca = FALSO;
        atual = &paragem->primeira_carreira;

        while (*atual != NULL && (*atual)->next != NULL) {
            if (strcmp((*atual)->nome, (*atual)->next->nome) > 0) {
                temp = *atual;
                *atual = temp->next;
                temp->next = (*atual)->next;
                (*atual)->next = temp;
                houveTroca = VERDADE;
            }
            atual = &(*atual)->next;
        }
    }
}


/* Função para tratar o comando 'i'. */
void intersecoes(Paragem **headParagens) {
    int numCarreiras;
    Paragem *paragemAtual;
    Carreira *carreiraAtual;
    leAteFinalLinha();

    for (paragemAtual = *headParagens;
         paragemAtual != NULL; paragemAtual = paragemAtual->next) {
            numCarreiras = paragemAtual->numCarreiras;
            /* Caso haja interseções na paragem atual */
            if (numCarreiras > 1) {
                printf("%s %d:", paragemAtual->nome, numCarreiras);
                ordenaCarreiras(paragemAtual);
                for (carreiraAtual = paragemAtual->primeira_carreira;
                    carreiraAtual != NULL; carreiraAtual = carreiraAtual->next){
                        printf(" %s", carreiraAtual->nome);
                }
                printf("\n");
            }
    }
}

/* Funções para tratar as remoções de carreira. */

/* Função que encontra a carreira anterior a uma
carreira fornecida numa lista ligada */
Carreira* encontraCarreiraAnterior(Carreira **head, Carreira *carreira) {
    Carreira *atual, *aux = NULL;

    for (atual = *head; atual != NULL; atual = atual->next)
        if (strcmp(atual->next->nome, carreira->nome) == 0)
            return atual;
    return aux;
}

/* Função que remove uma carreira da lista de carreiras de uma paragem */
void removeCarreiraLista(Paragem *paragem, Carreira *carreira) {
    Carreira *atual = paragem->primeira_carreira, *achouCarreira = NULL;

    /* Percorrer lista de carreiras de uma paragem e ver se é igual*/
    while (atual!= NULL) {
        if (strcmp(atual->nome, carreira->nome) == 0) {
            achouCarreira = atual;
        }
        atual = atual->next;
    }

    /* Mudar lista ligada */
    if (achouCarreira == NULL) {
        return;
    }

    if (strcmp(carreira->nome, paragem->primeira_carreira->nome) != 0)  {
        Carreira *anterior = encontraCarreiraAnterior(
            &paragem->primeira_carreira,achouCarreira);
        anterior->next = achouCarreira->next;
    }
    /* Caso a carreira seja a head da lista ligada */
    else
        paragem->primeira_carreira = achouCarreira->next;
    
    /* Liberta a memória */
    free(achouCarreira->nome);
    free(achouCarreira);
}

/* Função que remove uma carreira das listas de carreiras de todas as paragens */
void removeCarreiraParagens(Paragem **headParagens, Carreira *carreira) {
    Paragem *atual = *headParagens;

    /* Percorre todas as paragens do sistema */
    while(atual != NULL) {
        if (atual->numCarreiras > 0) {
            removeCarreiraLista(atual, carreira);
            atual->numCarreiras--;
        }
        atual = atual->next;
    }
}

/* Função que remove uma carreira das listas ligada de carreiras */
void removeCarreiraListaLigada(Carreira **headCarreiras, Carreira *carreira) {
    if (strcmp(carreira->nome, (*headCarreiras)->nome) != 0)  {
        Carreira *anterior = encontraCarreiraAnterior(headCarreiras, carreira);
        anterior->next = carreira->next;
    }
    /* Caso a carreira seja a head da lista ligada */
    else
        *headCarreiras = carreira->next;
}

/* Função que apaga todas as ligações de uma carreira */
void apagaLigacoesCarreira(Carreira *carreira) {
    Ligacao *atual = carreira->primeira_ligacao, *prox;

    while(atual != NULL) {
        prox = atual->next;
        free(atual);
        atual = prox;
    }
    carreira->numLigacoes--;
}

/* Função que liberta toda a memória de uma carreira */
void libertaMemoriaCarreira(Carreira **headCarreiras, Paragem **headParagens,
                Carreira *carreira) {
    removeCarreiraListaLigada(headCarreiras, carreira);

    if (carreira->numLigacoes > 0) {
        removeCarreiraParagens(headParagens, carreira);
        apagaLigacoesCarreira(carreira);
    }
    free(carreira->nome);
    free(carreira);
}

/* Função para tratar o comando 'r'. */
void removeCarreira(Carreira **headCarreiras, Paragem **headParagens) {
    char *nomeCarreira;
    Carreira *carreira;

    leEspacos();
    nomeCarreira = leNome();
    carreira = encontraCarreira(headCarreiras, nomeCarreira);
    leAteFinalLinha();

    if (carreira == NULL) {
        printf("%s: no such line.\n", nomeCarreira);
        return;
    }
    libertaMemoriaCarreira(headCarreiras, headParagens, carreira);
    free(nomeCarreira);
}

/* Funções para tratar as eliminações de paragens. */

/* Função que encontra a paragem anterior a uma paragem
fornecida numa lista ligada */
Paragem* encontraParagemAnterior(Paragem **head, Paragem *paragem) {
    Paragem *atual, *aux = NULL;

    for (atual = *head; atual != NULL; atual = atual->next)
        if (strcmp(atual->next->nome, paragem->nome) == 0)
            return atual;
    return aux;
}

/* Função que devolve a primeira ligação de uma carreira onde esteja uma certa paragem.
    Caso contrário, devolve NULL */
Ligacao* paragemExisteCarreira(Carreira *carreira, Paragem *paragem) {
    Ligacao *atual = carreira->primeira_ligacao, *aux = NULL;

    while(atual != NULL) {
        if (strcmp(atual->origem->nome, paragem->nome) == 0 ||
            strcmp(atual->destino->nome, paragem->nome) == 0) {
                return atual;
        }
        atual = atual->next;
    }
    return aux;
}

/* Função que reinicia uma carreira */
void reiniciaCarreira(Carreira *carreira, Paragem *paragem) {
    Ligacao *primeira = carreira->primeira_ligacao;

    /* Casos em que a única ligação da carreira tem duas paragens diferentes */
    if (strcmp(primeira->origem->nome, paragem->nome) != 0) {
        removeCarreiraLista(primeira->origem, carreira);
        primeira->origem->numCarreiras--;
    }
    else if (strcmp(primeira->destino->nome, paragem->nome) != 0) {
        removeCarreiraLista(primeira->destino, carreira);
        primeira->destino->numCarreiras--;
    }
    carreira->primeira_ligacao = NULL;
    carreira->ultima_ligacao = NULL;
    carreira->custoTotal = 0;
    carreira->duracaoTotal = 0;
}

/* Função que altera as ligações caso o número das ligações da carreira seja 3 */
void alteraTresLigacoes(Ligacao *ligacao, Ligacao *nova) {
    Carreira *carreira = ligacao->carreira;
    Ligacao *anterior, *proxima;

    anterior = ligacao;
    proxima = ligacao->next;

    if (anterior->prev == NULL) {
        proxima->next->prev = nova;
        nova->next = proxima->next;
        nova->prev = NULL;
        carreira->primeira_ligacao = nova;
    }
    else if (proxima->next == NULL) {
        anterior->prev->next = nova;
        nova->prev = anterior->prev;
        nova->next = NULL;
        carreira->ultima_ligacao = nova;
    }
}

/* Função que remove uma paragem de uma das pontas da carreira */
void removeParagemPonta(Ligacao *ligacao, Paragem *paragem) {
    Carreira *carreira = ligacao->carreira;
    Ligacao *primeira = carreira->primeira_ligacao;
    Ligacao *ultima = carreira->ultima_ligacao;

    /* Altera lista ligada */
    if (strcmp(paragem->nome, primeira->origem->nome) == 0) {
        carreira->primeira_ligacao = primeira->next;
        carreira->primeira_ligacao->prev = NULL;
    }
    else if (strcmp(paragem->nome, ultima->destino->nome) == 0) {
        carreira->ultima_ligacao = ultima->prev;
        carreira->ultima_ligacao->next = NULL;
    }
    carreira->custoTotal -= ligacao->custo;
    carreira->duracaoTotal -= ligacao->duracao;
}

/* Função que remove uma paragem do meio da carreira */
void removeParagemMeio(Ligacao *ligacao) {
    Carreira *carreira = ligacao->carreira;
    Ligacao *anterior, *proxima, *nova;
    double novo_custo, nova_duracao;

    anterior = ligacao;
    proxima = ligacao->next;
    novo_custo = anterior->custo + proxima->custo;
    nova_duracao = anterior->duracao + proxima->duracao;

    nova = criaLigacao(carreira, anterior->origem, proxima->destino,
                        novo_custo, nova_duracao);
    
    /* Caso a carreira tenha 2 ligações */
    if (carreira->numLigacoes == 2) {
        carreira->primeira_ligacao = nova;
        carreira->ultima_ligacao = nova;
        nova->prev = NULL;
    }
    /* Caso a carreira tenha 3 ligações */
    else if (carreira->numLigacoes == 3) {
        alteraTresLigacoes(ligacao, nova);
    }
    /* Caso a carreira tenha mais que 3 ligações */
    else {
        if (anterior->prev != NULL) {
            anterior->prev->next = nova;
            nova->prev = anterior->prev;
        } 
        else
            carreira->primeira_ligacao = nova;

        if (proxima->next != NULL) {
            proxima->next->prev = nova;
            nova->next = proxima->next;
        }
        else
            carreira->ultima_ligacao = nova;
    }
    free(anterior);
    free(proxima);
}

/* Função que junta uma ligação a outra, criando assim uma nova */
void juntaLigacoes(Ligacao *ligacao, Paragem *paragem) {
    Carreira *carreira = ligacao->carreira;
    Ligacao *primeira = carreira->primeira_ligacao;
    Ligacao *ultima = carreira->ultima_ligacao;

    /* Caso a carreira só tenha uma ligação, reinicia-a */
    if (carreira->numLigacoes == 1) {
        reiniciaCarreira(carreira, paragem);
        free(ligacao);
    }
    /* Caso seja para remover uma paragem da ponta */
    else if (strcmp(paragem->nome, primeira->origem->nome) == 0 ||
             strcmp(paragem->nome, ultima->destino->nome) == 0) {
                removeParagemPonta(ligacao, paragem);
                /*free(ligacao);*/ 
        /* Com este free descomentado, os frees batiam certo com os allocs,
        mas tinha erros de memória, pelo que passava a menos testes privados.
        Assim, optei por deixá-lo comentado, uma vez que não consegui perceber o erro*/
    }
    /* Caso seja para remover uma paragem do meio da carreira */
    else {
        removeParagemMeio(ligacao);
    }
    carreira->numLigacoes--;
}

/* Função que faz as mudanças nas ligações para eliminar uma paragem */
void atualizaLigacoes(Carreira **headCarreiras, Paragem *paragem) {
    Carreira *atual = *headCarreiras;
    Ligacao *ligacao;

    while (atual != NULL) {
        ligacao = paragemExisteCarreira(atual, paragem);
        /* Enquanto a paragem a remover ainda estiver presente na carreira */
        while (ligacao != NULL) {
            if (atual->numLigacoes > 0) {
                juntaLigacoes(ligacao, paragem);
                ligacao = paragemExisteCarreira(atual, paragem);
            }
        }
        atual = atual->next;
    }
}

/* Função que remove a lista de carreiras de uma paragem */
void removeCarreirasParagem(Paragem *paragem) {
    Carreira *atual = paragem->primeira_carreira, *aux = NULL;

    while (atual != NULL) {
        aux = atual->next;
        free(atual->nome);
        free(atual);
        atual = aux;
    }
}

/* Função que remove paragem da lista ligada de paragens */
void removeParagemListaLigada(Paragem **headParagens, Paragem *paragem) {
    if (strcmp(paragem->nome, (*headParagens)->nome) != 0)  {
        Paragem *anterior = encontraParagemAnterior(headParagens, paragem);
        anterior->next = paragem->next;
    }
    /* Caso a carreira seja a head da lista ligada */
    else
        *headParagens = paragem->next;

}

/* Função que liberta toda a memória de uma paragem */
void libertaMemoriaParagem(Paragem **headParagens, Carreira **headCarreiras,
                Paragem *paragem) {

    atualizaLigacoes(headCarreiras, paragem);
    removeParagemListaLigada(headParagens, paragem);
    removeCarreirasParagem(paragem);

    free(paragem->nome);
    free(paragem);
}

/* Função para tratar o comando 'e'. */
void eliminaParagem(Paragem **headParagens, Carreira **headCarreiras) {
    char *nomeParagem;
    Paragem *paragem;

    leEspacos();
    nomeParagem = leNome();
    paragem = encontraParagem(headParagens, nomeParagem);
    leAteFinalLinha();

    if (paragem == NULL) {
        printf("%s: no such stop.\n", nomeParagem);
        return;
    }
    libertaMemoriaParagem(headParagens, headCarreiras, paragem);
    free(nomeParagem);
}

/* Funções para tratar da eliminação do sistema. */

/* Função que limpa toda a informação de uma carreira */
void limpaCarreira(Carreira **headCarreiras, Carreira *carreira) {
    removeCarreiraListaLigada(headCarreiras, carreira);
    apagaLigacoesCarreira(carreira);
    free(carreira->nome);
    free(carreira);
}

/* Função que limpa toda a informação de uma paragem */
void limpaParagem(Paragem **headParagens, Paragem *paragem) {
    removeParagemListaLigada(headParagens, paragem);
    free(paragem->nome);
    free(paragem);
}

/* Função para tratar o comando 'a'. */
void apagaSistema(Paragem **headParagens, Carreira **headCarreiras) {
    Paragem *paragemAtual = *headParagens, *paragemAux;
    Carreira *carreiraAtual = *headCarreiras, *carreiraAux;

    /* Elimina carreiras */
    while (carreiraAtual != NULL) {
        carreiraAux = carreiraAtual->next;
        limpaCarreira(headCarreiras, carreiraAtual);
        carreiraAtual = carreiraAux;
    }
    /* Elimina paragens */
    while (paragemAtual != NULL) {
        paragemAux = paragemAtual->next;
        limpaParagem(headParagens, paragemAtual);
        paragemAtual = paragemAux;
    }
}


/* Função MAIN */
int main() {
    int c;
    Carreira *headCarreiras = NULL;
    Paragem *headParagens = NULL;

    do {
        c = getchar();
        switch(c) {
            case 'c':
                carreiras(&headCarreiras);
                break;
            case 'p':
                paragens(&headParagens);
                break;
            case 'l':
                ligacoes(&headCarreiras, &headParagens);
                break;
            case 'i':
                intersecoes(&headParagens);
                break;    
            case 'r':
                removeCarreira(&headCarreiras, &headParagens);
                break;
            case 'e':
                eliminaParagem(&headParagens, &headCarreiras); 
                break;   
            case 'a':
                apagaSistema(&headParagens, &headCarreiras);
                break;
	    case 'q':
	        break;
            default:
	        /* Ignorar linhas em branco */
	        if (c == ' ' || c == '\t' || c == '\n') break;
        }
    } while (c != 'q');
    apagaSistema(&headParagens, &headCarreiras);
    return 0;
}
