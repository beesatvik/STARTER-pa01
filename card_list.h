// card_list.h
// Author: Satvik Balakrishnan
// All class declarations related to defining a BST that represents a player's hand

#ifndef CARD_LIST_H
#define CARD_LIST_H
#include <iostream>
#include "card.h"

using namespace std;

class CardList{
private:
    struct Node {
        Card info;
        Node *left, *right, *parent;
        Node(const Card& card) : info(card), left(nullptr), right(nullptr), parent(nullptr) { }
    };

    Node *root;

    void clear(Node* node);
    Node* findNode(const Card& card) const;
    Node* minNode(Node* node) const;
    Node* maxNode(Node* node) const;
    Node* successor(Node* node) const;
    Node* predecessor(Node* node) const;

public:
    CardList();
    ~CardList();

    bool insert(const Card& card);
    bool contains(const Card& card) const;
    bool erase(const Card& card);

    class Iterator {
    private:
        Node* current;
        const CardList* list;
        Iterator(Node* node, const CardList* list) : current(node), list(list) { }
        friend class CardList;

    public:
        Iterator() : current(nullptr), list(nullptr) {}

        const Card& operator*() const;
        const Card* operator->() const;
        Iterator& operator++(); 
        Iterator& operator--();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    };

    Iterator begin() const;
    Iterator end() const;
    Iterator rbegin() const;
    Iterator rend() const;

};

#endif


