#include <iostream>
#include <string>
#include <iomanip>

#include <display.h>

using namespace std;

//------------------------------------------
//Display helper
//------------------------------------------

void clearScreen() {
    cout << "\033[2J\033[H";
}

void wait() {
    string wait;
    getline(cin, wait);
}

void drawLine(int type) {
    if (type == 1) {
        cout << "|--------------------------------------|\n";
    } else {
        cout << "  " << string(type, '-') << "\n";
    }
}

//------------------------------------------
//String formatting
//------------------------------------------

string centerText(const string& text, int width) {
    int padding = width - text.length();
    if (padding <= 0) return text;
    int left = padding / 2;
    int right = padding - left;
    return string(left, ' ') + text + string(right, ' ');
}

string warning(const string& text) {
    return "\033[1;31m[" + text + "]\033[0m";
}

string confirm(const string& text) {
    return "\033[1;32m[" + text + "]\033[0m";
}

string highlight(const string& text) {
    return "\033[1;33m[" + text + "]\033[0m";
}

//------------------------------------------
//Table header formatting
//------------------------------------------

void printDeviceHeader() {
    drawLine(87);
    cout << "  " << centerText("Display", 33) << "|" << centerText("Quantity", 11) << "|" << centerText("Tags", 41) << "\n";
    drawLine(87);
}

void printTagHeader() {
    drawLine(66);
    cout << "  " << centerText("Tag name", 34) << "|" << centerText("QNumber of device linked", 30) << "\n";
    drawLine(66);
}

void printBRRecordHeader() {
    drawLine(91);
    cout << "  " << centerText("User", 29) << "|" << centerText("Device", 35) << "|" << centerText("Date", 25) << "\n";
    drawLine(91);
}

void printCBRecordHeader() {
    drawLine(65);
    cout << "  " << centerText("User", 29) << "|" << centerText("Device", 35) << "\n";
    drawLine(65);
}

void clearHeader() {
    cout << "\033[F\033[2K\033[F\033[2K\033[F\033[2K";
}

//------------------------------------------
//Print form of entities
//------------------------------------------

void printForm(const Device& device) {
    string line = "";
    bool firstLine = true;

    if (device.Tcount == 0) {
        line = "[No tag] ";
    } else {
        for (int i = 0; i < device.Tcount; i++) {
            string tag = "[" + device.TList[i]->name + "] ";

            if (line.length() + tag.length() <= 39) {
                line += tag;
            } else {
                if (firstLine) {
                    cout << "   " << left << setw(31) << device.name
                         << " | " << centerText(to_string(device.quantity), 9)
                         << " | " << left << setw(39) << line << endl;
                    firstLine = false;
                } else {
                    cout << "   " << setw(31) << " "
                         << " | " << setw(9) << " "
                         << " | " << left << setw(39) << line << endl;
                }
                line = tag;
            }
        }
    }

    if (!line.empty()) {
        if (firstLine) {
            cout << "   " << left << setw(31) << device.name
                 << " | " << centerText(to_string(device.quantity), 9)
                 << " | " << left << setw(39) << line << endl;
        } else {
            cout << "   " << setw(31) << " "
                 << " | " << setw(9) << " "
                 << " | " << left << setw(39) << line << endl;
        }
    }

    drawLine(87);
}

void printForm(const Tag& tag) {
    cout << "   " << left << setw(31) << ("Tag: " + tag.name) << " | " << centerText(to_string(tag.Dcount), 31) << "\n";
    drawLine(66);
}

void printForm(const BorrowAndReturnRecord& br) {
    stringstream dateStream, timeStream;
    dateStream << setfill('0') << setw(2) << br.brDate.day << "/"
               << setw(2) << br.brDate.month << "/"
               << setw(4) << br.brDate.year;
    string dateStr = dateStream.str();
    timeStream << setfill('0') << setw(2) << br.brDate.hour << ":"
               << setw(2) << br.brDate.minute;
    string timeStr = timeStream.str();
    cout << "   " << left << setw(27) << br.user
         << " | " << setw(33) << right << br.deviceName
         << " | " << setw(24) << right << dateStr << "\n"
         << "   " << left << setw(27) << br.MSSV
         << " | " << setw(33) << right << br.brQuantity
         << " | " << setw(24) << right << timeStr << "\n";
    drawLine(91);
}


void printForm(const currentBorrowRecord& br) {
    cout << "   " << left << setw(26) << br.user
         << " | " << setw(34) << right << br.device->name << "\n"
         << "   " << left << setw(26) << br.MSSV
         << " | " << setw(34) << right << br.brQuantity << "\n";
    drawLine(65);
}