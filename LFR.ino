/*
 * LFR.ino
 * This file is part of Line Follower Demo, MIU
 *
 * Copyright (C) 2023 - Mubashshir <ahmubashshir@gmail.com>
 *
 * Line Follower Demo, MIU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Line Follower Demo, MIU is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Line Follower Demo, MIU. If not, see <http://www.gnu.org/licenses/>.
 */

/* @fqbn arduino:avr:uno
 * @port /dev/ttyACM0
 * @compile:build-property build.extra_flags=-DSLOWER
 */

// Motor Definition
typedef const struct _MPin {
  uint8_t LP;
  uint8_t RP;
} MPin;
// pinArray typedef
typedef uint8_t PIN;

// Function declaration
void	  setupMotor  (const MPin&);
void	  motorForward(const MPin&, uint8_t);
void	  motorReverse(const MPin&, uint8_t);
void	  motorStop   (const MPin&);
void 	  setPinArrayInput  (size_t,  PIN[]);
uint8_t	readPinAsBitfield (size_t, PIN[]);
#define dumpbits(x) \
        Serial.print((x >> 2) & 1); \
        Serial.print((x >> 1) & 1); \
        Serial.print((x >> 0) & 1);

// bitmask functions declared inline for performance
inline uint8_t reverse3msb(uint8_t); // Get 3 most significant bits in reverse order
inline uint8_t  normal3lsb(uint8_t); // Get 3 lest significant bits in normal  order
inline uint8_t  bitfieldOr(uint8_t); // return 1 if any bit in field is 1

#define DELAY 1000
MPin LM = {.LP = 10, .RP = 9};
MPin RM = {.LP = 5 , .RP = 6};

uint32_t val = 0, steps = 0;
PIN ArrIR[] = {A4, A3, A2, A1, A0}; // sensor pins, in reverse order

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
  setPinArrayInput(5, ArrIR);
  setupMotor(LM);
  setupMotor(RM);
}

void loop() {
  delay(DELAY / 10);

  val = readPinAsBitfield(5, ArrIR);
  uint8_t bits[] = {
    reverse3msb(val),
    normal3lsb(val)
  };
#ifdef DEBUG
  dumpbits(bits[0]);
  Serial.print("\t");
  dumpbits(bits[1]);
  Serial.print("\t");
  Serial.print(bitfieldOr(val));
  Serial.print("\n");
#else
  if(bitfieldOr(val) == 0) {
    if(steps++ < 5) {
      motorForward(LM, 1 << 1);
      motorForward(RM, 1 << 1);
    } else {
      motorStop(LM);
      motorStop(RM);
    }
    delay(DELAY / 10);
  } else {
    steps = 0;
    if(bits[0] == bits[1]) {
      motorForward(LM, bits[0]);
      motorForward(RM, bits[1]);
    } else if(bits[0] < bits[1]) {
      motorForward(LM, bits[1]);
      motorReverse(RM, bits[0]);
    } else {
      motorReverse(LM, bits[1]);
      motorForward(RM, bits[0]);
    }
    delay(DELAY / 10);
  }
#endif
}

// vim: ft=arduino:ts=2:et
