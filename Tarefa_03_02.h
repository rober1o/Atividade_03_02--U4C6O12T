#ifndef TAREFA_03_02  // Previne múltiplas inclusões do cabeçalho
#define TAREFA_03_02

// Bibliotecas padrão e específicas do Raspberry Pi Pico
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"   // Biblioteca padrão do Raspberry Pi Pico
#include "hardware/pio.h"  // Biblioteca para controle do PIO
#include "hardware/clocks.h" // Biblioteca para manipulação de clock
#include "hardware/i2c.h"  // Biblioteca para comunicação I2C
#include "pico/bootrom.h"  // Biblioteca para funções de bootloader
#include "pico/time.h"     // Biblioteca para manipulação de tempo

// Bibliotecas do projeto
#include "pio_wave.pio.h"  // Código PIO para geração de ondas
#include "numeros.h"       // Biblioteca com definições numéricas
#include "inc/ssd1306.h"   // Biblioteca do display OLED SSD1306
#include "inc/font.h"      // Biblioteca para manipulação de fontes

// Definições de pinos para LEDs e botões
#define LED_GREEN 11  // Pino do LED verde
#define LED_BLUE 12   // Pino do LED azul
#define LED_RED 13    // Pino do LED vermelho
#define BUTTON_A 5    // Pino do botão A
#define BUTTON_B 6    // Pino do botão B

// Configurações gerais
#define BRILHO_PADRAO 100  // Intensidade padrão do brilho
#define DEBOUNCE_TIME 200  // Tempo de debounce para os botões (em ms)
#define MATRIZ_PIN 7       // Pino da matriz de LEDs
#define NUM_PIXELS 25      // Número de pixels na matriz

// Definições de hardware para comunicação I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO_DISPLAY 0x3C // Endereço I2C do display OLED

// Variáveis globais
volatile uint32_t ultimo_tempoA = 0;  // Última leitura do botão A
volatile uint32_t ultimo_tempoB = 0;  // Última leitura do botão B
volatile bool ligado = false;         // Estado do sistema
int numero = 0;                       // Número a ser exibido
PIO pio;                              // Instância do PIO
int sm;                               // Máquina de estado do PIO

// Declaração das funções
void piscar_led_red();
bool debounce_ok(uint gpio);
void gpio_irq_handler(uint gpio, uint32_t events);
void exibir_numero(int numero);
void iniciar_pinos(void);
void desenha_fig(uint32_t *_matriz, uint8_t _intensidade, PIO pio, uint sm);

#endif // TAREFA_03_02
