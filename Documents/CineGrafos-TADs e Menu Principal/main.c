#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "grafo.h"

// Evita que o '\n' do scanf estrague as leituras com fgets seguintes 
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Retorna a struct do utilizador caso logue ou registe, ou NULL se pedir para fechar 
Utilizador* ecra_autenticacao(Grafo *g) {
    int opcao, id;
    
    while (1) {
        printf("\n===== CineGrafos  (Autenticacao) =====\n");
        printf("1. Entrar com ID Conta\n");
        printf("2. Criar Nova Conta\n");
        printf("0. Fechar Aplicacao\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) { limpar_buffer(); continue; }
        limpar_buffer();
        
        if (opcao == 0) return NULL;
        if (opcao == 1) {
            printf("\n--- Entrar com ID ---\n");
            printf("Introduza o seu ID de Utilizador (ou 0 para VOLTAR): ");
            if (scanf("%d", &id) != 1 || id == 0) { limpar_buffer(); continue; }
            return criar_utilizador(id, "Utilizador Teste", "teste@email.com");
        }
        if (opcao == 2) {
            printf("\n--- Criar Nova Conta ---\n");
            printf("[SUCESSO] Conta criada! Guarde o seu ID de acesso obrigatorio: 1\n");
            return criar_utilizador(1, "Novo Utilizador", "novo@email.com");
        }
    }
}

int main() {
    HashTable *ht = criar_hash();
    Grafo *g = criar_grafo();
    if (!ht || !g) return 1;
    
    Utilizador *utilizador_sessao = ecra_autenticacao(g);
    if (!utilizador_sessao) return 0;

    int opcao;
    char termo[100];
    
    // Loop interno para as funcionalidades do utilizador autenticado 
    do {
        printf("\n===== Cinegrafos (Sessao: %s) =====\n", utilizador_sessao->nome);
        printf("1. Ver Catalogo Completo\n");
        printf("2. Pesquisar por Titulo\n");
        printf("3. Explorar por Genero\n");
        printf("4. Avaliar/Dar Nota a Filme\n");
        printf("5. Historico de Avaliacoes\n");
        printf("6. Recomendacoes (BFS)\n");
        printf("0. Guardar e Terminar Sessao (Voltar ao Login)\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) { limpar_buffer(); continue; }
        limpar_buffer();
        
        switch (opcao) {
            case 1:
                printf("\n[SUCESSO] Listagem completa do catalogo:\n");
                hash_listar(ht);
                printf("\nPressione Enter para voltar ao menu..."); getchar();
                break;
                
            case 2:
                printf("Nome do filme (ou 0 para voltar): ");
                fgets(termo, sizeof(termo), stdin);
                termo[strcspn(termo, "\n")] = '\0';
                
                hash_pesquisar(ht, termo);
                printf("\nPressione Enter para voltar ao menu..."); getchar();
                break;
                
            case 3:
                printf("Generos: Acao, Drama, Ficcao, Comedia, Terror, Romance, Animacao\n");
                printf("Escolha (ou 0 para voltar): ");
                fgets(termo, sizeof(termo), stdin);
                termo[strcspn(termo, "\n")] = '\0';
                
                hash_listar_por_categoria(ht, termo);
                printf("\nPressione Enter para voltar ao menu..."); getchar();
                break;
                
            case 4: {
                int id_f, nota;
                printf("Introduza o ID numerico do Filme (ou 0 para voltar): ");
                if (scanf("%d", &id_f) != 1 || id_f == 0) { limpar_buffer(); break; }
                printf("Avaliacao (1 a 5 estrelas ou 0 para abortar): ");
                if (scanf("%d", &nota) != 1 || nota == 0) { limpar_buffer(); break; }
                
                grafo_adicionar_avaliacao(g, utilizador_sessao->id, id_f, nota);
                printf("[SUCESSO] Avaliacao registada e guardada com exito.\n");
                break;
            }
            case 5:
                printf("\n--- Historico de Avaliacoes Visualizado ---\n");
                grafo_listar_avaliados_por_user(g, utilizador_sessao->id);
                printf("\nPressione Enter para voltar ao menu..."); getchar();
                break;
                
            case 6:
                printf("\n--- Processando Algoritmo BFS em tempo real ---\n");
                grafo_recomendar_bfs(g, utilizador_sessao->id);
                printf("\nPressione Enter para voltar ao menu..."); getchar();
                break;
                
            case 0:
                printf("[SISTEMA] Sessao terminada. Voltando ao ecra de Autenticacao.\n");
                break;
        }
    } while (opcao != 0);

    destruir_hash(ht);
    destruir_grafo(g);
    return 0;
}
