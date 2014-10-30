#include <SPI.h>
#include <Wire.h>
#include <avr/pgmspace.h>


#define I2CADDR 0x50
#define WICSVER "0.2"
#undef REALRTC
//#define REALRTC
#define ZONE1     12
#define ZONE2     11
#define ZONE3     10
#define ZONE4      9
#define RAIN1      4
#define MOISTURE1  7
#define ECHO      6
#define TRIGGER   5
#define THERM     A0
#define SDA       A4
#define SCL       A5

int outpins[] = {ZONE1,ZONE2,ZONE3,ZONE4,TRIGGER};
int inpins[] = {8,RAIN1, MOISTURE1,INPUT};
int totout = 5;
int totin = 4;
int totzones = 4;
int newRequest = 0;
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
  Serial.println(F("Initializing...."));
  pinsToOut();
  pinsToIn();
  delay(2000);
  Wire.begin(I2CADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.println(F("Initializing.... Done!"));
}

void loop () {
   // Serial.println(millis());
   // Serial.println(freeRam());
   for (int zone = 0 ; zone < totzones ; zone++)
   {
     /*Serial.print(F("Checking zone "));
     Serial.print(zone);
     Serial.print(F(" Status is "));
     Serial.println(zonestart[zone]);*/
     if (zonestart[zone] == 1)
     {
       digitalWrite(zoneport[zone],LOW);
    /* Serial.print("Zone ID ");
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
   Serial.println(String(distanceSensor()));
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

int distanceSensor() 
{
  long duration, distance;
  digitalWrite(TRIGGER,HIGH);
  delayMicroseconds(1000);
  digitalWrite(TRIGGER,LOW);
  duration = pulseIn(ECHO, HIGH);
  //Serial.println(duration);
  distance = (duration/2) / 29.1;
  if ( distance >= 300 || distance <= 0 )
  {
    //Serial.println("Out of Range");
    return 0;
  } else {
    return String(distance).toInt();
  }
}

int rainStatus() {
  unsigned int rainStatus;
  Serial.println("It is Rainint?");
  rainStatus=digitalRead(RAIN1);
  Serial.println(rainStatus);
  return rainStatus;
}

int moistureStatus() {
  unsigned int moistureStatus;
  Serial.println("It is Soil Wet?");
  moistureStatus=digitalRead(MOISTURE1);
  Serial.println(moistureStatus);
  return moistureStatus;
}

void pinsToIn(){
  for (int in = 0 ; in < totin; in++)
  {
    Serial.print(F("Setting Pin "));
    Serial.print(inpins[in]);
    Serial.println(F(" Input"));
    pinMode(inpins[in],INPUT_PULLUP);
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
  //int pinVal;                            // inititalize the variable for holding the pin number as integer
  //int awValueVal;                        // initialize the variable for holding the analog write value as integer (only PWM pins!)   
    
  while(Wire.available())                // loop through all incoming bytes
  {
    command[receiveByte] = Wire.read();  // receive byte as a character
    receiveByte++;                       // increase index by 1
  }
  
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
  
  //Zone Status
  if (String(command[0]) == "S" && String(command[1]) == "Z")
  {
    int zone = (String(command[2]).toInt());
    String localStatus;
    if (zone > 4) { 
      localStatus="SZEE"; 
    } else {
      int status = digitalRead(zoneport[zone-1]);
      if (status == 0 )
      {
        status = 1;
      } else {
        status = 0;
      }
      localStatus = "SZ" + String(zone) + String(status);
    }
    localStatus.toCharArray(sendStatus,31);
    Serial.print("Status = |");
    Serial.print(sendStatus);
    Serial.println("|");
    newRequest = 1;
  }
  
  //Rain Status
  if (String(command[0]) == "S" && String(command[1]) == "R")
  {
    String localStatus="SR" + String(rainStatus());
    localStatus.toCharArray(sendStatus,31);
    Serial.print("Status = |");
    Serial.print(sendStatus);
    Serial.println("|");
    newRequest = 1;
  }
    //Rain Status
  if (String(command[0]) == "S" && String(command[1]) == "M")
  {
    String localStatus="SM" + String(moistureStatus());
    localStatus.toCharArray(sendStatus,31);
    Serial.print("Status = |");
    Serial.print(sendStatus);
    Serial.println("|");
    newRequest = 1;
  }
  
  if (String(command[0]) == "S" && String(command[1]) == "D")
  {
    String distance = "0";
    String temp;;
    distance=String(distanceSensor());
    if (distance.toInt() < 10) 
    {
      temp = "00" + distance;
      distance = temp;
    } else if (distance.toInt() < 100)
    {
      temp = "0" + distance;
      distance = temp;
    }
    String localStatus="SD" + distance;
    localStatus.toCharArray(sendStatus,31);
    Serial.print("Status = |");
    Serial.print(sendStatus);
    Serial.println("|");
    newRequest = 1;
  }
}



void requestEvent() 
{
  if (newRequest == 1) 
  {
    newRequest = 0;
    index      = 0;
  }

  Wire.write(sendStatus[index]);
  Serial.println("Wire.write()");
  ++index;
  if (index >= 30) {
       index = 0;
  }
 }


