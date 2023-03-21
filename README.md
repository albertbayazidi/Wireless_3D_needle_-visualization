# AccuNeedle
AccuNeedle is a software and hardware solution that uses a gyroscope and accelerometer to provide real-time 3D visualization of a needle during medical procedures. The tool is designed to aid in finding the needle during an ultrasound operation.

The tool uses ESP NOW protocol and is powered by C++ and Processing. It wirelessly connects to another esp32 that is connected to a computer using serial for real-time visualization and feedback.

## Features
Real-time rendering of needle in 3D.
Complementary filter for accurate angle readout 
Calibration 

To use the tool the following components are required. 
-	2 ESP32
-	2 MPU6050 from adafruit
-	Wires and breadboard 

## Usage
1.	Connecting everything according to the diagrams.
2.	Calibrate both devices using the calibration code.
3.	Get MAC address of receiver.
4.	Upload receiver code to receiver and sender to sender. 

Remember to install necessary libraries.

The project was developed by Albert Bayazidi and Saim Naveed Iqbal 

