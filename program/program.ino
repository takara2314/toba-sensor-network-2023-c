#include "BluetoothSerial.h"
#include "M5_ENV.h"
#include <M5Stack.h>

// SHT30 (温度、湿度センサ)
SHT3X sht30;

// Bluetooth
BluetoothSerial bts;

float temperature = 0.0;
float humidity = 0.0;

void setup()
{
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

void loop()
{
    // 温度と湿度を取得
    getEnvirons();

    // Bluetooth通信
    bts.println(temperature);

    // 液晶表示
    M5.lcd.setCursor(5, 50);
    M5.Lcd.printf("Temperature: %2.1f'C", temperature);
    M5.lcd.setCursor(5, 80);
    M5.Lcd.printf("Humidity: %2.0f%%", humidity);

    delay(1000);
}
