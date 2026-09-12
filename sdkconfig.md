# Get Started

See [https://docs.tactilityproject.org]

# Useful Parameters

You can copy these into `sdkconfig` manually or set them via `idf.py menuconfig`

## LVGL FPS Counter

```properties
CONFIG_LV_USE_OBSERVER=y
CONFIG_LV_USE_PERF_MONITOR=y
```

## LVGL Layer debugging

```properties
CONFIG_LV_USE_REFR_DEBUG=y
CONFIG_LV_USE_LAYER_DEBUG=y
```

## Halt on error

```properties
CONFIG_ESP_SYSTEM_PANIC_PRINT_HALT=y
# CONFIG_ESP_SYSTEM_PANIC_PRINT_REBOOT is not set
```

## Stack debugging

[Stack smashing protection mode:](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/kconfig-reference.html#config-compiler-stack-check-mode)

> - In NORMAL mode (GCC flag: -fstack-protector) only functions that call alloca, and functions with buffers larger than 8 bytes are protected.
> - STRONG mode (GCC flag: -fstack-protector-strong) is like NORMAL, but includes additional functions to be protected -- those that have local array definitions, or have references to local frame addresses.
> - In OVERALL mode (GCC flag: -fstack-protector-all) all functions are protected.

```properties
CONFIG_STACK_CHECK_STRONG=y
```

or:

```properties
CONFIG_STACK_CHECK_ALL=y
```

## Remote screen interaction (WebSocket)

Required by the web server's `/ws/remote` endpoint, which the WebSocket-based
remote screen viewer/controller connects to. Without it the httpd WebSocket API
(`httpd_ws_recv_frame`/`httpd_ws_send_frame`) isn't compiled in.

```properties
CONFIG_HTTPD_WS_SUPPORT=y
```

A mirror session holds one socket open for its whole duration, on top of whatever
keep-alive sockets the browser already has, so the socket pool also needs a little
headroom (the web server asks httpd for 6).

```properties
CONFIG_LWIP_MAX_SOCKETS=16
```

Two things about this protocol are easy to get wrong, and both cost time here.

**Input is in device pixels; frame regions are not.** `p`/`m`/`r` coordinates go straight to
`remoteInputPush` with no scaling, so a tap is always in the 480x320 panel space no matter what
scale the client asked for frames at. The `D`/`J` reply rectangles, by contrast, are in the
requested scale's space. Driving the UI from a scale-2 client therefore means doubling whatever
coordinate is read off the returned frame - tapping the frame's own coordinates lands in the
status bar instead.

**The web server's settings do not live in the settings directory.** A flash rewrites the data
partition image, so they are wiped, and the server comes back up on defaults with authentication
*off* - the API then answers any client, with or without credentials. They are restored by
writing `webserver.properties` into the app's own user-data directory, not
`/data/tactility/settings`:

```
/data/tactility/user/app/tactility.webserversettings/webserver.properties
```

with `webServerAuthEnabled=1`, `webServerUsername`, `webServerPassword`, `webServerPort` and
`webServerEnabled`, followed by `POST /admin/reboot`. Check it against a protected endpoint:
with the file in the wrong place `/fs/list` keeps returning 200 to an unauthenticated request.
`/api/sysinfo` is unauthenticated by design, so it proves nothing either way.

## Core dumps

A panic is written to flash and can be decoded afterwards, which is the only way to diagnose a
crash that nobody was watching when it happened. Three things are needed, and the first is the one
that is easy to miss.

**The component has to be in the build.** The root `CMakeLists.txt` scopes the build with
`set(COMPONENTS Tactility)`. `espcoredump` is not a dependency of `Tactility`, so it was excluded -
and excluding a component also excludes its Kconfig, which means `ESP_COREDUMP_ENABLE_TO_FLASH` did
not merely default to off, it did not exist and could not be turned on from any menu. It is now
named alongside `Tactility` in that list.

**A coredump partition**, added to `partitions-16mb-no-sd.csv` ahead of `data`, taking 128 KB from
it. That is more than the dumps need - a single-task dump measured 3232 bytes without
`CONFIG_ESP_COREDUMP_CAPTURE_DRAM` - but it is space `data` will not miss.

**Pointing dumps at flash:**

```properties
CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH=y
# CONFIG_ESP_COREDUMP_ENABLE_TO_NONE is not set
```

To read the most recent dump:

```shell
idf.py coredump-info -p COM11
```

Verified end to end by crashing the device on purpose with a temporary `abort()` in `app_main()`:
the panic was saved to flash, the checksum was verified on the next boot (`Core dump data checksum
is correct`, `Found core dump 3232 bytes in flash @ 0x430000`), and `coredump-info` printed the task
list with backtraces - `Tmr Svc` in `prvTimerTask`, among others. The trigger was removed and the
partition erased afterwards, so the next dump will be a real one.

A dump survives a reboot but not a flash: flashing rewrites the partition table and the app, and a
dump only means anything against the ELF that produced it.

## A hang, and why the watchdog has to panic

A core dump is only written by the panic handler, so it captures crashes and nothing else. The
failures reported against the Bluetooth app have been a mix of the two: some produced a dump and
were diagnosed exactly from it, and some left the device unreachable with an empty coredump
partition, which is a hang - a task spinning, or two tasks deadlocked - and leaves no trace at all.

`CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU0` is on, so a task that saturates the CPU starves the idle
task and the watchdog notices. With the default setting it only *prints*, which is no use to anyone
who was not watching the console at the time. Making it panic turns that case into a crash, and a
crash into a dump:

```properties
CONFIG_ESP_TASK_WDT_PANIC=y
```

The trade is that an overloaded device now reboots instead of freezing, which is usually the better
of the two anyway. What it does *not* catch is a deadlock: if every task is blocked on a lock the
other holds, the idle task keeps running, the watchdog never trips, and neither a dump nor a
watchdog message will appear. A hang that leaves no dump and no watchdog message is that case, and
it needs the console attached while it happens.

## PSRAM speed

The remote screen mirror's capture pass is bound by PSRAM bandwidth, not by its own
loop: it moves 614 KB per 480x320 frame (read 307 KB out of the display's shadow
frame, write 307 KB), and at the 40 MHz default that took 67 ms, i.e. ~9 MB/s for
interleaved read and write against a 20 MB/s bus. Quad SPI at 80 MHz halves that
pass:

```properties
CONFIG_SPIRAM_SPEED_80M=y
```

Measured on the ESP32-C5 device: the capture pass went 67 ms -> 38 ms, and a
240x160 whole frame went 45 ms -> 37 ms of device time. `CONFIG_SPIRAM_MEMTEST`
runs at boot and passes at this speed, which is a useful sanity check that the
part is happy with it.

What it does *not* help is the JPEG encode: that measured 125-133 ms at 40 MHz and
121-129 ms at 80 MHz, i.e. unchanged. The encoder is CPU-bound (colour conversion
plus DCT on a single 240 MHz core), so a faster bus does nothing for it. Anything
that only reduces encode time has to reduce the number of pixels, which is what the
mirror's scale setting does.

This depends on the module's PSRAM being rated for 80 MHz. If it is not, the
device fails during PSRAM initialisation and will not boot - the symptom is no
serial output after the ROM banner - so keep the ability to set it back:

```properties
CONFIG_SPIRAM_SPEED_40M=y
```

