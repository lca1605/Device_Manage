#ifndef UTILS_H
#define UTILS_H

#include "structures.h"
#include "avltree.h"

#include <string>

using namespace std;


//Time helper
DateTime getCurrentDateTime();
bool isWithinLast30Days(const DateTime& dt);

// String helper
string trim(const string& s);
string toLowerCase(const string& input);

// Input helper
bool menuReturn(string key);
bool getNonEmptyString(string& result, const string& prompt, bool allowEmpty = false);
bool getOptionalString(string& result, const string& prompt, bool allowEmpty = false);
int getValidatedIntForChoice(const string& prompt, int minValue, int maxValue);
int getValidatedInt(const string& prompt, int minValue);
char getYesNo(const string& prompt);

// Find by approx device name
Device* getDeviceByName();

// Dynamic array process
void linkTagtoDevice(Device* device, Tag* tag);
void removeCurrentBorrowRecordFromDevice(Device* device, currentBorrowRecord* record);
void linkBorrowRecordtoDevice(Device* device, currentBorrowRecord* borrowRecord);
bool linkTagLinetoDevice(Device* device);
void deleteRecordByDevice(AVLNode<BorrowAndReturnRecord>* node, Device* deletedDevice);

// Print helper
template <typename T, typename K>
int printInOrderGenericByKey(AVLNode<T>* node, int (*printFunc)(const T&, const K&), const K& key) {
    if (!node) return 0;
    int founded = 0;
    founded += printInOrderGenericByKey(node->left, printFunc, key);
    founded += printFunc(node->data, key);
    founded += printInOrderGenericByKey(node->right, printFunc, key);
    return founded;
}

// Print by key
template <typename T>
int printList(const T& t, const string& nokey) {
    printForm(t);
    return 1;
}
template <typename T>
int printByApproxName(const T& t, const string& keyword) {
    string nameLower = toLowerCase(t.name);
    if (nameLower.find(keyword) != string::npos) {
        printForm(t);
        return 1;
    } else {
        return 0;
    }
}
int printDeviceByMultiKey(const Device& device, const AdvanceSearch& key);
int printByType(const BorrowAndReturnRecord& br, const RecordType& type);

#endif
