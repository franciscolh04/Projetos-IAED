/* iaed-23 - ist1106970 - project2 */

/*
* Ficheiro: prototipos.h
* Autor: Francisco Lourenço Heleno - 106970
* Descrição: O ficheiro de protótipos da minha resolução do projeto 2 da UC de IAED
*/

#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H

typedef struct paragem Paragem;
typedef struct ligacao Ligacao;
typedef struct carreira Carreira;

/* Definição da estrutura Paragem */
struct paragem {
    char *nome;
    double latitude, longitude;
    Carreira *primeira_carreira;
    int numCarreiras;
    struct paragem *next;
};

/* Definição da estrutura Ligação */
struct ligacao {
    Carreira *carreira;
    Paragem *origem, *destino;
    double custo, duracao;
    struct ligacao *prev, *next;
};

/* Definição da estrutura Carreira */
struct carreira {
    char *nome;
    double custoTotal;
    double duracaoTotal;
    Ligacao *primeira_ligacao;
    Ligacao *ultima_ligacao;
    int numLigacoes;
    struct carreira *next;
};

/* Função para terminar o programa de forma controlada caso a memória se esgote */
void verificaMemoria(void *ptr);

/* Funções para tratar o comando c */

void mostraCarreira(Carreira *atual);
void mostraLigacoesCarreira(Carreira *carreira, int inverso);
void listaCarreiras(Carreira **head);
Carreira* encontraCarreira(Carreira **head, char *nomeCarreira);
int encontraParagemCarreira(Carreira *carreira, Paragem *paragem);
void criaCarreira(Carreira **head, char *nomeCarreira);
int verificaInversoOk(char *s);
void carreiras(Carreira **head);

/* Funções para tratar o comando p */

void mostraParagem(Paragem *atual);
void listaParagens(Paragem **head);
Paragem* encontraParagem(Paragem **head, char *nomeParagem);
void criaParagem(Paragem **head, char *nomeParagem, double latitude, double longitude);
void paragens(Paragem **head);

/* Funções para tratar o comando l */

Ligacao* criaLigacao(Carreira *carreira, Paragem *origem, Paragem *destino, double custo, double duracao);
void copiaLigacao(Ligacao *nova, Ligacao *atual);
void acrescentaLigacaoFim(Carreira *carreira, Ligacao *ligacao);
void acrescentaLigacaoInicio(Carreira *carreira, Ligacao *ligacao);
void adicionaCarreiraParagem(Paragem *paragem, Carreira *carreira);
void adicionaPrimeiraLigacao(Carreira *carreira, Paragem *origem, Paragem *destino, double custo, double duracao);
void adicionaLigacao(Carreira *carreira, Paragem *origem, Paragem *destino, double custo, double duracao);
void leNomesComando(char **nomeCarreira, char **nomeOrigem, char **nomeDestino);
void ligacoes(Carreira **headCarreiras, Paragem **headParagens);

/* Funções para tratar o comando i */

void ordenaCarreiras(Paragem *paragem);
void intersecoes(Paragem **headParagens);

/* Funções para tratar o comando r */

Carreira* encontraCarreiraAnterior(Carreira **head, Carreira *carreira);
void removeCarreiraLista(Paragem *paragem, Carreira *carreira);
void removeCarreiraParagens(Paragem **headParagens, Carreira *carreira);
void removeCarreiraListaLigada(Carreira **headCarreiras, Carreira *carreira);
void apagaLigacoesCarreira(Carreira *carreira);
void libertaMemoriaCarreira(Carreira **headCarreiras, Paragem **headParagens, Carreira *carreira);
void removeCarreira(Carreira **headCarreiras, Paragem **headParagens);

/* Funções para tratar o comando e */

Paragem* encontraParagemAnterior(Paragem **head, Paragem *paragem);
Ligacao* paragemExisteCarreira(Carreira *carreira, Paragem *paragem);
void reiniciaCarreira(Carreira *carreira, Paragem *paragem);
void alteraTresLigacoes(Ligacao *ligacao, Ligacao *nova);
void removeParagemPonta(Ligacao *ligacao, Paragem *paragem);
void removeParagemMeio(Ligacao *ligacao);
void juntaLigacoes(Ligacao *ligacao, Paragem *paragem);
void atualizaLigacoes(Carreira **headCarreiras, Paragem *paragem);
void removeCarreirasParagem(Paragem *paragem);
void removeParagemListaLigada(Paragem **headParagens, Paragem *paragem);
void libertaMemoriaParagem(Paragem **headParagens, Carreira **headCarreiras, Paragem *paragem);
void eliminaParagem(Paragem **headParagens, Carreira **headCarreiras);

/* Funções para tratar o comando a */

void limpaCarreira(Carreira **headCarreiras, Carreira *carreira);
void limpaParagem(Paragem **headParagens, Paragem *paragem);
void apagaSistema(Paragem **headParagens, Carreira **headCarreiras);

#endif
