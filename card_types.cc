export module card_types;

import card;
import playerSquareProperty;



export class MoveCard: public Card {
public:
  void apply(Player* p) override;
};

export class MoneyCard: public Card {
public:
  void apply(Player* p) override;
};

export class MoveToCard: public Card {
public:
  void apply(Player* p) override;
};

export class GoToTimsCard: public Card {
public:
  void apply(Player* p) override;
};
