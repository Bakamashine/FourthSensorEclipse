#include "ui/menu_items.hpp"

#include "constants/settings.hpp"
#include "constants/ui.hpp"
#include "contest.hpp"
#include "settings.hpp"

void writeFunc(void *value, int delta, MenuItem &it) {
  if (!value) return;
  const int dir = (delta > 0) ? +1 : -1;
  switch (it.kind) {
    case INT: {
      auto *val = static_cast<int *>(value);
      const int next = *val + dir * static_cast<int>(it.step);
      if (next < static_cast<int>(it.min) || next > static_cast<int>(it.max))
        return;
      *val = next;
      if (it.save_func != nullptr) it.save_func(*val);
      break;
    }
    case FLOAT: {
      auto *val = static_cast<float *>(value);
      const float next = *val + static_cast<float>(dir) * it.step;
      if (next < it.min || next > it.max) return;
      *val = next;
      break;
    }
    default:
      return;
  }
}

void buildItems(Sensors &sensors) {
  (void)sensors;
  int count = 0;

  _items[count].row = U8G2_FIRST_ROW;
  _items[count].preview = PREVIEW_HYST;
  _items[count].target = &hyst;
  _items[count].kind = INT;
  _items[count].step = 1;
  _items[count].save_func = _EEPROM::writeHyst;
  _items[count].min = MIN_HYSTERESIS;
  _items[count].max = MAX_HYSTERESIS;
  count++;

#ifdef ENABLE_VALIDATE
  _items[count].row = U8G2_SECOND_ROW;
  _items[count].preview = PREVIEW_MIN_TEMP;
  _items[count].target = &minPermOffset;
  _items[count].kind = INT;
  _items[count].step = 1;
  _items[count].save_func =  _EEPROM::writeMinPermOffset;
  _items[count].min = MIN_PERM_OFFSET;
  _items[count].max = static_cast<float>(maxPermOffset) - 1;
  count++;

  _items[count].row = U8G2_THIRD_ROW;
  _items[count].preview = PREVIEW_MAX_TEMP;
  _items[count].target = &maxPermOffset;
  _items[count].kind = INT;
  _items[count].step = 1;
  _items[count].save_func = _EEPROM::writeMaxPermOffset;
  _items[count].min = static_cast<float>(minPermOffset) + 1;
  _items[count].max = MAX_PERM_OFFSET;
  count++;

  _items[count].row = U8G2_FOURTH_ROW;
  _items[count].preview = PREVIEW_USER_TEMP;
  _items[count].target = &uTemp;
  _items[count].kind = INT;
  _items[count].step = 1;
  _items[count].save_func = _EEPROM::writeUserTemp;
  _items[count].min = static_cast<float>(minUserTemp());
  _items[count].max = static_cast<float>(maxUserTemp());
  count++;
#endif

  _itemsCount = count;
}
