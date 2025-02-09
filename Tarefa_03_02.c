#include "Tarefa_03_02.h"  // Inclui o cabeçalho do projeto



int main()
{
    // Define o PIO 0 para controle da matriz de LEDs
    pio = pio0;

    // Configura o clock do sistema para 133 MHz
    bool clock_setado = set_sys_clock_khz(133000, false);
    
    // Inicializa a comunicação serial
    stdio_init_all();
    
    // Exibe mensagem na serial caso o clock tenha sido configurado com sucesso
    if (clock_setado)
        printf("Clock setado %ld\n", clock_get_hz(clk_sys));

    // Carrega o programa PIO para controle da matriz de LEDs
    int offset = pio_add_program(pio, &Matriz_5x5_program);
    
    // Obtém um state machine livre para o PIO
    sm = pio_claim_unused_sm(pio, true);
    
    // Inicializa o programa PIO na matriz de LEDs
    Matriz_5x5_program_init(pio, sm, offset, MATRIZ_PIN);
    
    // Exibe o número 8 na matriz de LEDs como padrão
    desenha_fig(matriz_apagada, BRILHO_PADRAO, pio, sm);

    // Configura o barramento I2C para comunicação com periféricos
    configurar_i2c();
    
    // Configura o display OLED SSD1306
    configurar_display(&ssd);
    
    // Configura os pinos necessários para o funcionamento do sistema
    iniciar_pinos();

    // Loop principal
    while (true)
    {
        tratar_entrada_serial(); // Chama a função para processar a entrada da UART
        sleep_ms(100); // Pequeno delay para evitar processamento excessivo
    }
}

// Função para exibir um número na matriz de LEDs
void exibir_numero_matriz(int numero)
{
    // Vetor com os padrões de cada número
    const uint8_t *numeros[10] = {numero_0, numero_1, numero_2, numero_3, numero_4, 
                                  numero_5, numero_6, numero_7, numero_8, numero_9};

    // Exibe o número correspondente, se estiver no intervalo válido
    if (numero >= 0 && numero <= 9)
    {
        desenha_fig(numeros[numero], BRILHO_PADRAO, pio, sm);
    }
}




// Função para tratar entrada da serial
void tratar_entrada_serial()
{
    // Captura entrada do teclado via serial (timeout de 1000 microsegundos)
    int c = getchar_timeout_us(1000);

    // Se recebeu um caractere válido (diferente de timeout e de nova linha)
    if ((c != PICO_ERROR_TIMEOUT) && (c != 10))
    {
        char tecla = (char)c; // Converte para caractere
        
        // Exibe a tecla pressionada no terminal
        printf("Tecla: %c\n", tecla);

        // Limpa o display OLED
        ssd1306_fill(&ssd, false);
        
        // Exibe a tecla pressionada no display OLED
        ssd1306_draw_string(&ssd, &tecla, 20, 30);
        
        // Atualiza o display OLED
        ssd1306_send_data(&ssd);

        // Se for um número, exibe na matriz de LEDs
        if (tecla >= '0' && tecla <= '9') 
        {
            int numero = tecla - '0'; // Converte o caractere numérico para inteiro
            
            // Chama a função para exibir o número na matriz de LEDs
            exibir_numero_matriz(numero);
        }
    }
}




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

//Iniciar LED e BOTÕES

// Rotina para iniciar e configurar os LEDs e botões

void iniciar_pinos() {
    gpio_init(LED_RED); 
    gpio_init(LED_BLUE);
    gpio_init(LED_GREEN);    
    gpio_set_dir(LED_RED, GPIO_OUT); 
    gpio_set_dir(LED_BLUE, GPIO_OUT); 
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_A); 
    gpio_pull_up(BOTAO_B);

    // Configura interrupção no botão
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);
}

// Função para verificar debounce
bool debounce_ok(uint gpio) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time()); 
    
    if (gpio == BOTAO_A) {
        if (tempo_atual - ultimo_tempoA > DEBOUNCE_TIME) {
            ultimo_tempoA = tempo_atual;
            return true;
        }
    } else if (gpio == BOTAO_B) {
        if (tempo_atual - ultimo_tempoB > DEBOUNCE_TIME) {
            ultimo_tempoB = tempo_atual;
            return true;
        }
    }
    return false;
}

// Função de interrupção do botão
void gpio_irq_handler(uint gpio, uint32_t events) {

    if ((events & GPIO_IRQ_EDGE_FALL) && debounce_ok(gpio)) { 

        if (gpio == BOTAO_A) {
            LED_VERDE_ACESO = !LED_VERDE_ACESO;
            gpio_put(LED_GREEN, LED_VERDE_ACESO);
            if(LED_VERDE_ACESO){
                ssd1306_fill(&ssd, false); // Limpa o display
                printf("O LED VERDE FOI ACESO\n");
                ssd1306_draw_string(&ssd, "LED VERDE ON\n", 20, 30); // Exibe no OLED
                ssd1306_send_data(&ssd); // Atualiza o display
            }else{
                ssd1306_fill(&ssd, false); // Limpa o display
                printf("O LED VERDE FOI APAGADO\n");
                ssd1306_draw_string(&ssd, "LED VERDE OFF\n", 20, 30); // Exibe no OLED
                ssd1306_send_data(&ssd); // Atualiza o display
            }
        } else if (gpio == BOTAO_B) { 
            LED_AZUL_ACESO = !LED_AZUL_ACESO;
            gpio_put(LED_BLUE, LED_AZUL_ACESO);
            if(LED_AZUL_ACESO){
                ssd1306_fill(&ssd, false); // Limpa o display
                printf("O LED AZUL FOI ACESO\n");
                ssd1306_draw_string(&ssd, "LED AZUL ON\n", 20, 30); // Exibe no OLED
                ssd1306_send_data(&ssd); // Atualiza o display
            }else{
                ssd1306_fill(&ssd, false); // Limpa o display
                printf("O LED AZUL FOI APAGADO\n");
                ssd1306_draw_string(&ssd, "LED AZUL OFF\n", 20, 30); // Exibe no OLED
                ssd1306_send_data(&ssd); // Atualiza o display
            }
        }
    }
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
    
