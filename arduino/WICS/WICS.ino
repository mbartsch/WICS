#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


const int temps[] PROGMEM = { 0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 29, 
30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 
64, 65, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 88, 89, 90, 91, 92, 93, 94, 95, 
96, 97, 98, 99, 100, 101, 102, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 114, 115, 116, 117, 118, 119, 120, 121, 
122, 123, 124, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 143, 144, 145, 
146, 147, 148, 149, 150, 151, 151, 152, 153, 154, 155, 156, 157, 158, 159, 159, 160, 161, 162, 163, 164, 165, 166, 167, 167, 168, 169, 
170, 171, 172, 173, 174, 175, 175, 176, 177, 178, 179, 180, 181, 182, 182, 183, 184, 185, 186, 187, 188, 189, 190, 190, 191, 192, 193, 
194, 195, 196, 197, 197, 198, 199, 200, 201, 202, 203, 204, 205, 205, 206, 207, 208, 209, 210, 211, 212, 212, 213, 214, 215, 216, 217, 
218, 219, 220, 220, 221, 222, 223, 224, 225, 226, 227, 228, 228, 229, 230, 231, 232, 233, 234, 235, 235, 236, 237, 238, 239, 240, 241, 
242, 243, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 252, 253, 254, 255, 256, 257, 258, 259, 260, 260, 261, 262, 263, 264, 265, 
266, 267, 268, 269, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 289, 
290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315,
315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 335, 336, 337, 338, 339, 340, 
341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 
368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 392, 393, 394, 395, 
396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 422, 423, 424, 
425, 426, 427, 428, 429, 430, 432, 433, 434, 435, 436, 437, 438, 439, 441, 442, 443, 444, 445, 446, 448, 449, 450, 451, 452, 453, 455, 
456, 457, 458, 459, 460, 462, 463, 464, 465, 466, 468, 469, 470, 471, 472, 474, 475, 476, 477, 479, 480, 481, 482, 484, 485, 486, 487,
489, 490, 491, 492, 494, 495, 496, 498, 499, 500, 501, 503, 504, 505, 507, 508, 509, 511, 512, 513, 515, 516, 517, 519, 520, 521, 523,
524, 525, 527, 528, 530, 531, 532, 534, 535, 537, 538, 539, 541, 542, 544, 545, 547, 548, 550, 551, 552, 554, 555, 557, 558, 560, 561,
563, 564, 566, 567, 569, 570, 572, 574, 575, 577, 578, 580, 581, 583, 585, 586, 588, 589, 591, 593, 594, 596, 598, 599, 601, 603, 604,
606, 608, 609, 611, 613, 614, 616, 618, 620, 621, 623, 625, 627, 628, 630, 632, 634, 636, 638, 639, 641, 643, 645, 647, 649, 651, 653, 
654, 656, 658, 660, 662, 664, 666, 668, 670, 672, 674, 676, 678, 680, 683, 685, 687, 689, 691, 693, 695, 697, 700, 702, 704, 706, 708, 
711, 713, 715, 718, 720, 722, 725, 727, 729, 732, 734, 737, 739, 741, 744, 746, 749, 752, 754, 757, 759, 762, 764, 767, 770, 773, 775, 
778, 781, 784, 786, 789, 792, 795, 798, 801, 804, 807, 810, 813, 816, 819, 822, 825, 829, 832, 835, 838, 842, 845, 848, 852, 855, 859, 
862, 866, 869, 873, 877, 881, 884, 888, 892, 896, 900, 904, 908, 912, 916, 920, 925, 929, 933, 938, 942, 947, 952, 956, 961, 966, 971, 
976, 981, 986, 991, 997, 1002, 1007, 1013, 1019, 1024, 1030, 1036, 1042, 1049, 1055, 1061, 1068, 1075, 1082, 1088, 1096, 1103, 1110, 
1118, 1126, 1134, 1142, 1150, 1159, 1168, 1177, 1186, 1196, 1206, 1216, 1226, 1237, 1248, 1260, 1272, 1284, 1297, 1310, 1324, 1338, 
1353, 1369, 1385, 1402, 1420, 1439, 1459, 1480, 1502 };


/*
#define rele1 12
#define rele2 11
#define rele3 10
#define rele4 09
*/

// Initialization parameters
#define I2CADDR 0x50
#define RELE1   D0
#define RELE2   D1
#define RELE3   D2
#define RELE4   D3
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
//LiquidCrystal_I2C lcd(0x27,16,2);
char sendStatus[31] = "000000000000000000000000000000";  // initialize the container variable
int index = 0;                                           // initialize the index variable
char pwm[15] = "00000000000000";                         // initialize the PWM flag container
  
void setup() {
  Serial.begin(9600);
  
 // lcd.init(); 
  //lcd.autoscroll();
  //lcd.backlight();
  //pinsToOut();
  //pinsToIn();
  Wire.begin(I2CADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}


void loop() {
 
  
  Serial.println("Starting Cycle");
  String pinStatus="";
  for(int digitalPin = 0; digitalPin <= 13; digitalPin++) 
  {
    if (pwm[digitalPin] == 0)
    {
      pinStatus += String (digitalRead(digitalPin));
    }
    else
    {
      pinStatus += "P";
    }
  }
  for(int analogPin = 0; analogPin <= 3; analogPin++) 
  {
    pinStatus += String (1000+analogRead(analogPin));
  }
  pinStatus.toCharArray(sendStatus, 31); 
  Serial.print("Digital 8 = ");
  Serial.println(digitalRead(8));
  //lcd.clear();
  //lcd.print(digitalRead(8));
   for (int rele = 0 ; rele < 1; rele++)
  {
    Serial.print("Write ");  
    Serial.print(outpins[rele]);
    Serial.println(" to LOW\n");
    digitalWrite(outpins[rele],LOW);
    delay(2000);
    Serial.print("Write ");
    Serial.print(outpins[rele]);
    Serial.println(" to HIGH\n");
    digitalWrite(outpins[rele],HIGH);
    delay(2000);
  }
  Serial.println("End Cycle");
  delay(2000);
}



void pinsToOut(){
  for (int out = 0 ; out < totout; out++)
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

void requestEvent() 
{
    Wire.write(sendStatus[index]);
    ++index;
    if (index >= 30) {
         index = 0;
    }
}

void receiveEvent(int howMany)
{
  int receiveByte = 0;                   // set index to 0
  char command[7];                       // expect 7 char + 1 end byte
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
  
  pin = String(command[1]) + String(command[2]);                          // combine byte 2 and 3 in order to get the pin number
  awValue = String(command[4]) + String(command[5]) + String(command[6]); // combine byte 5, 6 and 7 in order to get the analogWrite value
  awValueVal = awValue.toInt();                                           // convert the awValue string to a value
  Serial.print("Command is = ");
  Serial.println(command);
  
  if (String(command[1]) != "A" ) { pinVal = pin.toInt();}                // in case of not an analog pin assignment convert into digital pin number
  if (String(command[1]) != "A" ) { pwm[pinVal] = 0;}                     // in case of not an analog pin assignment set PWM flag to 0

// incase of analog pin assignment determine analog pin to be set  
  if (String(command[1]) == "A" && String(command[2]) == "0") { pinVal = A0;}
  if (String(command[1]) == "A" && String(command[2]) == "1") { pinVal = A1;}
  if (String(command[1]) == "A" && String(command[2]) == "2") { pinVal = A2;}
  if (String(command[1]) == "A" && String(command[2]) == "3") { pinVal = A3;}
  if (String(command[1]) == "A" && String(command[2]) == "4") { pinVal = A4;}
  if (String(command[1]) == "A" && String(command[2]) == "5") { pinVal = A5;}
  if (String(command[1]) == "A" && String(command[2]) == "6") { pinVal = A6;}
  if (String(command[1]) == "A" && String(command[2]) == "7") { pinVal = A7;}


// if requested set pinmode  
  if (String(command[0]) == "S" && String(command[3]) == "I") { Serial.print("set Pin input"); Serial.println(pinVal); pinMode(pinVal, INPUT);}
  if (String(command[0]) == "S" && String(command[3]) == "O") { Serial.print("set Pin output"); Serial.println(pinVal);pinMode(pinVal, OUTPUT);}
  if (String(command[0]) == "S" && String(command[3]) == "P") { Serial.print("set Pin input_pull"); Serial.println(pinVal);pinMode(pinVal, INPUT_PULLUP);}

// if requested perform digital write
// WHXX = Write High XX Pin
// WLXX = Write Low XX Pin
  if (String(command[0]) == "W" && String(command[3]) == "H") { digitalWrite(pinVal, HIGH);}
  if (String(command[0]) == "W" && String(command[3]) == "L") { digitalWrite(pinVal, LOW);}

// DG = Date Get Clock
// DSYyyyy = Set Year to YYYY
// DSMmm = Set Month to MM
// DSDdd = Set Day to DD
// DSHhh = Set Hours to HH
// DSmmm = Set Minutes to MM
// DSC = Commit Change to RTC

  if (String(command[0]) == "D" && String(command[1]) == "G") { showClock();};
  if (String(command[0]) == "D" && String(command[1]) == "S" && String(command[2]) == "Y")
  {
    String yearVal = "";
    int yearAsInt;
    yearVal=String(command[3]) + String(command [4]) + String(command[5]) + String(command [6]);
    yearAsInt=yearVal.toInt();
    setDate("Y",yearAsInt);
  }
  if (String(command[0]) == "D" && String(command[1]) == "S" && String(command[2]) == "M")
  {
    String month = "";
    int monthAsInt;
    month=String(command[3]) + String(command [4]);
    monthAsInt=month.toInt();
    setDate("M",monthAsInt);
  }  
  if (String(command[0]) == "D" && String(command[1]) == "S" && String(command[2]) == "D")
  {
    String day = "";
    int dayAsInt;
    day=String(command[3]) + String(command [4]);
    dayAsInt=day.toInt();
    setDate("D",dayAsInt);
  } 
  if (String(command[0]) == "D" && String(command[1]) == "S" && String(command[2]) == "H")
  {
    String hour = "";
    int hourAsInt;
    hour=String(command[3]) + String(command [4]);
    hourAsInt=hour.toInt();
    setDate("H",hourAsInt);
  } 
  if (String(command[0]) == "D" && String(command[1]) == "S" && String(command[2]) == "m")
  {
    String minute = "";
    int minuteAsInt;
    minute=String(command[3]) + String(command [4]);
    minuteAsInt=minute.toInt();
    setDate("m",minuteAsInt);
  } 
  if (String(command[0]) == "D" && String(command[1]) == "S" && String(command[2]) == "C")
  {
    Serial.println("Commit RTC Change");
  }
  
// if requested perform analog write
  if (String(command[0]) == "A" && pinVal == 3 || pinVal == 5 || pinVal == 6 || pinVal == 9 || pinVal == 10 || pinVal == 11 ) 
      { 
       analogWrite(pinVal, awValueVal);
       pwm[pinVal] = 1;
      }
  
}

int showClock()
{
  String clockVal="1223";
  Wire.write(clockVal.toInt());
}

int setDate(char field[1] , int value) 
{
  if (field == "Y") 
  {
    Serial.print("Setting Year to ");
    Serial.println(value);
  } 
  else if (field == "M")
  {
    Serial.print("Setting Month to ");
    Serial.println(value);
  } 
  else if (field == "D")
  {
    Serial.print("Setting Day to ");
    Serial.println(value);
  } 
  else if (field == "H")
  {
    Serial.print("Setting Hour to ");
    Serial.println(value);
  } 
  else if (field == "m")
  {
    Serial.print("Seting Minute to ");
    Serial.print(value);
    Serial.println(" and 00 Seconds");
  }
  else if (field == "C")
  {
    Serial.print("Seting Minute to ");
    Serial.print(value);
    Serial.println(" and 00 Seconds");
  }
  else 
  {
    Serial.println("Unknown parameter");
  }

}

int thermRead(int pin)
{
  int therm;
  float resistance;
  //pinMode(pin, OUTPUT);
  therm=analogRead(pin)-238;
  resistance=analogRead(pin);
  resistance=(1023/resistance) -1 ;
  Serial.print("Term Read RAW = ");
  Serial.print(resistance);
  Serial.print("r / ");
  Serial.print(therm);
  Serial.println( " Th");
  therm=pgm_read_word(&temps[therm]);
  Serial.print("Term Read CONVERTED = ");
  Serial.print(therm/10);
  Serial.println(" C");
  return therm;
}
