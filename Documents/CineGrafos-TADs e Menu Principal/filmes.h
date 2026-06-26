#ifndef FILME_H
#define FILME_H

#define MAX_TITULO 100
#define MAX_AUTOR 100
#define MAX_CATEGORIA 50
#define MAX_CATS_FILME 5

typedef struct {
    int id;
    char titulo[MAX_TITULO];
    char autor[MAX_AUTOR];
    char categorias[MAX_CATS_FILME][MAX_CATEGORIA];
    int num_categorias;
} Filme;

Filme* criar_filme(int id, const char *titulo, const char *autor);
void adicionar_categoria(Filme *f, const char *categoria);
void imprimir_filme(const Filme *f);
void destruir_filme(Filme *f);

#endif