
#include "LCD_destination_sort_0.6.h" //header file for the program

void setup() 
{  
 lcd.init();
 lcd.backlight();
 //instructions to call button interrupt routines
 pinMode (button1, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(button1), isr1, FALLING);
 pinMode (button2, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(button2), isr2, FALLING);

 delay(50);
 Bells = GetNumBells(); //call function to get number of bells
numBells = Bells - 1; //numBells is the index for the arrays (arrays start from zero)
   
}
//==================================================================================
void loop() 
{
char* patternName[14]={"Back rounds", "Bow bells", "Burdette", "Expl Tittums", "Hagdyke", "Rounds",
"Priory", "Kings", "Queens", "Jacks", "Jokers", "Tittums", "Whittington's"}; //Pattern name array  

//==================================================================
delay(100);
patternIndex = GetPatternName(patternName); //Function to select pattern
 lcd.clear();  //Displays the number of bells and the pattern of bells
 lcd.setCursor(0, 0);
 lcd.print("Number of bells = ");
 lcd.print(Bells);
 lcd.setCursor(0, 1);
 lcd.print("Form = ");
 lcd.print(patternName[patternIndex]);
 lcd.setCursor(0, 2);
 lcd.print("Press scroll to ");
 lcd.setCursor(0, 3);
 lcd.print("Progress changes");
 //===================================================================

while(buttonState1 != 0) //Pauses until button pressed
  { 
   delay(100); 
  }
 delay(10);
 buttonState1 = 1;
lcd.clear();
for(uint8_t i = 0; i <= Bells; i++) //load individual bell positions into destination array
{
 
if(Bells == 8)  
  {
   destination[i] = patterns8[patternIndex][i]; 
  }
  else if(Bells == 6)  
  {
   destination[i] = patterns6[patternIndex][i]; 
  }
  else if(Bells == 5)  
  {
   destination[i] = patterns5[patternIndex][i]; 
  }
  else
   {
    destination[i] = patterns4[patternIndex][i];
    } 
}
ptrBells = &bells[0]; //assign pointers to array addresses
ptrNextBells = &nextbells[0];
ptrDestination = &destination[0];

destinationSort(ptrBells, ptrNextBells, ptrDestination, numBells); //invokes sorting algorythm
lcd.clear();
lcd.setCursor(0, 0); 
lcd.print("final order"); 
lcd.setCursor(0, 1); 
LCDprintArray(ptrDestination, numBells); //Shows bell order after sorting
delay(5000); //Display final order persistence for 5 seconds

}

//=====================================================================================
//Destination sort is a sort to change one sequence of bells to another.  It does     
//this by checking the final bell in the destination and finding the same bell in the 
//current order and moving the bell towards it's correct position to match the
//destination. The next to last bell is then checked and the sequence is repeated
//until all of the bells are in the correct position.
//=====================================================================================

void destinationSort(int ptrBells[], int ptrNextBells[], int ptrDestination[], int ds_Numbells)
{
LCDinitBells(ptrBells, ptrNextBells, numBells); //Writes Bells to Nextbells so both arrays are the same 
{
lcd.clear();
lcd.setCursor(2, 0); 
LCDprintArray(ptrBells, ds_Numbells); //Prints out Bells array to line 1 of display

while(LCDarray_cmp(ptrBells, ptrDestination, Bells) == false)
//while loop that exits When all of the bells are in the correct order
 {
 for(element = numBells; element>=1; element --)
 //For loop that counts down from the number of bells (-1) to 1. 
 //Element refers to the position of the bell being worked on in the destination array. 
  { 
  a = 0; //A is a value that is set to one when a bell is in the correct position.
  m = element; 
  //Assigns the starting point to look for the current element (Destination[]) in the Bells[] array
 while(a != 1) //while loop to find current "destination[]" element in current "bell[]" 
 //order
 {
 
 if(bells[m]!=destination[element])//find equivalent bell
   {  
    m--;
   }
   else
    {
     a = 1;   //exit loop with "m" at the correct value 
    } 
 }
 int b = (m); //assigns equivalent bell pos'n to variable "b"
 for(int j = b; j <= (element-1); j++)
 //loop to move selected bell "element" into correct position
 {
  lcd.setCursor(2, 0); 
LCDprintArray(ptrBells, ds_Numbells);//print order of bells before swap
  LCDswapBells(ptrBells, ptrNextBells, j); //move misplaced bell towards correct position
  LCDinstructions(ptrBells, j, ds_Numbells);//prints instructions to ringers for change
  lcd.setCursor(2, 3);
  LCDprintArray(ptrNextBells, ds_Numbells); //prints out order after swap
   
 while(buttonState1 != 0) //Pauses operation until button pressed
  { 
   delay(100); 
   }
 delay(10);
 buttonState1 = 1;
 }
 }
 }
}
} 
//==============================================================
//Writes the contents of the first array perameter into the second
//The third perameter reflects the number of elements in use in 
//the arrays
//==============================================================
void LCDinitBells(int a[], int b[], int numBells) 
{
  for(uint8_t l = 0; l <= numBells; l++)  //loop to print bell order
    {
    b[l] = a[l]; //ensures arrays are equal
    }
}
//=================================================================

//==============================================================
//swaps the array element corresponding to the third parameter
//with the next element
//the change is written to both arrays 
//==============================================================

void LCDswapBells(int FirstArr[], int SecondArr[], int Index) 
{
  SecondArr[Index+1] = FirstArr[Index]; //swap routine
        SecondArr[Index] = FirstArr[Index+1];
        FirstArr[Index] = SecondArr[Index];     //writes the newly exchanged bells into the current array
        FirstArr[Index+1] = SecondArr[Index+1];
}
//==============================================================
void LCDinstructions(int _insLCDFirstArr[], int _insLCDIndex, int _insLCDNumBells) 
{
 if (_insLCDIndex==0)
  {
  lcd.setCursor(2, 1);  
  lcd.print(_insLCDFirstArr[_insLCDIndex]); 
  lcd.print(" to "); 
  lcd.print("lead");
  lcd.print(", ");
  lcd.print(_insLCDFirstArr[_insLCDIndex+1]);
  lcd.print(" to "); 
  lcd.print(_insLCDFirstArr[_insLCDIndex]);
  lcd.print(", "); 
  lcd.setCursor(2, 2); 
  lcd.print(_insLCDFirstArr[_insLCDIndex+2]);
  lcd.print(" to ");int x = 0;  
  lcd.print(_insLCDFirstArr[_insLCDIndex+1]);
  lcd.print("          ");
  }
  else
  { 
  lcd.setCursor(2, 1);  
  lcd.print(_insLCDFirstArr[_insLCDIndex+1]);
  lcd.print(" to "); 
  lcd.print(_insLCDFirstArr[_insLCDIndex]);
  lcd.print(",  "); 
  lcd.print(_insLCDFirstArr[_insLCDIndex]);
  lcd.print(" to ");
  lcd.print(_insLCDFirstArr[_insLCDIndex-1]);
  lcd.print(",  ");
  if(_insLCDIndex == _insLCDNumBells-1) //Routine to catch if the last two bells are changed
  {
    lcd.setCursor(2, 2);
    lcd.print(_insLCDFirstArr[_insLCDIndex+1]);
    lcd.print(" at the back   ");
    lcd.println();
  }
    else
    {  
    lcd.setCursor(2, 2);  
    lcd.print(_insLCDFirstArr[_insLCDIndex+2]);
    lcd.print(" to ");
    lcd.print(_insLCDFirstArr[_insLCDIndex+1]);
    lcd.print("          ");  
    }

  }
}

//==============================================================
//Prints the contents of the array (first perameter)
//The second perameter reflects the number of elements in use in 
//the array
//==============================================================

void LCDprintArray(int c[], int d)
{
  
   for(uint8_t l = 0; l <= d; l++) //prints array contents
    {
    lcd.print(c[l]);
    lcd.print(" ");
    }
    
    
}

//==============================================================

//================================================================
//Function to compare two arrays
//Posted by ShadowX on Arduino forum "Thank's ShadowX"
//================================================================
boolean LCDarray_cmp(int *a, int *b, int len_a)
    {
         uint8_t n;

         // test each element to be the same. if not, return false
         for (n=0;n<len_a;n++) if (a[n]!=b[n]) return false;

         //ok, if we have not returned yet, they are equal :)
         return true;
    }

//============================================================================
uint8_t GetNumBells(void) //Function to select number of bells currently 6 or 8
{
 uint8_t _Bells[5] = {8, 6, 5, 4}; //Array containing all possible numbers of bells
 uint8_t j = 0;
 initLCDNum(0);
 while( buttonState2 != 0) //sets up a loop exited by pressing button 2
 {
  if(buttonState1 == 0)
  {
    j++;
    buttonState1 = 1;
    if(j == 4) {j = 0;}
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Number of bells =  ");
 lcd.setCursor(16, 0);
 lcd.print(_Bells[j]);
 lcd.setCursor(0, 1);
 lcd.print("Press scroll to");
 lcd.setCursor(0, 2);
 lcd.print("change then set to");
 lcd.setCursor(0, 3);
 lcd.print("enter");
    
  }
  delay(10); //will not work without this delay???
 }
buttonState2 = 1;
return _Bells[j]; //Returns the number of bells selected
}
//============================================================
uint8_t GetPatternName(char *array[]) //Function to select the pattern of bells for destination
{
 uint8_t k = 0;
 initLCDNum(1);
 while( buttonState2 != 0) //sets up a loop exited by pressing button 2
 {
  if(buttonState1 == 0)
  {
    k++;
    buttonState1 = 1;
    if(k == 13) {k = 0;}
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Form = ");
    //lcd.setCursor(16, 0);
    lcd.print(array[k]);
    lcd.setCursor(0, 1);
    lcd.print("Press scroll to");
    lcd.setCursor(0, 2);
    lcd.print("change then set to");
    lcd.setCursor(0, 3);
    lcd.print("enter");
  }
  delay(10); //will not work without this delay???
 }
 buttonState2 = 1;
return k; //Returns the bell form
}
//=============================================================
void initLCDNum(int z)
{//Screen charachters to set up number of bells and pattern
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Press scroll button");
 lcd.setCursor(0, 1);
 if (z == 0)
 {
 lcd.print("to select number of");
 lcd.setCursor(0, 2);
 lcd.print("ringers, then ");
 }
 else
{
 lcd.print("to select form of ");
 lcd.setCursor(0, 2);
 lcd.print("bell pattern, then "); 
  }
 
 lcd.setCursor(0, 3);
 lcd.print("press set to enter");  
}
//=============================================================
