#include "common.hpp"

class Controller {
public:
  Controller();
  virtual ~Controller();
  virtual Result CheckGuess(int guess)=0;
};
