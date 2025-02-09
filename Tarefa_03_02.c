#include "Tarefa_03_02.h"  // Inclui o cabeçalho do projeto



// Configura o barramento I2C no Raspberry Pi Pico
void configurar_i2c()
{
    i2c_init(I2C_PORT, 400 * 1000); // Inicializa o I2C na velocidade de 400 kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Define os pinos para função I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA); // Ativa pull-up nos pinos I2C
    gpio_pull_up(I2C_SCL);
}

// Configuração inicial do display OLED SSD1306
void configurar_display(ssd1306_t *ssd)
{
    ssd1306_init(ssd, WIDTH, HEIGHT, false, ENDERECO_DISPLAY, I2C_PORT); // Inicializa o display
    ssd1306_config(ssd); // Configura o display
    ssd1306_fill(ssd, false); // Limpa o display
    ssd1306_send_data(ssd); // Envia os dados para o display
}


void desenha_fig(uint32_t *_matriz, uint8_t _intensidade, PIO pio, uint sm){
    uint32_t pixel = 0; uint8_t r, g, b;

    for(int i = 24; i>19; i--) //Linha 1
    {
        pixel = _matriz[i];
        b = ((pixel >> 16) & 0xFF)*(_intensidade/100.00); // Isola os 8 bits mais significativos (azul)
        g = ((pixel >> 8) & 0xFF)*(_intensidade/100.00);  // Isola os 8 bits intermediários (verde)
        r = (pixel & 0xFF)*(_intensidade/100.00);         // Isola os 8 bits menos significativos (vermelho)
        pixel = 0;
        pixel = (g << 16) | (r << 8) | b;
        pio_sm_put_blocking(pio, sm, pixel<<8u);
    }

    for(int i = 15; i<20; i++) //Linha 2
    {
        pixel = _matriz[i];
        b = ((pixel >> 16) & 0xFF)*(_intensidade/100.00); // Isola os 8 bits mais significativos (azul)
        g = ((pixel >> 8) & 0xFF)*(_intensidade/100.00);  // Isola os 8 bits intermediários (verde)
        r = (pixel & 0xFF)*(_intensidade/100.00);         // Isola os 8 bits menos significativos (vermelho)
        pixel = 0;
        pixel = (b << 16) | (r << 8) | g;
        pixel = (g << 16) | (r << 8) | b;
        pio_sm_put_blocking(pio, sm, pixel<<8u);

    }

    for(int i = 14; i>9; i--) //Linha 3
    {
        pixel = _matriz[i];
        b = ((pixel >> 16) & 0xFF)*(_intensidade/100.00); // Isola os 8 bits mais significativos (azul)
        g = ((pixel >> 8) & 0xFF)*(_intensidade/100.00);  // Isola os 8 bits intermediários (verde)
        r = (pixel & 0xFF)*(_intensidade/100.00);         // Isola os 8 bits menos significativos (vermelho)
        pixel = 0;
        pixel = (g << 16) | (r << 8) | b;
        pio_sm_put_blocking(pio, sm, pixel<<8u);
    }

    for(int i = 5; i<10; i++) //Linha 4
    {
        pixel = _matriz[i];
        b = ((pixel >> 16) & 0xFF)*(_intensidade/100.00); // Isola os 8 bits mais significativos (azul)
        g = ((pixel >> 8) & 0xFF)*(_intensidade/100.00);  // Isola os 8 bits intermediários (verde)
        r = (pixel & 0xFF)*(_intensidade/100.00);         // Isola os 8 bits menos significativos (vermelho)
        pixel = 0;
        pixel = (g << 16) | (r << 8) | b;
        pio_sm_put_blocking(pio, sm, pixel<<8u);
    }

    for(int i = 4; i > -1 ; i--) //Linha 5
    {
        pixel = _matriz[i];
        b = ((pixel >> 16) & 0xFF)*(_intensidade/100.00); // Isola os 8 bits mais significativos (azul)
        g = ((pixel >> 8) & 0xFF)*(_intensidade/100.00);  // Isola os 8 bits intermediários (verde)
        r = (pixel & 0xFF)*(_intensidade/100.00);         // Isola os 8 bits menos significativos (vermelho)
        pixel = 0;
        pixel = (g << 16) | (r << 8) | b;
        pio_sm_put_blocking(pio, sm, pixel<<8u);
    }
}

int main()
{
    pio = pio0; // Define o PIO 0
    bool clock_setado = set_sys_clock_khz(133000, false); // Configura o clock do sistema para 133 MHz
    stdio_init_all(); // Inicializa a comunicação serial
    if (clock_setado)
        printf("Clock setado %ld\n", clock_get_hz(clk_sys)); // Exibe mensagem na serial

    int offset = pio_add_program(pio, &Matriz_5x5_program); // Carrega programa PIO para matriz de LEDs
    sm = pio_claim_unused_sm(pio, true); // Obtém um state machine livre para PIO
    Matriz_5x5_program_init(pio, sm, offset, MATRIZ_PIN); // Inicializa o programa PIO
    desenha_fig(numero_1, BRILHO_PADRAO, pio, sm); // Exibe o número 8 na matriz de LEDs

    configurar_i2c(); // Configura o barramento I2C
    ssd1306_t ssd;
    configurar_display(&ssd); // Configura o display OLED

    while (true)
    {
        int c = getchar_timeout_us(1000); // Captura entrada da serial
        
        if ((c != PICO_ERROR_TIMEOUT) && (c != 10)) // Se recebeu um caractere válido
        {
            char tecla = (char)c; // Converte para caractere
            printf("Tecla: %c\n", tecla); // Exibe no terminal
            
            ssd1306_draw_string(&ssd, &tecla, 20, 30); // Exibe no OLED
            ssd1306_send_data(&ssd); // Atualiza o display
    
            // Se for um número, exibe na matriz de LEDs
            if (tecla >= '0' && tecla <= '9') 
            {
                int numero = tecla - '0'; // Converte o caractere para número inteiro
    
                // Exibe o número correspondente na matriz de LEDs
                switch (numero)
                {
                    case 0: desenha_fig(numero_0, BRILHO_PADRAO, pio, sm); break;
                    case 1: desenha_fig(numero_1, BRILHO_PADRAO, pio, sm); break;
                    case 2: desenha_fig(numero_2, BRILHO_PADRAO, pio, sm); break;
                    case 3: desenha_fig(numero_3, BRILHO_PADRAO, pio, sm); break;
                    case 4: desenha_fig(numero_4, BRILHO_PADRAO, pio, sm); break;
                    case 5: desenha_fig(numero_5, BRILHO_PADRAO, pio, sm); break;
                    case 6: desenha_fig(numero_6, BRILHO_PADRAO, pio, sm); break;
                    case 7: desenha_fig(numero_7, BRILHO_PADRAO, pio, sm); break;
                    case 8: desenha_fig(numero_8, BRILHO_PADRAO, pio, sm); break;
                    case 9: desenha_fig(numero_9, BRILHO_PADRAO, pio, sm); break;
                }
            }
        }
    
        sleep_ms(100); // Pequeno delay
    }
    

}
    
