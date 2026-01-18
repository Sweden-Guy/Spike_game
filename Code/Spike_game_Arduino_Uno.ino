#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

#define BUTTON_PIN 3

bool rectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
  return !(x1 + w1 < x2 || x1 > x2 + w2 || y1 + h1 < y2 || y1 > y2 + h2);
}

bool isJumping = false;
bool gameOver = false;
int dinoY = 40;
int velocity = 0;
const int gravity = 2;
const int groundY = 40;
float cactusX1 = 128.0;
float cactusX2 = 180.0;
float gameSpeed = 3.0;
unsigned long lastSpeedIncrease = 0;
unsigned long lastFrame = 0;
unsigned long score = 0;
unsigned long highScore = 0;

bool buttonPressed = false;

// OLED-display (128x64)
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Wire.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  display.clearDisplay();
  display.display();
  randomSeed(analogRead(0));

  EEPROM.get(0, highScore);
  if (highScore > 99999) highScore = 0;

  lastSpeedIncrease = millis();
  lastFrame = millis();
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastFrame < 50) return;
  lastFrame = currentTime;

  int buttonState = digitalRead(BUTTON_PIN);

  if (gameOver) {
    if (buttonState == HIGH && !buttonPressed) {
      buttonPressed = true;
    }
    if (buttonState == LOW && buttonPressed) {
      buttonPressed = false;
      SweepEffect(10);
      resetGame();
    }
    return;
  }

  // Jump
  if (buttonState == LOW && !buttonPressed && dinoY == groundY) {
    buttonPressed = true;
    isJumping = true;
    velocity = -13;
  }
  if (buttonState == HIGH) {
    buttonPressed = false;
  }

  // Physics
  if (isJumping) {
    dinoY += velocity;
    velocity += gravity;
    if (dinoY >= groundY) {
      dinoY = groundY;
      isJumping = false;
    }
  }

  // Moving spikes 
  cactusX1 -= gameSpeed;
  cactusX2 -= gameSpeed;

  if (cactusX1 < -10) {
    cactusX1 = 128 + random(10, 40);
  }
  if (cactusX2 < -10) {
    cactusX2 = 128 + random(10, 40);
  }

  // Speed increase var 5:e sekund, ökar med 0.2
  if (currentTime - lastSpeedIncrease > 5000) {
    gameSpeed += 0.2;
    lastSpeedIncrease = currentTime;
  }

  // Score
  score++;

  // Display
  display.clearDisplay();
  display.fillRoundRect(10, dinoY, 8, 14, 4, SSD1306_WHITE);
  display.fillTriangle((int)cactusX1, groundY + 17, (int)cactusX1 + 5, groundY, (int)cactusX1 + 10, groundY + 17, SSD1306_WHITE);
  display.fillTriangle((int)cactusX2, groundY + 17, (int)cactusX2 + 5, groundY, (int)cactusX2 + 10, groundY + 17, SSD1306_WHITE);
  display.drawLine(0, 58, 128, 58, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score / 10);
  display.display();

  // COLLISION DETECTION 
  bool hitCactus1 = rectCollision(10, dinoY, 8, 14, (int)cactusX1, groundY, 10, 15);
  bool hitCactus2 = rectCollision(10, dinoY, 8, 14, (int)cactusX2, groundY, 10, 15);

  if (hitCactus1 || hitCactus2) {
    gameOver = true;

    // High score save
    if (score > highScore) {
      highScore = score;
      EEPROM.put(0, highScore);
    }

    // Game Over display
    SweepEffect(10);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(43, 0);
    display.print("Score:");
    display.print(score / 10);
    display.setCursor(26, 40);
    display.print("High Score:");
    display.print(highScore / 10);
    display.setTextSize(2);
    display.setCursor(12, 15);
    display.print("Game Over");
    display.setTextSize(1);
    display.setCursor(15, 55);
    display.print("Press to restart");
    display.display();
  }
}

void resetGame() {
  isJumping = false;
  dinoY = groundY;
  velocity = 0;
  cactusX1 = 128.0;
  cactusX2 = 180.0;
  gameSpeed = 3.0;
  score = 0;
  gameOver = false;
  lastSpeedIncrease = millis();
  lastFrame = millis();
  display.clearDisplay();
  display.display();
}
void SweepEffect(int speed) {

 
  for (int i = 0; i < display.width(); i += speed) {
    display.fillRect(i, 0, speed, display.height(), WHITE);
    display.display();
  }

  
  for (int i = 0; i < display.width(); i += speed) {
    display.fillRect(i, 0, speed, display.height(), BLACK);
    display.display();
  }
}
