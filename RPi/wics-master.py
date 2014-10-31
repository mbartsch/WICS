#// Arduino I2C Wire master version 0.2
#// by Racer993 <http://raspberrypi4dummies.wordpress.com/>
#//
#// i2c LCD Libraries from https://bitbucket.org/thinkbowl/i2clibraries.git
#//

#// Turns the Raspberry Pi into a I2C master device using I2C-tools.
#// send commands to the I2C Arduino slave for configuring pins,
#// read and write to pins via a simple instruction set.

#// the i2c-tools are required use "sudo apt-get install i2c-tools"
#// the I2C slave software must be installed on the Arduino

#// Created 28 July 2013

#// This example code is in the public domain.

import smbus
import time
from i2clibraries import i2c_lcd_smbus
import time

lcd = i2c_lcd_smbus.i2c_lcd(0x27,1, 2, 1, 0, 4, 5, 6, 7, 3)

# RPi rev 1 = SMBus(0)
# RPi rev 2 = SMBus(1)
bus = smbus.SMBus(1)

# address of the Arduino use "i2cdetect -y 1" from the RPi prompt to detect the Arduinos (up to 127!)
device = 0x50

# initialize variables
pin        = ""  #holds the pin number 0 - 13 or A0 - A3
type       = ""  #holds the pin type: INPUT, OUTPUT, INPUT_PULLUP
mode       = ""  #holds the pinmode: HIGH, LOW, PWM
pwmValue   = ""  #holds the pwmValue
pwm        = ""  #holds the pwmValue in 3 digits
val        = ""  #holds a String to be converted into ASCII
cmd        = ""  #holds the first byte of the message for the Arduino
message    = ""  #holds the second - seventh byte of the message for the Arduino
valCmd     = 88                #holds the command as ASCII value 88 = "X"
valMessage = [88,88,88,88,88,88] #holds the Message as ASCII values
zonestatus = {0:'OPEN',1:'CLOSE'}
debug      = 0

# ------------------------------------------------------------------------------------------------------------------------------------------------
# this routine converts Strings to ASCII code
def StringToBytes(val):
  retVal = []
  for c in val:
    retVal.append(ord(c))
  return retVal


# ------------------------------------------------------------------------------------------------------------------------------------------------
# this routine actually transmits the command,
# sleep is required in order to prevent a request overload on the Arduino
def sendMessage(device, cmd, message):
  cmd=cmd.upper()
  message = message.upper()
  valCmd = ord(cmd)
  valMessage  = StringToBytes(message)
  if debug:
    print("Message: " + cmd + message + " send to device " + str(device)) 
  bus.write_i2c_block_data(device, valCmd, valMessage)
  time.sleep(1)


# ------------------------------------------------------------------------------------------------------------------------------------------------
# this routine converts a 1 or 2 digit pin into a 2 digit equivalent
def pinString(pin):
  while len(pin) < 2:
    pin = "0"+pin;
  return pin

# -----------------------------------------------------------------------------
# this routine send a request to the Arduino to provide a 30 byte status 
# update, return all 30 bytes

def getStatus(device,bytes):
  status = ""
  for i in range (0, bytes):
    status += chr(bus.read_byte(device))
    time.sleep(0.05);
  time.sleep(0.1)       
  return status


# -----------------------------------------------------------------------------
# This will return the rain sensor status
# Arduino will return a string like
# SR1 = No Rain Drops
# SR0 = Rain Drops detected
def getRainSensorStatus(device):
  status = "2"
  sendMessage(device,'S','R')
  time.sleep(0.5)
  status=getStatus(device,3)
  if (status == "SR0"):
    status=0
  elif (status == "SR1"):
    status=1
  else:
    status=2
  return status


# -----------------------------------------------------------------------------
# This will return the rain sensor status
# Arduino will return a string like
# SR1 = No Rain Drops
# SR0 = Rain Drops detected
def getMoistureSensorStatus(device):
  status = "2"
  sendMessage(device,'S','M')
  time.sleep(0.5)
  status=getStatus(device,3)
  if (status == "SM0"):
    status=0
  elif (status == "SM1"):
    status=1
  else:
    status=2
  return status

# -----------------------------------------------------------------------------
# This will return the soil moisture status
# Arduino will return a string like
# SM0 = No Soil Moisture 
# SM1 = Soil Moisture
def getSoilMoistureStatus(device):
  status = "2"
  sendMessage(device,'S','M')
  time.sleep(0.5)
  status=getStatus(device,3)
  if (status == "SM0"):
    status=0
  elif (status == "SM1"):
    status=1
  else:
    status=2
  return status

# -----------------------------------------------------------------------------
# This will return the rain sensor status
# Arduino will return a string like
# SZXO = Zone X is open
# SZXC = Zone C is close


def getAllZoneStatus(device):
  status = {}
  for zone in range (1, 5):
    status[zone]=str(getZoneStatus(device,zone))
  return status


def getZoneStatus(device,zone):
  status = "2"
  sendMessage(device,'S','Z'+str(zone))
  time.sleep(0.01)
  status=getStatus(device,5)
  if debug:
    print("Status = " + status)
  if (status[:2] == "SZ"):
    if debug:
      print("Status[2:4] = " + status[2:4])
    if (status[2:4] == "EE"):
       return -1
  else:
    status=2
  return int(status[3])


def getDistance(device):
  status = "-1"
  sendMessage(device,'S','D')
  time.sleep(0.01)
  status=getStatus(device,6)
  if debug:
    print("Status = " + status)
  if (status[:2] == "SD"):
    if debug:
      print("Status[2:5] = " + status[2:5])
  return int(status[2:5])
  
# ------------------------------------------------------------------------------------------------------------------------------------------------
# this is where the main program starts
"""
Valid commands are:
  * ONYXXX
    - Open Normal, this will allow the controller to check for rain, humidity and temperature and determine if will open or not the zone
  * CNY
    - Clone the zone before the timer expire
  * OF1XXX
    - Open Force, as ON but will not check for extra parameters and will just open the zone
  * SZY
    - Check if a zone is open or close and if it was forced or normal, also report the minutes it has been running
  * ST
    - Return current temperature
  * SH
    - Return curren humidity
  * SR
    - Return Rain Sensor status
  * SM
    - Return Soil Moisture status
  * SD
    - Return Distance measures

Valid Parameters
  * Y = Zone
  * XXX = Minutes that the zone must be open

""" 
lcd.command(lcd.CMD_Display_Control | lcd.OPT_Enable_Display)
lcd.backLightOn()
lcd.writeString("WICS v0.2")
while True:
  print(getDistance(device))
"""
  print(getRainSensorStatus(device))
  print(getMoistureSensorStatus(device))
  zones=(getAllZoneStatus(device))
  lcd.backLightOn()
  lcd.setPosition(1, 0)
  lcd.writeString("Z1:" + zones[1] + " Z2:" + zones[2])
  lcd.setPosition(2, 0)
  lcd.writeString("Z3:" + zones[3] + " Z2:" + zones[3])
  sendMessage(device,'O','N1030')
  zones=(getAllZoneStatus(device))
  lcd.setPosition(1, 0)
  lcd.writeString("Z1:" + zones[1] + " Z2:" + zones[2])
  lcd.setPosition(2, 0)
  lcd.writeString("Z3:" + zones[3] + " Z2:" + zones[3])
  time.sleep(1)
  print(getZoneStatus(device,1))
  print(getZoneStatus(device,2))
  time.sleep(15)
  print("Sleep Over")
  lcd.backLightOff()
  sendMessage(device,'C','N1')
  print(getZoneStatus(device,1))
  print(getZoneStatus(device,2))
  print(getAllZoneStatus(device))
  time.sleep(5)

"""
