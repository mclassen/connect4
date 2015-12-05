#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <string>
#include <time.h>
#include "constants.h"
#include "move_buffer.h"
#include "board.h"
#include "search_settings.h"
#include "hash.h"
#include "search.h"
#include "command.h"

//#define _connect4_debug_

int main() {
  std::cout << "DeepFour (by Michael Claßen) \n\n";

  board MainBoard;
  command cli;
  
  search Search(MainBoard);
  searchResult result;
  SearchSettings mainSearchSettings;
  
  cli.PrintCommands();
  cli.PrintPrompt();
    
  cli.Read();
  while(cli.GetId() != QUIT) {
    switch(cli.GetId()) {
    case DISPLAY:
      MainBoard.Display();
      std::cout << "Time limit: " << mainSearchSettings.GetTimeLimit()
		<< std::endl;
      break;
    case HELP:
      cli.PrintCommands();
      break;
    case MOVE:
      if(MainBoard.MoveIsValid(cli.GetParam(), true)) {
        int move = MainBoard.ConvertFileToMove(cli.GetParam());
	MainBoard.MakeMove(move);
        cli.PushMove(move);
        MainBoard.Display();
      }
      break;
    case TAKEBACK:
        if (MainBoard.GetNumberOfMove() > 0) {
            int move = cli.TakeMove();
          if (MainBoard.MoveIsValid(File(move), false)) {
            MainBoard.UnmakeMove(move);
            MainBoard.Display();
          }
        }
        break;
    case SEARCH:
      result = searchResult();
      Search = search(MainBoard);
      std::cout << "initing search on depth " << 
	cli.GetParam() << "..." << std::endl;
      Search.InitSearch(cli.GetParam(), result, mainSearchSettings);
      std::cout << "Value: " << result.GetValue() << std::endl;
      std::cout << "Principle variation: ";
      result.PrintVariation();
      std::cout << std::endl;
      std::cout << "Searched nodes: " << 
	Search.GetSearchedNodes() << std::endl;
      std::cout << "Elapsed time: " << 
	Search.GetElapsedTime() << std::endl;
      std::cout << "=> " << (((Search.GetElapsedTime() > 0) ? 
			      (Search.GetSearchedNodes() / 
			       Search.GetElapsedTime()) :
			      Search.GetSearchedNodes()) / 1000)
		<< " kNodes per second " << std::endl;
      if(result.GetValue() >= constants::BEST_VALUE) {
	std::cout << "You have won! \n" << std::endl;
      } else if(result.GetValue() <= constants::WORST_VALUE) {
	std::cout << "You have lost! \n" << std::endl;
      } else if(result.GetValue() > 
		(constants::BEST_VALUE - constants::MATE_RANGE)) {
	std::cout << "Won in " << constants::BEST_VALUE - result.GetValue()
		  << " moves!" << std::endl;
      } else if(result.GetValue() <
		(constants::WORST_VALUE + constants::MATE_RANGE)) {
	std::cout << "Lost in " << result.GetValue() - constants::WORST_VALUE
		  << " moves!" << std::endl;
      }
	
      break;
    case NEW:
      MainBoard = board();
      Search = search(MainBoard);
      break;
    case LIMIT:
      mainSearchSettings.SetTimeLimit(cli.GetParam());
      break;
    case NONE:
      std::cout << "Enter a command!" << std::endl;
      break;
    case ERROR:
      std::cout << "Unknown command!" << std::endl;
      break;
    default:
      std::cout << "Case fell through!";
      break;
    }
    cli.PrintPrompt();
    cli.Read();
  }
  return 0;
}
