/*
 * squib.cc
 *
 *  Created on: Oct 30, 2026
 *      Author: User
 */

#include "squib.hpp"

namespace Squib {
bool getSquibStatus() {
	return static_cast<bool>(digitalRead(SQUIB_LED_PIN));
}

/*
 * The squib must remain connected at all times.
 * The only exception is in the vent of an accident
 */
void run() {
	if (!getSquibStatus()) {
		digitalWrite(SQUIB_LED_PIN, HIGH);
	}
}

void off() {
	digitalWrite(SQUIB_LED_PIN, LOW);
}

// run squib with the burner
void runB() {
	Squib::run();
	Burner::manageBurner(Sens::all);
}

};
// namespace Squib
