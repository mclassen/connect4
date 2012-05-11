#ifndef __command__h
#define __command__h

enum commandId {DISPLAY, NEW, TAKEBACK, HELP, QUIT, MOVE, SEARCH, LIMIT,
		NONE = -1, ERROR = -10000};

class command {
 public:
  command();
  ~command();

  void Read();
  void PrintCommands() const;
  void PrintPrompt() const;

  void PushMove(int move) { moveStack.push(move); }
  int  TakeMove()         { 
    int move = moveStack.top();
    moveStack.pop();
    return move;
  }

  commandId GetId() const { return id; }
  int GetParam() const { return param; }

 private:
  commandId id;
  std::string commandStr;
  int param;
  std::stack<int> moveStack;

  static const int MAX_COMMANDS;

  static const std::string PROMPT;
  static const std::string VERSION;
  static const std::string commandSet[];
};

#endif
