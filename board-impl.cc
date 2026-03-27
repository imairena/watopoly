module board;

import <iostream>;
import <string>;
import <vector>;
import <sstream>;
import <algorithm>;
import <memory>;
import <map>;

import playerSquareProperty;
import non_properties;
import gym;
import residence;
import academicbuilding;

using namespace std;



// convert position on the board to location in in the string boardString
int Board::squareLocation(int position) {
  int topLeft = 1 + lineWidth;
  if (position <= 10) {  // bottom row
    int bottomRight = topLeft + 10 * squareWidth + 10 * rowHeight;
    return bottomRight - position * squareWidth;
  } else if (position <= 20) {  // left column
    int bottomLeft = topLeft + 10 * rowHeight;
    return bottomLeft - (position - 10) * rowHeight;
  } else if (position <= 30) {  // top row
    return topLeft + (position - 20) * squareWidth;
  } else {  // right column
    int topRight = topLeft + 10 * squareWidth;
    return topRight + (position - 30) * rowHeight;
  }
}

// ctor
Board::Board(istream& boardIn, istream& squaresIn,
             std::istream& cardsIn) {
  // read board.txt (boardIn) and store in boardString
  // create Squares based on squares.txt
  // update boardString during each square creation so it has the name there
  string line;
  while (getline(boardIn, line)) {  // create empty boardString
    boardString += line + "\n";
  }
  map<string, int> monopolyBlockCounts;
  while (getline(squaresIn, line)) {
    stringstream ss{line};
    string squareName, squareType;
    int squarePosition;
    ss >> squareName >> squareType >> squarePosition;
    std::replace(squareName.begin(), squareName.end(), '_', ' ');
    int location = squareLocation(squarePosition);
    unique_ptr<Square> squareptr;
    string squareString1 = "", squareString2 = "";
    
    if (squareType == "AcademicBuilding") {
      string squareMonopolyBlock;
      int squareCost, squareImprovementCost;
      int squareTuitionLevels[6];
      ss >> squareCost >> squareImprovementCost;
      for (int i = 0; i < 6; ++i) {
        ss >> squareTuitionLevels[i];
      }
      squareptr = make_unique<AcademicBuilding>(squareName, squarePosition,
                                                 squareCost, squareImprovementCost,
                                                 squareMonopolyBlock, squareTuitionLevels);
      monopolyBlockCounts[squareMonopolyBlock]++;
      squareString2 = "-------";
      // academic squares also print on third line
      boardString.replace(location + 2 * lineWidth, squareName.length(), squareName);
    }
    else if (squareType == "SLC") {
      squareptr = make_unique<SLC>(squareName, squarePosition);
      squareString1 = "SLC";
      // ======================================================================== card stuff goes here!
    }
    else if (squareType == "NeedlesHall") {
      squareptr = make_unique<NeedlesHall>(squareName, squarePosition);
      squareString1, squareString2 = "NEEDLES", "HALL";
      // ======================================================================== card stuff goes here!
    }
    else if (squareType == "CollectOSAP") {
      squareptr = make_unique<CollectOSAP>(squareName, squarePosition);
      squareString1, squareString2 = "COLLECT", "OSAP";
    }
    else if (squareType == "Tuition") {
      squareptr = make_unique<Tuition>(squareName, squarePosition);
      squareString1 = "TUITION";
    }
    else if (squareType == "Residence") {
      squareptr = make_unique<Residence>(squareName, squarePosition);
      squareString1 = squareName;
    }
    else if (squareType == "Gym") {
      squareptr = make_unique<Gym>(squareName, squarePosition);
      squareString1 = squareName;
    }
    else if (squareType == "DCTimsLine") {
      squareptr = make_unique<DCTimsLine>(squareName, squarePosition);
      squareString1, squareString2 = "DC TIMS", "LINE";
    }
    else if (squareType == "GooseNesting") {
      squareptr = make_unique<GooseNesting>(squareName, squarePosition);
      squareString1, squareString2 = "GOOSE", "NESTING";
    }
    else if (squareType == "GoToTims") {
      squareptr = make_unique<GoToTims>(squareName, squarePosition);
      squareString1, squareString2 = "GO TO", "TIMS";
    }
    else if (squareType == "CoopFee") {
      squareptr = make_unique<CoopFee>(squareName, squarePosition);
      squareString1, squareString2 = "COOP", "FEE";
    }
    
    squares.push_back(squareptr);
    boardString.replace(location, squareString1.length(), squareString1);
    boardString.replace(location + lineWidth, squareString2.length(), squareString2);

    // monopolyBlockSize depends on other entries in data, so
    // it must be set after initialization
    for (auto& squareptr : squares) {  // set blockSize of all academic buildings
      if (auto acBuild = dynamic_cast<AcademicBuilding*>(squareptr.get())) {
        acBuild->setBlockSize(monopolyBlockCounts[acBuild->getMonopolyBlock()]);
      }
    }

    // sort squares by position
    sort(squares.begin(), squares.end(),
         [](const Square &s1, const Square &s2) {
           return s1.getPosition() < s2.getPosition();
         });
  }
}

void Board::display(ostream& out = cout) {
  // modify boardString with players/improvements
  string improvementsString;
  int numImprovements;
  for (auto& squareptr : squares) {  // display improvements
    if (auto acBuild = dynamic_cast<AcademicBuilding*>(squareptr.get())) {
      numImprovements = acBuild->getNumImprovements();
      improvementsString = string("I") * numImprovements;
      boardString.replace(location + lineWidth, numImprovements, improvementsString);
    }
  }
  int playerPosition;
  char playerToken;
  int playerOffset = 0;  // ensures players can't be printed on top of each other
  for (auto playerptr : players) {  // display player positions
    playerPosition = playerptr->getPosition();
    playerToken = playerptr->getToken();
    boardString.replace(playerPosition + 3 * lineWidth, 1, 1, playerToken);
  }
  out << boardString;  // no endl because boardString already ends with "\n"
}

Square& Board::getSquare(int i) {
  return squares[i];
}
