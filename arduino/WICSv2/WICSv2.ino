#include <SPI.h>
#include <Wire.h>
#include <avr/pgmspace.h>


#define I2CADDR 0x50
#define WICSVER "0.2"
#undef REALRTC
//#define REALRTC
#define ZONE1   12
#define ZONE2   11
#define ZONE3   10
#define ZONE4   9
#define RAIN1   D4
#define ECHO    D5
#define TRIGGER D6
#define THERM   A0
#define SDA     A4
#define SCL     A5

int outpins[] = {12,11,10,9};
int inpins[] = {8};
int totout = 4;
int totin = 1;
int totzones = 4;
unsigned long zonetimer [] = {0,0,0,0};
unsigned long zonebegin [] = {0,0,0,0};
unsigned long zonestart [] = {0,0,0,0};
unsigned long zoneend [] = {0,0,0,0};
unsigned long zoneport [] = { ZONE1, ZONE2, ZONE3, ZONE4 };
char sendStatus[31] = "000000000000000000000000000000";  // initialize the container variable
int index = 0;                                           // initialize the index variable

void setup() {
  Serial.begin(9600);
  Serial.print(F("This is WICS Slave Version "));
  Serial.println(F(WICSVER));
  pinsToOut();
  delay(2000);
  Wire.begin(I2CADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop () {
   Serial.println(millis());
   Serial.println(freeRam());
   for (int zone = 0 ; zone < totzones ; zone++)
   {
     Serial.print(F("Checking zone "));
     Serial.print(zone);
     Serial.print(F(" Status is "));
     Serial.println(zonestart[zone]);
     if (zonestart[zone] == 1)
     {
       digitalWrite(zoneport[zone],LOW);
    /*   Serial.print("Zone ID ");
       Serial.println(zone);
       Serial.print("Zone Begin ");
       Serial.println(zonebegin[zone]);
       Serial.print("Zone Duration ");
       Serial.println(zonetimer[zone]);
       Serial.print("Zone End ");
       Serial.println(zoneend[zone]);
       Serial.print("Millis ");
       Serial.println(millis()); */
       if (zoneend[zone] < millis())
       {
         digitalWrite(zoneport[zone],HIGH);
         zonestart[zone]=0;
       }
     }
   }
     // if (zonestart[zone]==1) && (millis()-zonebegin[zone] > zonetime[zone]
   delay(1000);

};

    int freeRam ()
    {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    }
    
void setTimerZone(int zone, unsigned long duration)
{
  zone=zone-1;
  zonestart[zone]=1;
  zonebegin[zone]=millis();
  zonetimer[zone]=duration*1000;
  zoneend[zone]=zonebegin[zone]+zonetimer[zone];
  Serial.println("millis duration end");
  Serial.print(millis());
  Serial.print(duration);
  Serial.println(zoneend[zone]);
  return;
}

void pinsToOut(){
  for (int out = 0 ; out < totout ; out++)
  {
    Serial.print("Setting Pin ");
    Serial.print(outpins[out]);
    Serial.println(" Output");
    pinMode(outpins[out],OUTPUT);
    digitalWrite(outpins[out],HIGH);
  }
}


void pinsToIn(){
  for (int in = 0 ; in < totin; in++)
  {
    Serial.print("Setting Pin ");
    Serial.print(inpins[in]);
    Serial.println(" Input");
    pinMode(inpins[in],INPUT);
    //digitalWrite(inpins[in],HIGH);
  }
}

void receiveEvent(int howMany)
{
  int receiveByte = 0;                   // set index to 0
  char command[32];                       // expect 7 char + 1 end byte
  String mode = "";                      // initialize mode variable for holding the mode
  String pin = "";                       // initialize pin variable for holding the pin number as a String
  String awValue = "";                   // intitalize the variable for holding the analogWrite value
  int pinVal;                            // inititalize the variable for holding the pin number as integer
  int awValueVal;                        // initialize the variable for holding the analog write value as integer (only PWM pins!)   
    
  while(Wire.available())                // loop through all incoming bytes
  {
    command[receiveByte] = Wire.read();  // receive byte as a character
    receiveByte++;                       // increase index by 1
  }
  
  //pin = String(command[1]) + String(command[2]);                          // combine byte 2 and 3 in order to get the pin number
  //awValue = String(command[4]) + String(command[5]) + String(command[6]); // combine byte 5, 6 and 7 in order to get the analogWrite value
  //awValueVal = awValue.toInt();                                           // convert the awValue string to a value
  Serial.print("Command is = ");
  Serial.println(String(command).substring(0,receiveByte));
  if (String(command[0]) == "O" && String(command[1]) == "N") 
  { 
    Serial.print("Open Normal Zone "); 
    Serial.println(String(command[2]));
    String runtime=String(command).substring(3,6);
    Serial.println(runtime.toInt());
    int zone=String(command[2]).toInt();
    zonestart[zone]=1;
    zonebegin[zone]=millis();
    zonetimer[zone]=runtime.toInt()*1000;
    zoneend[zone]=zonebegin[zone]+zonetimer[zone];
    
  };
  if (String(command[0]) == "C" && String(command[1]) == "N") 
  { 
    Serial.print("Close Normal Zone "); 
    Serial.println(String(command[2]));
    int zone=String(command[2]).toInt();
    zoneend[zone]=0;
  };
  if (String(command[0]) == "S" && String(command[1]) == "Z")
  {
    int zone = (String(command[2]).toInt()-1);
    String localStatus;
    if (zone > 4) { 
      localStatus="SZEE"; 
    } else {
      int status = digitalRead(zoneport[zone]);
      localStatus = "SZ" + String(zone) + String(status);
    }
    localStatus.toCharArray(sendStatus,31);
  }
}



void requestEvent() 
{
    Wire.write(sendStatus[index]);
    ++index;
    if (index >= 30) {
         index = 0;
    }
}


