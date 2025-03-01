# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Detectar o sistema operacional para definir o caminho do SDK
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

# Definir versões do SDK e ferramentas
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)

# Incluir o suporte do VS Code se o arquivo existir
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# Definir o tipo de placa (altere para 'pico' ou 'pico_w' conforme necessário)
set(PICO_BOARD pico CACHE STRING "Board type")

# Incluir o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

# Definir o projeto
project(Tarefa_03_02 C CXX ASM)

# Inicializar o SDK do Pico
pico_sdk_init()

# Adicionar executável e arquivos necessários
add_executable(Tarefa_03_02 
    Tarefa_03_02.c 
    inc/ssd1306.c  # Arquivo de controle do display OLED
)

# Definir nome e versão do programa
pico_set_program_name(Tarefa_03_02 "Tarefa_03_02")
pico_set_program_version(Tarefa_03_02 "0.1")

# Gerar cabeçalho PIO (se necessário)
pico_generate_pio_header(Tarefa_03_02 ${CMAKE_CURRENT_LIST_DIR}/pio_wave.pio)

# Habilitar saída via UART/USB
pico_enable_stdio_uart(Tarefa_03_02 1)
pico_enable_stdio_usb(Tarefa_03_02 1)

# Vincular bibliotecas necessárias
target_link_libraries(Tarefa_03_02
    pico_stdlib
    hardware_pio   # Suporte para LEDs endereçáveis
    hardware_i2c   # Suporte para o display OLED via I2C
)

# Incluir diretórios necessários
target_include_directories(Tarefa_03_02 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/inc  # Certifique-se de que "inc" contém os headers adequados
)

# Gerar arquivos de saída adicionais
pico_add_extra_outputs(Tarefa_03_02)
