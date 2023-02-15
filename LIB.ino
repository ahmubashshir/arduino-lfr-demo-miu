/*
 * LIB.ino
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

/* d:upload
 * @fqbn arduino:avr:uno
 * @port /dev/ttyACM0
 * d@compile:build-property build.extra_flags=-DDEBUG -DSLOW_AF
 * @compile:build-property build.extra_flags=-DSLOW
 */

#if SLOW_AF
#   define SPEED_RATIO_BASE 12
#elif SLOWER
#   define SPEED_RATIO_BASE 18
#elif SLOW
#   define SPEED_RATIO_BASE 24
#else
#   define SPEED_RATIO_BASE 30
#endif

void setPinArrayInput(size_t num, PIN pins[]) {
	if(num <= 8)
		while(num--) pinMode(pins[num], INPUT);
}

void setupMotor(const MPin &motor) {
  pinMode(motor.LP, OUTPUT);
  pinMode(motor.RP, OUTPUT);
}

inline void __setMotor(const MPin &motor, uint8_t left, uint8_t right) {
  analogWrite(motor.LP, left);
  analogWrite(motor.RP, right);
}

void motorForward(MPin const& motor, uint8_t speed) {
  __setMotor(motor, SPEED_RATIO_BASE * (speed & 7), 0);
}

void motorReverse(MPin const& motor, uint8_t speed) {
  __setMotor(motor, 0, SPEED_RATIO_BASE * (speed & 7));
}

void motorStop(MPin const& motor) {
  __setMotor(motor, 0, 0);
}

uint8_t readPinAsBitfield(size_t num, PIN pins[]) {
	uint8_t res = 0;
	uint8_t pos = 1;
	if(num <= 8)
		while(num --)
			res += digitalRead(pins[num]) * pos, pos *= 2;
	return res;
}
// bitfield functions
inline uint8_t normal3lsb(uint8_t val) {
  return (val & 0b111);
}

inline uint8_t reverse3msb(uint8_t val) {
  return (
      (val >> 4) // first bit comes last 0b11111
    | ((val >> 2) & 0b010) // second bit keeps it place
    | (((val >> 2) & 0b001) << 2) // last bit moves to first place
  );
}

inline uint8_t  bitfieldOr(uint8_t val) {
  return (
      ((val >> 0) & 1)
    | ((val >> 1) & 1)
    | ((val >> 2) & 1)
    | ((val >> 3) & 1)
    | ((val >> 4) & 1)
    | ((val >> 5) & 1)
    | ((val >> 6) & 1)
    | ((val >> 7) & 1)
  );
}

// vim: ft=arduino:ts=2:et
