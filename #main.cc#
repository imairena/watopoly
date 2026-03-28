import <iostream>;
import <string>;
import <fstream>;
import <sstream>;

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
      cout << "Error: Unknown flag " << arg << endl;
      return 0;
    }
  }

  ifstream boardIn{boardFile};
  ifstream squaresIn{squaresFile};
  ifstream cardsIn{cardsFile};

  Game game{boardIn, squaresIn, cardsIn};

  // load saved game
  if (flagLoad) {
    ifstream gameIn{gameFile};
    game.loadGame(gameIn);
  }
  else { // else, add players manually
    int numPlayers;
    string numPlayersString;
    cout << "How many players are playing? (2-6)" << endl;
    
    while (true) {
      cin >> numPlayersString;
      cout << endl;
      istringstream ss{numPlayersString};
      if (ss >> numPlayers && 2 <= numPlayers && numPlayers <= 6) {
        break;
      } else cout << "Invalid number of players. Try again." << endl;
    }
    
    // add players
    string validTokens = "GBDPS$LT";
    for (int i = 0; i < numPlayers; ++i) {
      string name;
      string tokenString;
      char token;
      
      // get name
      cout << "Enter player " << i + 1 << "'s name" << endl
           << "(Note, name may not be \"BANK\")" << endl;
      while (true) {
        cin >> name;
        cout << endl;
        if (name != "BANK") break;
        else cout << "Invalid name. Try again." << endl
                  << "(Note, name may not be \"BANK\")" << endl;
      }
      
      // get token
      cout << "Enter player " << i + 1 << "'s token" << endl
           << "(Note, token must be one of G, B, D, P, S, $, L, or T)" << endl;
      while (true) {
        cin >> tokenString;
        cout << endl;
        istringstream ss{tokenString};
        if (tokenString.length() == 1 && ss >> token
            && validTokens.find(token) != string::npos) break;
        else cout << "Invalid token. Try again." << endl
                  << "(Note, token must be one of G, B, D, P, S, $, L, or T)" << endl;
      }
      
      game.addPlayer(name, token);
    }
  }
  
  bool runGame = true;
  while (runGame && game.getActivePlayers() > 1) {
    runGame = game.playTurn(flagTest);
  }
  
  cout << "\nGAME OVER!! We have a winner!" << endl;
  return 0;
}
