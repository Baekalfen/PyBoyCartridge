#define NOP __asm__ __volatile__ ("nop\n\t")


#define TrisFlags DDRB
#define TrisData DDRA
#define TrisAddrLo DDRF
#define TrisAddrHi DDRC

#define TRIS_OUT 0xFF
#define TRIS_IN 0x00

#define PinFlags PINB
#define PinData PINA
#define PinAddrLo PINF
#define PinAddrHi PINC

#define PortFlags PORTB
#define PortData PORTA
#define PortAddrLo PORTF
#define PortAddrHi PORTC

#define ClockPin 2

unsigned char measuring[4196];

unsigned char NintendoLogo[] = {0x00,0x00,0x00,0x00,
    /*REDACTED. Dump a cartridge and copy the Nintendo
      logo here.*/};

void setup() {
    Serial.begin(1000000);
    /* Serial.begin(250000); */

    TrisFlags = TRIS_IN;
    TrisAddrLo = TRIS_IN;
    TrisAddrHi = TRIS_IN;

    TrisData = TRIS_OUT;
    PortData = 0x00;

    pinMode(ClockPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(ClockPin), rising_clock, RISING);
}

void loop() {
    // Prevent reading while resetting
    delay(200);
    Serial.print("THIS IS THE FIRST MESSAGE");
    //////////////////////////////////

    /* byte a,b,c; */

    /* cli(); // Disable interrupts */
    /* while(1){ */
    /*     a = PinAddrHi; */
    /*     b = PinAddrLo; */

    /*     if (a == 0x01 && b >= 0x04){ */
    /*         for (i = 0; i < sizeof(measuring); i++){ */
    /*             /1* measuring[i] = 0; *1/ */
    /*             /1* measuring[i] = PinAddrHi; *1/ */
    /*             /1* i++; *1/ */
    /*             /1* measuring[i] = PinAddrLo; *1/ */
    /*             measuring[i] = PINB; */
    /*         } */
    /*         sei(); */
    /*         for (i = 0; i < sizeof(measuring); i++){ */
    /*             Serial.write(measuring[i]); */
    /*         } */
    /*         while(1){ */
    /*             NOP; */
    /*         } */
    /*         /1* PortData = NintendoLogo[b]; *1/ */
    /*         /1* if (b == 0x43){ *1/ */
    /*         /1*     break; *1/ */
    /*         /1* } *1/ */
    /*     } */
    /* } */

    /* while ((PINB & 1) == 0){NOP;} */

    /* while(1){ */
    /*     PortData = 0x76; //HALT */
    /* } */

    while(1){
        NOP;
    }
}

byte a,b,c;

void rising_clock(){
  asm volatile(
    "ldi r31, hi8(NintendoLogo)\n\t" // 1clk
    "nop\n\t" // 1clk
    "in r30, %1\n\t" //1clk
    "ld r24, Z\n\t" //2clks
    "out %0, r24\n\t" //1clk

    // Total: 6clks
    // Total without nop: 5clks
  :: "I" (_SFR_IO_ADDR(PortData)), "I" (_SFR_IO_ADDR(PinAddrLo)));

  /* PortData = NintendoLogo[PINL]; */
}
