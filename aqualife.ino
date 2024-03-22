#include <DHT.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
// #define ONE_WIRE_BUS 8
#define Buzzer A5
#define DHTPIN 8
#define DHTTYPE DHT11

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
// OneWire oneWire(ONE_WIRE_BUS);
// DallasTemperature sensors(&oneWire);
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial mySerial(9, 10);
byte tx = 1;

#define SensorPin 0          // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
float ph;
int buf[10],temp;
const float minVoltage = 0.0;
const float maxVoltage = 5.0;
String textForSMS;



float Celsius = 0;
float Fahrenheit = 0;

void setup()
{
  Serial.begin(9600);  
  Serial.println("Ready");    //Test the serial monitor
  // sensors.begin();
  mySerial.begin(9600);
   dht.begin();
  pinMode(tx, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  lcd.begin(20,4);
  lcd.setCursor(0,0);
  lcd.print(" Dear Sir/Madam "); 
  lcd.setCursor(0,1);
  lcd.print("welcome to Home Fish Pond system ");
  delay(500); 
  lcd.clear();
}

void loop()
{
  // sensors.requestTemperatures();
  
  float temperature = dht.readTemperature();
  
    // Convert the sensor value to voltage
  int sensorValue = analogRead(A1);
  float voltage = map(sensorValue, 0, 1023, minVoltage * 1000, maxVoltage * 1000) / 1000.0;

  // Calculate the percentage
  float percentage = (voltage - minVoltage) / (maxVoltage - minVoltage) * 100.0;

  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    ph=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;     //convert the millivolt into pH value
// phValue= map(ph, 0, 1023, 0, 14);
  
  
  // Serial.print("    pH:");  
  // // Serial.print(phValue,2);
  Serial.println(" "); 

  lcd.setCursor(0,0);
  lcd.print("temp *C :" + (String)(temperature));

  // lcd.setCursor(0,12);
  // lcd.print(phValue);
  // lcd.setCursor(0,24);
   lcd.setCursor(0,1);
  lcd.print("Oxygen: " +(String)(percentage));

  // lcd.print("T: " + (String)(Celsius)+ " " + "O :"+(String)(percentage) ); 
  lcd.setCursor(0, 2);
  lcd.print("pH value : "+ (String)(phValue)); 
  

  if(phValue > 9.0 || phValue < 5.0 || percentage < 80 || temperature > 38 || temperature < 20){
   digitalWrite(Buzzer, HIGH);
    Serial.print("AT+CMGS=\"+255748550000\"\r");
        Serial.print("Dear George there is an issue in YOur Fish Pond  \r");
        // Serial.print(" Ph: "+ (String)(phValue) + "\r");
        // Serial.print("oxygen %:  "+ (String)(percentage) + "\r");
        // Serial.print(" temp: "+ (String)(temperature) + "\r");

       if (phValue > 9.0) {
        Serial.print("Ph is high: " + (String)(phValue) + "\r"+ "add acid");
    } else if (phValue < 5.0) {
        Serial.print("Ph is low: " + (String)(phValue) + "\r"+"add base");
    }

    if (percentage < 80) {
        Serial.print("Oxygen % is low: " + (String)(percentage) + "\r");
    }

    if (temperature > 38) {
        Serial.print("Temperature is high: " + (String)(temperature) + "\r");
    } else if (temperature < 20) {
        Serial.print("Temperature is low: " + (String)(temperature) + "\r");
    }
       
        delay(100);
  }
  
  else {
      digitalWrite(Buzzer, LOW);
  }


  delay(500);
 


}







