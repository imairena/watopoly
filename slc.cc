export module slc;

import <vector>;
import <string>;
import <memory>;

import cardsquare;
import card;



export class SLC: public CardSquare {
  static std::vector<std::unique_ptr<Card>> cards;
  std::vector<std::unique_ptr<Card>>& getCards() override;
public:
  SLC(int position);
  void addCard(std::unique_ptr<Card> card);
};
