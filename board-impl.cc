module board;

import <iostream>;
import <string>;
import <vector>;
import <sstream>;
import <algorithm>;
import <memory>;
import <map>;

import <square>;
import <player>;

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
Board::Board(istream& boardIn, istream& squaresIn) {
  // read board.txt (boardIn)
  // put in boardString
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
      squares.push_back(std::move(squareptr));
      monopolyBlockCounts[squareMonopolyBlock]++;
      // add text to boardString
      boardString.replace(location + lineWidth, 7, "-------");
      boardString.replace(location + 2 * lineWidth, squareName.length(), squareName);
    } else if (squareType == "SLC") {
      squareptr = make_unique<SLC>(squareName, squarePosition);
      squares.push_back(squareptr);
      boardString.replace(location, 3, "SLC");
      // ======================================================================== card stuff goes here!
    } else if (squareType == "NeedlesHall") {
      squareptr = make_unique<NeedlesHall>(squareName, squarePosition);
      squares.push_back(squareptr);
      boardString.replace(location, 7, "NEEDLES");
      boardString.replace(location + lineWidth, 4, "HALL");
      // ======================================================================== card stuff goes here!
    } else if (squareType == "CollectOSAP") {
      squareptr = make_unique<CollectOSAP>(squareName, squarePosition);
      squares.push_back(squareptr);
      boardString.replace(location, 7, "COLLECT");
      boardString.replace(location + lineWidth, 4, "OSAP");
    } else if (squareType == "Tuition") {
      squareptr = make_unique<Tuition>(squareName, squarePosition);
      squares.push_back(squareptr);
      boardString.replace(location, 7, "TUITION");
    } else if (squareType == "Residence") {
      squareptr = make_unique<Residence>(squareName, squarePosition);
      squares.push_back(squareptr);
      boardString.replace(location, squareName.length(), squareName);
    } else if (squareType == "Gym") {
      squareptr = make_unique<Gym>(squareName, squarePosition);
      squares.push_back(squareptr);
      boardString.replace(location, squareName.length(), squareName);
    } else if (squareType == "DCTimsLine") {
      squareptr = make_unique<DCTimsLine>(squareName, squarePosition);
      squares.push_back(squareptr);
      boardString.replace(location, 7, "DC TIMS");
      boardString.replace(location + lineWidth, 4, "LINE");
    } else if (squareType == "GooseNesting") {
      squareptr = make_unique<GooseNesting>(squareName, squarePosition);
      squares.push_back(squareptr);
      boardString.replace(location, 5, "GOOSE");
      boardString.replace(location + lineWidth, 7, "NESTING");
    } else if (squareType == "GoToTims") {
      squareptr = make_unique<GoToTims>(squareName, squarePosition);
      squares.push_back(squareptr);
      boardString.replace(location, 5, "GO TO");
      boardString.replace(location + lineWidth, 4, "TIMS");
    } else if (squareType == "CoopFee") {
      squareptr = make_unique<CoopFee>(squareName, squarePosition);
      squares.push_back(squareptr);
      boardString.replace(location, 4, "COOP");
      boardString.replace(location + lineWidth, 3, "FEE");
    }

    for (auto& squareptr : squares) {  // set blockSize of all academic buildings
      if (auto acBuild = dynamic_cast<AcademicBuilding*>(squareptr.get())) {
        acBuild->setBlockSize(monopolyBlockCounts[acBuild->getMonopolyBlock()]);
      }
    }
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