export module card;

import <string>;

import playerSquareProperty;



export class Card {
protected:
  std::string text;
  int val;
public:
  Card(std::string text, int val);
  virtual void apply(Player* p) = 0;
  Card& operator=(Card&& o);
};
