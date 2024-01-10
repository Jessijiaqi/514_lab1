#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SERIAL_BAUD 115200
const int vibrationPin = 6;

Adafruit_MPU6050 mpu;
Adafruit_BME280 bme;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial) {} // Wait for Serial to initialize

  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("MPU6050 sensor not found");
    while (1);
  }
  Serial.println("MPU6050 sensor found");

  if (!bme.begin(0x76)) {
    Serial.println("BME280 sensor not found");
    while (1);
  }
  Serial.println("BME280 sensor found");

  pinMode(vibrationPin, OUTPUT);
  digitalWrite(vibrationPin, LOW); // Ensure vibration motor is initially off

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) {}
  }
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float temperature = temp.temperature;
  float accelX = fabs(a.acceleration.x);
  float accelY = fabs(a.acceleration.y);
  float accelZ = fabs(a.acceleration.z);

  float humidity = bme.readHumidity(); // 读取湿度值

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temperature: ");
  display.println(temperature);
  display.print("Humidity: ");
  display.println(humidity);
  // display.print("Accel X: "); // 显示X轴加速度值
  // display.println(accelX);
  // display.print("Accel Y: "); // 显示Y轴加速度值
  // display.println(accelY);
  // display.print("Accel Z: "); // 显示Z轴加速度值
  // display.println(accelZ);
  display.display();

  if (temperature > 20 && humidity < 50 && (accelX > 13.0 || accelY > 13.0 || accelZ > 22)) {
    // Activate vibration motor
    digitalWrite(vibrationPin, HIGH);
    delay(1000); // Vibrate for 1 second
    digitalWrite(vibrationPin, LOW);
    delay(500); // Wait for 0.5 seconds before re-checking sensors

    // Display message on OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Accel X,Y,Z: "); // 显示X、Y和Z轴加速度值
    display.print(accelX);
    display.print(",");
    display.print(accelY);
    display.print(",");
    display.println(accelZ);

    display.println("Don't move too fast & drink water!");
    display.display();
    delay(5000); // Display the message for 5 seconds
  }

  delay(1000); // Delay between sensor readings
}
