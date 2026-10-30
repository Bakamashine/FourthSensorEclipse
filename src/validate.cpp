#include "validate.hpp"

#include "burner.hpp"
#include "constants/pin.hpp"
#include "helper.hpp"
#include "page.hpp"
#include "sensor.hpp"
#include "settings.hpp"
#define PERMITTED_TEMP_DIFFERENCE 5

namespace Val {


namespace check {
static int temp(Sensors &sns) {
#ifdef ENABLE_VALIDATE
  float averageVal = getFAverageTemp(sns);

  // the bounds themselves are valid readings, so compare strictly
  if (maxPermOffset - hyst < averageVal) {
    return 201;  // average too hot
  }
  if (minPermOffset + hyst > averageVal) {
    return 202;  // average too cold
  }
#endif

  return 0;  // no error
}
void error() {
  if (errorCode > 0) {
    return haltSystem();
  }
}
static int checkSensors(Sensors &sn) {
  /*
  codes:
  100 - all sensors agree
  101 - first sensor is defective
  102 - second sensor is defective
  103 - third sensor is defective
  104 - fourth sensor is defective

  */

	// not implemented
  return 0;  // no error
}
static int squib() {
	if (!Squib::getSquibStatus()) return 303;
	return 0;
//	if (Burner::checkUTemp(Sens::all)) {
//		if (!Squib::getSquibStatus()) {
//			return 303;
//		}
//	}
//	return 0;
}
} // namespace check



static int basePipeline(Sensors &sns) {
  if (!isSensorsReady(sns) || errorCode > 0) return 0;
  int squib_code = Val::check::squib();
  int sensor_code = Val::check::checkSensors(sns);
  int temp_code = Val::check::temp(sns);


  if (squib_code> 0) return squib_code;
  else if (sensor_code > 0) return sensor_code;
  else if (temp_code > 0) return temp_code;

  return 0; // no error
}

void pipeline(Sensors &sns) {
  int code = basePipeline(sns);
  if (code > 0) {
    errorCode = code;
    haltSystem();
  }
}

int pipelineWithoutHalt(Sensors &sns) { return basePipeline(sns); }

void haltSystem() {
  // always re-assert the safe state: a late write (e.g. from the squib
  // button ISR) must not leave an output energized after a halt
  digitalWrite(BURNER_LED_PIN, LOW);
#ifdef BURNER_PREPARE
  digitalWrite(VENT_LED_PIN, LOW); // ventilation led
  digitalWrite(IGN_PIN, LOW); // ignition transformer led
  digitalWrite(PILOT_PIN, LOW); // pilot valve
  digitalWrite(CHECK_SQUIB_LED_PIN, LOW); // burner led
  digitalWrite(SQUIB_LED_PIN, LOW);  // squib led
#endif
  digitalWrite(ERROR_LED_PIN, HIGH);
  if (!systemHalted) {
    Serial.print("ERROR: ");
    Serial.println(errorCode);
  }
  systemHalted = 1;
  currentPage = ERROR;
}


}  // namespace Val
