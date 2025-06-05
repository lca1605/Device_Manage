#include "structures.h"
#include "avltree.h"
#include "utils.h"
#include "features.h"
#include "file_io.h"
#include "allTree.h"
#include "display.h"


AVLTree<Device> deviceTree;
AVLTree<Tag> tagTree;
AVLTree<BorrowAndReturnRecord> recordTree;

int main() {
    loadFromFile();

    //Choose feature
    while (true) {
        clearScreen();
        showMainMenu();
        int choice = getValidatedIntForChoice("   \033[4;33mChoose an option:\033[0m ", 1, 4);

        switch (choice) {
            case 1:
                deviceManage();
                break;
            case 2:
                tagManage();
                break;
            case 3:
                borrowManage();
                break;
            case 4:
                saveAndExit();
                return 0;
        }
    }

    return 0;
}
