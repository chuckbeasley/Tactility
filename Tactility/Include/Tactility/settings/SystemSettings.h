#pragma once

#include "Language.h"
#include "Units.h"

namespace tt::settings {

struct SystemSettings {
    Language language = Language::en_US;
    bool timeFormat24h = true;
    std::string dateFormat = std::string("DD/MM/YYYY"); // MM/DD/YYYY, DD/MM/YYYY, YYYY-MM-DD, YYYY/MM/DD
    /**
     * Metric or imperial, for every measurement the UI shows. Chosen in Region & Language and read
     * through tt::units, which is also where the conversions live. Metric by default: every internal
     * value is already metric (metres, km/h, °C, pascals), so the default costs no conversion.
     */
    UnitSystem unitSystem = UnitSystem::Metric;
};

bool loadSystemSettings(SystemSettings& properties);

bool saveSystemSettings(const SystemSettings& properties);

}
