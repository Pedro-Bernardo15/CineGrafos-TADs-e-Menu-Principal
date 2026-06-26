#ifndef HASH_H
#define HASH_H

#include "filme.h"

#define TAMANHO_HASH 97

typedef struct NoHash {
    Filme *filme;
    struct NoHash *proximo;
} NoHash;

typedef struct {
    NoHash *tabela[TAMANHO_HASH];
} HashTable;

HashTable* criar_hash();
unsigned long djb2_hash(const char *str);
int hash_inserir(HashTable *ht, Filme *f);
Filme* hash_pesquisar(HashTable *ht, const char *titulo);
void hash_listar(const HashTable *ht);
void hash_listar_por_categoria(const HashTable *ht, const char *categoria);
void hash_listar_por_autor(const HashTable *ht, const char *autor);
void destruir_hash(HashTable *ht);

#endif