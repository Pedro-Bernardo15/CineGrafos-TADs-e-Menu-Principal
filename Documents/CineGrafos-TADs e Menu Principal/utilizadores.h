#ifndef UTILIZADOR_H
#define UTILIZADOR_H

#define MAX_NOME 80
#define MAX_EMAIL 80

typedef struct {
    int id;
    char nome[MAX_NOME];
    char email[MAX_EMAIL];
} Utilizador;

Utilizador* criar_utilizador(int id, const char *nome, const char *email);
void destruir_utilizador(Utilizador *u);

#endif