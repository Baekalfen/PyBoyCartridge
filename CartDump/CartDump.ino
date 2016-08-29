#define TrisAddrLo DDRA
#define TrisAddrHi DDRC
#define TrisData DDRL
#define AddrLow PORTA
#define AddrHigh PORTC
#define DataPort PORTL
#define DataInput PINL

#define CLK 38
#define WR 39
#define CS 40
#define RD 41
#define RST 52
#define AUDIO 53

void setup() {
    // put your setup code here, to run once:
    Serial.begin(250000);
    TrisAddrLo = 0xFF; //Address 0-7
    TrisAddrHi = 0xFF; //Address 8-15
    TrisData = 0x00; // Data

    pinMode(CLK, INPUT);
    pinMode(WR, OUTPUT);
    pinMode(CS, OUTPUT);
    pinMode(RD, OUTPUT);
    pinMode(RST, OUTPUT);
    pinMode(AUDIO, INPUT);
    PORTG = 0b00000111;
    digitalWrite(RST, HIGH);
}

void loop() {
    // Prevent reading while resetting
    delay(200);
    Serial.print("THIS IS THE FIRST MESSAGE");
    //////////////////////////////////

    // GET HEADER
    extract_section(0x0100, 0x014F);

    extract_section(0x0000, 0x3FFF);
    int i;
    short MBC = get_MBC();
    // bank 0 is hardcoded to 0x0000-0x3FFF. We can't switch to it.
    for (i = 1; i < get_ROM_banks(); i++) {
        switch_bank(MBC, i);
        extract_section(0x4000, 0x7FFF);
    }

    while (1) {};
}

void wait_for_serial() {
    int d = 0;
    while (1) {
        if (Serial.available() > 0) {
            d = Serial.read();
            Serial.write(d);
            return;
        }
    }
}

short get_MBC() {
    digitalWrite(WR, HIGH);
    digitalWrite(RD, LOW);
    digitalWrite(CS, LOW);
    AddrHigh = 0x01;
    AddrLow = 0x47;
    delayMicroseconds(1);

    switch (DataInput) {
        case 0x0:
            return 0;
        case 0x1:
        case 0x2:
        case 0x3:
            return 1;
        case 0x5:
        case 0x6:
            return 2;
        case 0x8:
        case 0x9:
            return 0;
        case 0xF:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            return 3;
        case 0x15:
        case 0x16:
        case 0x17:
            return 4;
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
            return 5;
    }
}

int get_ROM_banks() {
    digitalWrite(WR, HIGH);
    digitalWrite(RD, LOW);
    digitalWrite(CS, LOW);
    AddrHigh = 0x01;
    AddrLow = 0x48;
    delayMicroseconds(1);

    switch (DataInput) {
        case 0x0:
            return 2;
        case 0x1:
            return 4;
        case 0x2:
            return 8;
        case 0x3:
            return 16;
        case 0x4:
            return 32;
        case 0x5:
            return 64; //only 63 banks used by MBC1
        case 0x6:
            return 128; //only 125 banks used by MBC1
        case 0x7:
            return 256;
        case 0x52:
            return 72;
        case 0x53:
            return 80;
        case 0x54:
            return 90;
    }
}

void write_cartridge(int addr, byte data) {
    //               WCR
    //               RSD
    PORTG = 0b00000111;

    AddrHigh = (addr >> 8) & 0xFF;
    AddrLow = addr & 0xFF;

    DDRL = 0xFF;
    PORTL = data;
    PORTG = 0b00000010;
    delayMicroseconds(1);
    PORTG = 0b00000111;
    DDRL = 0x00;
    delayMicroseconds(2);
}

void switch_bank(short MBC, short bank) {
    switch (MBC) {
        case 0:
            break;
        case 1:
            write_cartridge(0x2000, bank & 0b00011111);
            write_cartridge(0x4000, bank & 0b01100000);
            break;
        case 2:
            write_cartridge(0x2000, bank & 0b00001111);
            break;
        case 3:
            write_cartridge(0x2000, bank & 0b01111111);
            break;
    }
    delayMicroseconds(5);
}

void extract_section(int from, int to){
    short from_high = (from >> 8) & 0xFF;
    short from_low = from & 0xFF;

    short to_high = (to >> 8) & 0xFF;
    short to_low = to & 0xFF;

    extract_section(from_high, from_low, to_high, to_low);
}

void extract_section(short from_high, short from_low, short to_high, short to_low){
    digitalWrite(WR, HIGH);
    digitalWrite(RD, LOW);
    digitalWrite(CS, LOW);

    byte data;
    int i, j;

    for (j = from_high; j<=to_high; j++){
        AddrHigh = j;
        for (i = from_low; i<=to_low; i++){
            AddrLow = i;
            if (i==0){
                delayMicroseconds(1);
            }
            data = DataInput;
            Serial.write((short)data);
        }
    }

    wait_for_serial();
}


