export module non_properties;
import playerSquareProperty;
import <string>;

export class CoopFee : public Square {
public:
  CoopFee(std::string name, int position);
  void landOn(Player* p) override;
};
export class Tuition : public Square {
public:
  Tuition(std::string name, int position);
  void landOn(Player* p) override;
};
export class CollectOSAP : public Square {
public:
  CollectOSAP(std::string name, int position);
  void landOn(Player* p) override;
};
export class DCTimsLine : public Square {
public:
  DCTimsLine(std::string name, int position);
  void landOn(Player* p) override;
};
export class GoToTims : public Square {
public:
  GoToTims(std::string name, int position);
  void landOn(Player* p) override;
};
export class GooseNesting : public Square {
public:
  GooseNesting(std::string name, int position);
  void landOn(Player* p) override;
};
