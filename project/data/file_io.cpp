#include "file_io.h"
#include "structures.h"
#include "avltree.h"
#include "utils.h"
#include "allTree.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

//------------------------------------------
//Save helper
//------------------------------------------

void inorderSaveDevices(AVLNode<Device>* node, ofstream& ofs) {
    if (!node) return;
    inorderSaveDevices(node->left, ofs);
    const Device& device = node->data;

    ofs << device.name << "\n";
    ofs << device.quantity << "\n";
    ofs << device.Bcount << "\n";

    for (int i = 0; i < device.Bcount; i++) {
        ofs << device.BList[i]->user << "\n";
        ofs << device.BList[i]->MSSV << "\n";
        ofs << device.BList[i]->brQuantity << "\n";
    }

    inorderSaveDevices(node->right, ofs);
}

void inorderSaveTags(AVLNode<Tag>* node, ofstream& ofs) {
    if (!node) return;
    inorderSaveTags(node->left, ofs);
    const Tag& tag = node->data;

    ofs << tag.name << "\n";
    for (int i = 0; i < tag.Dcount; i++) {
        ofs << tag.DList[i]->name;
        if (i != tag.Dcount - 1) {
            ofs << ", ";
        }
    }
    ofs << "\n";

    inorderSaveTags(node->right, ofs);
}

void inorderSaveBorrowAndReturnRecords(AVLNode<BorrowAndReturnRecord>* node, ofstream& ofs) {
    if (!node) return;
    inorderSaveBorrowAndReturnRecords(node->left, ofs);
    const BorrowAndReturnRecord& br = node->data;

    ofs << br.user << "\n";
    ofs << br.type << "\n";
    ofs << br.MSSV << "\n";
    ofs << br.brQuantity << "\n";
    ofs << br.brDate.day << " " << br.brDate.month << " " << br.brDate.year << " "
        << br.brDate.hour << " " << br.brDate.minute << "\n";
    ofs << br.deviceName << "\n";

    inorderSaveBorrowAndReturnRecords(node->right, ofs);
}

//------------------------------------------
//Save 
//------------------------------------------

void saveToFile() {
    ofstream ofs("project/data/device.txt");
    if (!ofs) {
        cerr << "Cannot open file to write.\n";
        return;
    }

    inorderSaveDevices(deviceTree.getRoot(), ofs);
    ofs << "---\n";
    inorderSaveTags(tagTree.getRoot(), ofs);
    ofs << "---\n";
    inorderSaveBorrowAndReturnRecords(recordTree.getRoot(), ofs);
    ofs << "---\n";
}

void loadFromFile() {
    ifstream ifs("project/data/device.txt");
    string line;

    // ----- Device -----
    while (getline(ifs, line)) {
        if (line == "---") break;
        Device device;
        device.name = line;
        ifs >> device.quantity;
        ifs >> device.Bcount;
        device.Bcapacity = device.Bcount + 10;
        ifs.ignore();
        Device* insertedDevice = deviceTree.insert(device);
        insertedDevice->BList = new currentBorrowRecord*[insertedDevice->Bcapacity];
        
        for (int i = 0; i < device.Bcount; i++) {
            currentBorrowRecord* record = new currentBorrowRecord;

            getline(ifs, record->user);
            ifs >> record->MSSV;
            ifs >> record->brQuantity;
            ifs.ignore();

            record->device = insertedDevice;
            insertedDevice->BList[i] = record;
        }
    }

    // ----- Tag -----
    while (getline(ifs, line)) {
        if (line == "---") break;
        Tag tag;
        tag.name = line;
        Tag* insertedTag = tagTree.insert(tag);

        getline(ifs, line);
        stringstream ss(line);
        string Dname;
        while (getline(ss, Dname, ',')) {
            Dname = trim(Dname);
            Device* findDevice = deviceTree.search(Device{Dname});
            if (findDevice) {
                linkTagtoDevice(findDevice, insertedTag);
            }
        }
    }

    // ----- Borrow History -----
    while (getline(ifs, line)) {
        if (line == "---") break;
        BorrowAndReturnRecord br;
        br.user = line;
        int typeInt;
        ifs >> typeInt;
        br.type = static_cast<RecordType>(typeInt);
        ifs >> br.MSSV >> br.brQuantity;
        ifs >> br.brDate.day >> br.brDate.month >> br.brDate.year >> br.brDate.hour >> br.brDate.minute;
        ifs.ignore();
        if (!isWithinLast30Days(br.brDate)) continue;

        getline(ifs, br.deviceName);
        BorrowAndReturnRecord* inserted = recordTree.insert(br);
    }
}
