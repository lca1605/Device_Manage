#ifndef DISPLAY_H
#define DISPLAY_H

#include "structures.h"
#include <string>

using namespace std;

//Display helper
void clearScreen();
void wait();
void drawLine(int type);

//String formatting
string centerText(const string& text, int width);
string warning(const string& text);
string confirm(const string& text);
string highlight(const string& text);

//Table header formatting
void printDeviceHeader();
void printTagHeader();
void printBRRecordHeader();
void printCBRecordHeader();
void clearHeader();

//Print form of entities
void printForm(const Device& device);
void printForm(const Tag& tag);
void printForm(const BorrowAndReturnRecord& br);
void printForm(const currentBorrowRecord& br);

#endif