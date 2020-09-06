// ESP32 I2C Scanner
// Based on code of Nick Gammon  http://www.gammon.com.au/forum/?id=10896
// ESP32 DevKit - Arduino IDE 1.8.5
// Device tested PCF8574 - Use pullup resistors 3K3 ohms !
// PCF8574 Default Freq 100 KHz 

#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;


#include <ArduinoJson.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <TimeLib.h>
#define mphal_i2c_wait_a() os_delay_us(20)
#define mphal_i2c_wait_b() os_delay_us(10)

#define TFT_CS   27
#define TFT_DC   26
#define TFT_MOSI 23
#define TFT_CLK  18
#define TFT_RST  5
#define TFT_MISO 12
//#define TFT_LED   5  // GPIO not managed by library

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

/*
#define TFT_CS  22
#define TFT_DC  21
#define TFT_RST  5

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
*/
const char* ssid = "Half-Life";
const char* password = "funfunfun";

const char host[] = "api.coinmarketcap.com";

 // Colors
 int ILI9341_COLOR;
 #define CUSTOM_DARK 0x4228 // Background color

void writeI2CRegister8bit(int addr, int value) {
  Wire.beginTransmission(addr);
  Wire.write(value);
  Wire.endTransmission();
}

unsigned int readI2CRegister16bit(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(20);
  Wire.requestFrom(addr, 2);
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}

void setup()
{
  Serial.begin (115200); 
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(CUSTOM_DARK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextWrap(true);
  tft.setCursor(0, 170);
  tft.setTextSize(2);
 
  tft.println(">>> Connecting to: ");
  tft.println(" ");
  tft.println(ssid);
  Serial.println("start");
  WiFi.begin(ssid, password);

  tft.println(" ");
  tft.print(">>> WiFi connected");
  //tft.print("IP address: ");
  //tft.println(WiFi.localIP());

  delay(1500);
  tft.fillScreen(CUSTOM_DARK); // Clear Screen
  tft.setTextColor(ILI9341_BLUE);
  tft.setCursor(0, 150);
  tft.setTextSize(2);
  tft.println("Josiah's Garden");
  tft.drawLine(0,130,240,130, ILI9341_WHITE);
  tft.drawLine(0,185,240,185, ILI9341_WHITE);
  delay(1500);
  tft.setTextSize(1);
  tft.setCursor(5, 307);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Data from: CoinMarketCap.com");
  Wire.begin (19, 00);   // sda= GPIO_21 /scl= GPIO_22
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
  writeI2CRegister8bit(0x20, 6); //reset


}

void loop() {

  Serial.print("Moisture = ");
  Serial.print(readI2CRegister16bit(0x20, 0)); //read capacitance register
  Serial.println(" ");
  
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  tft.fillScreen(CUSTOM_DARK); // Clear Screen
  tft.setTextColor(ILI9341_BLUE);
  tft.setCursor(0, 150);
  tft.setTextSize(2);
  tft.println("Temperature: ");
  tft.print(bmp.readTemperature());
  tft.println(" *C");
  tft.println();

  tft.drawLine(0,130,240,130, ILI9341_WHITE);
  tft.drawLine(0,285,240,285, ILI9341_WHITE);
  tft.println("Altitude: ");
  tft.print(bmp.readAltitude());
  tft.println(" meters");
  tft.println();
  tft.println("Moisture: ");     
  tft.print(readI2CRegister16bit(0x20, 0));
  tft.println(" ");
  tft.setTextSize(1);
  tft.setCursor(5, 307);
  tft.setTextColor(ILI9341_WHITE);
  tft.println(bmp.readPressure());


    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
    
    Serial.println();
    delay(10000);
}
