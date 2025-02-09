# Atividade - COMUNICAÇÃO UART

Este projeto foi desenvolvido para aprimorar o conhecimento sobre comunicão UART e do protocolo I2C utilizando o **Raspberry Pi Pico W**. Foi realizado com o auxílio do simulador de eletrônica **Wokwi** e da placa **BitDogLab**.

## Componentes Utilizados

1. **LED RGB** (Vermelho, Verde e Azul)
2. **Matriz de LED 5x5 WS2812**
3. **Display SSD1306**
4. **Microcontrolador Raspberry Pi Pico W**
5. **Dois botões Pushbutton**

## Funcionalidade

Usando comunicação UART ao digitar qualquer letra de A-Z (maiúscula ou minúscula) irá ser exibida no display SSD1306.
Caso seja digitado um número de 0 a 9, o mesmo será exibido na matriz de led WS2812 bem como no display SSD1306;

### Funcionalidades dos Botões

- **Botão A**: ligará/desligará o LED verde informando no display SSD1306 bem como no serial monitor.
- **Botão B**: ligará/desligará o LED azul informando no display SSD1306 bem como no serial monitor.

### Como Usar

#### Usando o Simulador Wokwi

- Clone este repositório: git clone https://github.com/rober1o/Atividade_03_02--U4C6O12T.git;
- Usando a extensão Raspberry Pi Pico importar o projeto;
- Compilar o código e clicar no arquivo diagram.json.

#### Usando a BitDogLab

- Clone este repositório: git clone https://github.com/rober1o/Atividade_03_02--U4C6O12T.git;
- Usando a extensão Raspberry Pi Pico importar o projeto;
- Compilar o projeto;
- Plugar a BitDogLab usando um cabo apropriado e gravar o código.

## Atenção

O brilho dos LEDs é muito intenso, para evitar danos aos olhos é recomendável que o brilho_padrao seja ajustado para 10% quando estiver usando o kit BitDogLab.

## Demonstração

<!-- TODO: adicionar link do vídeo -->
Vídeo demonstrando as funcionalidades da solução implementada: [Demonstração](https://youtu.be/j9AB2I5si5c)
