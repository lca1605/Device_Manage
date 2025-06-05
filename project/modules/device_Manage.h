#ifndef DEVICE_MANAGE_H
#define DEVICE_MANAGE_H

#include "structures.h"
#include "avltree.h"
#include "allTree.h"

void addNewDevice();
void assignExistingTagsToDevice();
void quantityChange();
void showDeviceListByApproxName();
void showDeviceListByMultiKey();
void showDeviceList();
void deleteDevice();

#endif