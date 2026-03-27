export module card_types;

import <string>;

import card;
import playerSquareProperty;



export class MoveCard: public Card {
public:
  MoveCard(std::string text, int val);
  void apply(Player* p) override;
};

export class MoneyCard: public Card {
public:
  MoneyCard(std::string text, int val);
  void apply(Player* p) override;
};

export class MoveToCard: public Card {
public:
  MoveToCard(std::string text, int val);
  void apply(Player* p) override;
};

export class GoToTimsCard: public Card {
public:
  GoToTimsCard(std::string text, int val);
  void apply(Player* p) override;
};
