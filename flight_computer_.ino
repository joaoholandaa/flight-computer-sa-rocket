#include <MPU6050.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <SPI.h>
#include <I2Cdev.h>
#include <Servo.h>
#include <SD.h>

MPU6050 mpu;
Adafruit_BMP085 mySensor;
const int chipSelect = 4;
double tempoInicio = 0;
double tempoFim = 0;
int16_t ax, ay, az; 
int16_t gx, gy, gz;
int vall;
int prevVall;
int buzzer = 8;
float tempc;
float pressure;
Servo servo1;

void setup() 
{
  Serial.begin(9600);
  mySensor.begin();
  Wire.begin();
  mpu.initialize();
  servo1.attach(9);
  pinMode(buzzer, OUTPUT);
  if(!SD.begin(chipSelect))
  {
   Serial.println("Falha ao conectar o cartao!");
   return;
  }
  Serial.println("O cartão foi conectado!");
  Serial.println();
}

void loop() 
{
  tone(buzzer, 2500, 100);
  tempoInicio = millis();
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  tempc = mySensor.readTemperature(); 
  pressure = mySensor.readPressure();
  vall = map(az,-17000, 17000, 120, 10);

  Serial.print(tempoInicio/1000);
  Serial.print(" , "); Serial.print(ax);
  Serial.print(" , "); Serial.print(ay);
  Serial.print(" , "); Serial.print(az);
  Serial.print(" , "); Serial.print(gx);
  Serial.print(" , "); Serial.print(gy);
  Serial.print(" , "); Serial.print(gz);
  Serial.print(" , "); Serial.print(tempc);
  Serial.print(" , "); Serial.println(pressure);

  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if(dataFile)
  {
    dataFile.print(tempoInicio/1000);
    dataFile.print(" , "); dataFile.print(ax);
    dataFile.print(" , "); dataFile.print(ay);
    dataFile.print(" , "); dataFile.print(az);
    dataFile.print(" , "); dataFile.print(gx);
    dataFile.print(" , "); dataFile.print(gy);
    dataFile.print(" , "); dataFile.print(gz);
    dataFile.print(" , "); dataFile.print(tempc);
    dataFile.print(" , "); dataFile.print(pressure);

    dataFile.close();
  } else {
    Serial.println("Erro ao abrir o arquivo datalog.txt");
  }

  if(vall != prevVall)
  {
    servo1.write(vall);
    prevVall = vall;
  }
  tempoFim = millis();
  delay(1000 - (tempoFim - tempoInicio));
}
