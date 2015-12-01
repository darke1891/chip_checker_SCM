//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <C8051F340.h>                 // SFR declarations
#include <stdio.h>

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F34x
//-----------------------------------------------------------------------------

sfr16 SBRL1 = 0xB4;

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       48000000          // SYSCLK frequency in Hz
//#define BAUDRATE0     115200           // Baud rate of UART0 in bps
//#define BAUDRATE1     115200           // Baud rate of UART1 in bps
#define BAUDRATE0     9600           // Baud rate of UART0 in bps
#define BAUDRATE1     9600           // Baud rate of UART1 in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSTEMCLOCK_Init (void);
void PORT_Init (void);
void UART0_Init (void);
void UART1_Init (void);
void Delay (void);

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

bit UART = 0;                          // '0 is UART0; '1' is UART1
//-----------------------------------------------------------------------------
// Buffer holder
//-----------------------------------------------------------------------------

char buffer[100];
int bufferLength = 0;

//-----------------------------------------------------------------------------
// Testing Info
//-----------------------------------------------------------------------------
int mydebug = 0;
int chip;
int syncCount;
int pin_total[] = {14,14,14,14,14,14,14,16,16,14,14,14,16,16,14};
//-----------------------------------------------------------------------------
// getchar for UART1 bluetooth
// proceedBuffer    
//-----------------------------------------------------------------------------

char getchar2();
int proceedBuffer();
int proceedString(int);

int getInput();
void sendOutput(int, char *);

int getNumber(int);
int findSemicolon();
void bufferToleft(int);

void mysleep(int time);
void myprintint(int n,int t);
void myprintchar(char c,int t);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main(){
    int res= -1;
    // START WITH P[1]
    char p[20] = {'0','0','1','2','0','1','2','0','1','2','0','1','2','0','1','2','0','1','2'};
    
    PCA0MD &= ~0x40;                    // Disable Watchdog timer
    SYSTEMCLOCK_Init ();                // initialize oscillator
    PORT_Init ();                       // initialize crossbar and GPIO
    UART0_Init ();                      // initialize UART0
    UART1_Init ();                      // initialize UART1
    

    P3 = 0x0;
    
    while(1/*res == -1*/){
        
        res = getInput();
        if (res != -1) {
            // testing number (int chip;)
            sendOutput(pin_total[chip],p);
            P3 = res+1;
            res = -1;   
        }
    }
    
    
    
    //sendOutput('');
}

void sendOutput(int n,char *result) {
    int mpause = 25;int i;
    UART = 1;
    myprintchar('#',mpause);
    myprintint(syncCount,mpause);
    myprintchar(';',mpause);
    myprintchar('@',mpause);
    myprintint(chip,mpause);
    myprintchar(';',mpause);     
    for(i=1;i<n+1;i++){
        myprintchar('p',mpause);
        myprintint(i,mpause);
        myprintchar('s',mpause);
        myprintchar(result[i],mpause);
        myprintchar(';',mpause);
    }
}

int getInput (void) {

    char input_char;
    int returnResult = 0;
    UART = 1;                
    input_char = getchar2();
    if(input_char >='#' && input_char <= 'z')
        buffer[bufferLength++] = input_char;
    return proceedBuffer();
}

char getchar2() {
    char c;
    if (SCON1 & 0x01) {
        c = SBUF1;
        SCON1 &= ~0x01;
    }else
        c = 1;
    return  c;
}

int proceedBuffer(){// change later
    int rank = -2;
    int returnres;
    while(rank != -1){
        int rank = findSemicolon();
        if(rank == -1){
            return -1;
        }
        returnres =  proceedString(rank);
        return returnres;
    }   
}
/** */
int proceedString(int rank){
    int returnResult = -1;
    UART = 0;
    if(buffer[0] == '#'){
        bufferToleft(1);
        rank --;
        syncCount = getNumber(rank);
        returnResult = -1;
    }else if(buffer[0] == '@'){
        bufferToleft(1);
        rank --;
        chip = getNumber(rank);
        returnResult = chip;
    }
        bufferToleft(rank+1);
        return returnResult;
}

int findSemicolon(){
    int i;
    for(i = 0;i < bufferLength;i++){
        
        if(buffer[i] == ';'){
            return i;
        }
    }
    
    return -1;
}

int getNumber(int rank){
    int result = 0,i;
    for(i = 0;i< rank;i++){
        if(buffer[i]>'9' || buffer[i] < '0'){
            return -1;
    }
    result = result * 10 + buffer[i]-'0';
    }
    return result;
}

void bufferToleft(int n){
    int i;
    for(i=0;i<n;i++){
        buffer[i] = buffer[i+1];
    }bufferLength = bufferLength-n;
}

void mysleep(int time) {
    int x1,x2;
    for(x2 = 0;x2 < time;x2){
        for(x1 = 0;x1 < 20000;x1)//4secs
            x1++;
        x2++;
    }
}

void myprintint(int n,int t) {
    int a = 0;

    mysleep(t); 
    if(n>9){
        printf("%d",n/10);
        mysleep(t);
    }
    printf("%d",n%10);
}

void myprintchar(char c,int t) {
    mysleep(t);
    putchar(c);
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSTEMCLOCK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None

// This routine initializes the system clock to use the internal system clock
// routed through the clock multiplier as its clock source.
//
//-----------------------------------------------------------------------------

void SYSTEMCLOCK_Init (void){
    OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency and enable
                                       // missing clock detector
    CLKMUL  = 0x00;                     // Select internal oscillator as
                                       // input to clock multiplier

    CLKMUL |= 0x80;                     // Enable clock multiplier
    Delay();                            // Delay for clock multiplier to begin
    CLKMUL |= 0xC0;                     // Initialize the clock multiplier
    Delay();                            // Delay for clock multiplier to begin

    while(!(CLKMUL & 0x20));            // Wait for multiplier to lock
    CLKSEL  = 0x03;                     // Select system clock
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None

// Configure the Crossbar and GPIO ports
//
// P0.0     TX1 (UART 1)
// P0.1     RX1 (UART 1)
// P0.2     
// P0.3
// P0.4     TX0 (UART 0)
// P0.5     RX0 (UART 0)
// P0.6
// P0.7

// P1       not used in this example

// P2.0
// P2.1
// P2.2     LED
// P2.3    
// P2.4
// P2.5
// P2.6
// P2.7
//-----------------------------------------------------------------------------

void PORT_Init (void)
{  
    P0SKIP    = 0x03;
    XBR0 = 0x01;                        // route UART 0 to crossbar
    XBR2 = 0x01;                        // route UART 1 to crossbar
    XBR1 = 0x40;                        // enable crossbar
    P0MDOUT |= 0x11;                    // set P0.4 to push-pull output
    P3MDOUT |= 0x0F;
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for baudrate; and 8-N-1.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
    SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits

    if (SYSCLK/BAUDRATE0/2/256 < 1) {
        TH1 = -(SYSCLK/BAUDRATE0/2);
        CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
        CKCON |=  0x08;
    } else if (SYSCLK/BAUDRATE0/2/256 < 4) {
        TH1 = -(SYSCLK/BAUDRATE0/2/4);
        CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01                 
        CKCON |=  0x09;
    } else if (SYSCLK/BAUDRATE0/2/256 < 12) {
        TH1 = -(SYSCLK/BAUDRATE0/2/12);
        CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
    } else {
        TH1 = -(SYSCLK/BAUDRATE0/2/48);
        CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
        CKCON |=  0x02;
    }

    TL1 = TH1;                          // init Timer1
    TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
    TMOD |=  0x20;                       
    TR1 = 1;                            // START Timer1
    TI0 = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// UART1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure UART1 for baudrate1 and 8-N-1.
//
//-----------------------------------------------------------------------------

void UART1_Init (void)
{
    SMOD1 = 0x0C;                       // set to disable parity, 8-data bit,
                                       // disable extra bit, 
                                       // stop bit 1 bit wide

    SCON1 = 0x10;                       // SCON1: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits

    if (SYSCLK/BAUDRATE1/2/0xFFFF < 1) {
        SBRL1 = -(SYSCLK/BAUDRATE1/2);
        SBCON1 |= 0x03;                  // set prescaler to 1
    } else if (SYSCLK/BAUDRATE1/2/0xFFFF < 4) {
        SBRL1 = -(SYSCLK/BAUDRATE1/2/4);
        SBCON1 &= ~0x03;
        SBCON1 |= 0x01;                  // set prescaler to 4

    } else if (SYSCLK/BAUDRATE1/2/0xFFFF < 12) {
        SBRL1 = -(SYSCLK/BAUDRATE1/2/12);
        SBCON1 &= ~0x03;                 // set prescaler to 12
    } else {
        SBRL1 = -(SYSCLK/BAUDRATE1/2/48);
        SBCON1 &= ~0x03;
        SBCON1 |= 0x02;                  // set prescaler to 4
    }

    SCON1 |= 0x02;                      // indicate ready for TX
    SBCON1 |= 0x40;                     // enable baud rate generator
}


//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
// Return Value : UART0/1 buffer value
// Parameters   : character to be transmitted across UART0/1
//
// This is an overloaded fuction found in the stdio library.  When the
// function putchar is called, either by user code or through calls to stdio
// routines such as printf, the following routine will be executed instead 
// of the function located in the stdio library.
//
// The function checks the UART global variable to determine which UART to 
// use to receive a character.
//
// The routine expands '\n' to include a carriage return as well as a 
// new line character by first checking to see whether the character  
// passed into the routine equals '\n'.  If it is, the routine waits for 
// TI0/TI1 to be set, indicating that UART 0/1 is ready to transmit another 
// byte.  The routine then clears TI0/TI1 bit, and sets the UART0/1 output 
// buffer to '0x0d', which is the ASCII character for carriage return.
//
// The routine the waits for TI0/TI1 to be set, clears TI0/TI1, and sets
// the UART output buffer to <c>.  
//
//-----------------------------------------------------------------------------

char putchar (char c)  {

    if (UART == 0) {

        if (c == '\n')  {                // check for newline character
            while (!TI0);                 // wait until UART0 is ready to transmit
            TI0 = 0;                      // clear interrupt flag
            SBUF0 = 0x0d;                 // output carriage return command
        }
        while (!TI0);                    // wait until UART0 is ready to transmit
        TI0 = 0;                         // clear interrupt flag
        return (SBUF0 = c);              // output <c> using UART 0
    }

    else if (UART == 1) {
        if (c == '\n')  {                // check for newline character
            while (!(SCON1 & 0x02));      // wait until UART1 is ready to transmit
            SCON1 &= ~0x02;               // clear TI1 interrupt flag
            SBUF1 = 0x0d;                 // output carriage return
        }
            while (!(SCON1 & 0x02));         // wait until UART1 is ready to transmit
            SCON1 &= ~0x02;                  // clear TI1 interrupt flag
            return (SBUF1 = c);              // output <c> using UART 1
    }
}

//-----------------------------------------------------------------------------
// _getkey
//-----------------------------------------------------------------------------
//
// Return Value : byte received from UART0/1
// Parameters   : none

// This is an overloaded fuction found in the stdio library.  When the
// function _getkey is called, either by user code or through calls to stdio
// routines such as scanf, the following routine will be executed instead 
// of the function located in the stdio library.
//
// The function checks the UART global variable to determine which UART to 
// use to receive a character.
//
// The routine waits for RI0/RI1 to be set, indicating that a byte has
// been received across the UART0/UART1 RX line.  The routine saves the 
// received character into a local variable, clears the RI0/RI1 interrupt
// flag, and returns the received character value.
//
//-----------------------------------------------------------------------------
char _getkey ()  {
    char c;

    if (UART == 0) {
        while (!RI0);                      // wait until UART0 receives a character
        c = SBUF0;                         // save character to local variable
        RI0 = 0;                           // clear UART0 receive interrupt flag
        return (c);                        // return value received through UART0
    }

    else if (UART == 1) {
        while (!(SCON1 & 0x01));           // wait until UART1 receives a character
        c = SBUF1;                         // save character to local variable
        SCON1 &= ~0x01;                    // clear UART1 receive interrupt flag
        return (c);                        // return value received through UART1
    }
}

//-----------------------------------------------------------------------------
// Delay
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : none
//
// Used for a small pause of approximately 40 us.
//
//-----------------------------------------------------------------------------

void Delay(void)
{
    int x;
    for(x = 0;x < 500;x)
        x++;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
