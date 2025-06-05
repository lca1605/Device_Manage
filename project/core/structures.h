#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <hashTable.h>
#include <string>

using namespace std;

struct Device;
struct Tag;

enum RecordType {
    RETURNED,
    HISTORY
};

struct DateTime {
    int day;
    int month;
    int year;
    int hour;
    int minute;

    bool operator==(const DateTime& other) const {
        return day == other.day && month == other.month && year == other.year &&
               hour == other.hour && minute == other.minute;
    }

    bool operator<(const DateTime& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        if (day != other.day) return day < other.day;
        if (hour != other.hour) return hour < other.hour;
        return minute < other.minute;
    }

    bool operator>(const DateTime& other) const {
        return other < *this;
    }
};

struct currentBorrowRecord {
    string user;
    int MSSV;
    int brQuantity;
    Device* device = nullptr;

    bool operator<(const currentBorrowRecord& other) const {
        if (user < other.user) return true;
        if (user > other.user) return false;

        if (MSSV < other.MSSV) return true;
        if (MSSV > other.MSSV) return false;

        return brQuantity < other.brQuantity;
    }

    bool operator>(const currentBorrowRecord& other) const {
        return other < *this;
    }

    bool operator==(const currentBorrowRecord& other) const {
        return user == other.user &&
            MSSV == other.MSSV &&
            brQuantity == other.brQuantity &&
            device == other.device;
    }
};

struct BorrowAndReturnRecord {
    string user;
    int MSSV;
    int brQuantity;
    DateTime brDate;
    string deviceName;
    RecordType type;

    bool operator<(const BorrowAndReturnRecord& other) const {
        if (brDate < other.brDate) return true;
        if (brDate > other.brDate) return false;

        if (user < other.user) return true;
        if (user > other.user) return false;

        if (MSSV < other.MSSV) return true;
        if (MSSV > other.MSSV) return false;

        return brQuantity < other.brQuantity;
    }

    bool operator>(const BorrowAndReturnRecord& other) const {
        return other < *this;
    }

    bool operator==(const BorrowAndReturnRecord& other) const {
        return brDate == other.brDate &&
            user == other.user &&
            MSSV == other.MSSV &&
            brQuantity == other.brQuantity &&
            deviceName == other.deviceName;
    }
};

struct Device {
    string name;
    int quantity = 1;

    //DynamicArray BorrowRecord
    currentBorrowRecord** BList = nullptr;
    int Bcount = 0;
    int Bcapacity = 0;

    //DynamicArray Tag
    Tag** TList = nullptr;
    int Tcount = 0;
    int Tcapacity = 0;

    ~Device() {
        delete[] BList;
        delete[] TList;
    }

    bool operator<(const Device& other) const {
        return name < other.name;
    }
    bool operator>(const Device& other) const {
        return other < *this;
    }
    bool operator==(const Device& other) const {
        return name == other.name;
    }
};

struct Tag {
    string name;

    //DynamicArray<Device> devices;
    Device** DList = nullptr;
    int Dcount = 0;
    int Dcapacity = 0;

    ~Tag() {
        delete[] DList;
    }


    bool operator<(const Tag& other) const {
        return name < other.name;
    }
    bool operator>(const Tag& other) const {
        return other < *this;
    }
    bool operator==(const Tag& other) const {
        return name == other.name;
    }
};

//Advance search for device helper
struct AdvanceSearch {
    int min;
    int max;
    string tag_line;
};

#endif