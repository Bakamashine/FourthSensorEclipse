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

// run squib with the burner
void runB(bool st) {
	Squib::run();
	digitalWrite(BURNER_LED_PIN, st);
}
}
;
// namespace Squib
