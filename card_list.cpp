// card_list.cpp
// Author: Satvik Balakrishnan
// Implementation of the classes defined in card_list.h

#include "card_list.h"
#include "card.h"

#include <iostream>
using std::cout;

CardList::CardList(){
    root = nullptr;
}

CardList::~CardList(){
    clear(root);
}

void CardList::clear(Node* node){
    if(node != nullptr){
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

CardList::Node* CardList::findNode(const Card& card) const{
    Node* current = root;
    while(current != nullptr){
        if(card == current->info){
            return current;
        }
        else if(card < current->info){
            current = current->left;
        }
        else{
            current = current->right;
        }
    }
    return nullptr;
}

CardList::Node* CardList::minNode(Node* node) const{
    while(node->left != nullptr){
        node = node->left;
    }
    return node;
}

CardList::Node* CardList::maxNode(Node* node) const{
    while(node->right != nullptr){
        node = node->right;
    }
    return node;
}

CardList::Node* CardList::successor(Node* node) const{
    if(node->right != nullptr){
        return minNode(node->right);
    }
    Node* parent = node->parent;
    while(parent != nullptr && node == parent->right){
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

CardList::Node* CardList::predecessor(Node* node) const{
    if(node->left != nullptr){
        return maxNode(node->left);
    }
    Node* parent = node->parent;
    while(parent != nullptr && node == parent->left){
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

bool CardList::contains(const Card& card) const{
    return findNode(card) != nullptr;
}

bool CardList::insert(const Card& card){
    if(root == nullptr){
        root = new Node(card);
        return true;
    }

    Node* parent = nullptr;
    Node* current = root;
    while(current != nullptr){

        parent = current;
        if(card == current->info){
            return false; 
        }

        else if(card < current->info){
            current = current->left;
        }

        else{
            current = current->right;
        }

    }

    Node* newNode = new Node(card);
    newNode->parent = parent;

    if(card < parent->info){
        parent->left = newNode;
    }

    else{
        parent->right = newNode;
    }

    return true;
}

bool CardList::erase(const Card& card){
    Node* toErase = findNode(card);
    if(toErase == nullptr) return false;

    if(toErase->left && toErase->right){
        Node* next = successor(toErase);
        toErase->info = next->info;
        toErase = next;
    }

    Node* child = nullptr;
    if(toErase->left != nullptr) child = toErase->left;
    else child = toErase->right;

    if(child != nullptr) child->parent = toErase->parent;

    if(toErase->parent == nullptr){
        root = child;
    }

    else if(toErase == toErase->parent->left){
        toErase->parent->left = child;
    }

    else{
        toErase->parent->right = child;
    }

    delete toErase;
    return true;
}

const Card& CardList::Iterator::operator*() const{
    return current->info;
}

const Card* CardList::Iterator::operator->() const{
    return &(current->info);
}

CardList::Iterator& CardList::Iterator::operator++(){
    if(list == nullptr) return *this;
    if(current == nullptr) return *this;
    current = list->successor(current);
    return *this;
}

CardList::Iterator& CardList::Iterator::operator--(){
    if (list == nullptr) return *this;

    if (current == nullptr) {
        current = list->maxNode(list->root);
        return *this;
    }

    current = list->predecessor(current);
    return *this;
}

bool CardList::Iterator::operator==(const Iterator& other) const{
    return current == other.current;
}

bool CardList::Iterator::operator!=(const Iterator& other) const{
    return current != other.current;
}

CardList::Iterator CardList::begin() const{
    if(!root) return end();
    return Iterator(minNode(root), this);
}

CardList::Iterator CardList::end() const{
    return Iterator(nullptr, this);
}

CardList::Iterator CardList::rbegin() const{
    if(!root) return rend();
    return Iterator(maxNode(root), this);
}

CardList::Iterator CardList::rend() const{
    return Iterator(nullptr, this);
}
