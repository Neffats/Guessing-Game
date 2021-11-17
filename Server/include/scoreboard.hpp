// Singleton class.
class ScoreBoard {
private:
  static ScoreBoard* instance;

  ScoreBoard() = default;

  ScoreBoard(const ScoreBoard&) = delete;
  ScoreBoard& operator=(const ScoreBoard&) = delete;
  ScoreBoard(ScoreBoard&&) = delete;
  ScoreBoard& operator=(ScoreBoard&&) = delete;

public:
   
  static ScoreBoard& GeInstance() {
    static ScoreBoard* sb = new ScoreBoard();
    return *sb;
  }
 
};
