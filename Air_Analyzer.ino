#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define LCD_I2C_ADDRESS 0x27 
#define LCD_DISP_COLS   16
#define LCD_DISP_ROWS   2

#include   <MQUnifiedsensor.h>
#define DHTTYPE DHT11
#define DHTPIN 4
#define BuzzerPIN 3
#define         Board                   ("Arduino NANO")
#define          Pin135                   (A0)  //Analog input 0 of your arduino
#define          Pin9                     (A1)  //Analog input 1 of your arduino
#define          Pin4                     (A2)  //Analog input 2 of your arduino
#define          Pin3                     (A3)  //Analog input 3 of your arduino
#define          Pin2                     (A6)  //Analog input 6 of your arduino

LiquidCrystal_I2C   lcd( LCD_I2C_ADDRESS, LCD_DISP_COLS, LCD_DISP_ROWS );
DHT dht(DHTPIN, DHTTYPE);

#define         RatioMQ3CleanAir           (60) //RS / R0 = 60 ppm 
#define         RatioMQ4CleanAir          (4.4)   //RS / R0 = 4.4 ppm 
#define         RatioMQ135CleanAir        (3.6) //RS / R0   = 10 ppm 
#define         RatioMQ2CleanAir          (9.83) //RS / R0 = 9.83 ppm   
#define         RatioMQ9CleanAir          (9.6) //RS / R0 = 9.6 ppm 
#define         ADC_Bit_Resolution         (10) // 10 bit ADC 
#define         Voltage_Resolution        (5) //   Volt resolution to calc the voltage
#define         Type                      ("Arduino   NANO") //Board used
//Declare Sensor
MQUnifiedsensor MQ3(Board, Voltage_Resolution,   ADC_Bit_Resolution, Pin3, Type);
MQUnifiedsensor MQ4(Board, Voltage_Resolution,   ADC_Bit_Resolution, Pin4, Type);
MQUnifiedsensor MQ135(Board, Voltage_Resolution,   ADC_Bit_Resolution, Pin135, Type);
MQUnifiedsensor MQ2(Board, Voltage_Resolution,   ADC_Bit_Resolution, Pin2, Type);
MQUnifiedsensor MQ9(Board, Voltage_Resolution,   ADC_Bit_Resolution, Pin9, Type);


void setup() {
  Serial.begin(9600);
  pinMode(BuzzerPIN, OUTPUT);
  lcd.init();

  lcd.backlight();
                 
  lcd.setCursor   (0,0);                  
  lcd.print("                ");
  lcd.setCursor   (0,1);
  lcd.print("                ");
  lcd.setCursor (0,0);
  lcd.print("    Air");
  lcd.setCursor (0,1);
  lcd.print("    Analyzer");
  delay(1000);
  lcd.setCursor (12,1);
  lcd.print(".");
  delay(1000);
  lcd.setCursor   (13,1);
  lcd.print(".");
  delay(1000);
  lcd.setCursor (14,1);
  lcd.print(".");
  delay(1000);
  lcd.setCursor (15,1);
  lcd.print(".");
   delay(1000);  
  dht.begin();                       

  MQ3.init();
  MQ3.setRegressionMethod(1);   //_PPM =  a*ratio^b
  MQ3.setR0(0.57);
  MQ4.init();
  MQ4.setRegressionMethod(1);   //_PPM =  a*ratio^b
  MQ4.setR0(40.25);
  MQ135.init();
  MQ135.setRegressionMethod(1);   //_PPM =  a*ratio^b
  MQ135.setR0(25.69);
  MQ2.init();
  MQ2.setRegressionMethod(1);   //_PPM =  a*ratio^b
  MQ2.setR0(14.19);
  MQ9.init();
  MQ9.setRegressionMethod(1);   //_PPM =  a*ratio^b
  MQ9.setR0(3.38);



//While calibrating Your   sensor Uncomment this calibration portion and calibrate for R0.
  /*****************************   MQ CAlibration ********************************************
  Serial.print("Calibrating   please wait.");
  float  MQ3calcR0 = 0,
         MQ4calcR0 = 0,
         MQ135calcR0   = 0,
         MQ2calcR0 = 0,
         MQ9calcR0   = 0;
  for (int i = 1; i <= 10; i ++)
  {
    //Update the voltage lectures
     MQ3.update();
    MQ4.update();
    MQ135.update();
    MQ2.update();
    MQ9.update();

    MQ3calcR0 += MQ3.calibrate(RatioMQ3CleanAir);
     MQ4calcR0 += MQ4.calibrate(RatioMQ4CleanAir);
    MQ135calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
     MQ2calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
     MQ9calcR0 += MQ9.calibrate(RatioMQ9CleanAir);

    Serial.print(".");
   }
  MQ3.setR0(MQ3calcR0 / 10);
  MQ4.setR0(MQ4calcR0 / 10);
  MQ135.setR0(MQ135calcR0   / 10);
  MQ2.setR0(MQ2calcR0 / 10);
  MQ9.setR0(MQ9calcR0   / 10);
  Serial.println("  done!.");

  Serial.print("(MQ3 - MQ9):");
   Serial.print(MQ3calcR0 / 10); Serial.print(" | ");
  Serial.print(MQ4calcR0   / 10); Serial.print(" | ");
  Serial.print(MQ135calcR0 / 10); Serial.print("   | ");
  Serial.print(MQ2calcR0 / 10); Serial.print(" | ");
  Serial.print(MQ9calcR0 / 10); Serial.println("   |");

  /*****************************  MQ CAlibration ********************************************/   
}

void loop() {
  
  //Update the voltage lectures
  MQ3.update();
  MQ4.update();
  MQ135.update();  
  MQ2.update();
  MQ9.update();
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }  

  MQ3.setA(0.3934); MQ3.setB(-1.504); //Alcohol
float   Alcohol = MQ3.readSensor(); 

  MQ3.setA(4.8387); MQ3.setB(-2.68); //Benzene
float   Benzene = MQ3.readSensor(); 
  
  MQ3.setA(7585.3); MQ3.setB(-2.849); //Hexane
float   Hexane = MQ3.readSensor(); 

  MQ4.setA(1012.7); MQ4.setB(-2.786); //CH4
float   CH4 = MQ4.readSensor(); 

  MQ4.setA(30000000); MQ4.setB(-8.308); //smoke   
float smoke = MQ4.readSensor(); 
 
  MQ135.setA(110.47); MQ135.setB(-2.862);   //CO2 
float CO2 = MQ135.readSensor(); 
  
  MQ135.setA(44.947); MQ135.setB(-3.445);   // Toluene
float Toluene = MQ135.readSensor(); 
  
  MQ135.setA(102.2 );   MQ135.setB(-2.473); //NH4 
float NH4 = MQ135.readSensor(); 
  
  MQ135.setA(34.668);   MQ135.setB(-3.369); //Acetone
float Acetone = MQ135.readSensor(); 
 
  MQ2.setA(99.042);   MQ2.setB(-1.518); //CO
float LPG = MQ2.readSensor(); 

  MQ9.setA(1000.5);   MQ9.setB(-2.186); //flamable gas
float FG = MQ9.readSensor();
if(LPG < 1){
  digitalWrite(BuzzerPIN, LOW);
}
else {
  digitalWrite(BuzzerPIN, HIGH);
}

//Alcohol:Benzene:Hexane:Methane:Smoke:CO2:Toluene:NH4:Acetone:LPG:FG:
String str =String(t)+String("=")+String(h)+String("=")+String(Alcohol)+String("=")+String(Benzene)+String("=")+String(Hexane)+String("=")+String(CH4)+String("=")+String(smoke)+String("=")+String(CO2)+String("=")+String(Toluene)+String("=")+String(NH4)+String("=")+String(Acetone)+String("=")+String(LPG)+String("=")+String(FG);
Serial.println(str);

lcd.clear();
delay(70);
lcd.setCursor   (0,0);
lcd.print("Alcohol ");
lcd.print(Alcohol);
lcd.setCursor (13,0);
lcd.print("ppm");
lcd.setCursor   (0,1);
lcd.print("Benzene ");
lcd.print(Benzene);
lcd.setCursor (13,1);
lcd.print("ppm");
delay(3000);

lcd.clear();
delay(70);
lcd.setCursor   (0,0);
lcd.print("Hexane  ");
lcd.print(Hexane);
lcd.setCursor (13,0);
lcd.print("ppm");
lcd.setCursor   (0,1);
lcd.print("CH4     ");
lcd.print(CH4);
lcd.setCursor (13,1);
lcd.print("ppm");
delay(3000);


lcd.clear();
delay(70);
lcd.setCursor   (0,0);
lcd.print("Smoke   ");
lcd.print(smoke);
lcd.setCursor (13,0);
lcd.print("ppm");
lcd.setCursor   (0,1);
lcd.print("CO2     ");
lcd.print(CO2);
lcd.setCursor (13,1);
lcd.print("ppm");
delay(3000);


lcd.clear();
delay(70);
lcd.setCursor   (0,0);
lcd.print("Toluene ");
lcd.print(Toluene);
lcd.setCursor (13,0);
lcd.print("ppm");
lcd.setCursor   (0,1);
lcd.print("NH4     ");
lcd.print(NH4);
lcd.setCursor (13,1);
lcd.print("ppm");
delay(3000);


lcd.clear();
delay(70);
lcd.setCursor   (0,0);
lcd.print("Acetone ");
lcd.print(Acetone);
lcd.setCursor (13,0);
lcd.print("ppm");
lcd.setCursor   (0,1);
lcd.print("LPG     ");
lcd.print(LPG);
lcd.setCursor (13,1);
lcd.print("ppm");
delay(3000);


lcd.clear();
delay(70);
lcd.setCursor   (0,0);
lcd.print("FG      ");
lcd.print(FG);
lcd.setCursor (13,0);
lcd.print("ppm");
lcd.setCursor   (0,1);
lcd.print("Temp:");
lcd.print(t);
lcd.print(" Hum:");
lcd.print(h);
delay(3000);

}
