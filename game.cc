export module game;

import <iostream>;
import <vector>;
import <string>;

import playerSquareProperty;
import board;



export class Game {
  std::vector<Player> players;
  Board board;
  int currentPlayer = 0;
  int totalCups = 0;
  const int maxCups = 4;

  // helper methods (private)
  void handleRoll(Player& currPlayer, bool& hasRolled, bool testMode);
  void handleBankrupt(Player& currPlayer);
  void handleTrade(Player& currPlayer);
  void handleMortgage(Player& currPlayer);
  void handleUnmortgage(Player& currPlayer);
  void handleImprove(Player& currPlayer);
  
public:
  Game(std::istream& boardIn, std::istream& squaresIn,
       std::istream& cardsIn);
  bool playTurn(bool testMode);
  void nextPlayer(std::ostream& out = std::cout);
  void loadGame(std::istream& gameIn);
  void saveGame(std::ostream& gameOut) const;
  void addPlayer(const std::string name, const char token);
  void all() const;
};
