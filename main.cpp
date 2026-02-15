// This file should implement the game using a custom implementation of a BST (based on your earlier BST implementation)
//Satvik Balakrishnan


#include <iostream>
#include <fstream>
#include <string>
#include "card.h"
#include "card_list.h"
//Do not include set in this file


using namespace std;

void playGame(CardList& aliceDeck, CardList& bobDeck){
  bool button = true; 
    while(true){

      bool endLoop = false;

      if(button){
        for(auto aliceCard = aliceDeck.begin(); aliceCard != aliceDeck.end(); ++aliceCard){
          if(bobDeck.contains(*aliceCard)){

            cout<<"Alice picked matching card "<<*aliceCard<<endl;

            bobDeck.erase(*aliceCard);
            aliceDeck.erase(*aliceCard);
            endLoop = true;
            break;
          }
        }

      }

      else{
        for(auto bobCard = bobDeck.rbegin(); bobCard != bobDeck.rend(); --bobCard){
          if(aliceDeck.contains(*bobCard)){

            cout<<"Bob picked matching card "<<*bobCard<<endl;

            aliceDeck.erase(*bobCard);
            bobDeck.erase(*bobCard);
            endLoop = true;
            break;
          }
        }
      }

      if(endLoop == false){
        break;
      }
      button = !button;

    }
}

int main(int argv, char** argc){
  if(argv < 3){
    cout << "Please provide 2 file names" << endl;
    return 1;
  }
  
  ifstream cardFile1 (argc[1]);
  ifstream cardFile2 (argc[2]);
  string line;

  if (cardFile1.fail() || cardFile2.fail() ){
    cout << "Could not open file " << argc[2];
    return 1;
  }

  CardList aliceDeck;
  CardList bobDeck;

  //Read each file
  while (getline (cardFile1, line) && (line.length() > 0)){
    string suit = line.substr(0, 1);
    string value = line.substr(2);
    Card toInsert = Card(suit,value);
    aliceDeck.insert(toInsert); 
  }
  cardFile1.close();


  while (getline (cardFile2, line) && (line.length() > 0)){
    string suit = line.substr(0, 1);
    string value = line.substr(2);
    Card toInsert = Card(suit,value);
    bobDeck.insert(toInsert);

  }
  cardFile2.close();

  playGame(aliceDeck, bobDeck);

  cout << "\nAlice's cards:\n";
  for (auto it = aliceDeck.begin(); it != aliceDeck.end(); ++it) {
    cout << *it << "\n";
  }

  cout << "\nBob's cards:\n";
  for (auto it = bobDeck.begin(); it != bobDeck.end(); ++it) {
    cout << *it << "\n";
  }

  return 0;
}
