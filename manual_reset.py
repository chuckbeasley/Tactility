import serial, time, sys
s = serial.Serial()
s.port = sys.argv[1]
s.baudrate = 115200
s.open()
# ESP32 auto-reset circuit: RTS -> GPIO0 (boot), DTR -> EN (reset).
# Boot to app: GPIO0 high (rts=True), then pulse EN low->high via DTR.
s.rts = True   # GPIO0 = high -> normal boot
time.sleep(0.05)
s.dtr = False  # EN = low  -> reset asserted
time.sleep(0.15)
s.dtr = True   # EN = high -> release reset
time.sleep(0.05)
s.close()
print("manual reset pulse sent")
