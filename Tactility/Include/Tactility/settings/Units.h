// SPDX-License-Identifier: Apache-2.0
#pragma once

/**
 * @brief The unit system the UI displays measurements in.
 *
 * This is a settings value, not a formatting API: the formatters that read it are in
 * `<Tactility/units/Units.h>`. It lives in its own header because both the settings store
 * (SystemSettings) and the formatting module need the type, and having the store depend on the
 * formatter would be the wrong way round.
 */
namespace tt::settings {

enum class UnitSystem {
    Metric,
    Imperial,
};

}
