#include "BluetoothSerial.h"
#include <M5Stack.h>

// SHT3X (温度、湿度センサ) I2C通信アドレス
#define SHT3X_ADDR 0x44

BluetoothSerial bts;

void setup()
{
    M5.begin();

    // Bluetooth
    bts.begin("Sensor Network C-team");

    // I2C
    Wire.begin(32, 33);

    // 液晶
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(4);
}

void getEnvirons(float environs[2])
{
    // 温度、湿度データ一時格納用
    unsigned int data[6];

    // I2C
    Wire.beginTransmission(SHT3X_ADDR);
    Wire.write(0x2C);
    Wire.write(0x06);
    Wire.endTransmission(true);

    // データ取得
    Wire.requestFrom(SHT3X_ADDR, 6);
    for (int i = 0; i < 6; i++)
    {
        data[i] = Wire.read();
    }

    // 温度
    environs[0] = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
    // 湿度
    environs[1] = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);
}

void loop()
{
    float environs[2] = {0.0, 0.0};
    getEnvirons(environs);

    // Bluetooth通信
    bts.println(environs[0]);

    // 液晶表示
    M5.lcd.setCursor(5, 50);
    M5.Lcd.printf("Temperature: %2.1f'C  ", environs[0]);
    M5.lcd.setCursor(5, 80);
    M5.Lcd.printf("Humidity: %2.0f%%  ", environs[1]);

    delay(1000);
}
