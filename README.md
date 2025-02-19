This is the arduino code for a moving podium build off of spare FRC parts. 
Hardware list is as follows:
  - Arduino Uno
  - Usb host shield
  - Logitech extreme 3d pro joystick
  - Victor SPX Motor controller (x4)
  - FRC Power dist. pannel
  - FRC 2.5 in Cim motors (x4)
  - 120 amp circuit breaker
  - 12 volt battery


Two cim motors control the left and right side of the drive base respectfully. In order to cut back on stray wires, one pin from the arduino sends the same pwm signal to both grouped motors. I'll eventually upload pictures of the wiring once it gets cleaned up a bit.

cad model:

![Screenshot_20250129_161013_Fusion](https://github.com/user-attachments/assets/041b114b-aaf8-43ae-9765-f4b1d15c5085)

![Screenshot_20250129_161034_Fusion](https://github.com/user-attachments/assets/ca465b7c-b705-4228-a9a1-7d2f236c5acc)


Wiring. Left and right side motor controllers are daisy chained on their respective sides.
![20250218_151840](https://github.com/user-attachments/assets/b1fb3f71-0274-4a6a-aea7-239d7ac9160f)
