import sys, time, serial
ser = serial.Serial()
ser.port = sys.argv[1]
ser.baudrate = 115200
ser.timeout = 0.5
ser.dtr = False
ser.rts = False
ser.dsrdtr = False
ser.rtscts = False
ser.open()
data = b""
end = time.time() + 40
while time.time() < end:
    c = ser.read(8192)
    if c:
        data += c
sys.stdout.buffer.write(data[-20000:])
