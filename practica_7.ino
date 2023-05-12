#include <DallasTemperature.h>
#include <OneWire.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>




//Directivas
#define btn1 4
#define btn2 3
#define btn3 2
#define foco1 12
#define foco2 A3
#define Cantidad_Leds 7
#define pin_Led 11
#define RadarPin 10
#define pin_Servo 5
#define buzzer A2
#define R 8
#define G 7
#define B 6

int personacerca =0;
int value;
int estado1 = 0;
int estado2 = 0;
// Constructor
Adafruit_NeoPixel rueda(Cantidad_Leds,pin_Led, NEO_GRB + NEO_KHZ800);
//Caracteres
byte puerta_cerrada[] = {
  B11111,
  B11111,
  B11111,
  B11101,
  B11111,
  B11111,
  B11111,
  B11111
};
byte puerta_entreabierta[] = {
  B11000,
  B11100,
  B11110,
  B11010,
  B11110,
  B11110,
  B11100,
  B11000
};
byte puerta_media[] = {
  B11000,
  B11100,
  B11110,
  B11010,
  B11110,
  B11110,
  B11100,
  B11000
};
byte puerta_abierta[] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};
byte luz_off[] = {
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B01010,
  B01110,
  B01110
};
byte luz_on[] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B01110,
  B01110,
  B01110
};
byte Centigrados[] = {
  B01000,
  B10100,
  B01000,
  B00011,
  B00100,
  B00100,
  B00100,
  B00011
};
byte persona_detectada[] = {
  B01000,
  B11100,
  B00000,
  B01110,
  B01110,
  B01110,
  B00100,
  B11111
};
byte persona_nodetec[] = {
  B10100,
  B01000,
  B10100,
  B01110,
  B01110,
  B01110,
  B00100,
  B11111
};
float medicion();
//Constructores
OneWire ourWire(9);                //Se establece el pin 2  como el pin que utilizaremos para el protocolo OneWire
DallasTemperature sensor(&ourWire); //Se declara una objeto para nuestro sensor
LiquidCrystal_I2C LCD_Diego(0x27, 16, 2);
Servo Servo_Diego;
float temperatura;

void setup() {
 
  Servo_Diego.attach(pin_Servo);
  pinMode(RadarPin, INPUT);
  rueda.begin();
  pinMode(foco1, OUTPUT);
  pinMode(foco2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  Serial.begin(9600);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
sensor.begin();
LCD_Diego.init();
LCD_Diego.backlight();
 LCD_Diego.createChar(1, puerta_abierta);
 LCD_Diego.createChar(2, puerta_media);
 LCD_Diego.createChar(3, puerta_cerrada);
 LCD_Diego.createChar(4, luz_off);
 LCD_Diego.createChar(5, luz_on);
 LCD_Diego.createChar(6, Centigrados);
 LCD_Diego.createChar(7, persona_detectada);
 LCD_Diego.createChar(8, persona_nodetec);
 LCD_Diego.setCursor(0,1);
  LCD_Diego.print("Puerta:");
  LCD_Diego.setCursor(9,1);
    LCD_Diego.print("Luz 2");
    LCD_Diego.write(5);
    LCD_Diego.setCursor(9,0);
    LCD_Diego.print("Luz 1");
    LCD_Diego.write(4);
    Servo_Diego.write(0);
 }
 
void loop() {
  value = digitalRead(RadarPin);
  focos();
  puerta();
  temperatura = medicion();
  focos();
  puerta();
LCD_Diego.setCursor(0,0);
LCD_Diego.print(temperatura);
LCD_Diego.write(6);
focos();
puerta();
proximidad();
focos();
puerta();
condicion_temp();
focos();
puerta();
}

float medicion(){
  sensor.requestTemperatures();   //Se envía el comando para leer la temperatura
  float temp = sensor.getTempCByIndex(0); //Se obtiene la temperatura en ºC
  Serial.print("Temperatura= ");
  Serial.print(temp);
  Serial.println(" C");
  delay(100);  
  return temp;
  }
void proximidad(){  
  if( value == LOW){
    LCD_Diego.setCursor(7,0);
    LCD_Diego.write(8);
     for(int i = 0; i < 7; i++){
      rueda.setPixelColor(i,rueda.Color(0,255,0));
          rueda.show();
          delay(50);
          rueda.setPixelColor(i,rueda.Color(0,40,10));
          rueda.show();
          delay(50);
      }
    }
    if(value == HIGH){
    LCD_Diego.setCursor(7,0);
    LCD_Diego.write(7);
    digitalWrite(buzzer, HIGH);
    for(int i = 0; i < 7; i++){
      rueda.setPixelColor(i,rueda.Color(255,0,0));
          rueda.show();
          delay(50);
          rueda.setPixelColor(i,rueda.Color(40,10,0));
          rueda.show();
          delay(50);
    }
    digitalWrite(buzzer, LOW);
    }if(personacerca == 0){
     
}
}
void focos(){
if(digitalRead(btn1) && estado1==0){  // si pulsador presionado y led apagado
    digitalWrite(foco1, LOW);          // se enciende el led
   
    LCD_Diego.setCursor(9,0);
    LCD_Diego.print("Luz 1");
    LCD_Diego.write(5);
    delay(300);
    estado1=1;                       // guardamos el estado encendido  
  }
  if(digitalRead(btn1) && estado1==1){  // si pulsador presionado y led encendido

    digitalWrite(foco1, HIGH);           // se apaga el led
   
    LCD_Diego.setCursor(9,0);
    LCD_Diego.print("Luz 1");
    LCD_Diego.write(4);
    delay(300);
    estado1=0;                       // guardamos el estado apagado  
  }
  if(digitalRead(btn2) && estado2==0){  // si pulsador presionado y led apagado
    digitalWrite(foco2, LOW);          // se enciende el led
   
    LCD_Diego.setCursor(9,1);
    LCD_Diego.print("Luz 2");
    LCD_Diego.write(5);
    delay(300);
    estado2=1;                       // guardamos el estado encendido  
  }
  if(digitalRead(btn2) && estado2==1){  // si pulsador presionado y led encendido

    digitalWrite(foco2, HIGH);           // se apaga el led
   
    LCD_Diego.setCursor(9,1);
    LCD_Diego.print("Luz 2");
    LCD_Diego.write(4);
    delay(300);
    estado2=0;                       // guardamos el estado apagado  
  }
}
void puerta(){
  if(digitalRead(btn3)==HIGH){
  Servo_Diego.write(90);
    LCD_Diego.setCursor(0,1);
    LCD_Diego.print("Puerta:");
    LCD_Diego.write(3);
    delay(100);
    LCD_Diego.setCursor(0,1);
    LCD_Diego.print("Puerta:");
    LCD_Diego.write(2);
    delay(100);
    LCD_Diego.setCursor(0,1);
    LCD_Diego.print("Puerta:");
    LCD_Diego.write(1);
    delay(2000);
    Servo_Diego.write(0);
    LCD_Diego.setCursor(0,1);
    LCD_Diego.print("Puerta:");
    LCD_Diego.write(1);
    delay(100);
    LCD_Diego.setCursor(0,1);
    LCD_Diego.print("Puerta:");
    LCD_Diego.write(2);
    delay(100);
    LCD_Diego.setCursor(0,1);
    LCD_Diego.print("Puerta:");
    LCD_Diego.write(3);
    delay(1000);
    Servo_Diego.write(0);
  } else{
    Servo_Diego.write(0);
    }
}
void condicion_temp(){
if(temperatura > 15 && temperatura <= 21){
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(B, HIGH);
}
if(temperatura > 21 && temperatura <= 25){
  digitalWrite(R, LOW);
  digitalWrite(G, HIGH);
  digitalWrite(B, LOW);
}
if(temperatura > 25 && temperatura < 45){
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(B, LOW);
}
}
