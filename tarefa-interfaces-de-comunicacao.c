#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"

// bibliotecas personalizadas
#include "libs/leds.h"
#include "libs/animation.h"
#include "pio_matrix.pio.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define BUTTON_A 5
#define BUTTON_B 6
#define GREEN_LED 11
#define BLUE_LED 12

uint last_interrupt_a = 0;
uint last_interrupt_b = 0;
uint DEBOUNCE_MS = 200;
ssd1306_t ssd;

void PIO_setup(PIO *pio, uint *sm)
{
    // configurações da PIO
    *pio = pio0;
    uint offset = pio_add_program(*pio, &pio_matrix_program);
    *sm = pio_claim_unused_sm(*pio, true);
    pio_matrix_program_init(*pio, *sm, offset, LED_PIN);
}

void generateAscTest(char *upperCase, char *lowerCase)
{
    if (*upperCase <= 'Z')
    {
        (*upperCase)++;
        (*lowerCase)++;
    }
    else
    {
        *upperCase = 'A';
        *lowerCase = 'a';
    }
    return;
}

void displayCharacter(char ch)
{
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);

    char string[2] = {ch, '\0'};

    ssd1306_draw_string(&ssd, "Caractere lido", 8, 10);
    ssd1306_draw_string(&ssd, string, 50, 25);

    ssd1306_send_data(&ssd);
    sleep_ms(1000);
}
void display_message(char *message)
{
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);

    ssd1306_draw_string(&ssd, message, 50, 25);

    ssd1306_send_data(&ssd);
}

void setup_button(uint gpio)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
}

void setup_led(uint gpio)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
    gpio_put(gpio, false);
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint current_time = to_ms_since_boot(get_absolute_time());

    if (gpio == BUTTON_A)
    {
        if (current_time - last_interrupt_a > DEBOUNCE_MS)
        {
            last_interrupt_a = current_time;
            gpio_put(BLUE_LED, false);
            gpio_put(GREEN_LED, true);
            printf("led verde aceso");
            display_message("led verde aceso");
        }
    }
    if (gpio == BUTTON_B)
    {
        if (current_time - last_interrupt_b > DEBOUNCE_MS)
        {
            last_interrupt_b = current_time;
            gpio_put(GREEN_LED, false);
            gpio_put(BLUE_LED, true);
            printf("led azul aceso");
            display_message("led azul aceso");
        }
    }
}

void enable_interrupt()
{
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
}
int main()
{

    PIO pio;
    uint sm;
    // char string[3] = {upperCase, lowerCase, '\0'};
    // generateAscTest(&upperCase, &lowerCase);
    stdio_init_all();
    // Inicializa a I2c
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    char lowerCase = 'a', upperCase = 'A';

    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    ssd1306_draw_string(&ssd, "ESCREVA UM", 8, 10);
    ssd1306_draw_string(&ssd, "CARACTERE", 8, 24);
    ssd1306_send_data(&ssd);

    // leds
    setup_led(GREEN_LED);
    setup_led(BLUE_LED);
    // PIO
    PIO_setup(&pio, &sm);
    // botões
    setup_button(BUTTON_A);
    setup_button(BUTTON_B);
    // interrupções
    enable_interrupt();

    clear_all_leds(pio, sm);

    while (true)
    {

        // display_message("abcdefghijklmnopqrstuvwxyz");
        if (stdio_usb_connected())
        { // Certifica-se de que o USB está conectado
            char c;
            if (scanf("%c", &c) == 1)
            { // Lê caractere da entrada padrão
                    clear_all_leds(pio, sm);
                printf("Recebido: '%c'\n", c);
                displayCharacter(c);

                if (c >= '0' && c <= '9')
                {
                    uint index = c - '0';
                    draw_number(pio, sm, index, true);
                }
            }
        }
        sleep_ms(40);
    }
}
