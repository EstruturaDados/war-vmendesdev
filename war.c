// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CODIGO
// ============================================================================
//
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o codigo em funcoes especializadas.
// - Implementar um sistema de missoes para um jogador.
// - Criar uma funcao para verificar se a missao foi cumprida.
// - Utilizar passagem por referencia (ponteiros) para modificar dados e
//   passagem por valor/referencia constante (const) para apenas ler.
// - Foco em: Design de software, modularizacao, const correctness, logica de jogo.
//
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ctype.h> // Necessario para validar digitos e letras

#define MAX_TERRITORIOS 5
#define MAX_NOME 50

typedef struct {
    char nome[MAX_NOME];
    char corExercito[20];
    int tropas;
} Territorio;

// --- Prototipos das Funcoes ---
Territorio* alocarMapa(int quantidade);
void inicializarTerritorios(Territorio *mapa, int quantidade);
void liberarMemoria(Territorio *mapa);
int existeTerritorioOuCor(const Territorio *mapa, int quantidade, const char *nome, const char *cor);

void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa, int quantidade);
void exibirMissao(int missao);

void faseDeAtaque(Territorio *mapa, int quantidade);
void simularAtaque(Territorio *origem, Territorio *destino);

int sortearMissao();
int verificarVitoria(const Territorio *mapa, int quantidade, const char *corJogador, int missao);

void limparBufferEntrada();
int lerNumeroTropas(); 
void lerCorExercito(char *cor, int tamanho); // Nova funcao para validar cor

// --- Funcao Principal ---
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    int quantidade = MAX_TERRITORIOS;
    Territorio *mapa = alocarMapa(quantidade);
    if (mapa == NULL) {
        printf("Erro: falha ao alocar memoria para o mapa.\n");
        return 1;
    }

    inicializarTerritorios(mapa, quantidade);

    char corJogador[MAX_NOME] = "Azul";
    int missao = sortearMissao(); // Sorteia missao

    printf("\nBem-vindo ao WAR ESTRUTURADO!\n");
    printf("Voce comandara o exercito %s.\n", corJogador);
    printf("Sua missao secreta foi sorteada!\n\n");
    
    int opcao;
    do {
        exibirMapa(mapa, quantidade);
        exibirMissao(missao);
        exibirMenuPrincipal();

        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, quantidade);
                break;
            case 2:
                if (verificarVitoria(mapa, quantidade, corJogador, missao))
                    printf("\nParabens! Voce venceu!\n");
                else
                    printf("\nMissao ainda nao cumprida. Continue tentando!\n");
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

        printf("\nPressione ENTER para continuar...");
        getchar();

    } while (opcao != 0);

    liberarMemoria(mapa);
    return 0;
}

// --- Implementacao das Funcoes ---
Territorio* alocarMapa(int quantidade) {
    Territorio *mapa = (Territorio*) calloc(quantidade, sizeof(Territorio));
    return mapa;
}

int existeTerritorioOuCor(const Territorio *mapa, int quantidade, const char *nome, const char *cor) {
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(mapa[i].nome, nome) == 0 || strcmp(mapa[i].corExercito, cor) == 0)
            return 1;
    }
    return 0;
}

// --- Funcao auxiliar: ler e validar numero de tropas ---
int lerNumeroTropas() {
    char buffer[20];
    int numero = 0;
    int valido = 0;

    while (!valido) {
        printf("Numero de tropas: ");
        fgets(buffer, 20, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        int i;
        valido = 1;
        for (i = 0; buffer[i] != '\0'; i++) {
            if (!isdigit(buffer[i])) {
                valido = 0;
                break;
            }
        }

        if (valido) {
            numero = atoi(buffer);
            if (numero <= 0) {
                valido = 0;
                printf("Digite um numero maior que zero!\n");
            }
        } else {
            printf("Entrada invalida! Digite apenas numeros.\n");
        }
    }

    return numero;
}

// --- Funcao auxiliar: ler e validar cor do exercito ---
void lerCorExercito(char *cor, int tamanho) {
    int valido = 0;
    while (!valido) {
        printf("Cor do exercito dominante: ");
        fgets(cor, tamanho, stdin);
        cor[strcspn(cor, "\n")] = '\0';

        int i;
        valido = 1;
        for (i = 0; cor[i] != '\0'; i++) {
            if (!isalpha(cor[i]) && cor[i] != ' ') {
                valido = 0;
                break;
            }
        }

        if (!valido) {
            printf("Entrada invalida! Use apenas letras e espacos.\n");
        }
    }
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

            // Uso da funcao de validacao da cor
            lerCorExercito(corTemp, 20);

            // Uso da funcao de validacao de tropas
            tropasTemp = lerNumeroTropas();

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

void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

void exibirMenuPrincipal() {
    printf("\n===== MENU PRINCIPAL =====\n");
    printf("1. Iniciar ataque\n");
    printf("2. Verificar vitoria\n");
    printf("0. Sair do jogo\n");
    printf("==========================\n");
}

void exibirMapa(const Territorio *mapa, int quantidade) {
    printf("\n=== MAPA MUNDIAL ===\n");
    printf("%-15s | %-10s | %-7s\n", "Territorio", "Exercito", "Tropas");
    printf("--------------------------------------\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%-15s | %-10s | %-7d\n", mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
    }
    printf("--------------------------------------\n");
}

void exibirMissao(int missao) {
    printf("\nSua missao: ");
    switch (missao) {
        case 0: printf("Conquistar todos os territorios!\n"); break;
        case 1: printf("Destruir um exercito inimigo!\n"); break;
        case 2: printf("Conquistar 3 territorios diferentes!\n"); break;
    }
}

void faseDeAtaque(Territorio *mapa, int quantidade) {
    int origem, destino;
    printf("\n=== FASE DE ATAQUE ===\n");

    for (int i = 0; i < quantidade; i++) {
        printf("%d - %s (Exercito: %s, Tropas: %d)\n",
               i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
    }

    printf("\nEscolha o territorio de origem (numero): ");
    scanf("%d", &origem);
    limparBufferEntrada();

    printf("Escolha o territorio de destino (numero): ");
    scanf("%d", &destino);
    limparBufferEntrada();

    origem--; destino--;

    if (origem < 0 || origem >= quantidade || destino < 0 || destino >= quantidade) {
        printf("\nTerritorio invalido!\n");
        return;
    }

    if (origem == destino) {
        printf("\nOrigem e destino sao iguais! Escolha territorios diferentes.\n");
        return;
    }

    if (mapa[origem].tropas < 2) {
        printf("\nTerritorio de origem nao possui tropas suficientes para atacar.\n");
        return;
    }

    simularAtaque(&mapa[origem], &mapa[destino]);
}

void simularAtaque(Territorio *origem, Territorio *destino) {
    int ataque, defesa;

    printf("\nSimulando ataque de %s para %s...\n", origem->nome, destino->nome);

    int maxAtaque = (origem->tropas - 1 > 3) ? 3 : origem->tropas - 1;
    int maxDefesa = (destino->tropas > 2) ? 2 : destino->tropas;

    ataque = rand() % maxAtaque + 1;
    defesa = rand() % maxDefesa + 1;

    printf("Tropas atacantes: %d | Tropas defensoras: %d\n", ataque, defesa);

    if (ataque > defesa) {
        printf("Ataque bem-sucedido! %s conquistado.\n", destino->nome);
        destino->tropas -= defesa;
        destino->tropas = (destino->tropas <= 0) ? 1 : destino->tropas;
        strcpy(destino->corExercito, origem->corExercito);
        origem->tropas -= 1;
    } else {
        printf("Ataque falhou! %s continua sob controle do exercito %s.\n",
               destino->nome, destino->corExercito);
        origem->tropas -= 1;
    }
}

int sortearMissao() {
    return rand() % 3;
}

int verificarVitoria(const Territorio *mapa, int quantidade, const char *corJogador, int missao) {
    int total = 0;
    int inimigoVivo = 0;

    switch (missao) {
        case 0: // Conquistar todos os territorios
            for (int i = 0; i < quantidade; i++)
                if (strcmp(mapa[i].corExercito, corJogador) == 0)
                    total++;
            return (total == quantidade);

        case 1: // Destruir um exercito inimigo
            for (int i = 0; i < quantidade; i++)
                if (strcmp(mapa[i].corExercito, corJogador) != 0)
                    inimigoVivo = 1;
            return !inimigoVivo;

        case 2: // Conquistar 3 territorios diferentes
            for (int i = 0; i < quantidade; i++)
                if (strcmp(mapa[i].corExercito, corJogador) == 0)
                    total++;
            return (total >= 3);

        default:
            return 0;
    }
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}