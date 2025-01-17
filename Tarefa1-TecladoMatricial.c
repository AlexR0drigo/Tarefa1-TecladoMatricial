#include "pico/stdlib.h"
#include <stdio.h>

#define LED_B 11   // Pino do LED azul
#define LED_G 10   // Pino do LED verde
#define LED_R 9    // Pino do LED vermelho
#define BUZZ 12    // Pino do buzzer

const uint8_t col[4] = {1, 2, 3, 4}; // Pinos das colunas do teclado
const uint8_t lin[4] = {5, 6, 7, 8}; // Pinos das linhas do teclado

const char tecl[4][4] = 
{
    {'1', '2', '3', 'A'}, 
    {'4', '5', '6', 'B'}, 
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char ler_teclado();

int main() 
{
    // Inicializa comunicação serial
    stdio_init_all();

    // Configura LEDs como saída
    gpio_init(LED_B);
    gpio_init(LED_G);
    gpio_init(LED_R);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_R, GPIO_OUT);

    gpio_put(LED_B, false);
    gpio_put(LED_G, false);
    gpio_put(LED_R, false);

    // Configura buzzer como saída
    gpio_init(BUZZ);
    gpio_set_dir(BUZZ, GPIO_OUT);
    gpio_put(BUZZ, false);

    // Configura colunas como saídas digitais
    for (int i = 0; i < 4; i++) 
    {
        gpio_init(col[i]);
        gpio_set_dir(col[i], GPIO_OUT);
        gpio_put(col[i], 1); // Colunas iniciam em alto
    }

    // Configura linhas como entradas com pull-up
    for (int i = 0; i < 4; i++) 
    {
        gpio_init(lin[i]);
        gpio_set_dir(lin[i], GPIO_IN);
        gpio_pull_up(lin[i]); // Habilita pull-up nas linhas
    }

    while (true) 
    {
        char tecla = ler_teclado();

        if (tecla != 'n') // Só executa se uma tecla for pressionada
        {
            printf("Tecla pressionada: %c\n", tecla);

            // Ações para cada tecla
            switch (tecla) 
            {
                case 'A': // Acende LED azul
                    gpio_put(LED_B, true);
                    sleep_ms(500);
                    gpio_put(LED_B, false);
                    break;

                case 'B': // Acende LED verde
                    gpio_put(LED_G, true);
                    sleep_ms(500);
                    gpio_put(LED_G, false);
                    break;

                case 'C': // Acende LED vermelho
                    gpio_put(LED_R, true);
                    sleep_ms(500);
                    gpio_put(LED_R, false);
                    break;

                case 'D': // Aciona buzzer
                    gpio_put(BUZZ, true);
                    sleep_ms(500);
                    gpio_put(BUZZ, false);
                    break;

                default:
                    break;
            }
        }

        sleep_ms(200); // Pausa antes da próxima leitura
    }

    return 0;
}

// Função para ler o teclado
char ler_teclado() 
{
    char tecla = 'n'; // Valor padrão caso nenhuma tecla seja pressionada

    // Desliga todas as colunas
    for (int i = 0; i < 4; i++) 
    {
        gpio_put(col[i], 1);
    }

    for (int c = 0; c < 4; c++) 
    {
        // Ativa a coluna atual
        gpio_put(col[c], 0);

        for (int l = 0; l < 4; l++) 
        {
            // Verifica se a linha foi ativada
            if (gpio_get(lin[l]) == 0) 
            {
                tecla = tecl[l][c]; // Mapeia a tecla pressionada

                // Espera a tecla ser liberada (debounce)
                while (gpio_get(lin[l]) == 0) 
                {
                    sleep_ms(10);
                }
                break; // Sai do laço após detectar a tecla
            }
        }

        // Desativa a coluna atual
        gpio_put(col[c], 1);

        if (tecla != 'n') // Sai do laço de colunas se uma tecla for detectada
        {
            break;
        }
    }

    return tecla; // Retorna a tecla pressionada
}
