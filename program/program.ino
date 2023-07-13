#include "BluetoothSerial.h"
#include "M5_ENV.h"
#include <M5Stack.h>

// SHT30 (温度、湿度センサ)
SHT3X sht30;

// Bluetooth
BluetoothSerial bts;

// 温度
float temperature = 0.0;
// 湿度
float humidity = 0.0;
// 光センサの値
int lightValue = 0;

// 適温の範囲
const float optimumTempMin = 20.0;
const float optimumTempMax = 30.0;

// 日なたの上限値
const float hinataValueMax = 100;

void setup()
{
    // M5Stack
    M5.begin();
    M5.Power.begin();
    // Bluetooth
    bts.begin("Sensor Network C-team");
    // I2C
    Wire.begin();
    // 液晶
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(4);
}

// 温度と湿度を取得
void getEnvirons()
{
    if (sht30.get() == 0)
    {
        temperature = sht30.cTemp;
        humidity = sht30.humidity;
    }
    else
    {
        temperature = 0;
        humidity = 0;
    }
}

bool IsOptimumTemp()
{
    return optimumTempMin <= temperature && temperature < optimumTempMax;
}

bool IsHinataValue()
{
    return lightValue <= hinataValueMax;
}

void loop()
{
    // 温度と湿度を取得
    getEnvirons();

    // 光センサの値を取得
    lightValue = analogRead(36);

    // 液晶表示
    M5.lcd.setCursor(5, 50);
    M5.Lcd.printf("Temperature: %2.1f'C", temperature);
    M5.lcd.setCursor(5, 80);
    M5.Lcd.printf("Humidity: %2.0f%%", humidity);
    M5.lcd.setCursor(5, 110);
    M5.Lcd.printf("Light: %d", lightValue);

    M5.lcd.setCursor(5, 140);

    if (IsOptimumTemp() && IsHinataValue())
    {
        M5.Lcd.setTextColor(GREEN);
        M5.lcd.printf("This is optimum temperature and hinata!");
        M5.Lcd.setTextColor(WHITE);
        bts.println("適温かつひなたです");
    }
    else if (!IsOptimumTemp())
    {
        M5.Lcd.setTextColor(RED);
        M5.lcd.printf("This is not optimum temperature!");
        M5.Lcd.setTextColor(WHITE);
        bts.println("適温ではありません");
    }
    else if (!IsHinataValue())
    {
        M5.Lcd.setTextColor(RED);
        M5.lcd.printf("This is hikage!");
        M5.Lcd.setTextColor(WHITE);
        bts.println("日かげになっています");
    }
    else
    {
        M5.Lcd.setTextColor(RED);
        M5.lcd.printf("This is not optimum temperature and hikage!");
        M5.Lcd.setTextColor(WHITE);
        bts.println("適温ではないかつ、日かげです");
    }
 
    delay(10000);
    M5.Lcd.clear(BLACK);
}
