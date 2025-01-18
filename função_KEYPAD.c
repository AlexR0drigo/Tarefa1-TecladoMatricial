#include "pico/stdlib.h"
#include <stdio.h>

#define LED_BLUE 11  // Pino conectado ao LED azul
#define LED_GREEN 10 // Pino conectado ao LED verde
#define LED_RED 9    // Pino conectado ao LED vermelho
#define BUZZER 12    // Pino conectado ao buzzer

const uint8_t colunas[4] = {1, 2, 3, 4}; // Pinos das colunas do teclado matricial
const uint8_t linhas[4] = {5, 6, 7, 8};  // Pinos das linhas do teclado matricial

const char teclado[4][4] = 
{
    {'1', '2', '3', 'A'}, 
    {'4', '5', '6', 'B'}, 
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char leitura_teclado();

int main() 
{
    // Inicializa a UART (Serial)
    stdio_init_all();

    // Inicializa LEDs como saída
    gpio_init(LED_BLUE);
    gpio_init(LED_GREEN);
    gpio_init(LED_RED);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_RED, GPIO_OUT);

    gpio_put(LED_BLUE, false);
    gpio_put(LED_GREEN, false);
    gpio_put(LED_RED, false);

    // Inicializa o buzzer como saída
    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
    gpio_put(BUZZER, false);

    // Configuração dos pinos das colunas como saídas digitais
    for (int i = 0; i < 4; i++) 
    {
        gpio_init(colunas[i]);
        gpio_set_dir(colunas[i], GPIO_OUT);
        gpio_put(colunas[i], 1); // Inicializa todas as colunas como alto
    }

    // Configuração dos pinos das linhas como entradas digitais
    for (int i = 0; i < 4; i++) 
    {
        gpio_init(linhas[i]);
        gpio_set_dir(linhas[i], GPIO_IN);
        gpio_pull_up(linhas[i]); // Habilita pull-up para as linhas
    }

    while (true) 
    {
        char tecla = leitura_teclado();

        if (tecla != 'n') // Só processa se uma tecla foi pressionada
        {
            printf("Tecla pressionada: %c\n", tecla);

            // Ações associadas às teclas
            switch (tecla) 
            {
                case 'A': // Acende o LED azul
                    gpio_put(LED_BLUE, true);
                    sleep_ms(1000);
                    gpio_put(LED_BLUE, false);
                    break;

                case 'B': // Acende o LED verde
                    gpio_put(LED_GREEN, true);
                    sleep_ms(1000);
                    gpio_put(LED_GREEN, false);
                    break;

                case 'C': // Acende o LED vermelho
                    gpio_put(LED_RED, true);
                    sleep_ms(1000);
                    gpio_put(LED_RED, false);
                    break;

                case 'D': // Aciona o buzzer
                    gpio_put(BUZZER, true);
                    sleep_ms(1000); // Tempo que o buzzer ficará ativo
                    gpio_put(BUZZER, false);
                    break;

                default:
                    break;
            }
        }

        sleep_ms(200); // Pequena pausa antes da próxima leitura
    }

    return 0;
}

// Função para ler o teclado matricial
char leitura_teclado() 
{
    char numero = 'n'; // Valor padrão para quando nenhuma tecla for pressionada

    // Desliga todas as colunas
    for (int i = 0; i < 4; i++) 
    {
        gpio_put(colunas[i], 1);
    }

    for (int coluna = 0; coluna < 4; coluna++) 
    {
        // Ativa a coluna atual
        gpio_put(colunas[coluna], 0);

        for (int linha = 0; linha < 4; linha++) 
        {
            // Verifica se a linha está ativa
            if (gpio_get(linhas[linha]) == 0) 
            {
                numero = teclado[linha][coluna]; // Mapeia a tecla pressionada

                // Aguarda a tecla ser liberada (debounce)
                while (gpio_get(linhas[linha]) == 0) 
                {
                    sleep_ms(10);
                }
                break; // Sai do laço após detectar a tecla
            }
        }

        // Desativa a coluna atual
        gpio_put(colunas[coluna], 1);

        if (numero != 'n') // Sai do laço de colunas se uma tecla foi detectada
        {
            break;
        }
    }

    return numero; // Retorna a tecla pressionada
}
