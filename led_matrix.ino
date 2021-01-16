#include "LedControl.h"
#include "DS3231.h"
#include <Wire.h>

#define DATA_IN_PIN 12
#define CLK_PIN 11
#define CS_PIN 10

#define DISPLAY_COUNT 4
#define ROW_COUNT 8
#define COL_COUNT 8

LedControl lc = LedControl(DATA_IN_PIN, CLK_PIN, CS_PIN, DISPLAY_COUNT);

byte numbers[][8] = {
    {B00111100, //0
     B01000010,
     B01000110,
     B01001010,
     B01010010,
     B01100010,
     B01000010,
     B00111100},
    {B00001000, //1
     B00011000,
     B00001000,
     B00001000,
     B00001000,
     B00001000,
     B00001000,
     B00011100},
    {B00111100, //2
     B01000010,
     B00000010,
     B00000100,
     B00010000,
     B00100000,
     B01000000,
     B01111110},
    {B01111110, //3
     B00000100,
     B00001000,
     B00010000,
     B00001000,
     B00000100,
     B01000010,
     B00111100},
    {B00001000, //4
     B00001000,
     B00011000,
     B00101000,
     B01111110,
     B00001000,
     B00001000,
     B00001000},
    {B01111110, //5
     B01000000,
     B01000000,
     B01111100,
     B00000010,
     B00000010,
     B01000010,
     B00111100},
    {B00111100, //6
     B01000010,
     B01000010,
     B01000000,
     B01111100,
     B01000010,
     B01000010,
     B00111100},
    {B01111110, //7
     B01000010,
     B00000010,
     B00000100,
     B00001000,
     B00010000,
     B00010000,
     B00010000},
    {B00111100, //8
     B01000010,
     B01000010,
     B00111100,
     B01000010,
     B01000010,
     B01000010,
     B00111100},
    {B00111100, //9
     B01000010,
     B01000010,
     B00111110,
     B00000010,
     B00000010,
     B01000100,
     B00111000}
};
void setup()
{
    for (int d = 0; d < DISPLAY_COUNT; d++)
    {
        lc.shutdown(d, false); // shutdown false => wake up from power saving mode
        lc.setIntensity(d, 0); // Set intensity level 0 for d
        lc.clearDisplay(d);    // Clear Displays clear display d
    }
    Wire.begin();
}

void printNumber(int deviceNumber, int num)
{
    for (int row = 0; row < ROW_COUNT; row++) {
        for (int col = 0; col < COL_COUNT; col++) {
            lc.setLed(deviceNumber, ROW_COUNT-row-1, col, numbers[num][row]>>col & 1);
        }
    }
}





RTClib RTC;
int h1_old, h2_old, m1_old, m2_old;

void loop()
{
    DateTime now = RTC.now();
    int hour = now.hour();
    int min = now.minute();

    
    // hour digits
    int h1 = hour/10;
    int h2 = hour%10;

    // minute digits
    int m1 = min/10;
    int m2 = min%10;

    if (h1_old != h1 || h1 == 0) printNumber(0,h1);
    if (h2_old != h2 || h2 == 0) printNumber(1,h2);
    if (m1_old != m1 || m1 == 0) printNumber(2,m1);
    if (m2_old != m2 || m2 == 0) printNumber(3,m2);

    h1_old = h1;
    h2_old = h2;
    m1_old = m1;
    m2_old = m2;

    // loop every 30 seconds
    // not every 60 seconds, because if the code compiled exactly at 1200 hours
    // and the refresh duration is 60 seconds, it may be possible that the minute hasn't yet rolled over to the next value
    // which might make it look as if the clock is stuck for ~ 2 minutes.
    delay(30000);
}
