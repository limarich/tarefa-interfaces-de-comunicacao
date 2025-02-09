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

void displayCharacter(char ch, ssd1306_t ssd)
{
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);

    char string[2] = {ch, '\0'};

    ssd1306_draw_string(&ssd, "Caractere lido", 8, 10);
    ssd1306_draw_string(&ssd, string, 50, 25);

    ssd1306_send_data(&ssd);
    sleep_ms(1000);
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
    ssd1306_t ssd;
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

    // PIO
    PIO_setup(&pio, &sm);

    while (true)
    {
        if (stdio_usb_connected())
        { // Certifica-se de que o USB está conectado
            char c;
            if (scanf("%c", &c) == 1)
            { // Lê caractere da entrada padrão
                printf("Recebido: '%c'\n", c);
                displayCharacter(c, ssd);

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
