#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Process.h>
#include <Bridge.h>

//設定閾值，當加速度大於這個值時，傳送數據到ThingSpeak
#define THRESHOLD 15 
String APIKey = "JXQW0PZLW72EQ0IP";
Adafruit_MPU6050 mpu;
unsigned long timer = 0;

void setup() {
  //初始化串口、I2C通訊、Bridge庫
  Serial.begin(9600);
  Wire.begin();
  Bridge.begin();

  //初始化MPU-6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  //計算總加速度
  float totalAcceleration = sqrt(a.acceleration.x * a.acceleration.x +
                                 a.acceleration.y * a.acceleration.y +
                                 a.acceleration.z * a.acceleration.z);
  Serial.println(totalAcceleration);
  //如果總加速度超過閾值，發送數據到ThingSpeak
  if (totalAcceleration > THRESHOLD && millis() - timer > 10000) {
    String url = "https://api.thingspeak.com/update";    // form the string for the URL parameter

    // Send the HTTP POST request
    Process p;
    Serial.print("\n\nSending data... ");

    String cmd = "curl --silent --request POST --header \"X-THINGSPEAKAPIKEY: " +APIKey;
    cmd = cmd + "\" --data \"field1=" + totalAcceleration;
    cmd = cmd + "\" http://api.thingspeak.com/update";
    p.runShellCommand(cmd);
    Console.println(cmd);
    p.close();

    Serial.println(cmd);
    Serial.println("done!");

    // If there's incoming data from the net connection, send it out the Serial:
    while (p.available()>0) {
      char c = p.read();
      Serial.write(c);
    }
  }

  delay(100);
}
