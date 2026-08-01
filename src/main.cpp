#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_SDA 8
#define I2C_SCL 9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum class PlayerState {
  STOPPED,
  PLAYING,
  PAUSED
};
PlayerState currentState = PlayerState::STOPPED;

String playlist[] = {
  "Bohemian Rhapsody",
  "Stairway to Heaven",
  "Hotel California",
  "Comfortably Numb"
};

const int totalSongs = 4;
int currentSong = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Hello, ESP32-S3 alive!");

  Wire.begin(I2C_SDA, I2C_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error: OLED display not detected");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("OLED display initialized correctly");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Hola Josue!");
  display.display();
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "play") {
      currentState = PlayerState::PLAYING;
      Serial.println("Playing: " + playlist[currentSong]);

    } else if (command == "pause") {
      currentState = PlayerState::PAUSED;
      Serial.println("Song paused");

    } else if (command == "stop") {
      currentState = PlayerState::STOPPED;
      Serial.println("Stopped");

    } else if (command == "next") {
      currentSong = (currentSong + 1) % totalSongs;
      Serial.println("Next: " + playlist[currentSong]);

    } else if (command == "prev") {
      currentSong = (currentSong - 1 + totalSongs) % totalSongs;
      Serial.println("Prev: " + playlist[currentSong]);

    } else {
      Serial.println("Unknown command: " + command);
    }
  }
}