#include "application.h"
#include "IRSender.h"

// Heavily based on Ken Shirriff's IRRemote library:
// https://github.com/shirriff/Arduino-IRremote
//

// #define _BV(bit) (1 << (bit));


IRSender::IRSender(byte pin)
{
  _pin = pin;
}

// Set the PWM frequency. The selected pin determines which timer to use
// Not used since we don't use PWM with timer, digitalWrite is enough for 40kHz
void IRSender::setFrequency(int frequency)
{
/* Started version with PWM manually set freq. Abondoned for digitalWrite all the way through.
  uint8_t pwmval8 = SystemCoreClock / 2000 / (frequency);
  uint16_t pwmval16 = SystemCoreClock / 2000 / (frequency);

  #define PWM_FREQ frequency; //Set PWM Freq
  uint16_t TIM_ARR = (uint16_t)(24000000 / PWM_FREQ) - 1; //Calculate period
*/

  // Enables IR output.  The khz value controls the modulation frequency in kilohertz.
  // MAX frequency is 166khz.
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);

  // This is the time to wait with the IR LED on and off to make the frequency, in microseconds.
  // The - 3.0 at the end is because digitalWrite() takes about 3 microseconds. Info from:
  // https://github.com/eflynch/sparkcoreiremitter/blob/master/ir_emitter/ir_emitter.ino
  // burstWait = round(1.0 / frequency * 1000.0 / 2.0 - 3.0); //40kHz=>9,5us
  burstWait = round(1.0 / frequency * 1000.0 / 2.0 - 3.0); //Timing was not 100%, testing....

  // This is the total time of a period, in microseconds.
  // burstLength = round(1.0 / frequency * 1000.0); //40kHz=>25us
  burstLength = round(1.0 / frequency * 1000.0 + 1); //40kHz=>26us


}

// Send a byte (8 bits) over IR
void IRSender::sendIRByte(byte sendByte, int bitMarkLength, int zeroSpaceLength, int oneSpaceLength)
{
  for (int i=0; i<8 ; i++)
  {
    if (sendByte & 0x01)
    {
      mark(bitMarkLength);
      space(oneSpaceLength);
    }
    else
    {
      mark(bitMarkLength);
      space(zeroSpaceLength);
    }

    sendByte >>= 1;
  }
}

// The Carrier IR protocol has the bits in a reverse order (compared to the other heatpumps)
// See http://www.nrtm.org/index.php/2013/07/25/reverse-bits-in-a-byte/
byte IRSender::bitReverse(byte x)
{
  //          01010101  |         10101010
  x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
  //          00110011  |         11001100
  x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
  //          00001111  |         11110000
  x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
  return x;
}

// Send an IR 'mark' symbol, i.e. transmitter ON
void IRSender::mark(int markLength)
{
    noInterrupts();

    while (markLength > 0) {
        digitalWrite(_pin, HIGH); // this takes about 3 microseconds to happen
        delayMicroseconds(burstWait);
        digitalWrite(_pin, LOW); // this also takes about 3 microseconds
        delayMicroseconds(burstWait);

        markLength -= burstLength;
    }

    interrupts();

  // delayMicroseconds(markLength); //old
}

// Send an IR 'space' symbol, i.e. transmitter OFF
void IRSender::space(int spaceLength)
{
    // Sends an IR space (no output) for the specified number of microseconds.
    digitalWrite(_pin, LOW); // Takes about 3 microsecondes
    if (spaceLength > 3) {
        delayMicroseconds(spaceLength - 3);
    }

    // delayMicroseconds(spaceLength); // Used in old version (Arduino)
}
