LCD DestinationSort is an attempt to produce a device that is a
practical aid to instruct ringers in the changes necessary to reach fixed
bell patterns such as tittums etc. The target patterns are held in arrays
for 8, 6, 5 or 4 bells
The LCD displays the previous order, the change instructions and the 
current order of bells on an i2C 20 x 4 display. Two buttons scroll
through the menus and progress the change instructions.
The libray used is New LiquidCrystal_I2C.h, which works with arduino uno rev 3 
Here is a video showing the device in action https://youtube.com/shorts/_Vu4FqlqnnY
To operate:
select the number of bells in use
Select the bell pattern you wish to move towards
Press a button to make one change at a time until the pattern is achieved.
When the pattern is acheived, the unit displays the final order for 5 seconds
Then you can press the button and the changes will go back to rounds
