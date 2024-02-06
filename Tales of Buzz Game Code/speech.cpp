//=================================================================
// The speech class file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

#include "speech.h"

#include "globals.h"
#include "hardware.h"

///////////////////////////////
//Static function declarations
///////////////////////////////
/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);


///////////////////////////////
//Drawing function declarations
///////////////////////////////


//***
// HINT: for this function and below: Check out the ULCD demo in the docs to see what
// uLCD methods you can use to draw graphics 

//Link: https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/
//***


void draw_speech_bubble()
{
    //****************
    // TODO: Implement
    //****************
    
    //1. Draw a speech bubble at the bottom of the screen 
    // Hint: You can stack ULCD rectangles to make a border
    // uLCD.rectangle(0, 70, 120, 110, GREEN);
    // uLCD.filled_rectangle(1, 71, 119, 109, BLACK);
    int screen_h = 160; // Set screen height = pixles
    int lowY = (screen_h * 6) / 10; 
    uLCD.filled_rectangle(0, lowY, 127, 159, BLACK);
}

// void draw_end_speech_bubble()
// {
//     //****************
//     // TODO: Implement
//     //****************
    
//     //1. Draw a speech bubble at the bottom of the screen 
//     // Hint: You can stack ULCD rectangles to make a border
//     uLCD.rectangle(0, 1, 120, 119, GREEN);
//     uLCD.filled_rectangle(1, 2, 119, 118, BLACK);
// }

void erase_speech_bubble()
{
    //****************
    // TODO: Implement
    //****************
    
    //1. Erase the speech bubble at the bottom of the screen
    // Hint: You can overwrite the bubble with a empty (black) one
    // uLCD.filled_rectangle(0, 70, 120, 110, BLACK);
    int screen_h = 160; // Set screen height = pixles
    int lowY = (screen_h * 6) / 10; 
    uLCD.filled_rectangle(0, lowY, 127, 159, BLACK);
}

void erase_health_speech_bubble(){
    int screen_h = 160; // Set screen height = pixles
    int lowY = (screen_h * 6) / 10; 
    uLCD.filled_rectangle(0, lowY, 127, 159, BLACK);
}

void draw_speech_line(const char* line, int which)
{
    //****************
    // TODO: Implement
    //****************
    //1. Set the location which line of text will go the uLCD
    // Hint: Change the y coordinate based on which line (top or bottom)

    //2. For each character in the text, write it to uLCD
    for (int i = 0; line[i] && i < 17; i++) //We can simplify by limiting each line to 17 char
    {
        // uLCD.text_char(line[i], (char) i, (char) 10 + which, WHITE);
        uLCD.background_color(WHITE);
        uLCD.text_char(line[i], i, 13 + which, 0xFF0000);
        //TODO: Write the character
        wait_ms(30); 
    }
}

void speech_bubble_wait()
{
    //****************
    // TODO: Implement
    //****************
    
    // 1. Keep waiting until the action button is pressed 
    //   Hint: What type of loop can we use to stay until a condition is met?
    GameInputs inputs = read_inputs();
    while (!inputs.b2) {
        inputs = read_inputs();
        Timer c; 
        c.start();
        if (c.read_ms() > 1000000) {
            c.stop();
            break;
        }
    }
}

void speech(const char* line1, const char* line2)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Place the two lines into an array of lines
    // 2. Pass in to long_speech with the corresponding number of lines
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();
}

void speech2(const char* line1, const char* line2)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Place the two lines into an array of lines
    // 2. Pass in to long_speech with the corresponding number of lines
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();
}
// void end_speech() {
//     draw_end_speech_bubble();
//     draw_speech_line("Game", TOP);
//     draw_speech_line("Completed!", BOTTOM);
//     speech_bubble_wait();
//     // main();
// }
// void long_speech(const char* lines[], int n)
// {

//     //****************
//     // TODO: Implement
//     //****************

//     //1. Create a speech bubble
//     int len;
//     if (n % 2 == 0) {
//         len = n;
//     } else {
//         len = n - 1;
//     }
//     for (int i = 0; i < len; i++) {
//         draw_speech_bubble();
//         speech(lines[i], lines[i+1]);
//         i++;
//     }
//     if (len == n - 1) {
//         draw_speech_bubble();
//         speech(lines[n-1], "");
//     }
//     speech_bubble_wait();
//     erase_speech_bubble();
//     //2. For each lines, display only two lines at a time
//     //   If two lines are displayed, make sure to wait (call the wait function)

//     //3. Erase the speech bubble when you are done

// }

void long_speech(const char* lines[], int n)
{ 
    //****************

    //1. Create a speech bubble

    //2. For each lines, display only two lines at a time
    //   If two lines are displayed, make sure to wait (call the wait function)

    //3. Erase the speech bubble when you are done

    //****************
    for (int i = 0; i < n; i = i + 2) {
    draw_speech_bubble();
    draw_speech_line(lines[i], 0);

    if (i + 1 < n) {
    draw_speech_line(lines[i + 1], 1);
    } 
    speech_bubble_wait();
    erase_speech_bubble();
}
}

void menu_page() {
    uLCD.cls();
    uLCD.text_string((char*) "~~~~~~MENU~~~~~~", 1, 2, FONT_12X16, WHITE);
    uLCD.text_string((char*) "CONT. (UP)", 1, 4, FONT_8X12, WHITE);
    uLCD.text_string((char*) "SKILLS (RIGHT)", 1, 6, FONT_8X12, WHITE);
    uLCD.text_string((char*) "RESTART (DOWN)", 1, 8, FONT_8X12, WHITE);
}

void start_page() {
    uLCD.cls();
    uLCD.text_string((char*) "~~~~~~MENU~~~~~~", 1, 2, FONT_12X16, WHITE);
    uLCD.text_string((char*) "1.CTRL (UP)", 1, 4, FONT_8X12, WHITE);
    uLCD.text_string((char*) "2.STORY (RIGHT)", 1, 6, FONT_8X12, WHITE);
    uLCD.text_string((char*) "3.START (DOWN)", 1, 8, FONT_8X12, WHITE);
}

void ctrl_page() {
    uLCD.cls();
    uLCD.text_string((char*) "~~~~~~CTRL~~~~~~", 1, 2, FONT_12X16, WHITE);
    uLCD.text_string((char*) "1.Move (NavSwitch)", 1, 4, FONT_8X12, WHITE);
    uLCD.text_string((char*) "2.Show Menu (Menu Button)", 1, 6, FONT_8X12, WHITE);
    uLCD.text_string((char*) "a.Sacred Power (Red)", 1, 8, FONT_8X12, RED);
    uLCD.text_string((char*) "b.Angel Blessing (Yellow)", 1, 10, FONT_8X12, 0xFFFF00);
    uLCD.text_string((char*) "c.Intsta Kill (Green)", 1, 12, FONT_8X12, GREEN);
    uLCD.text_string((char*) "d.Speed Up (Blue)", 1, 14, FONT_8X12, BLUE);
}

void skill_page() {
    uLCD.cls();
    uLCD.text_string((char*) "~~~~~~SKILLS~~~~~~", 1, 2, FONT_12X16, RED);
    uLCD.text_string((char*) "1.SACRED POWER (UP)", 1, 4, FONT_8X12, WHITE);
    uLCD.text_string((char*) "2.ANGEL BLESSING (RIGHT)", 1, 6, FONT_8X12, 0xFFFF00);
    uLCD.text_string((char*) "3.INSTA KILL (DOWN)", 1, 8, FONT_8X12, GREEN);
    uLCD.text_string((char*) "4.SPEED UP (LEFT)", 1, 10, FONT_8X12, BLUE);
}

void story_page() {
    uLCD.cls();
    uLCD.text_string((char*) "~~~~~~STORY~~~~~~", 1, 2, FONT_12X16, RED);
    uLCD.text_string((char*) "The story is long,", 1, 4, FONT_8X12, WHITE);
    uLCD.text_string((char*) "I dont wanna say.", 1, 6, FONT_8X12, WHITE);
    uLCD.text_string((char*) "Omg u r still here.", 1, 8, FONT_8X12, WHITE);
    uLCD.text_string((char*) "Press button to play.", 1, 10, FONT_8X12, WHITE);
}

void quiz_page() {
    uLCD.cls();
    uLCD.text_string((char*) "~~~~~~QUIZ~~~~~~", 1, 2, FONT_12X16, GREEN);
    uLCD.text_string((char*) "RD is a GOAT GTA", 1, 4, FONT_8X12, RED);
    uLCD.text_string((char*) "TRUE      FALSE", 1, 6, FONT_8X12, WHITE);
    uLCD.text_string((char*) "(LEFT)   (RIGHT)", 1, 8, FONT_8X12, WHITE);
}


