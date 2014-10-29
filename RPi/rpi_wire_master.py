#// Arduino I2C Wire master version 0.2
#// by Racer993 <http://raspberrypi4dummies.wordpress.com/>

#// Turns the Raspberry Pi into a I2C master device using I2C-tools.
#// send commands to the I2C Arduino slave for configuring pins,
#// read and write to pins via a simple instruction set.

#// the i2c-tools are required use "sudo apt-get install i2c-tools"
#// the I2C slave software must be installed on the Arduino

#// Created 28 July 2013

#// This example code is in the public domain.

import smbus
import time

# RPi rev 1 = SMBus(0)
# RPi rev 2 = SMBus(1)
bus = smbus.SMBus(1)

# address of the Arduino use "i2cdetect -y 1" from the RPi prompt to detect the Arduinos (up to 127!)
device = 0x50

# initialize variables
pin      = ""  #holds the pin number 0 - 13 or A0 - A3
type     = ""  #holds the pin type: INPUT, OUTPUT, INPUT_PULLUP
mode     = ""  #holds the pinmode: HIGH, LOW, PWM
pwmValue = ""  #holds the pwmValue
pwm      = ""  #holds the pwmValue in 3 digits
val      = ""  #holds a String to be converted into ASCII
cmd      = ""  #holds the first byte of the message for the Arduino
message  = ""  #holds the second - seventh byte of the message for the Arduino
valCmd   = 88                #holds the command as ASCII value 88 = "X"
valMessage  = [88,88,88,88,88,88] #holds the Message as ASCII values


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
  print("Message: " + cmd + message + " send to device " + str(device))
  bus.write_i2c_block_data(device, valCmd, valMessage)
  time.sleep(1)


# ------------------------------------------------------------------------------------------------------------------------------------------------
# this routine converts a 1 or 2 digit pin into a 2 digit equivalent
def pinString(pin):
  while len(pin) < 2:
    pin = "0"+pin;
  return pin

# ------------------------------------------------------------------------------------------------------------------------------------------------
# this routine send a request to the Arduino to provide a 30 byte status update, return all 30 bytes

def getStatus(device,bytes):
  status = ""
  for i in range (0, bytes):
    status += chr(bus.read_byte(device))
    time.sleep(0.05);
  time.sleep(0.1)       
  return status


# ------------------------------------------------------------------------------------------------------------------------------------------------
# This will return the rain sensor status
# Arduino will return a string like
# SR0 = No Rain Drops
# SR1 = Rain Drops detected
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

# ------------------------------------------------------------------------------------------------------------------------------------------------
# This will return the rain sensor status
# Arduino will return a string like
# SR0 = No Rain Drops
# SR1 = Rain Drops detected
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

# ------------------------------------------------------------------------------------------------------------------------------------------------
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

# ------------------------------------------------------------------------------------------------------------------------------------------------
# This will return the rain sensor status
# Arduino will return a string like
# SZXO = Zone X is open
# SZXC = Zone C is close

def getZoneStatus(device,zone):
  status = "2"
  sendMessage(device,'S','Z'+str(zone))
  time.sleep(0.5)
  status=getStatus(device,4)
  if (status[:2] == "SZ"):
    if (status[3:4] == "EE"):
       return -1
    return status[4]
  else:
    status=2
  return status

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

Valid Parameters
  * Y = Zone
  * XXX = Minutes that the zone must be open

""" 
while True:
  #setPin(0x50, "10", "Output")  #on Arduino with I2C ID #33 set pin 13 to OUTPUT
  #writePin(0x50,"10", "Low")   #on Arduino with I2C ID #33 set pin 13 HIGH
  #print ("Status pin 10 =" + pinValue(0x50,'10'))
  #time.sleep(5)
  #writePin(0x50,"10","High")     #on Arduino with I2C ID #33 set pin 13 LOW
  #print("0x30 byte status:" + getStatus(0x50))
  #getClock(0x50)
  #print("0x30 byte status:" + getStatus(0x50))
  sendMessage(0x50,'O','N1030')
  time.sleep(15)
  print("Sleep Over")
  time.sleep(1)
  sendMessage(0x50,'C','N1')
  time.sleep(5)

"""
def sendMessage(device, cmd, message):
  cmd=cmd.upper()
  message = message.upper()
  valCmd = ord(cmd)
  valMessage  = StringToBytes(message)
  print("Message: " + cmd + message + " send to device " + str(device))
  bus.write_i2c_block_data(device, valCmd, valMessage)
  time.sleep(1)
"""
