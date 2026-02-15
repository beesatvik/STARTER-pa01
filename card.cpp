// card.cpp
// Author: Satvik Balakrishnan
// Implementation of the classes defined in card.h

#include <string>
#include <iostream>
#include "card.h"
using namespace std;

Card::Card(string suit, string value) : suit(-1), value(-1){
    for (size_t i = 0; i < suits.size(); i++){
        if (suit == suits[i]){
            this->suit = i;
            break;
        }
    }

    for (size_t i = 0; i < values.size(); i++){
        if (value == values[i]){
            this->value = i;
            break;
        }
    }
    
    if (this->suit == -1 || this->value == -1){
        cout<<"Invalid suit or value"<<endl;
    }

}

string Card::getSuit() const{
    return suits[suit];
}

string Card::getValue() const{
    return values[value];
}

int Card::getSuitIndex() const{
    return suit;
}

int Card::getValueIndex() const{
    return value;
}

void Card::setSuit(const string& suit){
    for (size_t i = 0; i < suits.size(); i++){
        if (suit == suits[i]){
            this->suit = i;
            return;
        }
    }

    cout << "Invalid suit" << endl;
}

void Card::setValue(const string& value){
    for (size_t i = 0; i < values.size(); i++){
        if (value == values[i]){
            this->value = i;
            return;
        }
    }

    cout << "Invalid value" << endl;
}

bool Card::operator==(const Card& other) const{
    return this->suit == other.suit && this->value == other.value;
}

bool Card::operator>(const Card& other) const{
    if (this->suit > other.suit){
        return true;
    } else if (this->suit == other.suit){
        return this->value > other.value;
    } else {
        return false;
    }
}

bool Card::operator<(const Card& other) const{
    if (this->suit < other.suit){
        return true;
    } else if (this->suit == other.suit){
        return this->value < other.value;
    } else {
        return false;
    }
}

bool Card::operator>=(const Card& other) const{
    return *this > other || *this == other;
}

bool Card::operator<=(const Card& other) const{
    return *this < other || *this == other;
}

ostream& operator<<(ostream& os, const Card& card){
    os << card.getSuit() << " " << card.getValue();
    return os;
}


