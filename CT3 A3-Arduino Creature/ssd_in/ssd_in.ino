//库
#include "config.h"
#include <Adafruit_NeoPixel.h>
//脉搏传感器链接开发板A2
#define pulsemn A2
//蜂鸣器链接开发板33
const int Buzzer = 33;
//风扇链接开发板27和12
const int Fan1 = 27;
const int Fan2 = 12;
//状态
float current = 0;
int last = -1;
int m1;
//设置pulse为脉搏数据馈送
AdafruitIO_Feed *pulse1 = io.feed("pulse");
//设置muscle为肌肉数据馈送
AdafruitIO_Feed *muscle1 = io.feed("muscle");
//设置muscle为倾斜数据馈送
AdafruitIO_Feed *Tiltsensor1 = io.feed("Tiltsensor");
void setup() 
{
  pinMode(Fan1, OUTPUT); 
  pinMode(Fan2, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  //波特率115200
  Serial.begin(115200);
  //等待串行监视器打开
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  muscle1->onMessage(handleMessage1);
  Tiltsensor1->onMessage(handleMessage2);
  //等待连接
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
  muscle1->get();
  Tiltsensor1->get();
}

void loop() 
{
  //保持客户端与服务器的连接,并处理任何传入的数据
  io.run();
  //读取脉搏传感器数值并赋值给状态变量current
  current = analogRead(pulsemn);
  //出口输出脉搏数据
  Serial.print("sending -> ");
  Serial.println(current);
  //脉搏数据上传到服务器
  pulse1->save(current);
  //等待1秒
  delay(1000);
}

void handleMessage1(AdafruitIO_Data *data) 
{
  Serial.print("received <- ");
  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");
  digitalWrite(Buzzer, data->toPinLevel());
}
void handleMessage2(AdafruitIO_Data *data) 
{
  //读取服务器muscle肌肉数据并赋值到变量m1
  m1 = data->toInt();
  if(m1 > 150)
  {
    digitalWrite(Fan1, HIGH);
    digitalWrite(Fan2, HIGH);
  }else
  {
    digitalWrite(Fan1, LOW);
    digitalWrite(Fan2, LOW);
  }
}
