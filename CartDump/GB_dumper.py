import serial

ser = serial.Serial('COM3', 250000, timeout=5)

# Prevent reading while resetting
garbage = ""
while True:
    garbage += ser.read()
    if "THIS IS THE FIRST MESSAGE" in garbage:
        break
##################################


cartTypes = {
    "0x0": "ROM ONLY",
    "0x1": "MBC1",
    "0x2": "MBC1+RAM",
    "0x3": "MBC1+RAM+BATTERY",
    "0x5": "MBC2",
    "0x6": "MBC2+BATTERY",
    "0x8": "ROM+RAM",
    "0x9": "ROM+RAM+BATTERY",
    "0xB": "MMM01",
    "0xC": "MMM01+RAM",
    "0xD": "MMM01+RAM+BATTERY",
    "0xF": "MBC3+TIMER+BATTERY",
    "0x10": "MBC3+TIMER+RAM+BATTERY",
    "0x11": "MBC3",
    "0x12": "MBC3+RAM",
    "0x13": "MBC3+RAM+BATTERY",
    "0x15": "MBC4",
    "0x16": "MBC4+RAM",
    "0x17": "MBC4+RAM+BATTERY",
    "0x19": "MBC5",
    "0x1A": "MBC5+RAM",
    "0x1B": "MBC5+RAM+BATTERY",
    "0x1C": "MBC5+RUMBLE",
    "0x1D": "MBC5+RUMBLE+RAM",
    "0x1E": "MBC5+RUMBLE+RAM+BATTERY",
    "0xFC": "POCKET CAMERA",
    "0xFD": "BANDAI TAMA5",
    "0xFE": "HuC3",
    "0xFF": "HuC1+RAM+BATTERY"
}

ROMSizes = {
    "0x0": 2,
    "0x1": 4,
    "0x2": 8,
    "0x3": 16,
    "0x4": 32,
    "0x5": 64,  # only 63 banks used by MBC1
    "0x6": 128,  # only 125 banks used by MBC1
    "0x7": 256,
    "0x52": 72,
    "0x53": 80,
    "0x54": 90
}

RAMSizes = {
    "0x0": 0,
    "0x1": 2,
    "0x2": 8,
    "0x3": 32
}

header = ser.read(0x50)
ser.write("1")
print "header echo", ser.read()

# TODO: Dump a cartridge and insert the Nintendo logo here
# dataCheck = [REDACTED]

# TODO: Change this to the sum function in the boot-rom
# print "Nintendo logo match:",
# if dataCheck == map(lambda x: ord(x), header[0x04:0x04 + len(dataCheck)]):
#     print "Ok"
# else:
#     print "Failed"
#     print map(lambda x: ord(x), header[0x04:0x04 + len(dataCheck)])
#     print dataCheck
#     exit(3)

print "Header checksum:",
x = 0
for m in range(0x34, 0x4D):
    x = x - ord(header[m]) - 1
    x &= 0xff

if ord(header[0x4D]) == x:
    print "Ok"
else:
    print "Failed"
    print hex(x)
    print hex(ord(header[0x4D]))
    exit(3)

print ""

name = header[0x34:0x43].split("\0")[0]
print "Name:", name
print "Manufacturer code:", header[0x3F:0x42]
print "CGB flag:", hex(ord(header[0x43]))
print "New license code:", header[0x44:0x46]
print "SGB flag:", hex(ord(header[0x46]))
print "Cart type:", cartTypes[hex(ord(header[0x47]))]
print "ROM banks:", ROMSizes[hex(ord(header[0x48]))]
if "MBC2" in cartTypes[hex(ord(header[0x47]))]:
    print "PanDocs: When using a MBC2 chip 00h must be specified in this entry, even though the MBC2 includes a built-in RAM of 512 x 4 bits."
print "RAM size: %s KB" % RAMSizes[hex(ord(header[0x49]))]
print "Destination code:", hex(ord(header[0x4A]))
print "Old license code:", hex(ord(header[0x4B]))
print "ROM version:", hex(ord(header[0x4C]))
print "Header checksum:", hex(ord(header[0x4D]))
print "Global checksum:", " ".join(map(lambda x: hex(ord(x)), header[0x4E:0x50]))


with open(name + ".gb", 'wb') as f:
    for n in range(ROMSizes[hex(ord(header[0x48]))]):
        print "Bank", n,
        data = bytearray([0] * (16 * 1024))
        if not ser.readinto(data) == (16 * 1024):
            print "Error!"
            exit(4)
        print "done."
        f.write(data)
        ser.write("2")
        r = ser.read()
        if not r == "2":
            print "Error2!", r, ord(r)
            exit(7)

    ser.close()

