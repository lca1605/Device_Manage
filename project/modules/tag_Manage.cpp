#include "tag_Manage.h"
#include "utils.h"
#include "display.h"

#include <iostream>

using namespace std;

void addNewTag() {
    drawLine(1);

    //Get tag infor
    Tag T;
    if (!getNonEmptyString(T.name, "  New Tag: ")) return;

    //Check for duplicated
    Tag* findTag = tagTree.search(T);
    if (findTag != nullptr) {
        drawLine(1);
        cout << "  " << centerText(warning("Tag existed"), 41) << "\n";
        wait();
        return;
    }

    //Insert to tag tree
    tagTree.insert(T);

    //Confirm success
    drawLine(1);
    cout << "  " << centerText(confirm("Tag created"), 41) << "\n";
    wait();
}

void showTagListByApproxName() {
    drawLine(1);

    //Get keyword
    string keyword;
    if (!getNonEmptyString(keyword, "  Enter keyword: ")) return;
    keyword = toLowerCase(keyword);

    //Print List
    drawLine(1);
    cout << "\n";
    printTagHeader();
    if (printInOrderGenericByKey(tagTree.getRoot(), printByApproxName, keyword) == 0) {
        clearHeader();
        cout << "\033[F  " << centerText(warning("No matching Tag"), 41) << "\n";
    }
    wait();
}

void showTagList() {
    drawLine(1);
    cout << "\n";

    //Check number of tag
    if (tagTree.countNodes() == 0) {
        cout << "\033[F  " << centerText(highlight("Tag list empty"), 41) << "\n";
        wait();
        return;
    }

    //Print list
    printTagHeader();
    printInOrderGenericByKey<Tag, string>(tagTree.getRoot(), printList<Tag>, "0");
    wait();
}

void deleteTag() {
    drawLine(1);

    //Get tag infor
    string tagName;
    if (!getNonEmptyString(tagName, "  Tag name: ")) return;
    Tag* findTag = tagTree.search(Tag{tagName});
    if (findTag == nullptr) {       
        drawLine(1);
        cout << "  " << centerText(warning("Tag not founed"), 41) << "\n";
        wait();
        return;
    }

    //Confirm again
    char deleteConfirm = getYesNo("  Are you sure you want to delete this tag? (y/n): ");
    if (deleteConfirm != 'y') return;

    //Delete from Device
    for (int i = 0; i < findTag->Dcount; i++) {
        Device* device = findTag->DList[i];
        for (int j = 0; j < device->Tcount; ++j) {
            if (device->TList[j] == findTag) {
                for (int k = j; k < device->Tcount - 1; ++k) {
                    device->TList[k] = device->TList[k + 1];
                }
                device->Tcount--;
                break;
            }
        }
    }

    //Delete from tag tree
    tagTree.remove(*findTag);

    //Confirm success
    drawLine(1);
    cout << "  " << centerText(confirm("Tag deleted"), 41) << "\n";
    wait();
}