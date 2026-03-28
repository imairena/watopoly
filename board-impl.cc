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
import card;
import slc;
import needleshall;
import card_types;
import tools;

using namespace std;



// convert position on the board to location in in the string boardString
int Board::squareLocation(const int position) const {
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
             std::istream& cardsIn, vector<Player>* players): players{players} {
  // read board.txt (boardIn) and store in boardString
  // create Squares based on squares.txt
  // update boardString during each square creation so it has the name there
  string line;
  while (getline(boardIn, line)) {  // create empty boardString
    boardString += line + "\n";
  }
  map<string, int> monopolyBlockCounts;
  SLC *slcptr = nullptr;
  NeedlesHall *needleshallptr = nullptr;
  
  while (getline(squaresIn, line)) {
    stringstream ss{line};
    string squareName, squareType;
    int squarePosition;
    ss >> squareName >> squareType >> squarePosition;
    replace(squareName.begin(), squareName.end(), '_', ' ');
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
      squareptr = make_unique<SLC>(squarePosition);
      squareString1 = "SLC";
      if (!slcptr) slcptr = dynamic_cast<SLC*>(squareptr.get());
    }
    else if (squareType == "NeedlesHall") {
      squareptr = make_unique<NeedlesHall>(squarePosition);
      squareString1 = "NEEDLES";
      squareString2 = "HALL";
      if (!needleshallptr) needleshallptr = dynamic_cast<NeedlesHall*>(squareptr.get());
    }
    else if (squareType == "CollectOSAP") {
      squareptr = make_unique<CollectOSAP>(squareName, squarePosition);
      squareString1 = "COLLECT";
      squareString2 = "OSAP";
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
      squareString1 = "DC TIMS";
      squareString2 = "LINE";
    }
    else if (squareType == "GooseNesting") {
      squareptr = make_unique<GooseNesting>(squareName, squarePosition);
      squareString1 = "GOOSE";
      squareString2 = "NESTING";
    }
    else if (squareType == "GoToTims") {
      squareptr = make_unique<GoToTims>(squareName, squarePosition);
      squareString1 = "GO TO";
      squareString2 = "TIMS";
    }
    else if (squareType == "CoopFee") {
      squareptr = make_unique<CoopFee>(squareName, squarePosition);
      squareString1 = "COOP";
      squareString2 = "FEE";
    }
    
    squares.push_back(move(squareptr));
    boardString.replace(location, squareString1.length(), squareString1);
    boardString.replace(location + lineWidth, squareString2.length(), squareString2);

    // monopolyBlockSizes depend on other entries in data, so
    // they must be set after initialization
    for (auto& squareptr : squares) {  // set blockSize of all academic buildings
      if (auto acBuild = dynamic_cast<AcademicBuilding*>(squareptr.get())) {
        acBuild->setBlockSize(monopolyBlockCounts[acBuild->getMonopolyBlock()]);
      }
    }

    // create cards for SLC and NeedlesHall squares
    createCards(slcptr, needleshallptr, cardsIn);

    // sort squares by position
    sort(squares.begin(), squares.end(),
         [](const std::unique_ptr<Square>& s1, const std::unique_ptr<Square>& s2) {
           return s1->getPosition() < s2->getPosition();
         });
  }
}

// prints a text display of the current board state
void Board::display(ostream& out) {
  // modify copy of boardString with players/improvements
  string boardStringCopy = boardString;
  string improvementsString;
  int numImprovements, location;
  for (auto& squareptr : squares) {  // display improvements
    if (auto acBuild = dynamic_cast<AcademicBuilding*>(squareptr.get())) {
      numImprovements = acBuild->getNumImprovements();
      improvementsString = string("I") * numImprovements;
      location = squareLocation(acBuild->getPosition());
      boardStringCopy.replace(location + lineWidth, numImprovements, improvementsString);
    }
  }
  int playerPosition;
  char playerToken;
  int playerOffset = 0;  // ensures players can't be printed on top of each other
  for (auto& playerptr : *players) {  // display player positions
    playerPosition = squareLocation(playerptr.getPosition());
    playerPosition += 3 * lineWidth + playerOffset;
    playerToken = playerptr.getToken();
    // update boardString
    boardStringCopy.replace(playerPosition, 1, 1, playerToken);
    ++playerOffset;
  }
  out << boardStringCopy;  // no endl because boardString already ends with "\n"
}

// Accessors
Square& Board::getSquare(const int i) const {
  return *squares[i];
}

Property* Board::getProperty(const string propName) const {
  for (auto& squareptr : squares) {
    if (auto prop = dynamic_cast<Property*>(squareptr.get())) {
      return prop;
    }
  }
  return nullptr;
}

// Card initialization
void Board::createCards(SLC *slcptr, NeedlesHall *needleshallptr,
                        istream& cardsIn) const {
  if (!slcptr && !needleshallptr) return;
  string line;
  string cardSquare, cardType, cardText;
  int numCards, cardVal;
  
  while (getline(cardsIn, line)) {
    stringstream ss{line};
    ss >> cardSquare >> numCards >> cardType >> cardVal >> cardText;
    replace(cardText.begin(), cardText.end(), '_', ' ');
    
    for (int i = 0; i < numCards; ++i) {
      unique_ptr<Card> cardptr; 
    
      if (cardType == "Move") {
        cardptr = make_unique<MoveCard>(cardText, cardVal);
      }
      else if (cardType == "Money") {
        cardptr = make_unique<MoneyCard>(cardText, cardVal);
      }
      else if (cardType == "MoveTo") {
        cardptr = make_unique<MoveToCard>(cardText, cardVal);
      }
      else if (cardType == "GoToTims") {
        cardptr = make_unique<GoToTimsCard>(cardText, cardVal);
      }

      if (cardSquare == "SLC" && slcptr) {
        slcptr->addCard(move(cardptr));
      }
      else if (cardSquare == "NeedlesHall" && needleshallptr) {
        needleshallptr->addCard(move(cardptr));
      }
    }
  }
}

// Saving the current board state
void Board::saveBoard(ostream& gameOut) const {
  for (auto& squareptr : squares) {
    if (auto prop = dynamic_cast<Property*>(squareptr.get())) {
      gameOut << prop->getName() << " " << prop->getOwner() << " ";
      if (prop->isMortgaged()) {
        gameOut << -1;
      }
      else if (auto acBuild = dynamic_cast<AcademicBuilding*>(squareptr.get())) {
        gameOut << acBuild->getNumImprovements();
      } else {
        gameOut << 0;
      }
      gameOut << endl;
    }
  }
}
