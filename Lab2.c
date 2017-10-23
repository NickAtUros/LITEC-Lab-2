/*  Names: Adrianne Kunkel & Nick Lockwood
	Section: 2
	Date: 3/9/15
	File name: lab 2
	Description: lab 2
*/

#include <c8051_SDCC.h>// include files. This file is available online
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void Port_Init(void);  	// Initialize ports for input and output
void Timer_Init(void); 	// Initialize Timer 0
void Interrupt_Init(void); //Initialize interrupts
void Timer0_ISR(void) __interrupt 1;
unsigned char random(void); //random number generator
unsigned char read_AD_input(unsigned char pin_number);
void ADC_Init(void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
__sbit __at 0xB7 BILED1G; //right
__sbit __at 0xB2 BILED1R;
__sbit __at 0xB3 BILED2G; //middle
__sbit __at 0xB4 BILED2R;
__sbit __at 0xB5 BILED3G; //left
__sbit __at 0xB6 BILED3R;
__sbit __at 0xA0 SS1;
__sbit __at 0xA1 SS2;
__sbit __at 0xA2 SS3;
__sbit __at 0xA3 SS4;
__sbit __at 0xA7 SS5;
__sbit __at 0xA4 SS6;
__sbit __at 0x84 PB1;
__sbit __at 0x85 LED1; //amber
__sbit __at 0x82 LED2; //grn
__sbit __at 0x83 BUZZER;

unsigned int counts;
unsigned int bouncecounts;
unsigned int countsmax;
unsigned int tmax;
unsigned char AD_value;
unsigned char led1;
unsigned char led2;
unsigned char led3;
unsigned char correctcolor;
unsigned char correctposition;
unsigned char guess1;
unsigned char guess2;
unsigned char guess3;
unsigned char pointsA;
unsigned char pointsG;
unsigned char color;
unsigned char button;

//***************
void main(void)
{
	Sys_Init();  	// System Initialization
	Port_Init(); 	// Initialize ports
	Interrupt_Init();
	Timer_Init();	// Initialize Timer 0
    ADC_Init();

	putchar(' ');	// the quote fonts may not copy correctly into SiLabs IDE

	while(1)
    {

   	 counts = 0;
   	 color = 0;
   	 printf("\rPush button to start game \n");
   	 LED1 = 1;
   	 LED2 = 1;
   	 BUZZER = 1;
   	 BILED1G = 1;
   	 BILED1R = 1;
   	 BILED2G = 1;
   	 BILED2R = 1;
   	 BILED3G = 1;
   	 BILED3R = 1;
   	 TR0 = 0;
   	 pointsA = 0;
   	 pointsG = 0;

   	 while (PB1)
   	 {
   		 LED1 = 1; //before button pushed set all to default values
   		 LED2 = 1;
   		 BUZZER = 1;
   		 BILED1G = 1;
   		 BILED1R = 1;
   		 BILED2G = 1;
   		 BILED2R = 1;
   		 BILED3G = 1;
   		 BILED3R = 1;

   	 }

   	 if (!PB1) //start game by pushing button
   	 {
   		 TR0 = 1; //debounce button
   		 bouncecounts = 0;
   		 while (bouncecounts < 50)
   		 {;}
   		 TR0 = 0;

   		 while (color < 2)//two turns
   		 {
   		 if (color == 0) //color = 0: amber
   		 {
   			 AD_value = read_AD_input(0); //read A/D value on P1.0
   			 tmax = ((ADC1*.00296*45000)+15000); //calc tmax in ms
   			 printf("\r%u ms per turn\n",tmax);
   			 countsmax = 337*.001*tmax; //calc max counts

   			 LED1 = 0;//start amber turn
   			 printf("\rAmber player's turn\n");
   		 }

   		 else //color = 1: green
   		 {
   			 LED1 = 1;
   			 LED2 = 0;
   			 printf("\rGreen player's turn\n");
   		 }

   		 led1 = random(); //randomize pattern
   		 led2 = random();
   		 led3 = random();
   		 correctcolor = 0; //reset position/color
   		 correctposition = 0;

   			 while (correctposition < 3) //while guess is incorrect
   			 {
   			 correctposition = 0;
   			 correctcolor = 0;

   				 while (1)
   				 {
   					 TR0 = 1; //start timer
   					 if(SS1 == 1) //determine state of biled1
   					 {
   						 BILED1G = 1;
   						 BILED1R = 1;
   						 guess1 = 0; //off
   					 }
   					 else if(SS2 == 0)
   					 {
   						 BILED1G = 0;
   						 BILED1R = 1;
   						 guess1 = 1; //green
   					 }
   					 else
   					 {
   						 BILED1G = 1;
   						 BILED1R = 0;
   						 guess1 = 2; //red
   					 }

   					 if(SS3 == 1) //determine state of biled2
   					 {
   						 BILED2G = 1;
   						 BILED2R = 1;
   						 guess2 = 0;//off
   					 }
   					 else if(SS4 == 0)
   					 {
   						 BILED2G = 0;
   						 BILED2R = 1;
   						 guess2 = 1; //green
   					 }
   					 else
   					 {
   						 BILED2G = 1;
   						 BILED2R = 0;
   						 guess2 = 2; //red
   					 }

   					 if(SS5 == 1) //determine state of biled3
   					 {
   						 BILED3G = 1;
   						 BILED3R = 1;
   						 guess3 = 0; //off
   					 }
   					 else if(SS6 == 0)
   					 {
   						 BILED3G = 0;
   						 BILED3R = 1;
   						 guess3 = 1; //green
   					 }
   					 else
   					 {
   						 BILED3G = 1;
   						 BILED3R = 0;
   						 guess3 = 2; //red
   					 }

   					 if (!PB1) //if button is pushed break from while loop and end turn
   					 {
   						 bouncecounts = 0;
   						 while (bouncecounts < 100)//disable button long enough so more than one turn isn't made
   						 {;}
   						 break;//exit loop
   					 }

   				 }

   				 TR0 = 0;    //disable timer
   				 //determine number of correct position/color

   				 if (guess1 == led1) //determine if guess value is equal to actual value
   					 correctposition++;

   				 if (guess2 == led2)
   					 correctposition++;

   				 if (guess3 == led3)
   					 correctposition++;

   				 if (guess1 == led1 || guess1 == led2 || guess1 == led3) //determine if there are any color matches
   					 correctcolor++;

   				 if (guess2 == led1 || guess2 == led2 || guess2 == led3)
   					 correctcolor++;

   				 if (guess3 == led1 || guess3 == led2 || guess3 == led3)
   					 correctcolor++;

   				 if((guess1==guess2 && guess1==guess3) && (led1 != led2 || led1 != led3 || led2 != led3)) //check for false triples
   					 {
   						 if(correctcolor>1)
   							 correctcolor -=2;
   					 }
   				 else if((guess1 == guess2 || guess1 == guess3 || guess2 == guess3)&&(led1 != led2 && led1 != led3 && led2 != led3))
   					 correctcolor--; //check for false doubles
   				 if((guess1==guess2 && guess1==guess3) && ((guess1 == led1 && guess1 == led2) || (guess1 == led2 && guess1 == led3) || (guess1 == led1 && guess1 == led3)))
   					 correctcolor++; //correction

   				 if (counts < countsmax) //if full time was not used
   				 {
   					 if (color == 0)
   						 pointsA = pointsA + (((counts*.00296)*1000*5)/tmax) + 1; //calculate points

   					 if (color == 1)
   						 pointsG = pointsG + (((counts/337)*1000*5)/tmax) + 1;
   				 }

   				 if (counts >= countsmax) //if full time was used
   				 {
   					 if (color == 0)
   						 pointsA = pointsA + 6; //max points per turn

   					 if (color == 1)
   						 pointsG = pointsG + 6;
   				 }

   				 counts = 0; //reset counts

   				 if (guess1 == 0)   //output guess in secure crt
   					 printf("\r0  ");

   				 if (guess1 == 1)
   					 printf("\rG  ");

   				 if (guess1 == 2)
   					 printf("\rR  ");

   				 if (guess2 == 0)
   					 printf("0  ");

   				 if (guess2 == 1)
   					 printf("G  ");

   				 if (guess2 == 2)
   					 printf("R  ");

   				 if (guess3 == 0)
   					 printf("0  ");

   				 if (guess3 == 1)
   					 printf("G  ");

   				 if (guess3 == 2)
   					 printf("R  ");


   				 printf("Correct color guesses: %d   ",correctcolor); //output number of correct colors and positions
   				 printf("Correct position guesses: %d  ",correctposition);
   				 if (color == 0)
   					 printf("Points: %d\n",pointsA); //display cumulative points

   				 if (color == 1)
   					 printf("Points: %d\n",pointsG);


   				 if (correctcolor == 0 && correctposition == 0) //if no correct colors or positions sound buzzer for 500 ms
   				 {
   					 TR0 = 1;
   					 counts = 0;
   					 while (counts < 170)
   						 BUZZER = 0;
   					 BUZZER = 1;
   					 TR0 = 0;
   					 counts = 0;
   				 }

   			 }


   			 if (color == 0)
   				 printf("\rCorrect! Points: %d\n",pointsA); //if correct congratulate player

   			 if (color == 1)
   				 printf("\rCorrect! Points: %d\n",pointsG);

   			 TR0 = 1; //enable timer
   			 counts = 0;
   			 while (counts<50) //5 short bursts
   				 BUZZER = 0;
   			 while (counts<100)
   				 BUZZER = 1;
   			 while (counts<150)
   				 BUZZER = 0;
   			 while (counts<200)
   				 BUZZER = 1;
   			 while (counts<250)
   				 BUZZER = 0;
   			 while (counts<300)
   				 BUZZER = 1;
   			 while (counts<350)
   				 BUZZER = 0;
   			 while (counts<400)
   				 BUZZER = 1;
   			 while (counts<450)
   				 BUZZER = 0;

   			 BUZZER = 1;
   			 color++; //increment color
   	 }
   	 }

   	 LED1 = 1; //turn leds off
   	 LED2 = 1;

   	 if (pointsA<pointsG)
   		 printf("\rAmber wins!\n"); //determine winner

   	 else if (pointsA>pointsG)
   		 printf("\rGreen wins!\n");

   	 else
   		 printf("\rTie!\n");

    }

}
//***************
void Port_Init(void)
{

	// Port 3
    P3MDOUT |= 0x7E; // set Port 3 output pins to push-pull mode


	// Port 2
    P2MDOUT &= 0x9F; //set Port 2 input to open drain
    P2 |= 0x9F; //Set Port 2 input to high impedance

    //Port 1
    P1MDIN &= ~0x01; //set P1.0 for analog input
    P1MDOUT &= ~0x01; //set port 1 input to open drain
    P1 |= 0x01; //set port 1 input to high impedance

    //Port 0
    P0MDOUT &= 0x10; //input open drain
    P0MDOUT &= 0x2C; //output to push pull
    P0 |= 0x01; //input high impedance

}

void Interrupt_Init(void)
{
	IE |= 0x02;  	// enable Timer0 Interrupt request
	EA = 1;   	// enable global interrupts
}
//***************
void Timer_Init(void)
{

	CKCON |= 0x08;  // Timer0 uses SYSCLK as source
	TMOD &= 0xF0;   // clear the 4 least significant bits
	TMOD |= 0x01;   // Timer0 in mode 1
	TR0 = 0;       	// Stop Timer0
	TL0 = 0;       	// Clear low byte of register T0
	TH0 = 0;       	// Clear high byte of register T0

}

//***************
void Timer0_ISR(void) __interrupt 1
{
	TF0 = 0;  // clear interrupt request
	counts++; // increment overflow counter
    bouncecounts++;
}

//***************
void ADC_Init(void)
{
    REF0CN = 0x03; //set Vref to use internal reference voltage (2.4V)
    ADC1CN = 0x80; //enable A/D converter (ADC1)
    ADC1CF |= 0x01; //set A/D converter gain to 1
}

unsigned char read_AD_input(unsigned char pin_number)
{
    AMX1SL = pin_number; //set P1.0 as the analog input for ADC1
    ADC1CN = ADC1CN & ~0x20; //clear conversion completed flag
    ADC1CN = ADC1CN | 0x10; //initiate A/D conversion
    while((ADC1CN & 0x20) == 0x00); //wait for conversion to complete

    return ADC1; //return digital value
}

//******************************************************************************


unsigned char random(void) //returns random number between 0-2
{
	return (rand()%3);
}


