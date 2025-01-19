#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define BUZZER 12 //define o pino do buzzer

// Configura o buzzer no pino GPIO especificado
void setup_buzzer(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);

    pwm_set_wrap(slice_num, 255); // Define o valor máximo do contador PWM
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio), 128); // Define o nível de saída PWM
    pwm_set_enabled(slice_num, true); // Habilita o PWM
}

// Emite um som no buzzer com a frequência especificada
void buzz(uint gpio, uint frequencia) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_clkdiv(slice_num, 125000000 / (256 * frequencia)); // Define a frequência do PWM
    pwm_set_enabled(slice_num, true); // Habilita o PWM
}

// Pinos para o teclado matricial
const uint8_t teclas_colunas[4] = {1, 2, 3, 4};
const uint8_t teclas_linhas[4] = {5, 6, 7, 8};

// Mapeamento do teclado matricial
const char matriz_teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Função para identificar a tecla pressionada
char capturar_tecla() {
    char tecla_detectada = 'n'; // Valor padrão se nenhuma tecla for pressionada

    // Reseta todas as colunas
    for (int i = 0; i < 4; i++) {
        gpio_put(teclas_colunas[i], 1);
    }

    for (int coluna = 0; coluna < 4; coluna++) {
        gpio_put(teclas_colunas[coluna], 0); // Ativa a coluna atual

        for (int linha = 0; linha < 4; linha++) {
            if (gpio_get(teclas_linhas[linha]) == 0) { // Detecta pressão na linha
                tecla_detectada = matriz_teclas[linha][coluna];

                // Aguarda a liberação da tecla
                while (gpio_get(teclas_linhas[linha]) == 0) {
                    sleep_ms(10);
                }
                break;
            }
        }

        gpio_put(teclas_colunas[coluna], 1); // Desativa a coluna atual

        if (tecla_detectada != 'n') { // Sai do laço se uma tecla foi detectada
            break;
        }
    }

    return tecla_detectada;
}


