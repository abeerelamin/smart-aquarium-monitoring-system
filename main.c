/*
Smart Aquarium Monitoring System
Dragon Plus Board (MC9S12DG256)

Reads:
- Temperature (DS18B20 on PB2)
- Turbidity (ADC channel 2)
- Water Level (ADC channel 3)
- pH (ADC channel 4)

Controls:
- Cooling Fan (PB0)
- Water Pump (PB1)

Transmits sensor data over serial (SCI 9600 baud).
*/

#include <hidef.h>
#include <mc9s12dg256.h>
#pragma LINK_INFO DERIVATIVE "mc9s12dg256b"

#include "main_asm.h"
#include <stdio.h>

/* ---------------- Pin Definitions ---------------- */

#define DS18B20_PIN  0x04
#define DS18B20_PORT PORTB
#define DS18B20_DDR  DDRB

/* ---------------- Function Prototypes ---------------- */

void delay_us(unsigned int us);
void delay_ms(unsigned int ms);

void OneWire_Reset(void);
void OneWire_WriteByte(unsigned char data);
unsigned char OneWire_ReadByte(void);
float ReadTemperature(void);

void intToStr(int num, char *str);

/* ---------------- Main ---------------- */

void main(void)
{
    char str1[6];
    char str2[6];
    char str3[6];
    char str4[6];

    char *t   = "TEMP:";
    char *tbd = "TBD:";
    char *ph  = "pH:";
    char *wl  = "WL:";

    int i = 0;
    float temperature = 0.0;
    int tbd_reading = 0;
    int ph_reading  = 0;
    int wl_reading  = 0;

    PLL_init();
    DDRB |= DS18B20_PIN;
    DDRB |= 0x03;

    lcd_init();
    ad0_enable();
    SCI0_init(9600);
    led_disable();
    seg7_disable();

    while (1)
    {
        /* -------- Temperature -------- */

        temperature = ReadTemperature();

        set_lcd_addr(0x00);
        type_lcd(t);
        set_lcd_addr(0x05);
        write_int_lcd((int)temperature);

        intToStr((int)temperature, str1);
        for (i = 0; str1[i] != '\0'; i++)
            outchar0(str1[i]);

        if (temperature > 29)
            PORTB |= 0x01;
        else
            PORTB &= ~0x01;

        ms_delay(2000);

        /* -------- Water Level -------- */

        wl_reading = ad0conv(3);

        set_lcd_addr(0x40);
        type_lcd(wl);
        set_lcd_addr(0x45);
        write_int_lcd(wl_reading);

        intToStr(wl_reading, str2);
        for (i = 0; str2[i] != '\0'; i++)
            outchar0(str2[i]);

        if (wl_reading < 29)
            PORTB |= 0x02;
        else
            PORTB &= ~0x02;

        ms_delay(2000);
        clear_lcd();

        /* -------- Turbidity -------- */

        tbd_reading = ad0conv(2);

        set_lcd_addr(0x00);
        type_lcd(tbd);
        set_lcd_addr(0x05);
        write_int_lcd(tbd_reading);

        intToStr(tbd_reading, str3);
        for (i = 0; str3[i] != '\0'; i++)
            outchar0(str3[i]);

        ms_delay(2000);

        /* -------- pH -------- */

        ph_reading = ad0conv(4);

        set_lcd_addr(0x40);
        type_lcd(ph);
        set_lcd_addr(0x45);
        write_int_lcd(ph_reading);

        intToStr(ph_reading, str4);
        for (i = 0; str4[i] != '\0'; i++)
            outchar0(str4[i]);

        ms_delay(2000);
        clear_lcd();
    }
}

/* ---------------- OneWire Functions ---------------- */

void OneWire_Reset(void)
{
    DS18B20_DDR |= DS18B20_PIN;
    DS18B20_PORT &= ~DS18B20_PIN;
    delay_us(480);

    DS18B20_PORT |= DS18B20_PIN;
    DS18B20_DDR &= ~DS18B20_PIN;
    delay_us(70);
    delay_us(410);
}

void OneWire_WriteByte(unsigned char data)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        DS18B20_DDR |= DS18B20_PIN;
        DS18B20_PORT &= ~DS18B20_PIN;
        delay_us(2);

        if (data & 0x01)
            DS18B20_PORT |= DS18B20_PIN;
        else
            DS18B20_PORT &= ~DS18B20_PIN;

        delay_us(60);
        DS18B20_PORT |= DS18B20_PIN;
        delay_us(1);

        data >>= 1;
    }
}

unsigned char OneWire_ReadByte(void)
{
    unsigned char data = 0;
    int i;

    for (i = 0; i < 8; i++)
    {
        DS18B20_DDR |= DS18B20_PIN;
        DS18B20_PORT &= ~DS18B20_PIN;
        delay_us(2);

        DS18B20_PORT |= DS18B20_PIN;
        DS18B20_DDR &= ~DS18B20_PIN;
        delay_us(8);

        if (DS18B20_PORT & DS18B20_PIN)
            data |= (1 << i);

        delay_us(60);
    }

    return data;
}

float ReadTemperature(void)
{
    unsigned char temp_lsb, temp_msb;
    int temp_raw;
    float temp_celsius;

    OneWire_Reset();
    OneWire_WriteByte(0xCC);
    OneWire_WriteByte(0x44);
    delay_ms(750);

    OneWire_Reset();
    OneWire_WriteByte(0xCC);
    OneWire_WriteByte(0xBE);

    temp_lsb = OneWire_ReadByte();
    temp_msb = OneWire_ReadByte();

    temp_raw = (temp_msb << 8) | temp_lsb;
    temp_celsius = temp_raw * 0.0625;

    return temp_celsius;
}

/* ---------------- Utility Functions ---------------- */

void delay_ms(unsigned int milliseconds)
{
    unsigned int i, j;
    for (i = 0; i < milliseconds; i++)
        for (j = 0; j < 4000; j++)
            asm("nop");
}

void delay_us(unsigned int microseconds)
{
    unsigned int i;
    for (i = 0; i < microseconds; i++)
        asm("nop");
}

void intToStr(int num, char *str)
{
    int i = 0;
    int place = 1;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0)
    {
        str[i++] = '-';
        num = -num;
    }

    while (num / place >= 10)
        place *= 10;

    while (place > 0)
    {
        int digit = num / place;
        str[i++] = '0' + digit;
        num %= place;
        place /= 10;
    }

    str[i] = '\0';
}
