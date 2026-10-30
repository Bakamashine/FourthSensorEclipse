/*
 * intilization.cc
 *
 *  Created on: Oct 30, 2026
 *      Author: User
 */

#include "initialization.hpp"

using namespace Burner;
using namespace Squib;
namespace Init {
//static bool shSB_JustInCase() {
//  if (!Val::systemHalted) return false;
//  digitalWrite(SQUIB_LED_PIN, LOW);
//  digitalWrite(BURNER_LED_PIN, LOW);
//  return true;
//}
static void changeStatus(bool &p_st, int pin, bool *st = nullptr) {
	if (st == nullptr) {
		p_st = !(p_st);
	} else {
		p_st = *st;
	}
	digitalWrite(pin, p_st);
}

static void burnerDelay(int ms) {
	for (int elapsed = 0; elapsed < ms; elapsed += 250) {
		wdt_reset();
		// UI::draw();
		ui_cls.draw();
		delay(250);
	}
}

static bool squibDetected() {
	changeStatus(_bur_sens, CHECK_SQUIB_LED_PIN);
	bool flameSeen = false;
	for (int i = 0; i < 5; i++) {
		burnerDelay(burner_sensor_step);
		if (Squib::getSquibStatus()) {
			flameSeen = true;
			break;
		}
	}
	changeStatus(_bur_sens, CHECK_SQUIB_LED_PIN);
	return flameSeen;
}

static void checkNoSquib() {
	if (squibDetected())
		Val::errorCode = 302;
}

static void checkSquibPresent() {
	loop_bur_sens = true;
	if (squibDetected())
		isBurnerReady = true;
}

// blue led
static void ventilation() {
	loop_vent_status = true;
	changeStatus(_vent_status, VENT_LED_PIN);
	burnerDelay(vent_delay);
	changeStatus(_vent_status, VENT_LED_PIN);
}

// yellow led
static void pilotValve() {
	loop_pil_val = true;
	changeStatus(_pil_val, PILOT_PIN);
	burnerDelay(pilot_valve_delay);
	changeStatus(_ign_trans, IGN_PIN);
	burnerDelay(pilot_valve_off_delay);
	changeStatus(_pil_val, PILOT_PIN);
}

// white led
static void ignitionTransformer() {
	loop_ign_trans = true;
	changeStatus(_ign_trans, IGN_PIN);
	burnerDelay(ignition_fp_delay);
	pilotValve();
}

static void clearStatusLoop() {
	loop_vent_status = false;
	loop_ign_trans = false;
	loop_pil_val = false;
	loop_bur_sens = false;
}

void prepare() {
	if (Val::errorCode == 0 && !suc_prep) {
		for (unsigned int i = 0; i < ATTEMPTS; i++) {
			ventilation();
			checkNoSquib();
			// error
			if (Val::errorCode > 0)
				return;
			ignitionTransformer();
			checkSquibPresent();

			// success
			if (isBurnerReady && Val::errorCode == 0) {
				// UI::closeStartPage();
				ui_cls.closeStartPage();
				suc_prep = true;
				runBurn(Sens::all);
				break;
			}

			clearStatusLoop();
		}

		if (!isBurnerReady && Val::errorCode == 0)
			Val::errorCode = 301;
	}
}
}
