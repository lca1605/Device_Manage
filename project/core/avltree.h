#ifndef AVLTREE_H
#define AVLTREE_H

#include <algorithm>

using namespace std;

template <typename T>
struct AVLNode {
    T data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const T& val) {
        data = val;
        left = nullptr;
        right = nullptr;
        height = 1 ;
    }
};

template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    int height(AVLNode<T>* node) {
        return node ? node->height : 0;
    }

    int balanceFactor(AVLNode<T>* node) {
        return height(node->left) - height(node->right);
    }

    void updateHeight(AVLNode<T>* node) {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    AVLNode<T>* rotateRight(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    AVLNode<T>* rotateLeft(AVLNode<T>* x) {
        AVLNode<T>* y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    AVLNode<T>* balance(AVLNode<T>* node) {
        updateHeight(node);
        int balance = balanceFactor(node);

        if (balance > 1) {
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }

        if (balance < -1) {
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode<T>* insert(AVLNode<T>* node, const T& key, T*& insertedData) {
        if (!node) {
            AVLNode<T>* newNode = new AVLNode<T>(key);
            insertedData = &newNode->data;
            return newNode;
        }
        if (key < node->data) {
            node->left = insert(node->left, key, insertedData);
        } else {
            node->right = insert(node->right, key, insertedData);
        }
        return balance(node);
    }

    AVLNode<T>* findMin(AVLNode<T>* node) {
        return node->left ? findMin(node->left) : node;
    }

    AVLNode<T>* removeMin(AVLNode<T>* node) {
        if (!node->left) {
            return node->right;
        }
        node->left = removeMin(node->left);
        return balance(node);
    }

    AVLNode<T>* remove(AVLNode<T>* node, const T& key) {
        if (!node) {
            return nullptr;
        }

        if (key < node->data) {
            node->left = remove(node->left, key);
        } else if (key > node->data) {
            node->right = remove(node->right, key);
        } else {
            AVLNode<T>* left = node->left;
            AVLNode<T>* right = node->right;
            delete node;
            if (!right) return left;
            AVLNode<T>* min = findMin(right);
            min->right = removeMin(right);
            min->left = left;
            return balance(min);
        }
        return balance(node);
    }

    AVLNode<T>* search(AVLNode<T>* node, const T& key) {
        if (!node) {
            return nullptr;
        }
        if (key == node->data) {
            return node;
        }
        if (key < node->data) {
            return search(node->left, key);
        }
        return search(node->right, key);
    }

    int countNodes(AVLNode<T>* node) const {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    void clear(AVLNode<T>* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        clear(root);
        root = nullptr;
    }

    T* insert(const T& key) {
        T* inserted = nullptr;
        root = insert(root, key, inserted);
        return inserted;
    }

    void remove(const T& key) {
        root = remove(root, key);
    }

    T* search(const T& key) {
        AVLNode<T>* result = search(root, key);
        return result ? &result->data : nullptr;
    }

    AVLNode<T>* getRoot() const {
        return root;
    }

    int countNodes() const {
        return countNodes(root);
    }
};

#endif
