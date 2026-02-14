// card.h
// Author: Satvik Balakrishnan
// All class declarations related to defining a single card go here

#ifndef CARD_H
#define CARD_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;


class Card{
private:
    int suit;
    int value;

    vector<string> suits = {"c", "d", "s", "h"};
    vector<string> values = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};


    

public:
    Card();
    Card(string suit, string value);

    string getSuit() const;
    string getValue() const;

    int getSuitIndex() const;
    int getValueIndex() const;

    void setSuit(const string& suit);
    void setValue(const string& value);

    bool operator==(const Card& other) const;
    bool operator>(const Card& other) const;
    bool operator<(const Card& other) const;
    bool operator>=(const Card& other) const;
    bool operator<=(const Card& other) const;
    friend ostream& operator<<(ostream& os, const Card& card);
};

#endif
