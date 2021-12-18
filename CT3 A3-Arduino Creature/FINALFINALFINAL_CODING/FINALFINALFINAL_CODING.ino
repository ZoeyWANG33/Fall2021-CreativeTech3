#include <Adafruit_NeoPixel.h>

#define PIN  14//RGB LED灯
#define NUMPIXELS 24//RGB LED灯数量为24个

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);//上述函数了LED的数量和Arduino引脚。

#define DELAYVAL 500 // 像素之间暂停的时间（毫秒）

int PulseSensor = A0;//脉搏传感器

const int TrigPin = 32;//超声波
const int EchoPin = 15; 

const int Buzzer = 33;//蜂鸣器

//const int Fan1 = 27;//风扇
const int Fan2 = 13;

int cm;//距离变量
void setup() 
{
  Serial.begin(9600); //波特率9600
  pinMode(TrigPin, OUTPUT); 
  pinMode(EchoPin, INPUT);
  pinMode(Buzzer, OUTPUT);
  //pinMode(Fan1, OUTPUT);
  pinMode(Fan2, OUTPUT);
  pixels.begin();
  //此函数执行初始化。
  pixels.clear();
  //函数设定光照亮度。 （最小数字为1，最大数字为255。）
  pixels.setBrightness(60);
}

void loop() 
{
    //超声波发射
   delayMicroseconds(2);
   digitalWrite(TrigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(TrigPin, LOW); 
   //算成厘米 
   cm = pulseIn(EchoPin, HIGH) / 58.0;
  //保留两位小数 
   cm = (int(cm * 100.0)) / 100.0;
  //读取脉搏传感器值并赋值给变量val
   int val=analogRead(PulseSensor);
   //如果脉搏过高RGB灯为红色
   if(val > 4500)                   
   {
    for(int x1;x1 < NUMPIXELS;x1++)
    {
      pixels.setPixelColor(x1, pixels.Color(255, 0, 0)); // Red Color   
    }
   }
   //否则如果脉搏正常RGB灯为绿色
   else if(val > 2500 && val < 4500)
   {
    for(int x2;x2 < NUMPIXELS;x2++)
    {
      pixels.setPixelColor(x2, pixels.Color(0, 255, 0));// Green Color
    }  
   }
   //否则没有检测RGB灯为蓝色
   else                          
   {
    for(int x3;x3 < NUMPIXELS;x3++)
    {
      pixels.setPixelColor(x3, pixels.Color(0, 0, 255));// Blue Color
    }   
   }
   //显示应用值
   pixels.show();
   //如果超声波检测距离小于5cm，风扇转起，蜂鸣器响起
   if(cm < 5)
   {
    //digitalWrite(Fan1, HIGH);
    digitalWrite(Fan2, HIGH);
    digitalWrite(Buzzer, HIGH);
   }
   //否则风扇不转，蜂鸣器不响
   else
   {
    //digitalWrite(Fan1, LOW);
    digitalWrite(Fan2, LOW);
    digitalWrite(Buzzer, LOW);
   }
    //串口输出距离和脉搏
    Serial.print(cm);
    Serial.print("cm");
    Serial.print("/"); 
    Serial.print(val);
    Serial.println(); 
    delay(1000); 
}
