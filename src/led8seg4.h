#define LED8SEG4_A  8
#define LED8SEG4_B  9
#define LED8SEG4_C  10
#define LED8SEG4_D  11
#define LED8SEG4_E  12
#define LED8SEG4_F  13
#define LED8SEG4_G  14
#define LED8SEG4_DP 15
#define LED8SEG4_DIG1 16
#define LED8SEG4_DIG2 17
#define LED8SEG4_DIG3 18
#define LED8SEG4_DIG4 19

void blink(int num);
void led8seg4_init(void);

/*
	Set all four LEDs simultaneously by raw value: led8seg4_all
	Set an LED by raw value: led8seg4_each[]
*/
extern unsigned int led8seg4_all;
#define led8seg4_each ((unsigned char*)&led8seg4_all)

/*
	Following array contain raw values corresponding to 0x0, 0x1, ... 0xF
	to set to led8seg4_each[]
*/
extern const unsigned char led8seg4_hex2raw[];

/*
	ON/OFF LED display
*/
extern volatile char led8seg4_on;

/*
	Show hex number (0x0 to 0xFFFF)
*/
void hexNumber(int num);

/*
	Show integer (0 to 9999)
*/
void integerNumber(int num);

/*
	Show integer with point (0. to 9999.)
*/
void zeroDecimalPlaceNumner(int num);

/*
	Show one decimal place number
	Provide the 10x number as integer
*/
void oneDecimalPlaceNumber(int num);

/*
	Show two decimal place number
	Provide the 100x number as integer
*/
void twoDecimalPlaceNumber(int num);

/*
	Show three decimal place number
	Provide the 1000x number as integer
*/
void threeDecimalPlaceNumber(int num);
