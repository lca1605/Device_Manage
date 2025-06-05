#include "borrow_Manage.h"
#include "utils.h"
#include "display.h"

#include <iostream>

using namespace std;

//Check current borrow record by borrow list in Device
currentBorrowRecord* findCurrentBorrowRecord(const string& user, int mssv, Device* device) {
    for (int i = 0; i < device->Bcount; i++) {
        if (device->BList[i]->user == user && device->BList[i]->MSSV == mssv) {
            return device->BList[i];
        }
    }

    return nullptr;
}

//Get classic infor of a record
bool back = false;
BorrowAndReturnRecord getBorrowAndReturnInfor() {
    back = false;

    BorrowAndReturnRecord barInfor;
    if (!getNonEmptyString(barInfor.user, "  User: ")) {
        back = true;
        return barInfor;
    }

    barInfor.MSSV = getValidatedInt("  MSSV: ", 1);
    if (barInfor.MSSV == -1) {
        back = true;
        return barInfor;
    }

    barInfor.brQuantity = getValidatedInt("  Quantity: ", 1);
    if (barInfor.brQuantity == -1) {
        back = true;
        return barInfor;
    }

    barInfor.brDate = getCurrentDateTime();

    return barInfor;
} 

void getNewBorrowRecord() {
    drawLine(1);

    //Get device infor
    Device* findDevice = getDeviceByName();
    if (!findDevice) return;

    //Get record classic infor
    BorrowAndReturnRecord borrowRecord = getBorrowAndReturnInfor();
    if (back == true) return;

    //Check device's quantity
    if (findDevice->quantity == 0) {
        drawLine(1);
        cout << "  " << centerText(warning("Device run out"), 41) << "\n";
        return;
    } else if (borrowRecord.brQuantity > findDevice->quantity) {
        cout << "  Only " << findDevice->quantity << " left.";
        char choice = getYesNo(" Do you want to borrow all left? (y/n): ");
        if (choice == 'y') {
            borrowRecord.brQuantity = findDevice->quantity;
            cout << "033[F\033[2K  Quantity: " << findDevice->quantity << "\n";
        } else {
            return;
        }
    }

    //Save borrow history record
    borrowRecord.type = HISTORY;
    borrowRecord.deviceName = findDevice->name;
    recordTree.insert(borrowRecord);

    //Check current borrow by device
    currentBorrowRecord* findBorrow = findCurrentBorrowRecord(borrowRecord.user, borrowRecord.MSSV, findDevice);

    //Save borrow current record
    if (findBorrow == nullptr) {
        currentBorrowRecord* newBorrowRecord = new currentBorrowRecord;
        newBorrowRecord->user = borrowRecord.user;
        newBorrowRecord->MSSV = borrowRecord.MSSV;
        newBorrowRecord->brQuantity = borrowRecord.brQuantity;
        linkBorrowRecordtoDevice(findDevice, newBorrowRecord);
    } else {
        findBorrow->brQuantity += borrowRecord.brQuantity;
    }

    //Change device's quantity
    findDevice->quantity -= borrowRecord.brQuantity;

    //Confirm success
    drawLine(1);
    cout << "  " << centerText(confirm("Borrow succesfull"), 41) << "\n";
    wait();
}

void getNewReturnRecord() {
    drawLine(1);

    //Get device infor
    Device* findDevice = getDeviceByName();
    if (!findDevice) return;

    //Get record classic infor
    BorrowAndReturnRecord returnRecord = getBorrowAndReturnInfor();
    if (back == true) return;

    //Check current borrow by device
    currentBorrowRecord* findBorrow = findCurrentBorrowRecord(returnRecord.user, returnRecord.MSSV, findDevice);
    if (findBorrow == nullptr) {
        cout << "  " << centerText(warning("Borrow Record not founded"), 41) << "\n";
        wait();
        return;
    }

    //Check current borrow quantity
    if (findBorrow->brQuantity >= returnRecord.brQuantity) {
        findBorrow->brQuantity -= returnRecord.brQuantity;

        //Remove if return all device
        if (findBorrow->brQuantity == 0) {
            removeCurrentBorrowRecordFromDevice(findDevice, findBorrow);
        }
    } else {
        cout << "  Too many Device, you only borrow " << findBorrow->brQuantity << " device. Return sucessfully, return to you " << returnRecord.brQuantity - findBorrow->brQuantity << " device.\n";
        returnRecord.brQuantity = findBorrow->brQuantity;
        removeCurrentBorrowRecordFromDevice(findDevice, findBorrow);
    }

    //Save return history record
    returnRecord.type = RETURNED;
    recordTree.insert(returnRecord);
    findDevice->quantity += returnRecord.brQuantity;

    //Confirm success
    drawLine(1);
    cout << "  " << centerText(confirm("Return succesfull"), 41) << "\n";
    wait();
}

void showReturnHistoryList() {
    drawLine(1);
    cout << "\n";

    printBRRecordHeader();

    if (printInOrderGenericByKey(recordTree.getRoot(), printByType, RETURNED) == 0) {
        clearHeader();
        cout << "\033[F  " << centerText(highlight("Return history is emty"), 41) << "\n";
        wait();
        return;
    }

    wait();
}

void showBorrowHistoryList() {
    drawLine(1);
    cout << "\n";

    printBRRecordHeader();

    if (printInOrderGenericByKey(recordTree.getRoot(), printByType, HISTORY) == 0) {
        clearHeader();
        cout << "\033[F  " << centerText(highlight("Borrow history is emty"), 41) << "\n";
        wait();
        return;
    }

    wait();
}

void showBorrowListByDeice() {
    drawLine(1);

    //Get device infor
    Device* findDevice = getDeviceByName();
    if (!findDevice) return;

    //Confirm no record
    drawLine(1);
    if (findDevice->Bcount == 0) {
        cout << "  " << centerText(warning("No borrow record"), 41) << "\n";
        wait();
        return;
    }

    cout << "\n";
    printCBRecordHeader();

    for (int i = 0; i < findDevice->Bcount; i++) {
        printForm(*findDevice->BList[i]);
    }

    wait();
}