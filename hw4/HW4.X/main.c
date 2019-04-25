#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<math.h>
// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // no boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable secondary osc
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // use slowest wdt
#pragma config WINDIS = OFF // wdt no window mode
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module

#define CS LATBbits.LATB15 
//function prototype
unsigned char spi_io(unsigned char o);
void initSPI1();
void setVoltage(char chn, float v);



int main(void) {
    TRISAbits.TRISA4 = 0;
    
    __builtin_disable_interrupts();
        // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;
    
    initSPI1();
    int i=0;
    __builtin_enable_interrupts(); 
    
    
    while(1) {
	_CP0_SET_COUNT(0);
     
     LATAbits.LATA4=1;
     
    
    float sin = 1.65+1.65*sin(i*2*3.1415/0.1);  //should make a 10Hz sin wave)
    
    int s=1,bias=0;
    if (i%100==1){s=s*(-1);bias=!bias;}
    float tri = 1.65*(2/3.1415)*asin(sin(i*3.1415/4000)) +1.65;
//    float tri= (3.3/100)*i;
	i++;
     setVoltage(0,tri);
     setVoltage(1,sin);
	while(_CP0_GET_COUNT() < 24000) {;}  //check this is 24Million
    ;
  }
  return 0;
}


// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}

void initSPI1(){
    
    RPB15Rbits.RPB15R=0b0011; //B15 = SS1
    RPA1Rbits.RPA1R=0b0011; // A1= SDO1
//    RPB14Rbits.RPB14R=0b0111; //B14= SCK1
    //SDI not used
    TRISBbits.TRISB15=0; //manually control SS1 as digital output 
    CS=1;
    
    SPI1CON = 0;              // turn off the spi module and reset it
    SPI1BUF;                  // clear the rx buffer by reading from it
    SPI1BRG = 0x1;             // baud rate to 10 MHz [SPI1BRG = (80000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0;  // clear the overflow bit
    SPI1CONbits.CKE = 1;      // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1;    // master operation
    SPI1CONbits.ON = 1;       // turn on spi

}
void setVoltage(char chn, float v) {
    int adc=0;
	unsigned char bgs=0b111;
    unsigned short c1 = 0, c2=0;
    
    adc=(int)(1023.*v/(3.3));
    c1=chn<<7;
    c1=c1|(bgs<<4);
    c1=c1|(adc>>6);
    c2=c2|(adc<<2);
	
	CS = 0;
	spi_io(c1);
    spi_io(c2);
	CS=1;
}

