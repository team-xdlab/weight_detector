#FUSES NOWDT                    //No Watch Dog Timer
#FUSES HS                       //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(clock=20000000)


//#use rs232(baud=19200,parity=N,xmit=PIN_B3,rcv=PIN_A1,bits=8,timeout = 100,stream=HITE)
#use rs232(baud=19200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,timeout = 40,stream=comp)
#use rs232(baud=19200,parity=N,xmit=PIN_B0,rcv=PIN_B1,bits=8,timeout = 10,stream=SL031)
#use rs232(baud=19200,parity=N,xmit=PIN_B2,rcv=PIN_B3,bits=8,timeout = 100,stream=WIFI)

#define HIGH     1
#define LOW      0

#define RFID_MODE   '1'
#define H_W_MODE    '0'
#define isRFID       !input(PIN_C0)

typedef unsigned  int8      uint8_t;
typedef unsigned  int16     uint16_t;
typedef unsigned  int32     uint32_t;
typedef signed    int8      sint8_t;
typedef signed    int16     sint16_t;
typedef signed    int32     sint32_t;



void setUp()
{    
    delay_ms(100);
    
    //setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);      //13.1 ms overflow
    //disable_interrupts(INT_RDA);
    //enable_interrupts(GLOBAL);
    set_tris_a(0x02);
    fprintf(comp,"HEIGHT&WEIGHT_V.0...\r");
}
