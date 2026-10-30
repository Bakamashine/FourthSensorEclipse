#include "burner.hpp"

#define ATTEMPTS 5

static constexpr int vent_delay = 1000 * 5;         // 5 sec
static constexpr int ignition_fp_delay = 1000 * 2;  // 2 sec
static constexpr int ignition_sp_delay = 1000 * 3;  // 3 sec
static constexpr int ignition_delay =
    ignition_fp_delay + ignition_sp_delay;  // 5 sec
static constexpr int pilot_delay = 1000 * 5;
static constexpr int pilot_valve_delay = 1000 * 3;      // pilot valve on time
static constexpr int pilot_valve_off_delay = 1000 * 2;  // pilot valve off time
static constexpr int burner_sensor_window = 1000 * 2;   // burner signal search
static constexpr int burner_sensor_step = burner_sensor_window / 5;

namespace Squib {
bool getSquibStatus() { return static_cast<bool>(digitalRead(SQUIB_LED_PIN)); }
// system halt squib burner _ just in case if system will be halted
// returns true when the system is halted and the outputs were forced safe
static bool shSB_JustInCase() {
  if (!Val::systemHalted) return false;
  digitalWrite(SQUIB_LED_PIN, LOW);
  digitalWrite(BURNER_LED_PIN, LOW);
  return true;
}

static void justRunSquib(bool st) {
	if (Squib::_squib_must_be_out) {
		digitalWrite(SQUIB_LED_PIN, false);
	} else {
		digitalWrite(SQUIB_LED_PIN, st);
	}
}

void run(bool st) {
  if (shSB_JustInCase()) return;
  digitalWrite(SQUIB_LED_PIN, st);
}

void runB(bool st) {
  if (shSB_JustInCase()) return;
  justRunSquib(st);
  digitalWrite(BURNER_LED_PIN, st);
}
};  // namespace Squib
namespace Burner {
bool checkUTemp(Sensors &sns) {
  if (Val::errorCode > 0) return false;
  return getIAverageTemp(sns) <= uTemp - hyst;
}

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
  if (squibDetected()) Val::errorCode = 302;
}

static void checkSquibPresent() {
  loop_bur_sens = true;
  if (squibDetected()) isBurnerReady = true;
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

void standard_runBurn() {
  if (!getReadySensors(Sens::all)) return;

//  bool uTemp_st = checkUTemp(Sens::all);
//  digitalWrite(BURNER_PIN, uTemp_st);
  Squib::runB(checkUTemp(Sens::all));
}

static void clearStatusLoop() {
  loop_vent_status = false;
  loop_ign_trans = false;
  loop_pil_val = false;
  loop_bur_sens = false;
}

static void runBurn(Sensors &sns) {
  if (suc_prep && Squib::getSquibStatus() &&
      Val::pipelineWithoutHalt(sns) == 0 && checkUTemp(sns)) {
    digitalWrite(BURNER_LED_PIN, HIGH);
  }
  // Squib::run(false);
}
void prepare() {
  if (Val::errorCode == 0 && !suc_prep) {
    for (unsigned int i = 0; i < ATTEMPTS; i++) {
      ventilation();
      checkNoSquib();
      // error
      if (Val::errorCode > 0) return;
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

    if (!isBurnerReady && Val::errorCode == 0) Val::errorCode = 301;
  }
}

}  // namespace Burner
