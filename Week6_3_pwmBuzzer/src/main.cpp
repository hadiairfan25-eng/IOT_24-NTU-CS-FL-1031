
#include <Arduino.h>

#define BUZZER_PIN 18
#define PWM_CHANNEL 0
#define PWM_RESOLUTION 8
#define PWM_INITIAL_FREQ 1000

void playTone(int frequency, int duration) {
  ledcWriteTone(PWM_CHANNEL, frequency);
  delay(duration);

  ledcWriteTone(PWM_CHANNEL, 0);
  delay(100);
}

void toneSequence() {
  playTone(300, 500);
  playTone(600, 500);
  playTone(1000, 500);
  playTone(1500, 500);
  playTone(2000, 500);
}

void alarmSound() {
  for (int i = 0; i < 5; i++) {
    playTone(1000, 300);
    playTone(2000, 300);
  }
}

void doorbell() {
  playTone(1000, 300);
  playTone(1500, 600);
  delay(300);
  playTone(1000, 300);
  playTone(1500, 600);
}

void shortBeeps() {
  for (int i = 0; i < 5; i++) {
    playTone(2000, 100);
    delay(100);
  }
}

void melody() {
  playTone(523, 300);
  playTone(587, 300);
  playTone(659, 300);
  playTone(698, 300);
  playTone(784, 300);
  playTone(880, 300);
  playTone(988, 300);
  playTone(1047, 600);
}

void setup() {
  Serial.begin(115200);

  ledcSetup(PWM_CHANNEL, PWM_INITIAL_FREQ, PWM_RESOLUTION);
  ledcAttachPin(BUZZER_PIN, PWM_CHANNEL);

  Serial.println("PWM Buzzer Ready!");
}

void loop() {
  Serial.println("1. Rising Tone Sequence");
  toneSequence();
  delay(500);

  Serial.println("2. Alarm");
  alarmSound();
  delay(500);

  Serial.println("3. Doorbell");
  doorbell();
  delay(500);

  Serial.println("4. Short Beeps");
  shortBeeps();
  delay(500);

  Serial.println("5. Melody");
  melody();
  delay(2000);
}
