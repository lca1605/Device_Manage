#include "utils.h"
#include "avltree.h"
#include "allTree.h"
#include "display.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cctype>

using namespace std;

//------------------------------------------
//Time helper
//------------------------------------------

DateTime getCurrentDateTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return {ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900,
            ltm->tm_hour, ltm->tm_min};
}


bool isWithinLast30Days(const DateTime& dt) {
    time_t now = time(0);
    tm ltm_now = *localtime(&now);

    tm record_tm = {};
    record_tm.tm_mday = dt.day;
    record_tm.tm_mon = dt.month - 1;
    record_tm.tm_year = dt.year - 1900;
    record_tm.tm_hour = dt.hour;
    record_tm.tm_min = dt.minute;
    record_tm.tm_sec = 0;

    time_t record_time = mktime(&record_tm);
    double diff = difftime(now, record_time);

    return diff <= 30 * 24 * 60 * 60;
}

//------------------------------------------
//String helper
//------------------------------------------

string trim(const string& s) {
    auto start = s.begin();
    while (start != s.end() && isspace(*start)) start++;
    auto end = s.end();
    do {
        end--;
    } while (distance(start, end) > 0 && isspace(*end));
    return string(start, end + 1);
}

string toLowerCase(const string& input) {
    string result = input;
    for (char& c : result) {
        if (c >= 'A' && c <= 'Z') {
            c = c + ('a' - 'A');
        }
    }
    return result;
}

//------------------------------------------
//Input helper
//------------------------------------------

bool menuReturn(string key) { //Check for return command
    return key == "./startmenu";
}

bool getNonEmptyString(string& result, const string& prompt, bool allowEmpty) {
    bool loop = false;
    while (true) {
        cout << prompt;
        getline(cin, result);
        result = trim(result);

        //Check for return command
        if (menuReturn(result)) return false;

        //Input again  if input emty string
        if (!allowEmpty && result.empty()) {
            loop = true;
            cout << "   " << warning("Invalid input") << " Input cannot be empty.\n";
            cout << "\033[F\033[F\033[2K";
            continue;
        }

        //Clear warning
        if (loop) cout << "\033[2K";
        
        return true;
    }
}

bool getOptionalString(string& result, const string& prompt, bool allowEmpty) {
    cout << prompt;
    getline(cin, result);
    result = trim(result);

    //Check for return command
    if (menuReturn(result)) return false;

    return true;
}

int getValidatedIntForChoice(const string& prompt, int minValue, int maxValue) { //Get int input for range
    int value;
    bool loop = false;
    while (true) {
        string input;

        //Check for return command and empty input
        if (!getNonEmptyString(input, prompt, true)) return -1;

        //Check for valid input number
        stringstream ss(input);
        if (ss >> value && ss.eof() && value >= minValue && value <= maxValue) {
            if (loop) cout << "\033[2K";
            break;
        }
        loop = true;
        cout << "   " << warning("Invalid input") << " Please enter an integer between " << minValue << " and " << maxValue << ".\n";
        cout << "\033[F\033[F\033[2K";
    }

    return value;
}

int getValidatedInt(const string& prompt, int minValue) { //Get int input with minimum value
    int value;
    bool loop = false;
    while (true) {
        string input;

        //Check for return command and empty input
        if (!getNonEmptyString(input, prompt, true)) return -1;

        //Check for valid input number
        stringstream ss(input);
        if (ss >> value && ss.eof() && value >= minValue) {
            if (loop) cout << "\033[2K";
            break;
        }
        loop = true;
        cout << "   " << warning("Invalid input") << " Please enter an integer greater than " << minValue << ".\n";
        cout << "\033[F\033[F\033[2K";
    }

    return value;
}

char getYesNo(const string& prompt) {
    string input;

    //Check for return commend
    if (!getNonEmptyString(input, prompt, true)) return 'b';

    //Check for answer
    if (input.length() == 1) {
        char c = tolower(input[0]);
        cout << "\033[F\033[2K";
        if (c == 'y') return 'y';
    }

    //Any other answer considered 'n'
    return 'n';
}

//------------------------------------------
//Find device by approx name
//------------------------------------------

//Create dynamic array to save device infor
Device** matched = nullptr;
int matchedCount = 0;
int maxDevices = 0;

void searchApprox(AVLNode<Device>* node, const string& keyword) { //Recersion function to searching all device tree
    if (!node || matchedCount >= maxDevices) return;

    searchApprox(node->left, keyword);

    //Check approx
    string devName = toLowerCase(node->data.name);
    if (devName.find(keyword) != string::npos) {
        matched[matchedCount++] = &node->data;
    }

    searchApprox(node->right, keyword);
}

Device* getDeviceByName() {
    //Get keyword
    string keyword;
    if (!getNonEmptyString(keyword, "  Device name: ")) {
        return nullptr;
    }

    //Check for exact name
    Device* exact = deviceTree.search(Device{keyword});
    if (exact) {
        delete[] matched;
        matched = nullptr;
        return exact;
    }

    //Create new dynamic array
    matchedCount = 0;
    maxDevices = deviceTree.countNodes();
    matched = new Device*[maxDevices];

    //Check for approx name by all device
    keyword = toLowerCase(keyword);
    searchApprox(deviceTree.getRoot(), keyword);

    //Warning no device have that name
    drawLine(1);
    if (matchedCount == 0) {
        cout << "  " << centerText(warning("No matching device found"), 41);
        delete[] matched;
        matched = nullptr;
        wait();
        return nullptr;
    }

    //Print all device approx
    cout << "  " << highlight("Matching Devices") << "\n";
    for (int i = 0; i < matchedCount; i++) {
        cout << "  " << (i + 1) << ". " << matched[i]->name
             << " (quantity: " << matched[i]->quantity << ")\n";
    }

    //Choose device by number
    int choice = getValidatedIntForChoice("  Choose device by number: ", 1, matchedCount);
    for (int i = 0; i < matchedCount + 4; i++) {
        cout << "\033[F\033[2K";
    }

    //If input return commend
    if (choice == -1) {
        delete[] matched;
        matched = nullptr;
        return nullptr;
    }

    //Return choosed device
    Device* selected = matched[choice - 1];
    cout << "  Device name: " << selected->name << "\n";
    delete[] matched;
    matched = nullptr;
    return selected;
}


//------------------------------------------
//Dynamic array process
//------------------------------------------

template <typename T>
void resizeArray(T*& arr, int& capacity, int count) { //Expand dynamic array
    int newCap = capacity == 0 ? 2 : capacity * 2;
    T* newArr = new T[newCap];
    for (int i = 0; i < count; i++) {
        newArr[i] = arr[i];
    }
    delete[] arr;
    arr = newArr;
    capacity = newCap;
}

void linkTagtoDevice(Device* device, Tag* tag) {
    //Check duplicate tag
    for (int i = 0; i < device->Tcount; i++) {
        if (device->TList[i] == tag) {
            cout << "Tag [" << tag->name << "] already linked with " << device->name << ".\n";
            return;
        }
    }

    //Add tag to tag list in device
    if (device->Tcount == device->Tcapacity) {
        resizeArray(device->TList, device->Tcapacity, device->Tcount);
    }
    device->TList[device->Tcount++] = tag;

    //Add device to device list in tag
    if (tag->Dcount == tag->Dcapacity) {
        resizeArray(tag->DList, tag->Dcapacity, tag->Dcount);
    }
    tag->DList[tag->Dcount++] = device;
}

void removeCurrentBorrowRecordFromDevice(Device* device, currentBorrowRecord* record) {
    if (!device || !record || device->BList == nullptr || device->Bcount == 0) return;

    for (int i = 0; i < device->Bcount; ++i) {
        if (device->BList[i] == record) {
            // Xoá phần tử khỏi mảng
            delete device->BList[i];  // Giải phóng vùng nhớ record
            for (int j = i; j < device->Bcount - 1; ++j) {
                device->BList[j] = device->BList[j + 1];
            }
            device->Bcount--;
            return;
        }
    }
}

void linkBorrowRecordtoDevice(Device* device, currentBorrowRecord* borrowRecord) {
    //Add Borrow Record to Borrow Record List in Device
    if (device->Bcount == device->Bcapacity) {
        resizeArray(device->BList, device->Bcapacity, device->Bcount);
    }
    device->BList[device->Bcount++] = borrowRecord;

    //Add Device to Borrow Record
    borrowRecord->device = device;
}

bool linkTagLinetoDevice(Device* device) {
    //Get tag line
    string tag_line;
    cout << "  Tags (Tag 1, Tag 2,...): ";
    getline(cin, tag_line);
    tag_line = trim(tag_line);
    if (menuReturn(tag_line)) return true;

    //Tag line process
    stringstream ss(tag_line);
    string tag;
    while (getline(ss, tag, ',')) {
        tag = trim(tag);
        if (tag == "./startmenu")  {
            cout << "  Tag name can not be ./startmenu\n";
            continue;
        }

        //Check duplicate
        Tag* findTag = tagTree.search(Tag{tag});
        if (findTag != nullptr) {
            linkTagtoDevice(device, findTag);
        } else if (!tag.empty()) {
            //Created new tag
            cout << "  Tag \033[1;33m[" << tag << "]\033[0m doesn't exist.";
            char choice = getYesNo(" Do you want to create new tag? (y/n): ");
            if (choice == 'b') return true;
            if (choice == 'y') {
                Tag T;
                T.name = tag;
                Tag* insertedTag = tagTree.insert(T);
                if (insertedTag != nullptr) {
                    linkTagtoDevice(device, insertedTag);
                    cout << "  Tag " << highlight(tag) <<" created and linked.\n";
                }
            } else {
                cout << "  Tag " << warning(tag) <<" not linked.\n";
            }
        }
    }

    return false;
}

//------------------------------------------
//Print by key
//------------------------------------------

int printDeviceByMultiKey(const Device& device, const AdvanceSearch& key) {
    if (!((key.max == 0 || device.quantity <= key.max) && device.quantity >= key.min)) {
        return 0;
    }

    stringstream ss(key.tag_line);
    string tag;
    int all_tag_founded = 1;
    while (getline(ss, tag, ',')) {
        tag = trim(tag);
        if (tag == "./startmenu")  {
            cout << "Tag name can not be ./startmenu";
            continue;
        }
        int tag_founded = 0;
        for (int i = 0; i < device.Tcount; i++) {
            if (device.TList[i]->name == tag) {
                tag_founded = 1;
                break;
            }
        }
        if (tag_founded == 0) {
            all_tag_founded = 0;
            break;
        }
    }

    if (all_tag_founded == 1) {
        printForm(device);
        return 1;
    }

    return 0;
}

int printByType(const BorrowAndReturnRecord& record, const RecordType& type) {
    if (record.type == type) {
        printForm(record);
        return 1;
    }
    return 0;
}