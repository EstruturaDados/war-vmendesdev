// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_TERRITORIOS 5
#define MAX_NOME 50

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_NOME];
    char corExercito[20];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(int quantidade);
void inicializarTerritorios(Territorio *mapa, int quantidade);
void liberarMemoria(Territorio *mapa);
// Nova função de validação
int existeTerritorioOuCor(const Territorio *mapa, int quantidade, const char *nome, const char *cor);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa, int quantidade);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio *mapa, int quantidade);
int verificarVitoria(const Territorio *mapa, int quantidade, const char *corJogador);

// Função utilitária:
void limparBufferEntrada();
// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.
        setlocale(LC_ALL, "Portuguese");
        srand(time(NULL));

    int quantidade = MAX_TERRITORIOS;
    Territorio *mapa = alocarMapa(quantidade);

    if (mapa == NULL) {
        printf("Erro: falha ao alocar memória para o mapa.\n");
        return 1;
    }

    inicializarTerritorios(mapa, quantidade);

    char corJogador[MAX_NOME] = "Azul";
    int missao = rand() % 3; // Sorteia missão simples apenas como placeholder

    printf("\nBem-vindo ao WAR ESTRUTURADO!\n");
    printf("Você comandara o exercito %s.\n", corJogador);
    printf("Sua missao secreta foi sorteada!\n\n");
    
    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
 int opcao;
    do {
        exibirMapa(mapa, quantidade);
        exibirMenuPrincipal();

        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, quantidade);
                break;
            case 2:
                if (verificarVitoria(mapa, quantidade, corJogador))
                    printf("\nParabens! Voce venceu!\n");
                else
                    printf("\nMissao ainda nao cumprida. Continue tentando!\n");
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\nOpção invalida! Tente novamente.\n");
        }

        printf("\nPressione ENTER para continuar...");
        getchar();

    } while (opcao != 0);

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
    Territorio* alocarMapa(int quantidade) {
    Territorio *mapa = (Territorio*) calloc(quantidade, sizeof(Territorio));
    return mapa;
}


// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
// verifica se já existe um território ou cor cadastrados
int existeTerritorioOuCor(const Territorio *mapa, int quantidade, const char *nome, const char *cor) {
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(mapa[i].nome, nome) == 0 || strcmp(mapa[i].corExercito, cor) == 0) {
            return 1; // já existe
        }
    }
    return 0; // não existe
}

void inicializarTerritorios(Territorio *mapa, int quantidade) {
  for (int i = 0; i < quantidade; i++) {
        char nomeTemp[MAX_NOME];
        char corTemp[20];
        int tropasTemp;
        int valido = 0;

        do {
            printf("\nCadastro do territorio %d:\n", i + 1);

            printf("Nome do territorio: ");
            fgets(nomeTemp, MAX_NOME, stdin);
            nomeTemp[strcspn(nomeTemp, "\n")] = '\0';

            printf("Cor do exercito dominante: ");
            fgets(corTemp, 20, stdin);
            corTemp[strcspn(corTemp, "\n")] = '\0';

            printf("Numero de tropas: ");
            scanf("%d", &tropasTemp);
            limparBufferEntrada();

            if (existeTerritorioOuCor(mapa, i, nomeTemp, corTemp)) {
                printf("\nNome ou cor ja cadastrado! Tente novamente.\n");
            } else {
                valido = 1;
            }
        } while (!valido);

        strcpy(mapa[i].nome, nomeTemp);
        strcpy(mapa[i].corExercito, corTemp);
        mapa[i].tropas = tropasTemp;
    }

}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
    printf("\n===== MENU PRINCIPAL =====\n");
    printf("1. Iniciar ataque\n");
    printf("2. Verificar vitoria\n");
    printf("0. Sair do jogo\n");
    printf("==========================\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio *mapa, int quantidade) {
    printf("\n=== MAPA MUNDIAL ===\n");
    printf("%-15s | %-10s | %-7s\n", "Territorio", "Exercito", "Tropas");
    printf("--------------------------------------\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%-15s | %-10s | %-7d\n", mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
    }
    printf("--------------------------------------\n");
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio *mapa, int quantidade) {
    printf("\nFuncao de ataque ainda sera implementada no nivel Aventureiro.\n");
}
// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria(const Territorio *mapa, int quantidade, const char *corJogador) {
    printf("\nFuncao de verificacao de vitoria sera implementada nos proximos niveis.\n");
    return 0;
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}