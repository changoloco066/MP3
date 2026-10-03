#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_SDA 8
#define I2C_SCL 9

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

void showOnScreen(String line1, String line2) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  display.setCursor(0, 0);
  display.print(line1);

  display.setCursor(0, 16);
  display.print(line2);

  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Hello, ESP32-S3 alive!");

  Wire.begin(I2C_SDA, I2C_SCL);

  if (!display.begin(0x3C, true)) {
    Serial.println("Error: OLED display not detected");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("OLED display initialized correctly");
  showOnScreen("MP3 Player", "Ready");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "play") {
      currentState = PlayerState::PLAYING;
      Serial.println("Playing: " + playlist[currentSong]);
      showOnScreen("Playing:", playlist[currentSong]);

    } else if (command == "pause") {
      currentState = PlayerState::PAUSED;
      Serial.println("Song paused");
      showOnScreen("Song paused", playlist[currentSong]);

    } else if (command == "stop") {
      currentState = PlayerState::STOPPED;
      Serial.println("Stopped");
      showOnScreen("Stopped", playlist[currentSong]);

    } else if (command == "next") {
      currentSong = (currentSong + 1) % totalSongs;
      Serial.println("Next: " + playlist[currentSong]);
      showOnScreen("Next", playlist[currentSong]);

    } else if (command == "prev") {
      currentSong = (currentSong - 1 + totalSongs) % totalSongs;
      Serial.println("Prev: " + playlist[currentSong]);
      showOnScreen("Prev", playlist[currentSong]);

    } else {
      Serial.println("Unknown command: " + command);
    }
  }
}