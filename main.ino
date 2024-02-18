#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
// #include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

#define SDA 10  // const int SDA = 10
#define RESET 9 

int buzzer = 8;

// LiquidCrystal_I2C lcd(0x27,16,2); 
//ou bien  0x3F,20,4

String numeroAutorise = "939582C5";
String numeroIdentificateur = "";

MFRC522 rfid(SDA, RESET);

int led_rouge = 4;
int led_blue = 5;

int verification = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Accès Sécurisé.");
  Serial.println("Veuillez presenter votre carte d'accès !");
  pinMode(led_rouge, OUTPUT);
  pinMode(led_blue, OUTPUT);
  pinMode(buzzer, OUTPUT);
  // lcd.init();                      // initialize the lcd 
  // lcd.backlight();
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  // put your main code here, to run repeatedly:
  // lcd.setCursor(0,0);
  // lcd.print("Acces securise");
  // lcd.setCursor(0,1);
  // lcd.print("votre carte SVP");

  while(lecture())
  {
    // lcd.clear();
    if(numeroIdentificateur == numeroAutorise)
    {
      Serial.println("Accès Autorisé");
      // lcd.print("Acces Autorise");
      digitalWrite(led_blue, HIGH);
      
      
      for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      delay(10000);
      digitalWrite(led_blue, LOW);
      // lcd.clear();
      for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                     // waits 15ms for the servo to reach the position
      }
      verification = 0;
      // lcd.clear();
    }
    else
    {
      Serial.println("Accès rejeté. ");
      // lcd.print("Acces rejete");
      digitalWrite(led_rouge, HIGH);
      if(verification >= 4){
        digitalWrite(buzzer, HIGH);
        delay(5000);
        digitalWrite(buzzer, LOW);
      }
      if(verification < 4){
        delay(5000);
      }
      digitalWrite(led_rouge, LOW);
      // delay(5000); 
      
      // delay(5000); 
      // lcd.clear();
      verification = verification + 1;
    }

    Serial.println("Numéro identificateur du badge : ");
    Serial.println(numeroIdentificateur);
    Serial.println("Accès Sécurisé.");
    Serial.println("Veuillez presenter votre carte d'accès !");

  }

}


bool lecture()
{
  if(! rfid.PICC_IsNewCardPresent())
  {
    return false;
  }
  if(! rfid.PICC_ReadCardSerial())
  {
    return false;
  }

  numeroIdentificateur = "";

  for(int i = 0; i < 4; i++)
  {
    numeroIdentificateur.concat(String(rfid.uid.uidByte[i], HEX));
  }

  numeroIdentificateur.toUpperCase();

  rfid.PICC_HaltA();


  return true;

}
