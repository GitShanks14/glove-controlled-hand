# glove-controlled-hand

## Arduino scripts

### Servo test 1
Tests the functionality of the motor by rotating it repeatedly between its two extreme positions. To be run first to test if all motors being used are functional. If any motors exhibit abnormal behaviour such as lowered rotation speeds, severe vibrations etc, replace the motor and proceed. 

Servo motor is attached to PWM pin 9. 

### Servo test 2
Tests the servo motors using a potentiometer. The values in the scripts are varied to perform calibration. Calibration is to map the two extreme positions of the potentiometer to the extremes of the motors capabilities. <br/>

Servo motor is attached to PWM pin 9. <br/>
Voltage divider comprising the potentiometer and a resistor is attached to pin A0. <br/>
Parameters R0min, ROmax and Rdiv are varied to calibrate the script. <br/>

### Servo test 3
Computer-based control of the servo motor's position. User inputs are provided through the serial interface. Used to test each motor and hence finger one at a time. Ideally, must be used for the calibration of arm testing scripts. The minimum and maximum motor positions for the control of the fingers may be estimated using this script. <br/>

Motor connected to PWM pin 9. <br/>

Inputs that can be provided: <br/>
* 'q' : LED blink <br/>
* 'w' : 180 position <br/>
* 'a' : motor position -> motor position - 10 <br/>
* 's' : 0 position <br/>
* 'd' : motor position -> motor position + 10 <br/>
* ' ' : Toggle motor position : motor position -> 180 - motor position  <br/>
* 'e' : 90 position <br/>
Default inputs redirect to the blink state <br/>

### Arm test 1
For testing the arm after calibration: both glove-based and computer-based. 

Inputs that can be provided: <br/>
* 'q' : LED blink state <br/>
* 'l' : Toggles little finger <br/>
* 'r' : Toggles ring finger <br/>
* 'm' : Toggles middle finger <br/>
* 'p' : Toggles pointer finger <br/>
* 't' : Toggles thumb finger <br/>
* 'c' : Redoes the calibration step <br/>
* 'u' : Switches between computer-based control and glove-based control. Default: computer-based <br/>
* 'x' : Exits glove-based control and resets hand's position to resting position. <br/>

For the calibration step, the hand is to be held still in a resting position. The mean values are computed and stored. These values are subtracted during the measurement stage henceforth. This helps accentuate the changes in the voltage divider readings.

#### Setup:
MPin[5] to contain the PWN pin numbers the motors are connected to. Default = {10,9,6,5,3}.
ResPos[5], GriPos[5] to be set using the "Servo test 3" script on the arm. 
Vavg[5] is set during the calibration step. Input 'c' to calibrate as described above. 
Vary VMax[5] to fine-tune the control of the hand. 
