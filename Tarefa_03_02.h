#ifndef TAREFA_03_02  // Previne múltiplas inclusões do cabeçalho
#define TAREFA_03_02

// =============================
// Bibliotecas padrão e específicas do Raspberry Pi Pico
// =============================
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"   // Biblioteca padrão do Raspberry Pi Pico
#include "hardware/pio.h"  // Controle do PIO (Programável I/O)
#include "hardware/clocks.h" // Manipulação de clock
#include "hardware/i2c.h"  // Comunicação I2C
#include "pico/bootrom.h"  // Funções de bootloader
#include "pico/time.h"     // Manipulação de tempo

// =============================
// Bibliotecas do projeto
// =============================
#include "pio_wave.pio.h"  // Código PIO 
#include "numeros.h"       // Arrays para exibir na matriz de LED
#include "inc/ssd1306.h"   // Controle do display OLED SSD1306
#include "inc/font.h"      // Manipulação de fontes

// =============================
// Definições de hardware
// =============================

// LEDs RGB
#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13

// Botões
#define BOTAO_A 5
#define BOTAO_B 6

// Configurações gerais
#define BRILHO_PADRAO 100  // Intensidade padrão do brilho
#define DEBOUNCE_TIME 200  // Tempo de debounce para os botões (ms)

// Matriz de LEDs
#define MATRIZ_PIN 7       // Pino da matriz de LEDs
#define NUM_PIXELS 25      // Número de pixels na matriz

// Configurações I2C (Display OLED)
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO_DISPLAY 0x3C  // Endereço I2C do display OLED

// =============================
// Variáveis globais
// =============================

volatile uint32_t ultimo_tempoA = 0;  // Última leitura do botão A
volatile uint32_t ultimo_tempoB = 0;  // Última leitura do botão B
volatile bool ligado = false;         // Estado do sistema


// Estado dos LEDs
bool LED_VERDE_ACESO = false;
bool LED_AZUL_ACESO = false;

// Instâncias de hardware
ssd1306_t ssd;  // Display OLED
PIO pio;        // Instância do PIO
int sm;         // Máquina de estado do PIO

// =============================
// Declaração das funções
// =============================

void configurar_i2c(void);
bool debounce_ok(uint gpio);
void gpio_irq_handler(uint gpio, uint32_t events);
void iniciar_pinos(void);
void desenha_fig(uint32_t *_matriz, uint8_t _intensidade, PIO pio, uint sm);
void configurar_display(ssd1306_t *ssd);

#endif // TAREFA_03_02
