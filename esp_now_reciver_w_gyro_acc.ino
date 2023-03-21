// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;

// Define a data structure
typedef struct struct_message {
  float complpitch;
  float complroll;
  float gyroAngleZ;
} struct_message;

float accPitch,accRoll; 
float XAxis,YAxis,ZAxis;
float loopTimer, loopTimer2, freq, dt;
float complpitch_val,complroll_val,gyroAngleZ_val;

// Create a structured object
struct_message myData;
// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // Serial.println(myData.karmanroll/(3.142/180)); //x
  // Serial.println(myData.karmanpitch/(3.142/180)); //y
  // Serial.println(myData.gyroAngleZ/(3.142/180)*0.01); // z
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  
    // Try to initialize of gryo
	if (!mpu.begin()) {
		Serial.println("Failed to find MPU6050 chip");
		while (1) {
		  delay(10);
		}
	}
  Serial.println("MPU6050 Found!");

  // set accelerometer range to +-8G
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

	// set gyro range to +- 500 deg/s
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // low pass filter
  mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);
  
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.print("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
  loopTimer = micros();
  loopTimer2 = micros();
}
 
void loop() {
  sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);

  // /* Acc angle */
  XAxis = a.acceleration.x-0.94;
  YAxis = a.acceleration.y+0.16;
  ZAxis = a.acceleration.z-7.72+9.81;

  accRoll  = atan(YAxis/sqrt(XAxis*XAxis+ZAxis*ZAxis)); 
  //accPitch = -atan(XAxis/sqrt(YAxis*YAxis+ZAxis*ZAxis));

  freq = 1/((micros() - loopTimer2) * 1e-6);
  loopTimer2 = micros();
  dt = 1/freq;

  // complementary calculations 
  complroll_val = (0.9 * (complroll_val + (g.gyro.x+0.07)*dt) + 0.1 * accRoll)/(3.142/180);
  // complpitch_val = (0.9 * (complpitch_val + (g.gyro.y+0.24)*dt) + 0.1 * accPitch);
  // gyroAngleZ_val = gyroAngleZ_val + (g.gyro.z-0.15)* dt;

  Serial.println(complroll_val/(3.142/180));
  delay(300);
}