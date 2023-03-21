#include <Wire.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;

// values of intrest
float accRoll,accPitch;
float XAxis,YAxis,ZAxis;
float loopTimer, loopTimer2, freq, dt;
float complpitch_val,complyroll_val,gyroAngleZ;

// calibration varibales
float calib_var_x = 0;
float calib_var_y = 0;
float calib_var_z = 0;

float calib_var_x_gyro = 0;
float calib_var_y_gyro = 0;
float calib_var_z_gyro = 0;

int i = 0;

void setup(void) {
	Serial.begin(115200);

	// Try to initialize!
	if (!mpu.begin()) {
		Serial.println("Failed to find MPU6050 chip");
		while (1) {
		  delay(10);
		}
	}
	// set accelerometer range to +-8G
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

	// set gyro range to +- 500 deg/s
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // set lowpass filter
  mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);

  loopTimer = micros();
  loopTimer2 = micros();
}


void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // /* Acc angle */
  XAxis = a.acceleration.x-0.97;
  YAxis = a.acceleration.y+0.17;
  ZAxis = a.acceleration.z-7.74+9.81;

  /* Print out the values for acc testing */
  // Serial.print("X: ");
  // Serial.println(XAxis);
  // Serial.print("Y: ");
  // Serial.println(YAxis); 
  // Serial.print("Z: ");
  // Serial.println(ZAxis);
  
  accPitch = -atan(XAxis/sqrt(YAxis*YAxis+ZAxis*ZAxis));  //(3.14159/180) 
  accRoll  = atan(YAxis/sqrt(XAxis*XAxis+ZAxis*ZAxis));   //(3.14159/180)

  /* ACC angle print */
  // Serial.print("accPitch:");
  // Serial.println(accPitch/(3.14159/180));
  // Serial.print("accRoll:");
  // Serial.println(accRoll/(3.14159/180));
  // Serial.print("");

  /* gyro angle */
  freq = 1/((micros() - loopTimer2) * 1e-6);
  loopTimer2 = micros();
  dt = 1/freq;


  complyroll_val = (0.9 * (complyroll_val + (g.gyro.x+0.07)*dt) + 0.1 * accRoll);
  complpitch_val = (0.9 * (complpitch_val + (g.gyro.y+0.0)*dt) + 0.1 * accPitch);
  gyroAngleZ = gyroAngleZ + (g.gyro.z+0.15) * dt; // deg/s * s = deg

  // /* complementary print*/
  // Serial.print("complementary roll: ");
  // Serial.println(complementaryroll_val/(3.14159/180));
  // Serial.print("complementary pich: ");
  // Serial.print(complpitch_val/(3.14159/180));
  // Serial.println("");  


  // // /* ACC CALIBRATION */
  // calib_var_x = a.acceleration.x + calib_var_x;
  // calib_var_y = a.acceleration.y + calib_var_y;
  // calib_var_z = a.acceleration.z + calib_var_z;

  // if (i == 2000){
  //     Serial.print("Acceleration X: ");
  //     Serial.println(calib_var_x/i);
  //     Serial.print(", Y: ");
  //     Serial.println(calib_var_y/i);
  //     Serial.print(", Z: ");
  //     Serial.println(calib_var_z/i);
  // }

  // // /* GYRO CALIBRATION */
  // calib_var_x_gyro = g.gyro.x + calib_var_x_gyro;
  // calib_var_y_gyro = g.gyro.y + calib_var_y_gyro;
  // calib_var_z_gyro = g.gyro.z + calib_var_z_gyro;

  // if (i == 2000){
  //     Serial.print("gyro X: ");
  //     Serial.println(calib_var_x_gyro/i);
  //     Serial.print(", Y: ");
  //     Serial.println(calib_var_y_gyro/i);
  //     Serial.print(", Z: ");
  //     Serial.println(calib_var_z_gyro/i);
  // }

  i += 1;
  delay(1);
}
