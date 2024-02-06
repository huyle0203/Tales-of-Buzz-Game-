//=================================================================
// The speech header file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

#ifndef SPEECH_H
#define SPEECH_H

/**
 * Display a speech bubble.
 */
void speech(const char* line1, const char* line2);
void speech2(const char* line1, const char* line2);
void menu_page();
void start_page();
void ctrl_page();
void skill_page();
void story_page();
void quiz_page();
/**
 * Display a long speech bubble, with scrolling.
 * 
 * @param lines The actual lines of text to display
 * @param n The number of lines to display.
 */
void long_speech(const char* lines[], int n);
// void end_speech();

#endif // SPEECH_H
