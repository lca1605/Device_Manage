#include "features.h"
#include "utils.h"
#include "file_io.h"
#include "display.h"

#include "allTree.h"
#include "device_Manage.h"
#include "tag_Manage.h"
#include "borrow_Manage.h"

#include <iostream>

using namespace std;

//------------------------------------------
//Menu display
//------------------------------------------

void showMainMenu() {
    cout << "\033[1;36m________________________________________\n";
    cout << "|             ===Menu===               |\n";
    cout << "| 1. Device manage                     |\n";
    cout << "| 2. Tag manage                        |\n";
    cout << "| 3. Borrow manage                     |\n";
    cout << "| 4. Save and exit                     |\n";
    cout << "|--------------------------------------|\033[0m\n";
}

void showDeviceMenu() {
    cout << "\033[1;34m________________________________________\n";
    cout << "|         ===Device manage===          |\n";
    cout << "| 1. Add new device                    |\n";
    cout << "| 2. Assign tag to existing device     |\n";
    cout << "| 3. Device's quantity change          |\n";
    cout << "| 4. Search by approx name             |\n";
    cout << "| 5. Advance search                    |\n";
    cout << "| 6. All devices list                  |\n";
    cout << "| 7. Delete device                     |\n";
    cout << "|--------------------------------------|\033[0m\n";
}

void showTagMenu() {
    cout << "\033[1;34m________________________________________\n";
    cout << "|            ===Tag Menu===            |\n";
    cout << "| 1. Add new tag                       |\n";
    cout << "| 2. Search by approx name             |\n";
    cout << "| 3. All devices list                  |\n";
    cout << "| 4. Delete tag                        |\n";
    cout << "|--------------------------------------|\033[0m\n";
}

void showBorrowMenu() {
    cout << "\033[1;34m________________________________________\n";
    cout << "|          ===Borrow Menu===           |\n";
    cout << "| 1. Borrow Device                     |\n";
    cout << "| 2. Return Device                     |\n";
    cout << "| 3. Return History                    |\n";
    cout << "| 4. Borrow History                    |\n";
    cout << "| 5. Borrow list by device             |\n";
    cout << "|--------------------------------------|\033[0m\n";
}

//------------------------------------------
// Control function
//------------------------------------------

void deviceManage() {
    clearScreen();
    showDeviceMenu();
    int deviceChoice = getValidatedIntForChoice("   \033[4;33mChoose an option:\033[0m ", 1, 7);

    switch (deviceChoice) {
    case 1:
        addNewDevice();
        break;
    case 2:
        assignExistingTagsToDevice();
        break;
    case 3:
        quantityChange();
        break;
    case 4:
        showDeviceListByApproxName();
        break;
    case 5:
        showDeviceListByMultiKey();
        break;
    case 6:
        showDeviceList();
        break;
    case 7:
        deleteDevice();
    default:
        break;
    }
}

void tagManage() {
    clearScreen();
    showTagMenu();
    int tagChoice = getValidatedIntForChoice("   \033[4;33mChoose an option:\033[0m ", 1, 4);

    switch (tagChoice) {
    case 1:
        addNewTag();
        break;
    case 2:
        showTagListByApproxName();
        break;
    case 3:
        showTagList();
        break;
    case 4:
        deleteTag();
        break;
    default:
        break;
    }
}

void borrowManage() {
    clearScreen();
    showBorrowMenu();
    int borrowChoice = getValidatedIntForChoice("   \033[4;33mChoose an option:\033[0m ", 1, 5);

    switch (borrowChoice) {
    case 1:
        getNewBorrowRecord();
        break;
    case 2:
        getNewReturnRecord();
        break;
    case 3:
        showReturnHistoryList();
        break;
    case 4:
        showBorrowHistoryList();
        break;
    case 5:
        showBorrowListByDeice();
        break;
    default:
        break;
    }
}

void saveAndExit() {
    saveToFile();
    cout << "Exiting...\n";
}