# Projeto: Interface de Comunicação com Raspberry Pi Pico

Este projeto implementa a comunicação serial e exibição de caracteres utilizando a **Raspberry Pi Pico**, um **display SSD1306** via **I2C**, botões de entrada com interrupções, LEDs indicadores e uma matriz de LEDs controlada pela PIO.

## 📌 Funcionalidades
- Leitura de caracteres via **UART (Serial)**
- Exibição de caracteres no **display SSD1306**
- Controle de LEDs com **botões físicos**
- Uso de **interrupções GPIO** para alternar LEDs
- Renderização de números em uma **matriz de LEDs**
- Comunicação via **I2C** e **PIO**
- Debounce nos botões

---

## 🛠️ Configuração de Hardware
### 📌 Componentes Utilizados
| Componente   | Descrição |
|-------------|-----------|
| Raspberry Pi Pico | Microcontrolador principal |
| Display OLED SSD1306 | Comunicação via **I2C** |
| Botões físicos | GPIOs configurados como entrada |
| LEDs (Verde e Azul) | Indicadores acionados via interrupções |
| Matriz de LEDs | Controlada via **PIO** |

### 📌 Conexões GPIO
| Componente | Pino Pico |
|------------|----------|
| SDA (OLED) | **GPIO14** |
| SCL (OLED) | **GPIO15** |
| Botão A | **GPIO5** |
| Botão B | **GPIO6** |
| LED Verde | **GPIO11** |
| LED Azul | **GPIO12** |

---

## 🌟 Estrutura do Código
```
├── inc/
│   ├── ssd1306.h      # Biblioteca do display OLED
│   ├── font.h         # Fonte para o display OLED
├── libs/
│   ├── leds.h         # Controle dos LEDs
│   ├── animation.h    # Animações para matriz de LEDs
│   ├── leds.c         # Implementação do controle de LEDs
│   ├── animation.c    # Animação da matriz de LEDs
├── pio_matrix.pio.h   # Código PIO para controle da matriz de LEDs
├── tarefa-interfaces-de-comunicacao.c  # Código principal
└── README.md          # Documentação do projeto
```

---

## 📌 Explicação das Funções Principais

### 🔹 `setup_display()`
Configura e inicializa o **display OLED SSD1306** via **I2C**.

### 🔹 `setup_led(uint gpio)`
Inicializa um LED como **saída** e o desliga.

### 🔹 `setup_button(uint gpio)`
Configura um **botão** como entrada com **pull-up** ativado.

### 🔹 `gpio_irq_handler(uint gpio, uint32_t events)`
- Alterna LEDs ao pressionar **botões físicos**.
- Utiliza um **sistema de debounce**.

### 🔹 `display_character(char ch)`
Mostra um **único caractere** no **display OLED**.

### 🔹 `display_message(char *message)`
Exibe uma **mensagem** no **display OLED**.

### 🔹 `enable_interrupt()`
Ativa **interrupções GPIO** para os botões físicos.

### 🔹 `process_serial_input()`
- Aguarda **entrada via UART**.
- Exibe o caractere lido no **display OLED**.
- Se for um número, aciona a **matriz de LEDs** via PIO.

---

## 💻 Interfaces de Comunicação
### 🔹 **UART (Serial)**
- Recebe caracteres via terminal serial.
- Exibe no **display OLED**.
- Aciona a **matriz de LEDs** se for um número (`0-9`).

### 🔹 **I2C**
- Controla o **display OLED SSD1306**.

### 🔹 **PIO**
- Aciona a **matriz de LEDs** para exibir números.

---

## 📝 Melhorias Futuras
✅ Adicionar suporte a comunicação SPI.  
✅ Criar um menu de interação no display OLED.  
✅ Implementar animações para a matriz de LEDs.  

---

## Vídeo

Para o projeto em questão o vídeo demnstrativo pode ser visto no link: https://youtu.be/3TBovhd8Wk0?si=LQKpDzXFd3xdS7Ja.

## 📢 Autor
👤 **Desenvolvido por Richard Lima Ribeiro**  
📧 Contato: **richard.esclima@gmail.com**  

---

