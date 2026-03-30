import <iostream>;
import <string>;
import <fstream>;
import <sstream>;
import <algorithm>;
import <vector>;

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
    } else if (arg == "-board") {
      boardFile = argv[i + 1];
      i += 2;
    } else if (arg == "-squares") {
      squaresFile = argv[i + 1];
      i += 2;
    } else if (arg == "-cards") {
      cardsFile = argv[i + 1];
      i += 2;
    } else {
      cout << "Error: Unknown flag " << arg << endl;
      return 0;
    }
  }

  ifstream boardIn{boardFile};
  ifstream squaresIn{squaresFile}; // requires 40 spaces with Tims = space 10
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
    vector<char> validTokens = {'G', 'B', 'D', 'P', 'S', '$', 'L', 'T'};
    vector<string> takenNames;
    vector<char> takenTokens;
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
        if (name == "BANK") {
          cout << "Invalid name. Try again." << endl
               << "(Note, name may not be \"BANK\")" << endl;
        }
        else if (find(takenNames.begin(), takenNames.end(), name) != takenNames.end()) {
          cout << "That name has already been taken. Try again." << endl;
        }
        else {
          takenNames.push_back(name);
          break;
        }
      }
      
      // get token
      cout << "Enter player " << i + 1 << "'s token" << endl
           << "(Note, token must be one of G, B, D, P, S, $, L, or T)" << endl;
      while (true) {
        cin >> tokenString;
        cout << endl;
        istringstream ss{tokenString};
        if (tokenString.length() != 1 || !(ss >> token)
            || find(validTokens.begin(), validTokens.end(), token) == validTokens.end()) {
          cout << "Invalid token. Try again." << endl
               << "(Note, token must be one of G, B, D, P, S, $, L, or T)" << endl;
        }
        else if (find(takenTokens.begin(), takenTokens.end(), token) != takenTokens.end()) {
          cout << "That token has already been taken. Try again." << endl;
        }
        else {
          takenTokens.push_back(token);
          break;
        }
      }
      
      game.addPlayer(name, token);
    }
  }

  bool runGame = true;
  while (runGame && game.getActivePlayers() > 1) {
    runGame = game.playTurn(flagTest);
  }
  
  return 0;
}
