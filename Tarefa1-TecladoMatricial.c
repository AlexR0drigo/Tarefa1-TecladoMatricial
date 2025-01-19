#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define BUZZER 10 // define o pino do buzzer

// Definição de pinos para LEDs
#define LED_AZUL 12
#define LED_VERDE 11
#define LED_VERMELHO 13

// Configura o buzzer no pino GPIO especificado
void setup_buzzer(uint gpio)
{
   gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);

    pwm_set_wrap(slice_num, 255);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio), 128);
    pwm_set_enabled(slice_num, true);
}

void buzz(uint gpio, uint frequencia)
{
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    uint32_t freq_relogio = 125000000;
    uint16_t contador = freq_relogio / frequencia;

    pwm_set_wrap(slice_num, contador);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio), contador / 2);

    pwm_set_enabled(slice_num, true);
    sleep_ms(50);  // Som curto (50ms)
    pwm_set_enabled(slice_num, false);
    
    sleep_ms(1000); // Pausa maior entre os sons para evitar efeito contínuo
}


// Pinos para o teclado matricial
const uint8_t teclas_colunas[4] = {1, 2, 3, 4};
const uint8_t teclas_linhas[4] = {5, 6, 7, 8};

// Mapeamento do teclado matricial
const char matriz_teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// Declaração das funções
char capturar_tecla();
void inicializar_pwm_buzzer(uint gpio, uint freq);
void ativar_buzzer(uint freq);

int main()
{
    // Configuração inicial do sistema serial
    stdio_init_all();

    // Configura os LEDs como saída
    gpio_init(LED_AZUL);
    gpio_init(LED_VERDE);
    gpio_init(LED_VERMELHO);

    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);

    gpio_put(LED_AZUL, false);
    gpio_put(LED_VERDE, false);
    gpio_put(LED_VERMELHO, false);

    // Configuração das colunas e linhas do teclado
    for (int i = 0; i < 4; i++)
    {
        gpio_init(teclas_colunas[i]);
        gpio_set_dir(teclas_colunas[i], GPIO_OUT);
        gpio_put(teclas_colunas[i], 1);
    }

    for (int i = 0; i < 4; i++)
    {
        gpio_init(teclas_linhas[i]);
        gpio_set_dir(teclas_linhas[i], GPIO_IN);
        gpio_pull_up(teclas_linhas[i]);
    }

    while (true)
    {
        char tecla = capturar_tecla();

        if (tecla != 'n')
        { // Apenas processa quando uma tecla é pressionada
            printf("Tecla pressionada: %c\n", tecla);

            if (tecla == 'D')
            {
                // Liga todos os LEDs
                gpio_put(LED_AZUL, true);
                gpio_put(LED_VERDE, true);
                gpio_put(LED_VERMELHO, true);
                printf("Todos os LEDs ligados.\n");

                // Mantém enquanto a tecla D estiver pressionada
                while (capturar_tecla() == 'D')
                {
                    sleep_ms(50);
                }

                // Desliga todos os LEDs ao soltar a tecla
                gpio_put(LED_AZUL, false);
                gpio_put(LED_VERDE, false);
                gpio_put(LED_VERMELHO, false);
                printf("Todos os LEDs desligados.\n");
            }
            else if (tecla == 'C')
            {
                // Ativa o LED vermelho
                gpio_put(LED_VERDE, true);
                printf("LED verde ativado.\n");

                // Mantém enquanto a tecla C estiver pressionada
                while (capturar_tecla() == 'C')
                {
                    sleep_ms(50);
                }

                // Desliga o LED vermelho ao soltar a tecla
                gpio_put(LED_VERDE, false);
                printf("LED verde desativado.\n");
            }
            else if (tecla == '#')
            {
            printf("Buzzer ativado!\n");
            buzz(BUZZER, 1000);
            }
            else // Teclas A e B
            {
                int pino_led = -1;
                switch (tecla)
                {
                case 'A':
                    pino_led = LED_VERMELHO;
                    break;
                case 'B':
                    pino_led = LED_AZUL;
                    break;
                }

                if (pino_led != -1)
                {
                    gpio_put(pino_led, true);
                    printf("LED ligado.\n");

                    // Mantém enquanto a tecla ainda estiver pressionada
                    while (capturar_tecla() == tecla)
                    {
                        sleep_ms(50);
                    }

                    gpio_put(pino_led, false);
                    printf("LED desligado.\n");
                }
            }
        }

        sleep_ms(50); // Pequena pausa antes de capturar a próxima tecla
    }

    return 0;
}

// Função para identificar a tecla pressionada
char capturar_tecla() {
    // Reseta todas as colunas
    for (int i = 0; i < 4; i++) {
        gpio_put(teclas_colunas[i], 1);
    }

    for (int coluna = 0; coluna < 4; coluna++) {
        gpio_put(teclas_colunas[coluna], 0); // Ativa a coluna atual

        for (int linha = 0; linha < 4; linha++) {
            if (gpio_get(teclas_linhas[linha]) == 0) { // Detecta pressão na linha
                gpio_put(teclas_colunas[coluna], 1); // Desativa a coluna atual
                return matriz_teclas[linha][coluna]; // Retorna imediatamente
            }
        }

        gpio_put(teclas_colunas[coluna], 1); // Desativa a coluna atual
    }

    return 'n'; // Nenhuma tecla pressionada
}