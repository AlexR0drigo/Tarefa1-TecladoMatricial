

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


