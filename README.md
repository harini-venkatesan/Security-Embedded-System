# Security System

### Harini Venkatesan


## Description:

This is a security system that is password enabled, with a functioning motion sensor, a
servo motor and a bluetooth module.
If the wrong password is entered, the system is enabled. If motion is detected, the
speaker emits an alarm sound and the LED flashes red. The bluetooth module
connected to a device also sends a “Break In!” signal alerting the user of the security
system.
The system from there can be reset if the correct password is entered.
If the correct password is entered, the motion sensor is disabled and the user can enter
the password again to enable the system.

## User guide:

The correct password to enable or disable the system is “#1234”. This functionality
could be expanded by making the user be able to set and change password and store it
using EEPROM.
Connect to the bluetooth module by pairing it with the HC-05 device.

## Technologies used:

1. **Atmel Studio 7.0** ​ to code and build the state machines
2. **ATmega1284** ​ to program the written code into
3. **PIR motion sensor** ​ that detects motion after system is locked
4. **HC-05 Bluetooth module** ​ that sends alert if there is a break in
5. **SG-90 servo motor** ​ that indicates door opening and closing
6. **4x4 keypad** ​to enter password
7. **LCD display** ​that acts as a password interface
8. **LED lights** ​that flashes red to signal intruder alert
9. **Buzzer** ​to emit an alarm signaling intruder


## Complexities or Build-Upons:

**1. PIR motion sensor -** ​This is a sensor that detects infrared heat radiations. This
    devices has three pins. The first one is connected to GND, the second is
    connected to PB0 which is the output and third is connected to VCC.
    
**2. HC-05 bluetooth module -** ​This is a Bluetooth device used for wireless
    communication. It works using USART. This device has 4 pins which is
    connected to VCC, GND, PD0 and PD1.
    
**3. SG-90 servo motor -** ​This is a motor that uses angular rotation to indicate doors
    opening and closing. This device has three pins which is connected to GND,
    VCC and PB6.


## Source files used:

1. [**Main.c**](https://github.com/harini-venkatesan/Security-Embedded-System/blob/master/Code/main.c)

This is the file containing the main source code. In this file, I use three state machines -
one for locking mechanism, one for motion sensor mechanism and one for detection
mechanism. The state machine logic is described above.

2. [**Includes -**](https://github.com/harini-venkatesan/Security-Embedded-System/tree/master/Code/Includes)

This is the folder containing all the include files used for this project. The biggest ones
used are

● **timer.h** ​ which performs the tasks of a timer

**● Keypad.h** ​which performs keypad tasks like GetKeypad()

**● Servo_PWM.h** ​which initializes the servo motor and rotates it based on the
desired amount

**● Speaker.h** ​which performs tasks of the speaker such as SpeakerInit()

**● usart.h** ​which helps bluetooth connect to the device. I added an extra function in
that file which is USART_SendString() that enables my code to send a string to
the device.

## Youtube link:

[CS120B Final Project](https://www.youtube.com/watch?v=7GKF7fNl220&t=1s)

## Project Images

![alt text](https://github.com/harini-venkatesan/Security-Embedded-System/blob/master/Images/Connections%20-%20Locked.jpg)




