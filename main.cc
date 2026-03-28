import <iostream>;
import <string>;
import <fstream>;

import game;

using namespace std;



int main(int argc, char *argv[]) {
  bool flagLoad = false, flagTest = false;
  string gameFile;
  string boardFile = "board.txt";
  string squaresFile = "squares.txt";
  string cardsFile = "cards.txt";

  int i = 1;
  while (i < argc) {
    string arg = argv[i];
    if (arg == "-load") {
      flagLoad = true;
      gameFile = argv[i + 1];
      i += 2;
    } else if (arg == "-test") {
      flagTest = true;
      ++i;
    } else {
      cout << "Error: Unknown flag " << arg;
      return 0;
    }
  }

  ifstream boardIn{boardFile};
  ifstream squaresIn{squaresFile};
  ifstream cardsIn{cardsFile};

  Game game{boardIn, squaresIn, cardsIn};
  
  if (flagLoad) {
    ifstream gameIn{gameFile};
    game.loadGame(gameIn);
  }

  bool runGame = true;
  while (runGame) {
    runGame = game.playTurn(flagTest);
  }
  
  return 0;
}
