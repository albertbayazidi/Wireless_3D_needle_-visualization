// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;

// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {};// MAC Address//};

// Define a data structure
typedef struct struct_message {
  float complpitch;
  float complroll;
  float gyroAngleZ;
} struct_message;

float accPitch,accRoll; 
float XAxis,YAxis,ZAxis;
float loopTimer, loopTimer2, freq, dt;
float complpitch_val,complroll_val, gyroAngleZ_val;

// Create a structured object
struct_message myData;

// Peer info
esp_now_peer_info_t peerInfo;

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
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
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  loopTimer = micros();
  loopTimer2 = micros();
}

void loop() {
  // Create test data
  sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);

  // /* Acc angle */
  XAxis = a.acceleration.x-0.60;
  YAxis = a.acceleration.y+0.24;
  ZAxis = a.acceleration.z-10.63+9.81;
  
  accPitch = -atan(XAxis/sqrt(YAxis*YAxis+ZAxis*ZAxis));
  accRoll  = atan(YAxis/sqrt(XAxis*XAxis+ZAxis*ZAxis)); // (3.142/180)

  freq = 1/((micros() - loopTimer2) * 1e-6);
  loopTimer2 = micros();
  dt = 1/freq;

  // complementary calculations 
  complroll_val = (0.9 * (complroll_val + (g.gyro.x+0.06)*dt) + 0.1 * accRoll);
  complpitch_val = (0.9 * (complpitch_val + (g.gyro.y+0.24)*dt) + 0.1 * accPitch);
  gyroAngleZ_val = gyroAngleZ_val + (g.gyro.z-0.15)* dt;

  // Format structured data
  myData.complpitch = complpitch_val;
  myData.complroll = complroll_val;
  myData.gyroAngleZ = gyroAngleZ_val;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(1);
}
