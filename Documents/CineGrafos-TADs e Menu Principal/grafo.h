#ifndef GRAFO_H
#define GRAFO_H

#include "filme.h"
#include "utilizador.h"

#define MAX_VERTICES 300

typedef enum { VERTICE_USER, VERTICE_FILME } TipoVertice;

typedef struct {
    TipoVertice tipo;
    int id_origem; 
    void *dado;    
    int ativo;
} Vertice;

typedef struct Aresta {
    int destino_idx;
    int peso; // Avaliacao de 1 a 5
    struct Aresta *proximo;
} Aresta;

typedef struct {
    Vertice vertices[MAX_VERTICES];
    Aresta *adjacencias[MAX_VERTICES];
    int total_vertices;
} Grafo;

Grafo* criar_grafo();
int grafo_adicionar_vertice(Grafo *g, TipoVertice tipo, void *dado, int id_origem);
int grafo_encontrar_vertice(const Grafo *g, TipoVertice tipo, int id_origem);
int grafo_adicionar_avaliacao(Grafo *g, int id_user, int id_filme, int nota);
void grafo_listar_avaliados_por_user(const Grafo *g, int id_user);
void grafo_recomendar_bfs(const Grafo *g, int id_user);
float grafo_calcular_media_filme(const Grafo *g, int id_filme);
void destruir_grafo(Grafo *g);

#endif