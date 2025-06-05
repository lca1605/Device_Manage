#ifndef ALLTREE_H
#define ALLTREE_H

#include "avltree.h"
#include "structures.h"

//Device manage
extern AVLTree<Device> deviceTree;

//Tag manage
extern AVLTree<Tag> tagTree;

//Borrow and Return record manage
extern AVLTree<BorrowAndReturnRecord> recordTree;

#endif