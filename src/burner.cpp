#include "burner.hpp"


namespace Burner {
bool checkUTemp(Sensors &sns) {
  if (Val::errorCode > 0) return false;
  return getIAverageTemp(sns) <= uTemp - hyst;
}

void standard_runBurn() {
  if (!getReadySensors(Sens::all)) return;
  Squib::runB(checkUTemp(Sens::all));
}

void runBurn(Sensors &sns) {
  if (suc_prep && Squib::getSquibStatus() &&
      Val::pipelineWithoutHalt(sns) == 0 && checkUTemp(sns)) {
    digitalWrite(BURNER_LED_PIN, HIGH);
  }
  // Squib::run(false);
}


}  // namespace Burner
