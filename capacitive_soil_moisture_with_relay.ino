const int soilMoisturePin = 27; // Pin analog untuk sensor kelembapan tanah
// Relay control pin
const int relayPin = 26;
int Treshold1 = 30;
int Treshold2 = 60;
int Treshold3 = 90;

void setup() {
// Initialize the relay pin as an output
Serial.begin(115200);
pinMode(relayPin, OUTPUT);
}

// Fungsi loop
void loop() {

// Baca nilai kelembapan tanah
int moistureValue = analogRead(soilMoisturePin);

// Konversi nilai ADC (0-4095) menjadi persen (0-100%)
int moisturePercent = map(moistureValue, 900, 2500, 100, 0);

// Cetak nilai kelembapan tanah ke serial monitor
Serial.print("Soil Moisture: ");
Serial.print(moisturePercent);
Serial.println("%");
Serial.println(moistureValue);


if (moisturePercent <= Treshold1){
  digitalWrite(relayPin, HIGH);
    delay(10000); // Wait for 10 second
// Turn the relay off
    digitalWrite(relayPin, LOW);
  }
  else if (moisturePercent > Treshold1 && moisturePercent <= Treshold2){
      digitalWrite(relayPin, HIGH);
      delay(7000); // Wait for 7 second
// Turn the relay off
      digitalWrite(relayPin, LOW);
  }
  else if (moisturePercent > Treshold2 && moisturePercent <= Treshold3){
      digitalWrite(relayPin, HIGH);
      delay(3000); // Wait for 7 second
// Turn the relay off
      digitalWrite(relayPin, LOW);
  }
  else if (moisturePercent > Treshold3){
  digitalWrite(relayPin, LOW);
}
delay(5000);
}