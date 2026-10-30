#pragma once
#include <avr/wdt.h>

#include "constants/pin.hpp"
#include "sensor.hpp"
#include "settings.hpp"
#include "ui/ui_class.hpp"
#include "validate.hpp"
#include "initialization.hpp"
#include "squib.hpp"

#define YES_OR_NO(value) value ? "yes" : "no"

/**
 *     Vent 5 sec
 *  |----------------|
 *                    Ign.Transf
 *
 *                   |-------------|
 *
 * Ventilation duration - 5 sec
 * Ignition Transformer - first phase - 2 sec, second phase - 3 sec
 *
 *
 */

namespace Burner {

void standard_runBurn();
bool checkUTemp(Sensors &sn);
inline bool isBurnerReady = false;

void runBurn(Sensors&);
void prepare();

}  // namespace Burner

