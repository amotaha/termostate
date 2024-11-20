#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SSD1306.h>

// تنظیمات برای نمایشگر OLED
#define SCREEN_WIDTH 128 // عرض نمایشگر
#define SCREEN_HEIGHT 64 // ارتفاع نمایشگر
#define OLED_RESET -1 // پین ریست OLED (در صورت نیاز)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// تنظیمات برای سنسور DS18B20
#define ONE_WIRE_BUS 2 // پین دیجیتال متصل به سنسور
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//varlable
int temperatureC = 0;
int number = temperatureC; // عدد منفی
int numDigits = 0;
int per_temp=30;
float previousTemperature = 0; // متغیر برای ذخیره دمای قبلی




const uint8_t celsius [] PROGMEM = {
  0x00, 0x00,
  0x00, 0x00,
  0x30, 0xf0,
  0x59, 0xf8,
  0x4b, 0x00,
  0x72, 0x00,
  0x06, 0x00,
  0x06, 0x00,
  0x02, 0x00,
  0x03, 0x00,
  0x01, 0x98,
  0x00, 0xf0,
  0x00, 0x00,
  0x00, 0x00
};

// 'images', 14x14px
// 'images', 14x14px
const unsigned char tick [] PROGMEM = {
	0x7f, 0xf8, 0xff, 0xfc, 0xc0, 0x0c, 0xc0, 0x0c, 0xc0, 0x6c, 0xc0, 0xec, 0xd9, 0xcc, 0xdf, 0x8c, 
	0xcf, 0x0c, 0xc6, 0x0c, 0xc0, 0x0c, 0xc0, 0x0c, 0xff, 0xfc, 0x7f, 0xf8
};
// 'New Project', 14x14px
const unsigned char snow [] PROGMEM = {
	0x0c, 0x00, 0x1e, 0x30, 0x12, 0x30, 0x12, 0x30, 0x12, 0x30, 0x12, 0x30, 0x12, 0x30, 0x12, 0x30, 
	0x21, 0x30, 0x2d, 0x30, 0x2d, 0x78, 0x21, 0x78, 0x1e, 0x30, 0x0c, 0x00
};
// 'New Project2', 14x14px
const unsigned char fire [] PROGMEM = {
	0x00, 0x00, 0x0c, 0x00, 0x1e, 0x30, 0x12, 0x78, 0x12, 0x78, 0x12, 0x30, 0x12, 0x30, 0x12, 0x30, 
	0x21, 0x30, 0x2d, 0x30, 0x2d, 0x30, 0x21, 0x30, 0x1e, 0x30, 0x0c, 0x00
};
void setup() {
  // شروع ارتباط سریال
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  // شروع ارتباط با نمایشگر OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // شروع ارتباط با سنسور DS18B20
  display.clearDisplay();
  
  sensors.begin();
}

void loop() {
  digitalWrite(4,HIGH);  
  digitalWrite(5,HIGH);
  digitalWrite(9,LOW);
    // خواندن دمای سنسور و نمایش آن
  sensors.requestTemperatures();
  temperatureC = sensors.getTempCByIndex(0);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.print("TERMOSTATE");
        
          // نمایش دما با اندازه فونت متناسب با تعداد ارقام
  if (temperatureC == -127) {
    display.setCursor(10, 30);
  } else {

    int numDigits = 0;
    int number = abs(temperatureC);
    while (number > 0) {
      number /= 10;
      numDigits++;
    }
    int xOffset = 60 - numDigits * 10; // محاسبه موقعیت شروع متن بر اساس تعداد ارقام
    display.setCursor(xOffset, 30);
  }
  display.setTextSize(4);
  display.print(temperatureC);
  display.drawBitmap(105, 25, celsius, 14, 14, WHITE);
  //display.drawBitmap(105, 50, t, 14, 14, WHITE);

    // مقایسه دما با مقدار قبلی و نمایش پیام مناسب
  if (temperatureC > previousTemperature) {
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.print("H");
    display.drawBitmap(8,30, fire, 14, 14, WHITE);
  } else if (temperatureC < previousTemperature) {
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("C");
    display.drawBitmap(8,50, snow, 14, 14, WHITE);
  } else if (temperatureC >= 45 & temperatureC <= 90) {
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.print("R");
    display.drawBitmap(8, 40, tick, 14, 14, WHITE);
  
  } else if (temperatureC== -127) {
    display.setTextSize(2);
    display.setCursor(110, 50);
    display.print("E");
  }
  if (temperatureC > 45) {
  digitalWrite(8, HIGH); //high the realy
  }
  else {
  digitalWrite(8, LOW); //low the realy
  }

    previousTemperature = temperatureC;
  display.display();
  delay(10);
}
