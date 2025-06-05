#include "device_Manage.h"
#include "utils.h"
#include "display.h"

#include <iostream>
#include <sstream>

using namespace std;

void addNewDevice() {
    drawLine(1);

    //Get classic device infor
    Device D;
    if (!getNonEmptyString(D.name, "  Device name: ")) return;
    if (deviceTree.search(D) != nullptr) {  //Check for duplicated
        drawLine(1);
        cout << "  " << centerText(warning("Device existed"), 41) << "\n";
        wait();
        return;
    }
    D.quantity = getValidatedInt("  Quantity: ", 1);
    if (D.quantity == -1) return;
    Device* inserted = deviceTree.insert(D);

    //Confirm success
    drawLine(1);
    cout << "  " << centerText(confirm("Device added"), 41) << "\n";
    drawLine(1);

    //Link tags to device
    if(linkTagLinetoDevice(inserted)) return;

    //Confirm success
    drawLine(1);
    cout << "  " << centerText(confirm("Tag linked"), 41) << "\n";
    wait();
}

void assignExistingTagsToDevice() {
    drawLine(1);

    //Get device infor
    Device* findDevice = getDeviceByName();
    if (!findDevice) return;

    //Link tags to device
    if (linkTagLinetoDevice(findDevice)) return;

    //Confirm success
    drawLine(1);
    cout << "  " << centerText(confirm("Tag linked"), 41) << "\n";
    wait();
}

void quantityChange() {
    drawLine(1);

    //Get device infor
    Device* findDevice = getDeviceByName();
    if (!findDevice) return;

    //Get new quantity
    string change;
    if (!getNonEmptyString(change, "  Quantity change: ")) return;
    stringstream ss(change);
    int changevalue;
    while (!(ss >> changevalue) || !ss.eof()) { //Check valid format
        cout << "  " << centerText(warning("Invalid number format"), 41) << "\n";
        if (!getNonEmptyString(change, "  Quantity change: ")) return;
        ss.clear();
        ss.str(change);
    }

    //Quantity process
    if (change[0] == '+') {
        findDevice->quantity += changevalue;
    } else if (change[0] == '-') {
        if (findDevice->quantity < changevalue) {
            cout << "Only " << findDevice->quantity << " left.";
            char choice = getYesNo(" Do you want to set to 0? (y/n): ");
            if (choice == 'y') {
                findDevice->quantity = 0;
            } else {
                return;
            }
        } else {
            findDevice->quantity += changevalue;
        }
    } else {
        findDevice->quantity = changevalue;
    }

    //Confirm succes
    drawLine(1);
    stringstream confirmStream;
    confirmStream << "Quantity changed to " << findDevice->quantity;
    string confirmStr = confirmStream.str();
    cout << "  " << centerText(confirm(confirmStr), 41) << "\n";
    wait();
}

void showDeviceListByApproxName() {
    drawLine(1);

    //Get keyword
    string keyword;
    if (!getNonEmptyString(keyword, "  Enter keyword: ")) return;
    keyword = toLowerCase(keyword);

    //Print list
    drawLine(1);
    cout << "\n";
    printDeviceHeader();
    if (printInOrderGenericByKey(deviceTree.getRoot(), printByApproxName, keyword) == 0) {
        clearHeader();
        cout << "\033[F  " << centerText(warning("No matching Device"), 41) << "\n";
    }
    wait();
}

void showDeviceListByMultiKey() {
    drawLine(1);

    //Get key(Quantity range, tags)
    AdvanceSearch key;
    key.min = getValidatedInt("  Min quantity: ", 0);
    if (key.min == -1) return;
    key.max = getValidatedInt("  Max quantity(0 mean infinity): ", 0);
    if (key.max == -1) return;
    cout << "  Tags (Tag 1, Tag 2,... right name tag): ";
    getline(cin, key.tag_line);
    key.tag_line = trim(key.tag_line);

    //Check for return command
    if (menuReturn(key.tag_line)) return;

    //Confirm success
    drawLine(1);
    cout << "\n";
    printDeviceHeader();
    if (printInOrderGenericByKey(deviceTree.getRoot(), printDeviceByMultiKey, key) == 0) {
        clearHeader();
        cout << "\033[F  " << centerText(warning("No matching Device"), 41) << "\n";
    }
    wait();
}

void showDeviceList() {
    drawLine(1);
    cout << "\n";

    //Check for device number
    if (deviceTree.countNodes() == 0) {
        cout << "\033[F  " << centerText(warning("Device list empty"), 41) << "\n";
        wait();
        return;
    }

    //Print list
    printDeviceHeader();
    printInOrderGenericByKey<Device, string>(deviceTree.getRoot(), printList<Device>, "0");
    wait();
}

void deleteDevice() {
    drawLine(1);

    //Get device infor
    Device* findDevice = getDeviceByName();
    if (!findDevice) return;

    //Confirm again
    char deleteConfirm = getYesNo("  Are you sure you want to delete this device (also delete all borrow record)? (y/n): ");
    if (deleteConfirm != 'y') return;

    //Delete form other entities
    for (int i = 0; i < findDevice->Tcount; i++) {
        Tag* tag = findDevice->TList[i];
        for (int j = 0; j < tag->Dcount; ++j) {
            if (tag->DList[j] == findDevice) {
                for (int k = j; k < tag->Dcount - 1; ++k) {
                    tag->DList[k] = tag->DList[k + 1];
                }
                tag->Dcount--;
                break;
            }
        }
    }
    for (int i = 0; i < findDevice->Bcount; ++i) {
        delete findDevice->BList[i];  // Giải phóng từng bản ghi
        findDevice->BList[i] = nullptr;
    }

    //Delete from tree
    deviceTree.remove(*findDevice);

    //Confirm success
    drawLine(1);
    cout << "  " << centerText(confirm("Device deleted"), 41) << "\n";
    wait();
}