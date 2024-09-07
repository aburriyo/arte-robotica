#include <MIDIUSB.h>

// Definir pines digitales para los botones (pines del 5 al 14)
const int buttonPins[10] = {5, 6, 7, 8, 9, 10, 12, 14, 15, 16};

// Definir las notas MIDI para cada botón
const uint8_t notes[10] = {60, 62, 64, 65, 67, 69, 71, 72, 74, 76}; // C4, D4, E4, F4, G4, A4, B4, C5, D5, E5

// Variables para el valor y el último estado del botón
int buttonStates[10] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
int lastButtonStates[10] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

void setup() {
  // Configurar los pines de los botones como entradas con resistencia pull-up
  for (int i = 0; i < 10; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Inicializar comunicación serial
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 10; i++) {
    // Leer el valor del botón
    buttonStates[i] = digitalRead(buttonPins[i]);

    // Detectar el cambio en el estado del botón (transición de presionado a no presionado)
    if (buttonStates[i] == LOW && lastButtonStates[i] == HIGH) {
      // Botón presionado, enviar mensaje MIDI Note On
      midiEventPacket_t noteOn = {0x09, 0x90 | 0, notes[i], 64}; // Nota especificada en notes[i], canal 1
      MidiUSB.sendMIDI(noteOn);
      MidiUSB.flush();
      
      Serial.print("Botón ");
      Serial.print(i + 1);
      Serial.print(" presionado - Nota On: ");
      Serial.println(notes[i]);

      // Esperar un poco para evitar múltiples envíos
      delay(1); // Ajusta el tiempo según sea necesario
    } else if (buttonStates[i] == HIGH && lastButtonStates[i] == LOW) {
      // Botón liberado, enviar mensaje MIDI Note Off
      midiEventPacket_t noteOff = {0x09, 0x80 | 0, notes[i], 0}; // Nota especificada en notes[i], canal 1
      MidiUSB.sendMIDI(noteOff);
      MidiUSB.flush();

      Serial.print("Botón ");
      Serial.print(i + 1);
      Serial.print(" liberado - Nota Off: ");
      Serial.println(notes[i]);
      
      // Esperar un poco para evitar múltiples envíos
      delay(1); // Ajusta el tiempo según sea necesario
    }

    // Actualizar el estado del botón para la próxima iteración
    lastButtonStates[i] = buttonStates[i];
  }

  delay(1); // Ajusta el tiempo de muestreo según sea necesario
}
