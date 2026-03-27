export module board;

import <iostream>;
import <string>;
import <vector>;
import <memory>;

import playerSquareProperty;



export class Board {
  std::vector<std::unique_ptr<Square>> squares;  // owns Squares
  std::string boardString;
  std::vector<Player*> players;  // does not own Players

  const int lineWidth = 90;
  const int rowHeight = 5 * lineWidth;
  const int squareWidth = 8;

  // convert position on the board to location
  // in in the string boardString
  int squareLocation(int position);

public:
  Board(std::istream& boardIn, std::istream& squaresIn,
        std::istream& cardsIn);
  void display(std::ostream& out = std::cout);
  Square& getSquare(int i);
};
