#pragma once

#include <stdbool.h>

#include "pg/pg.h"

// Защита от принудительного дизарма (аналог TBF anti-disarm).
// Дизарм по ARM-тумблеру разрешён только когда выполнены условия:
//   - газ ниже disarm_guard_throttle (проценты от полного хода), И
//   - высота ниже disarm_guard_alt_m метров.
// Если оценка высоты равна 0 (нет баро/GPS) — условие по высоте игнорируется.
// disarm_guard = OFF полностью отключает защиту (штатное поведение).
typedef struct antiDisarmConfig_s {
    uint8_t enabled;         // OFF/ON
    uint8_t altM;            // 0..100, метры; порог по высоте
    uint8_t throttlePercent; // 0..100, проценты газа
} antiDisarmConfig_t;

PG_DECLARE(antiDisarmConfig_t, antiDisarmConfig);

/** true — дизарм, инициированный пилотом (тумблер/стики), разрешён. */
bool antiDisarmDisarmAllowed(void);
