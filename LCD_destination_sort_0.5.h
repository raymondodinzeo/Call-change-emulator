//As a hobbyist, I realise that this programme may not be particularly efficient
//however, my goal, as with all of my coding is to produce something that works and is 
//useful to me. This has been a very difficult project that has taken many forms
//and iterations but has resulted in me learning a great deal and has produced,
//something that I can actually use and is even better than I ever imagined it 
//could be. 
//I release this code under GPL so that anyone who wishes to can replicate this device
//as an aid to calling changes.
//======================================================================================  
//LCD DestinationSort is an attempt to produce a device that is a
//practical aid to instruct ringers in the changes necessary to reach fixed
//bell patterns such as tittums etc. The target patterns are held in arrays
//for 8, 6, 5 or 4 bells
//The LCD displays the previous order, the change instructions and the 
//current order of bells on an i2C 20 x 4 display. Two buttons scroll
//through the menus and progress the change instructions.
//The libray used is New LiquidCrystal_I2C.h, which works with arduino uno rev 3 
//=======================================================================================
//Bell Patterns
//* Back rounds: 543216 76543218
//* Bow bells:      13245768
//* Burdette: 213546 31247568
//* Expl Tittums: 342516 45362718
//* Hagdyke: 341256 1256347
//* Priory: 132546 13254768
//* Kings: 531246 75312468
//* Queens:135246  13572468 
//* Jacks: 145236 16745238
//* Jokers: 154326 17654328
//* Tittums:142536 15263748 
//* Whittington's: 531246 12753468 
//==========================================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
// set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27,20,4);
#define button1 2 //push button 1 pin interrupt #0
#define button2 3 //push button 2 pin interrupt #1
unsigned long buttonTime = 0; //timing for  button debounce
unsigned long lastButtonTime = 0;
uint8_t patternIndex = 0;
uint8_t buttonState1 = 1; //state of buttons (1, unpressed)
uint8_t buttonState2 = 1;
//=============================================================
void isr1() //interrupt service routines for buttons
{
  buttonTime = millis();
  if(buttonTime - lastButtonTime > 150) //debounce delay
  {
  buttonState1 = 0;
  lastButtonTime = buttonTime;
  }  
}
//==============================================================
void isr2()
{
  buttonTime = millis();
  if(buttonTime - lastButtonTime > 150)
  {
  buttonState2 = 0;  
  lastButtonTime = buttonTime;
  }
}
//===============================================================
//Arrays containing bell orders
uint8_t patterns8[14][9] = { {/*Back Rounds*/ 7, 6, 5, 4, 3, 2, 1, 8 },
{/*Bow bells*/ 1, 3, 2, 4, 5, 7, 6, 8 }, {/*Burdette*/ 3, 1, 2, 4, 7, 5, 6, 8 },
{/*Expl' tittums*/ 4, 5, 3, 6, 2, 7, 1, 8 }, {/*Hagdyke*/ 1, 2, 5, 6, 3, 4, 7, 8 },
{/*Rounds*/ 1, 2, 3, 4, 5, 6, 7, 8 }, {/*Priory*/ 1, 3, 2, 5, 4, 7, 6, 8 },
{/*Kings*/ 7, 5, 3, 1, 2, 4, 6, 8 }, {/*Queens*/ 1, 3, 5, 7, 2, 4, 6, 8 },
{/*Jacks*/ 1, 6, 7, 4, 5, 2, 3, 8 }, {/*Jokers*/ 1, 7, 6, 5, 4, 3, 2, 8 },
{/*Tittums*/ 1, 5, 2, 6, 3, 7, 4, 8 }, {/*Whittingtons*/ 1, 2, 7, 5, 3, 4, 6, 8 }};

uint8_t patterns6[14][7] = {{/*Back Rounds*/ 5, 4, 3, 2, 1, 6}, {/*Bow bells*/ 1, 3, 2, 5, 4, 6}, 
{/*Burdette*/ 2, 1, 3, 5, 4, 6}, {/*Expl' tittums*/ 3, 4, 2, 5, 1, 6},
{/*Hagdyke*/ 3, 4, 1, 2, 5, 6}, {/*Rounds*/ 1, 2, 3, 4, 5, 6 }, {/*Priory*/ 1, 3, 2, 5, 4, 6},
{/*Kings*/ 5, 3, 1, 2, 4, 6}, {/*Queens*/1, 3, 5, 2, 4, 6},{/*Jacks*/1, 4, 5, 2, 3, 6},
{/*Jokers*/1, 5, 4, 3, 2, 6}, {/*Tittums*/1, 4, 2, 5, 3, 6},{/*Whittingtons*/5, 3, 1, 2, 4, 6}};
//Array containing bell orders for destinations

uint8_t patterns4[14][5] = { {/*Back Rounds*/ 1, 3, 2, 4},
{/*Bow bells*/ 1, 4, 3, 2 }, {/*Burdette*/ 1, 4, 2, 3 },
{/*Expl' tittums*/ 1, 3, 4, 2 }, {/*Hagdyke*/ 1, 2, 4, 3},
{/*Rounds*/ 1, 2, 3, 4 }, {/*Priory*/ 3, 1, 2, 4 },
{/*Kings*/ 3, 2, 1, 4 }, {/*Queens*/ 2, 3, 1, 4 },
{/*Jacks*/ 2, 1, 3, 4}, {/*Jokers*/ 2, 1, 4, 3 },
{/*Tittums*/ 2, 4, 1, 3 }, {/*Whittingtons*/ 4, 2, 1, 3 }};

uint8_t patterns5[14][6] = { {/*Back Rounds*/ 1, 2, 5, 4, 3},
{/*Bow bells*/ 1, 4, 2, 3, 5 }, {/*Burdette*/ 1, 4, 2, 5, 3 },
{/*Expl' tittums*/ 1, 4, 5, 2, 3 }, {/*Hagdyke*/ 1, 3, 4, 2, 5},
{/*Rounds*/ 1, 2, 3, 4, 5 }, {/*Priory*/ 1, 5, 2, 4, 3 },
{/*Kings*/ 2, 1, 5, 4, 3 }, {/*Queens*/ 2, 4, 3, 5, 1 },
{/*Jacks*/ 3, 4, 1, 5, 2}, {/*Jokers*/ 3, 1, 5, 2, 4 },
{/*Tittums*/ 5, 3, 1, 2, 4 }, {/*Whittingtons*/ 5, 1, 4, 2, 3 }};

uint8_t Bells; //The number of bells currently in use
uint8_t numBells; 
//Indexes the pattern array, always one less than Bells (array index starts from zero)
int bells[9] = {1, 2, 3, 4, 5, 6, 7, 8}; //array holding bell order
int nextbells[9]; //intermediate store
int destination[9];//array holding the target bell order

uint8_t a; //variable to show that the bell being moved is in the correct place
uint8_t element; //operating point for destination array
uint8_t m; //operating point for Bells array
int*ptrBells; int*ptrNextBells; int *ptrDestination; //Pointers to bell arrays
//Function prototypes
void destinationSort(int [], int [], int [], int); //Sort function
void LCDprintArray(int [], int); // prints contents of array
void LCDinitBells(int [], int [], int); //writes bells elements to nextbells 
void LCDswapBells(int[], int[], int); //swaps the two bells corresponding to the third argument
void LCDinstructions(int[], int, int); //print instructions for bell swap
boolean LCDarray_cmp(int[], int[], int);//compare arrays
void destinationSort(int[], int[], int[], int);
