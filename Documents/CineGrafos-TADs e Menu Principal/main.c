
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "grafo.h"

#define NUM_CATEGORIAS_FIXAS 5
const char* CATEGORIAS_SISTEMA[NUM_CATEGORIAS_FIXAS] = {"Acao", "Drama", "Ficcao", "Comedia", "Terror"};

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Verifica se o email tem um @ e pelo menos um ponto depois dele
int validar_formato_email(const char *email) {
    char *arroba = strchr(email, '@');
    if (arroba == NULL || arroba == email) return 0;
    char *ponto = strchr(arroba, '.');
    if (ponto == NULL || ponto == arroba + 1 || *(ponto + 1) == '\0') return 0;
    return 1;
}

int obter_proximo_id_user(const Grafo *g) {
    int max_id = 0;
    for (int i = 0; i < g->total_vertices; i++) {
        if (g->vertices[i].ativo && g->vertices[i].tipo == VERTICE_USER) {
            if (g->vertices[i].id_origem > max_id) max_id = g->vertices[i].id_origem;
        }
    }
    return max_id + 1;
}

// Carregamentos automaticos
void carregar_dados_sistema(HashTable *ht, Grafo *g) {
    FILE *f = fopen("filmes.txt", "r");
    if (f) {
        char linha[256];
        while (fgets(linha, sizeof(linha), f)) {
            linha[strcspn(linha, "\n")] = '\0';
            char *token_id = strtok(linha, ";");
            char *token_tit = strtok(NULL, ";");
            char *token_aut = strtok(NULL, ";");
            char *token_cats = strtok(NULL, ";");
            if (token_id && token_tit && token_aut) {
                Filme *fl = criar_filme(atoi(token_id), token_tit, token_aut);
                if (fl) {
                    if (token_cats) {
                        char *cat = strtok(token_cats, ",");
                        while (cat) { adicionar_categoria(fl, cat); cat = strtok(NULL, ","); }
                    }
                    if (hash_inserir(ht, fl)) {
                        grafo_adicionar_vertice(g, VERTICE_FILME, fl, fl->id);
                    } else {
                        destruir_filme(fl);
                    }
                }
            }
        }
        fclose(f);
    }

    f = fopen("utilizadores.txt", "r");
    if (f) {
        char linha[256];
        while (fgets(linha, sizeof(linha), f)) {
            linha[strcspn(linha, "\n")] = '\0';
            char *token_id = strtok(linha, ";");
            char *token_nome = strtok(NULL, ";");
            char *token_email = strtok(NULL, ";");
            if (token_id && token_nome && token_email) {
                Utilizador *u = criar_utilizador(atoi(token_id), token_nome, token_email);
                if (u) grafo_adicionar_vertice(g, VERTICE_USER, u, u->id);
            }
        }
        fclose(f);
    }

    f = fopen("avaliacoes.txt", "r");
    if (f) {
        int id_u, id_f, nota;
        while (fscanf(f, "%d;%d;%d\n", &id_u, &id_f, &nota) == 3) {
            grafo_adicionar_avaliacao(g, id_u, id_f, nota);
        }
        fclose(f);
    }
}

void salvar_dados_sistema(const HashTable *ht, const Grafo *g) {
    FILE *f = fopen("filmes.txt", "w");
    if (f) {
        for (int i = 0; i < TAMANHO_HASH; i++) {
            NoHash *atual = ht->tabela[i];
            while (atual) {
                fprintf(f, "%d;%s;%s;", atual->filme->id, atual->filme->titulo, atual->filme->autor);
                for (int j = 0; j < atual->filme->num_categorias; j++) {
                    fprintf(f, "%s%s", atual->filme->categorias[j], (j == atual->filme->num_categorias - 1) ? "" : ",");
                }
                fprintf(f, "\n");
                atual = atual->proximo;
            }
        }
        fclose(f);
    }

    f = fopen("utilizadores.txt", "w");
    if (f) {
        for (int i = 0; i < g->total_vertices; i++) {
            if (g->vertices[i].ativo && g->vertices[i].tipo == VERTICE_USER) {
                Utilizador *u = (Utilizador*) g->vertices[i].dado;
                fprintf(f, "%d;%s;%s\n", u->id, u->nome, u->email);
            }
        }
        fclose(f);
    }

    f = fopen("avaliacoes.txt", "w");
    if (f) {
        for (int i = 0; i < g->total_vertices; i++) {
            if (g->vertices[i].ativo && g->vertices[i].tipo == VERTICE_USER) {
                Aresta *a = g->adjacencias[i];
                while (a) {
                    int f_idx = a->destino_idx;
                    fprintf(f, "%d;%d;%d\n", g->vertices[i].id_origem, g->vertices[f_idx].id_origem, a->peso);
                    a = a->proximo;
                }
            }
        }
        fclose(f);
    }
}

Utilizador* ecrã_autenticacao(Grafo *g) {
    int opcao, id;
    char nome[MAX_NOME], email[MAX_EMAIL];
    
    while (1) {
        printf("\n===== CineGrafo Entrar =====\n");
        printf("1. Entrar com ID Conta\n");
        printf("2. Criar Nova Conta\n");
        printf("0. Fechar Aplicacao\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) { limpar_buffer(); continue; }
        limpar_buffer();
        
        if (opcao == 0) return NULL;
        if (opcao == 1) {
            printf("Introduza o seu ID de Utilizador: ");
            if (scanf("%d", &id) != 1) { limpar_buffer(); continue; }
            limpar_buffer();
            
            int idx = grafo_encontrar_vertice(g, VERTICE_USER, id);
            if (idx != -1) {
                return (Utilizador*) g->vertices[idx].dado;
            }
            printf("[Erro] Conta nao encontrada.\n");
        } else if (opcao == 2) {
            printf("Nome Completo: ");
            fgets(nome, MAX_NOME, stdin);
            nome[strcspn(nome, "\n")] = '\0';
            
            // Loop que força o utilizador a introduzir um email valido no formato padrao
            do {
                printf("Correio Eletronico (nome@email.com): ");
                fgets(email, MAX_EMAIL, stdin);
                email[strcspn(email, "\n")] = '\0';
                
                if (!validar_formato_email(email)) {
                    printf("[Aviso] Formato de email invalido. Tente novamente.\n");
                }
            } while (!validar_formato_email(email));
            
            int novo_id = obter_proximo_id_user(g);
            Utilizador *u = criar_utilizador(novo_id, nome, email);
            if (u && grafo_adicionar_vertice(g, VERTICE_USER, u, novo_id)) {
                printf("[Sucesso] Registado! Guarde o seu ID de acesso: %d\n", novo_id);
                return u;
            }
        }
    }
}

int main() {
    HashTable *ht = criar_hash();
    Grafo *g = criar_grafo();
    if (!ht || !g) return 1;

    carregar_dados_sistema(ht, g);
    
    Utilizador *utilizador_sessao = ecrã_autenticacao(g);
    if (!utilizador_sessao) {
        destruir_hash(ht);
        destruir_grafo(g);
        return 0;
    }

    int opcao;
    char termo[100];
    do {
        printf("\n===== CineStream (Sessao: %s) =====\n", utilizador_sessao->nome);
        printf("1. Ver Catalogo Completo\n");
        printf("2. Pesquisar por Titulo\n");
        printf("3. Explorar por Genero\n");
        printf("4. Avaliar/Dar Nota a Filme\n");
        printf("5. Historico de Avaliacoes\n");
        printf("6. O Meu Feed de Recomendacoes (BFS)\n");
        printf("0. Guardar e Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) { limpar_buffer(); continue; }
        limpar_buffer();
        
        switch (opcao) {
            case 1: {
                int total = 0;
                for (int i = 0; i < TAMANHO_HASH; i++) {
                    if (ht->tabela[i]) total++;
                }
                if (total == 0) printf("\n[INFO] Lista de filmes vazia.\n");
                else hash_listar(ht);
                
                fflush(stdout); limpar_buffer();
                printf("\nPressione Enter para continuar..."); getchar();
                break;
            }
            case 2:
                printf("Nome do filme: ");
                fgets(termo, sizeof(termo), stdin);
                termo[strcspn(termo, "\n")] = '\0';
                Filme *f = hash_pesquisar(ht, termo);
                if (f) imprimir_filme(f);
                else printf("Filme inexistente no catalogo.\n");
                printf("\nPressione Enter para continuar..."); getchar();
                break;
                
            case 3:
                printf("Generos: Acao, Drama, Ficcao, Comedia, Terror\nEscolha: ");
                fgets(termo, sizeof(termo), stdin);
                termo[strcspn(termo, "\n")] = '\0';
                hash_listar_por_categoria(ht, termo);
                printf("\nPressione Enter para continuar..."); getchar();
                break;
                
            case 4: {
                int id_f, nota;
                printf("Introduza o ID numerico do Filme: ");
                if (scanf("%d", &id_f) != 1) { limpar_buffer(); break; }
                printf("Avaliacao (1 a 5 estrelas): ");
                if (scanf("%d", &nota) != 1) { limpar_buffer(); break; }
                limpar_buffer();
                
                if (nota < 1 || nota > 5) {
                    printf("Nota invalida.\n");
                } else {
                    int res = grafo_adicionar_avaliacao(g, utilizador_sessao->id, id_f, nota);
                    if (res) printf("Avaliacao guardada com sucesso.\n");
                    else printf("Nao foi possivel processar. Verifique o ID do filme.\n");
                }
                break;
            }
            case 5:
                grafo_listar_avaliados_por_user(g, utilizador_sessao->id);
                printf("\nPressione Enter para continuar..."); getchar();
                break;
                
            case 6:
                grafo_recomendar_bfs(g, utilizador_sessao->id);
                printf("\nPressione Enter para continuar..."); getchar();
                break;
        }
    } while (opcao != 0);

    salvar_dados_sistema(ht, g);
    destruir_hash(ht);
    destruir_grafo(g);
    return 0;
}