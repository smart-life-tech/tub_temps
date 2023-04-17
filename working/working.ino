#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

namespace
{
    Waveshare_ILI9486 Waveshield;

    //  Provide an alias so I don't have to search-and-replace all the code below.
    Adafruit_GFX &tft = Waveshield;

    //
    //  This is all copied from the original 'Waveshare _ILI9486' project.
    unsigned long testFillScreen();
    unsigned long testText();
    unsigned long testRects(uint16_t color);

    uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
    }

    typedef struct
    {
        PGM_P msg;
        uint32_t ms;
    } TEST;
    TEST result[12];

#define RUNTEST(n, str, test)      \
    {                              \
        result[n].msg = PSTR(str); \
        result[n].ms = test;       \
        delay(500);                \
    }
    void runtests(void)
    {
        uint8_t i, len = 24;
        uint32_t total;
        RUNTEST(0, "FillScreen               ", testFillScreen());
        yield();
        RUNTEST(1, "Text                     ", testText());
        yield();
    }
    // Standard Adafruit tests.  will adjust to screen size

    unsigned long testFillScreen()
    {
        unsigned long start = micros();
        //  tft.fillScreen(BLACK);
        //  tft.fillScreen(RED);
        //  tft.fillScreen(GREEN);
        // tft.fillScreen(BLUE);
        tft.fillScreen(WHITE);
        return micros() - start;
    }

    unsigned long testText()
    {
        unsigned long start;
        start = micros();
        tft.setCursor(0, 0);
        tft.setTextColor(BLACK);
        tft.setTextSize(1);
        tft.print(" Desired Temp");

        tft.setCursor(400, 0);
         tft.drawRect(400 , 0, 50, 100, BLACK);
        tft.setTextColor(BLACK);
        tft.setTextSize(1);
        tft.print("Settings");

        tft.setCursor(450, 0);
        tft.setTextColor(BLACK);
        tft.setTextSize(1);
        tft.println("wifi status");

        tft.setCursor(0, 50);
        tft.setTextColor(BLACK);
        tft.setTextSize(1);
        tft.print(" HotTub Temp");

        tft.setCursor(0, 100);
        tft.drawRect(0 , 100, 50, 100, BLACK);
        tft.setTextColor(BLACK);
        tft.setTextSize(1);
        tft.print(" Desired Temp");

        tft.setCursor(0, 150);
         tft.drawRect(0 , 150, 50, 100, BLACK);
        tft.setTextColor(BLACK);
        tft.setTextSize(1);
        tft.print(" valve open");

        tft.setCursor(400, 150);
         tft.drawRect(400 , 150, 50, 100, BLACK);
        tft.setTextColor(BLACK);
        tft.setTextSize(1);
        tft.print(" button control");
        return micros() - start;
    }         

}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    SPI.begin();
    Waveshield.begin();
}

// the loop function runs over and over again until power down or reset
void loop()
{
    //  Run the Adafruit tests.
    //  We'll run using a different rotation to show that everything works!
     //  Get raw touchscreen values.
    TSPoint p = Waveshield.getPoint();

    //  Remaps raw touchscreen values to screen co-ordinates.  Automatically handles
    //  rotation!
    Waveshield.normalizeTsPoint(p);

    //  Now that we have a point in screen co-ordinates, draw something there.
    Waveshield.fillCircle(p.x, p.y, 3, BLUE);
    tft.setRotation(0);
    runtests();
}
