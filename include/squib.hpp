/*
 * squib.hpp
 *
 *  Created on: Oct 30, 2026
 *      Author: User
 */

#ifndef SQUIB_HPP_
#define SQUIB_HPP_

#include <Arduino.h>
#include "constants/pin.hpp"
#include "burner.hpp"

namespace Squib {
bool getSquibStatus();
void run();

// running the squib with burner
void runB();
/*
 * Only for debug
 */
volatile inline bool _squib_must_be_out = false;

/*
 * start squib
 */
void run();
void off();
};  // namespace Squib



#endif /* SQUIB_HPP_ */
