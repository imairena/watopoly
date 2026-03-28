export module board;

import <iostream>;
import <string>;
import <vector>;
import <memory>;

import playerSquareProperty;
import slc;
import needleshall;



export class Board {
  std::vector<std::unique_ptr<Square>> squares;  // owns Squares
  std::string boardString;
  std::vector<Player*> players;  // does not own Players

  const int lineWidth = 90;
  const int rowHeight = 5 * lineWidth;
  const int squareWidth = 8;

  // convert position on the board to location
  // in in the string boardString
  int squareLocation(const int position) const;

  // creates cards for SLC and NeedlesHall squares
  void createCards(SLC *slcptr, NeedlesHall *needleshallptr,
                   std::istream& cardsIn) const;

public:
  Board(std::istream& boardIn, std::istream& squaresIn,
        std::istream& cardsIn);
  void display(std::ostream& out = std::cout);
  Square& getSquare(const int i) const;
  Property* getProperty(const std::string propName) const;
  void saveBoard(std::ostream& gameOut) const;
};
