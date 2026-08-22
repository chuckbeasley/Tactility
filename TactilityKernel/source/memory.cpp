#include <tactility/log.h>
#include <tactility/memory.h>

#ifdef ESP_PLATFORM
#include <esp_heap_caps.h>
#endif

constexpr auto* TAG = "memory";

extern "C" {

const struct MemoryPolicy MEMORY_POLICY_DEFAULT = {
    .required = 0,
    .desired = 0,
    .alignment = 0,
};

void memory_print_stats() {
#ifdef ESP_PLATFORM
    size_t heap_free = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    size_t heap_total = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
    // %zu is not supported by the reduced-size printf implementation used on ESP targets,
    // so the values are cast and printed with %lu instead.
    LOG_I(TAG, "Heap: %lu / %lu available", (unsigned long)heap_free, (unsigned long)heap_total);
    size_t ext_free = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    size_t ext_total = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
    LOG_I(TAG, "External: %lu / %lu available", (unsigned long)ext_free, (unsigned long)ext_total);
#endif
}

}
