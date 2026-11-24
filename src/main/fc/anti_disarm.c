#include "fc/anti_disarm.h"

#include "common/maths.h"     // constrainf
#include "fc/rc.h"            // rcCommand
#include "pg/pg_ids.h"
#include "sensors/altitude.h" // getEstimatedAltitudeCm

PG_REGISTER_WITH_RESET_TEMPLATE(antiDisarmConfig_t, antiDisarmConfig, PG_ANTI_DISARM_CONFIG, 0);

PG_RESET_TEMPLATE(antiDisarmConfig_t, antiDisarmConfig,
    .enabled = 0,        // по умолчанию выключено
    .altM = 3,           // требование: ALT = 3 м
    .throttlePercent = 3 // требование: газ 3 %
);

bool antiDisarmDisarmAllowed(void)
{
    const antiDisarmConfig_t *cfg = antiDisarmConfig();
    if (!cfg->enabled) {
        return true;
    }

    // В этом форке rcCommand[THROTTLE] — float в интервале [0;1]
    const float throttlePercent = constrainf(rcCommand[THROTTLE] * 100.0f, 0.0f, 100.0f);
    if (throttlePercent > (float)cfg->throttlePercent) {
        return false;
    }

    const int32_t altCm = getEstimatedAltitudeCm();
    if (altCm == 0) {
        // высоты нет (нет баро/GPS) — действуем только по газу
        return true;
    }
    return altCm <= (int32_t)cfg->altM * 100;
}