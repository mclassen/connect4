#include <string>
#include <stack>
#include <iostream>
#include "command.h"

const int command::MAX_COMMANDS = 8;

command::command() {
  id = NONE;
  param = 0;
  moveStack = std::stack<int>();
}

command::~command() {
}

void command::Read() {
  commandStr = std::string();
  std::cin >> commandStr;
  if(commandStr != "") {
    id = NONE;
    for(int i=0; i < MAX_COMMANDS; i++) {
      //std::cout << commandSet[i][0] << std::endl;
      if(commandSet[i][0] == commandStr[0])
	id = (enum commandId)i;
    }
    if(id >= MOVE) {
      std::cin >> param;
    }
  } else
    id = NONE;
  //std::cout << id << std::endl;
}

void command::PrintCommands() const {
  for(int i=0; i < MAX_COMMANDS; i++)
    std::cout << commandSet[i] << std::endl;
  std::cout << std::endl;
}

void command::PrintPrompt() const {
  std::cout << PROMPT << VERSION << " >"<< std::flush;
}

const std::string command::PROMPT = "DeepFour ";
const std::string command::VERSION = "1.07";

const std::string command::commandSet[] = { 
  "display \t- display the board",
  "new \t\t- start a new game ",
  "takeback\t- unmake last move",  
  "help \t\t- print this help screen",
  "quit \t\t- exit the program",
  "move x\t\t- make move in file x",
  "search x\t- search up to depth x",
  "limit x\t\t- limit search time to x seconds"
};
