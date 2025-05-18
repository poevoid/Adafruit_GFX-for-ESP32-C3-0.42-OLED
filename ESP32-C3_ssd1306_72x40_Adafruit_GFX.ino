#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_task_wdt.h>

// Custom Display Configuration
#define VISIBLE_WIDTH   72
#define VISIBLE_HEIGHT  40
#define X_OFFSET        28    // Horizontal content shift
#define Y_OFFSET        24    // Vertical graphics shift
#define TEXT_Y_OFFSET   36    // Vertical text alignment
#define BUFFER_WIDTH    128   // Controller buffer size
#define BUFFER_HEIGHT   64
#define RESET_PIN       4     // Hardware reset GPIO
#define SCREEN_ADDRESS  0x3C  // Your display's I2C address

// Offset macros
#define GX(x) (x + X_OFFSET)  // Graphic X position
#define GY(y) (y + Y_OFFSET)  // Graphic Y position
#define TX(x) (x + X_OFFSET)  // Text X position
#define TY(y) (y + TEXT_Y_OFFSET) // Text Y position
#define NUMFLAKES     5 // Number of snowflakes in the animation example

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };



Adafruit_SSD1306 display(BUFFER_WIDTH, BUFFER_HEIGHT, &Wire, -1);


void enterBootloader() {
   delay(100);
  
  // Proper watchdog configuration
  const esp_task_wdt_config_t wdt_config = {
    .timeout_ms = 1000,
    .idle_core_mask = 0,
    .trigger_panic = true
  };
  
  esp_task_wdt_init(&wdt_config);
  esp_task_wdt_add(xTaskGetCurrentTaskHandle());
  
  while(true); // Wait for watchdog to trigger
}

void checkUploadTrigger() {
  if(Serial.available() && Serial.read() == 'b') {
    enterBootloader();
    }
  
}

// Hardware reset function
void hardwareReset() {
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);
  delayMicroseconds(100);
  digitalWrite(RESET_PIN, HIGH);
  delay(10);
}

void setup() {
   // Hold BOOT button override
  pinMode(9, INPUT_PULLUP);  // GPIO9 = BOOT button
  
  // Upload rescue delay
  delay(2000);  // 2-second window for uploads
  
  // Check for serial upload trigger
  Serial.begin(115200);
  for(int i=0; i<20; i++) {
    checkUploadTrigger();
    delay(10);
  }
  hardwareReset();
  
  Wire.begin(5, 6); // SDA=GPIO5, SCL=GPIO6
  Wire.setClock(400000);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.display();
  delay(2000);

  // Original example flow with modified coordinates
  display.drawPixel(GX(10), GY(10), SSD1306_WHITE);
  display.display();
  delay(2000);

  testdrawline();
  testdrawrect();
  testfillrect();
  testdrawcircle();
  testfillcircle();
  testdrawroundrect();
  testfillroundrect();
  testdrawtriangle();
  testfilltriangle();
  testdrawchar();
  testdrawstyles();
  testscrolltext();
  testdrawbitmap();

  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);

  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT);
}

// Fully modified drawing functions
void testdrawline() {
  display.clearDisplay();

  for(int16_t i=0; i<VISIBLE_WIDTH; i+=4) {
    display.drawLine(GX(0), GY(0), GX(i), GY(VISIBLE_HEIGHT-1), SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(int16_t i=0; i<VISIBLE_HEIGHT; i+=4) {
    display.drawLine(GX(0), GY(0), GX(VISIBLE_WIDTH-1), GY(i), SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(int16_t i=0; i<VISIBLE_WIDTH; i+=4) {
    display.drawLine(GX(0), GY(VISIBLE_HEIGHT-1), GX(i), GY(0), SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(int16_t i=VISIBLE_HEIGHT-1; i>=0; i-=4) {
    display.drawLine(GX(0), GY(VISIBLE_HEIGHT-1), GX(VISIBLE_WIDTH-1), GY(i), SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(int16_t i=VISIBLE_WIDTH-1; i>=0; i-=4) {
    display.drawLine(GX(VISIBLE_WIDTH-1), GY(VISIBLE_HEIGHT-1), GX(i), GY(0), SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(int16_t i=VISIBLE_HEIGHT-1; i>=0; i-=4) {
    display.drawLine(GX(VISIBLE_WIDTH-1), GY(VISIBLE_HEIGHT-1), GX(0), GY(i), SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(int16_t i=0; i<VISIBLE_HEIGHT; i+=4) {
    display.drawLine(GX(VISIBLE_WIDTH-1), GY(0), GX(0), GY(i), SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(int16_t i=0; i<VISIBLE_WIDTH; i+=4) {
    display.drawLine(GX(VISIBLE_WIDTH-1), GY(0), GX(i), GY(VISIBLE_HEIGHT-1), SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(2000);
}

void testdrawrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<VISIBLE_HEIGHT/2; i+=2) {
    display.drawRect(GX(i), GY(i), 
                    VISIBLE_WIDTH-2*i, VISIBLE_HEIGHT-2*i, 
                    SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(2000);
}

void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<VISIBLE_HEIGHT/2; i+=3) {
    display.fillRect(GX(i), GY(i), 
                    VISIBLE_WIDTH-2*i, VISIBLE_HEIGHT-2*i, 
                    SSD1306_INVERSE);
    display.display();
    delay(1);
  }
  delay(2000);
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(VISIBLE_WIDTH,VISIBLE_HEIGHT)/2; i+=2) {
    display.drawCircle(GX(VISIBLE_WIDTH/2), GY(VISIBLE_HEIGHT/2), 
                      i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(2000);
}

void testfillcircle(void) {
  display.clearDisplay();

  for(int16_t i=max(VISIBLE_WIDTH,VISIBLE_HEIGHT)/2; i>0; i-=3) {
    display.fillCircle(GX(VISIBLE_WIDTH/2), GY(VISIBLE_HEIGHT/2), 
                      i, SSD1306_INVERSE);
    display.display();
    delay(1);
  }
  delay(2000);
}

void testdrawroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<VISIBLE_HEIGHT/2-2; i+=2) {
    display.drawRoundRect(GX(i), GY(i), 
                         VISIBLE_WIDTH-2*i, VISIBLE_HEIGHT-2*i,
                         VISIBLE_HEIGHT/4, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(2000);
}

void testfillroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<VISIBLE_HEIGHT/2-2; i+=2) {
    display.fillRoundRect(GX(i), GY(i), 
                         VISIBLE_WIDTH-2*i, VISIBLE_HEIGHT-2*i,
                         VISIBLE_HEIGHT/4, SSD1306_INVERSE);
    display.display();
    delay(1);
  }
  delay(2000);
}

void testdrawtriangle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(VISIBLE_WIDTH,VISIBLE_HEIGHT)/2; i+=5) {
    display.drawTriangle(
      GX(VISIBLE_WIDTH/2),  GY(VISIBLE_HEIGHT/2-i),
      GX(VISIBLE_WIDTH/2-i), GY(VISIBLE_HEIGHT/2+i),
      GX(VISIBLE_WIDTH/2+i), GY(VISIBLE_HEIGHT/2+i), 
      SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(2000);
}

void testfilltriangle(void) {
  display.clearDisplay();

  for(int16_t i=max(VISIBLE_WIDTH,VISIBLE_HEIGHT)/2; i>0; i-=5) {
    display.fillTriangle(
      GX(VISIBLE_WIDTH/2),  GY(VISIBLE_HEIGHT/2-i),
      GX(VISIBLE_WIDTH/2-i), GY(VISIBLE_HEIGHT/2+i),
      GX(VISIBLE_WIDTH/2+i), GY(VISIBLE_HEIGHT/2+i), 
      SSD1306_INVERSE);
    display.display();
    delay(1);
  }
  delay(2000);
}

void testdrawchar(void) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(TX(0), TY(0));
  display.cp437(true);

  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else display.write(i);
    if((i % 21) == 0 && i != 0) {  // Adjusted for visible area
      display.setCursor(TX(0), TY((i/21)*8));
    }
  }
  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(TX(0), TY(0));
  display.println(F("Hello World!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.println(3.141592);

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);
  display.display();
  delay(2000);
}
void testscrolltext(void) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Position text in center of visible area
  display.setCursor(TX(20), TY(16));
  display.println(F("Diagonal"));
  display.setCursor(TX(24), TY(24));
  display.println(F("Scroll"));
  display.display();
  delay(1000);

  // Configure scroll for visible area only
  uint8_t start_page = Y_OFFSET / 8;       // 24px/8 = page 3
  uint8_t end_page = (Y_OFFSET + VISIBLE_HEIGHT) / 8;  // 64px/8 = page 7

  // Vertical-Horizontal diagonal scroll
  display.startscrolldiagright(start_page, end_page);
  delay(4000);
  
  // Opposite diagonal scroll
  display.startscrolldiagleft(start_page, end_page);
  delay(4000);
  
  display.stopscroll();
}

void testdrawbitmap(void) {
  display.clearDisplay();
  display.drawBitmap(
    GX((VISIBLE_WIDTH - LOGO_WIDTH)/2),
    GY((VISIBLE_HEIGHT - LOGO_HEIGHT)/2),
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  for(f=0; f<NUMFLAKES; f++) {
    icons[f][XPOS] = random(GX(0), GX(VISIBLE_WIDTH - LOGO_WIDTH));
    icons[f][YPOS] = GY(-LOGO_HEIGHT);
    icons[f][DELTAY] = random(1, 3);
  }

  for(;;) {
    display.clearDisplay();
    for(f=0; f<NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], 
                        bitmap, w, h, SSD1306_WHITE);
    }
    display.display();
    //delay(200);

    for(f=0; f<NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      if (icons[f][YPOS] >= GY(VISIBLE_HEIGHT)) {
        icons[f][XPOS] = random(GX(0), GX(VISIBLE_WIDTH - LOGO_WIDTH));
        icons[f][YPOS] = GY(-LOGO_HEIGHT);
        icons[f][DELTAY] = random(1, 3);
      }
    }
    checkUploadTrigger();
    //delay(200);
  }
}

void loop() {checkUploadTrigger();}