#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

#define BUTTON_PIN 3
#define BUZZER_PIN 6

bool rectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
  return !(x1 + w1 < x2 || x1 > x2 + w2 || y1 + h1 < y2 || y1 > y2 + h2);
}

float PillarX3 = 260.0;
bool pillar3Active = false;

bool isJumping = false;
bool gameOver = false;
int PlayerY = 40;
int velocity = 0;
const int gravity = 2;
const int groundY = 40;

float PillarX1 = 128.0;
float Pillarx2 = 180.0;
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
  pinMode(BUZZER_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1)
      ;
  }

  display.clearDisplay();
  display.display();
  randomSeed(analogRead(A0));

  EEPROM.get(0, highScore);
  if (highScore > 99999) highScore = 0;
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastFrame < 50) return;
  lastFrame = currentTime;

  int buttonState = digitalRead(BUTTON_PIN);

  // Game over
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
  if (buttonState == LOW && !buttonPressed && PlayerY == groundY) {
    buttonPressed = true;
    isJumping = true;
    velocity = -13;
  }
  if (buttonState == HIGH) {
    buttonPressed = false;
  }

  // Physics
  if (isJumping) {
    PlayerY += velocity;
    velocity += gravity;
    if (PlayerY >= groundY) {
      PlayerY = groundY;
      isJumping = false;
    }
  }

  PillarX1 -= gameSpeed;
  Pillarx2 -= gameSpeed;

  if (pillar3Active) {
    PillarX3 -= gameSpeed;
  }

  if (PillarX1 < -10) {
    PillarX1 = 128 + random(30, 100);

    if (random(0, 100) < 40) {  // 40% chans
      pillar3Active = true;
      PillarX3 = PillarX1 + random(40, 120);
    }
  }

  if (Pillarx2 < -10) {
    Pillarx2 = 128 + random(15, 60);
  }

  if (pillar3Active && PillarX3 < -10) {
    pillar3Active = false;
  }

  if (currentTime - lastSpeedIncrease > 2000) {
    gameSpeed += 0.2;
    lastSpeedIncrease = currentTime;
  }

  // Score
  score++;

  // Display
  display.clearDisplay();
  display.fillRoundRect(10, PlayerY, 8, 14, 4, SSD1306_WHITE);
  display.fillTriangle((int)PillarX1, groundY + 17, (int)PillarX1 + 5, groundY, (int)PillarX1 + 10, groundY + 17, SSD1306_WHITE);
  display.fillTriangle((int)Pillarx2, groundY + 17, (int)Pillarx2 + 5, groundY, (int)Pillarx2 + 10, groundY + 17, SSD1306_WHITE);
  if (pillar3Active) {
    display.fillTriangle(
      (int)PillarX3, groundY + 17,
      (int)PillarX3 + 5, groundY,
      (int)PillarX3 + 10, groundY + 17,
      SSD1306_WHITE);
  }

  display.drawLine(0, 58, 128, 58, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score / 10);
  display.display();

  // COLLISION DETECTION
  bool Pillar1 = rectCollision(10, PlayerY, 8, 14, (int)PillarX1, groundY, 10, 15);
  bool Pillar2 = rectCollision(10, PlayerY, 8, 14, (int)Pillarx2, groundY, 10, 15);
  bool Pillar3 = false;

  if (pillar3Active) {
    Pillar3 = rectCollision(
      10, PlayerY, 8, 14,
      (int)PillarX3, groundY, 10, 15);
  }
  if (Pillar1 || Pillar2 || Pillar3) {
    gameOver = true;

    // High score Save
    if (score > highScore) {
      highScore = score;
      EEPROM.put(0, highScore);
    }

    //Game Over
    SweepEffect(10);
    if (score > highScore) {
      WinMelody();
    }
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
  PlayerY = groundY;
  velocity = 0;
  PillarX1 = 128.0;
  Pillarx2 = 180.0;
  gameSpeed = 3.0;
  score = 0;
  gameOver = false;
  lastSpeedIncrease = millis();
  lastFrame = millis();
  pillar3Active = false;
  PillarX3 = 260.0;
  display.clearDisplay();
  display.display();
}

void SweepEffect(int speed) {
  for (int i = 0; i < display.width(); i += speed) {
    display.fillRect(i, 0, speed, display.height(), WHITE);
    display.display();
    delay(10);
  }

  for (int i = 0; i < display.width(); i += speed) {
    display.fillRect(i, 0, speed, display.height(), BLACK);
    display.display();
    delay(10);
  }
}

void WinMelody() {
  tone(BUZZER_PIN, 988, 120);
  delay(150);
  tone(BUZZER_PIN, 1175, 120);
  delay(150);
  tone(BUZZER_PIN, 1568, 180);
  delay(200);
  tone(BUZZER_PIN, 2093, 300);
  delay(350);
  noTone(BUZZER_PIN);
}
