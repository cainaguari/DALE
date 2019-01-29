// falta transformar o Arduino em USB.MIDI definitivo e transformar os envios de notas necessário
// a programação do Encoder aqui presente é para o Traktor. A do Ableton Live é diferente e deve ser estudada se vai ser colocada ou não
//saber como vai funcionar a interação entre os módulos por conta da definição do Canal e das notas

#include <EnableInterrupt.h>
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define outputA 2 // pinos digitais 2 e 3 são os únicos que possuem os interruptores que podem ser acessados
#define outputB 3
   
int counter = 0;
int aState;
int aLastState;

//////////////////////////////////////////////////
const int buttonN = 5;
//i=index                i0 i1 i2  i3
int buttonPin[buttonN] = {4, 5, 6, 7, 8}; //o index 4 é o do encoder
int buttonPState[buttonN] = {0};
int buttonCState[buttonN] = {0};

int counterbutton0 = 0;
int counter0CState = {};
int counter0PState = {};

int counterbuttonCh = 0;

//int counterbutton1 = 0;
//int counter1CState = {};
//int counter1PState = {};

//////////////////////////////////////////////////////////////////
// Pinos dos lEDs
byte ledPin1 = 9;
byte ledPin2 = 10;
byte ledPin3 = 11;
byte ledPin4 = 12;

//////////////////////////////////////////////////////////////////////////////////
byte midiCh = 0; // *Canal midi a ser utilizado
//byte note = 36; // *Nota mais grave que sera utilizada
byte cc = 1; // *CC mais baixo que sera utilizado

int midiCChannel = 0;
int midiPChannel = 0;

void setup() {
  MIDI.begin();
  Serial.begin(115200);

  for (int i = 0; i < buttonN; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
  
///////////////////////////////////////////////////
//ativador do Interruptor para o encoder
  enableInterrupt(outputA, encoder, CHANGE);
  enableInterrupt(outputB, encoder, CHANGE);

  pinMode (outputA, INPUT_PULLUP);
  pinMode (outputB, INPUT_PULLUP);

  aLastState = digitalRead(outputA);

  /////////////////////////////////////////////////
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
}

void loop() {

  //controle dos botões
  for (int i = 0; i < buttonN; i++) {
    buttonCState[i] = digitalRead(buttonPin[i]);
  }

  for (int i = 0; i < buttonN; i++) {
    if (buttonPState[i] != buttonCState[i]) {

      if (buttonCState[i] == LOW) {
//            if(buttonCState[i] == LOW & i == 0) {
        
        if (i == 0) {

          counterbutton0++;

          if (counterbutton0 % 2 != 0) {
            digitalWrite(ledPin1, HIGH);
            digitalWrite(ledPin3, HIGH);
//            Serial.println (counterbutton0);
          }
          else {
            digitalWrite(ledPin1, LOW);
            digitalWrite(ledPin3, LOW);
          }
          if (counterbutton0 % 2 != 1) {
            digitalWrite(ledPin2, HIGH);
            digitalWrite(ledPin4, HIGH);
//            Serial.println (counterbutton0);
          }
          else {
            digitalWrite(ledPin2, LOW);
            digitalWrite(ledPin4, LOW);
          }
        }

        if (i == 1) {
          counterbuttonCh ++;

          if (counterbuttonCh = 1){
          midiCChannel = 2; 
          midiCh = midiCChannel;
          midiPChannel = midiCChannel;
          }
         else if (counterbuttonCh = 2){
            midiCChannel = 1;
            midiCh = midiCChannel;
            midiPChannel = midiCChannel;
//            counterbuttonCh = 0;
          }
           else if (counterbuttonCh = 3){
            counterbuttonCh = 0;
          }
        }
        
        if (i == 2) {
          counterbuttonCh = 0;
          midiCChannel = 1; 
          midiCh = midiCChannel;
          midiPChannel = midiCChannel;
        }
        

                MIDI.sendNoteOn(49 + i, 127, midiCh);
        buttonPState[i] = buttonCState[i];

      }
      else {
                MIDI.sendNoteOff(49 + i, 0, midiCh);
        buttonPState[i] = buttonCState[i];
      }
    }
}
}//fim do loop

////////////////////////////////////////////////////
//esta função é ativada somente quando o encoder é girado, interrompendo as outras atividades, o que dá maior capacidade de resposta ao encoder.

void encoder () {

  aState = digitalRead(outputA); // Lê o "estado" atual do Encoder
  // Se o estado previo e o estado atual do outputA são diferentes, significa que um pulso occorreu
  if (aState != aLastState) {
    // Se o estado da outputB esta diferente da OutputA signifca que o Encoder esta girando em sentido horário
    if (digitalRead(outputB) != aState) {
      //counter ++;
      
      MIDI.sendControlChange(1, 127, midiCh); //envio de sinal feito para funcionar no Traktor
//      MIDIUSB.flush(); //necessário para o USB.MIDI
      //      if (counter > 127) { // para contar de um em um pulso
      //        counter = 127;
      //      }
    } else {
      //counter --;
      
      MIDI.sendControlChange(1, 1, midiCh);
//      MIDIUSB.flush();
      //      if (counter < 0) {
      //        counter = 0;
      //      }
    }

  }
  aLastState = aState; //Atualiza o estado previo da outputA com o estado atual

}

///////////////////////////////////////////////////////////////////
//voids já preparados para USB.MIDI

//void noteOn(byte channel, byte pitch, byte velocity) {
//  MIDIEvent noteOn = {0x09, 0x90 | channel, pitch, velocity};
//  MIDIUSB.write(noteOn);
//}
//
//void noteOff(byte channel, byte pitch, byte velocity) {
//  MIDIEvent noteOff = {0x08, 0x80 | channel, pitch, velocity};
//  MIDIUSB.write(noteOff);
//}
//
//void controlChange(byte channel, byte control, byte value) {
//  MIDIEvent event = {0x0B, 0xB0 | channel, control, value};
//  MIDIUSB.write(event);
//}
