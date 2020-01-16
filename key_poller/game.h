struct Point {
  int x;
  int y;
};

enum Direction {
  UP,
  DOWN,
  RIGHT,
  LEFT
};

class Snake {
  public:
    Snake(int maxBoardX, int maxBoardY, int initialX, int initialY, Direction initialDirection) {
      position[0].x = initialX;
      position[0].y = initialY;
      
      position[1].x = initialX + (initialDirection == LEFT ? 1 : -1);
      position[1].y = initialY;
      
      position[2].x = initialX + (initialDirection == LEFT ? 2 : -2);
      position[2].y = initialY;
      
      size = 3;
      direction = initialDirection;
      maxX = maxBoardX;
      maxY = maxBoardY;
            
      for (int i = 3; i < maxSize; i++) {
        position[i].x = 0;
        position[i].y = 0;
      }
    }

  int getSize() {
    return size;
  }

  Point &getPosition(int index) {
    return position[index];
  }

  int getMaxX() {
    return maxX;
  }

  int getMaxY() {
    return maxY;
  }

  Direction getDirection() {
    return direction;
  }

  void setDirection(Direction newDirection) {
    if (direction == UP && newDirection == DOWN ||
        direction == DOWN && newDirection == UP ||
        direction == RIGHT && newDirection == LEFT ||
        direction == LEFT && newDirection == RIGHT) {
      return;
    }
    direction = newDirection;
  }

  void update() {
    for (int i = size - 1; i > 0; i--) {
      position[i] = position[i - 1];
    }
    
    switch(direction) {
      case UP:
        position[0].y = (position[0].y + 1) % maxY;
        break;
      case DOWN:
        position[0].y = (position[0].y - 1 + maxY) % maxY;
        break;
      case RIGHT:
        position[0].x = (position[0].x + 1) % maxX;
        break;
      case LEFT:
        position[0].x = (position[0].x - 1 + maxX) % maxX;
        break;
    }
  }

  void grow() {
    if (size < maxSize) {
      size++;
    }
  }

  bool containsPoint(Point &point) {
    for (int i = 0; i < size; i++) {
      if (point.x == position[i].x && point.y == position[i].y) {
        return true;
      }
    }
    return false;
  }

  bool selfIntersects() {
    Point head = position[0];
    for (int i = 1; i < size; i++) {
      if (head.x == position[i].x && head.y == position[i].y) {
        return true;
      }
    }
    return false;
  }

  static const int maxSize = 32;

  private:
    Point position[maxSize];
    Direction direction;
    int size;
    int maxX;
    int maxY;
};

enum GameState {
  RUNNING,
  PLAYER1_WIN,
  PLAYER2_WIN,
  DRAW
};

class Game {
  public:
    Game(int maxBoardX, int maxBoardY):
      player1(maxBoardX, maxBoardY, maxBoardX / 3, maxBoardY / 2, RIGHT),
      player2(maxBoardX, maxBoardY, 2 * maxBoardX / 3, maxBoardY / 2, LEFT) {
        maxX = maxBoardX;
        maxY = maxBoardY;
        food = generateFood();
        state = RUNNING;
      }

  void update() {
    if (isRunning()) {
      if (tryFeedSnake(player1) || tryFeedSnake(player2)) {
        food = generateFood();
      }
      player1.update();
      player2.update();
      bool player1Lost = player1.selfIntersects() || player2.containsPoint(player1.getPosition(0));
      bool player2Lost = player2.selfIntersects() || player1.containsPoint(player2.getPosition(0));
      if (player1Lost) {
        state = player2Lost ? DRAW : PLAYER2_WIN;
      }
      else if (player2Lost) {
        state = PLAYER1_WIN;
      }
    }
  }

  Snake &getPlayer1() {
    return player1;
  }
  Snake &getPlayer2() {
    return player2;
  }

  void setPlayer1Direction(Direction direction) {
    player1.setDirection(direction);
  }
  void setPlayer2Direction(Direction direction) {
    player2.setDirection(direction);
  }

  Point &getFood() {
    return food;
  }
  /*
  GameState &getState() {
    return state;
  }
  */
  bool isRunning() {
    return state == RUNNING;
  }
  bool isDraw() {
    return state == DRAW;
  }
  bool isPlayer1Win() {
    return state == PLAYER1_WIN;
  }

  private:
    Point generateFood() {
      Point result = food;
      result.x = random(0, maxX);
      result.y = random(0, maxY);

      if (player1.containsPoint(result) || player2.containsPoint(result)) {
        return generateFood();
      }
      /*
      while (player1.containsPoint(result) || player2.containsPoint(result)) {
        result.x = (result.x + 1) % maxX;
        result.y = (result.y + 1) % maxY;
      }
      */
      return result;
    }

    bool tryFeedSnake(Snake &snake) {
      if (snake.containsPoint(food)) {
        snake.grow();
        return true;
      }
      return false;
    }

    Snake player1;
    Snake player2;
    Point food;
    GameState state;
    int maxX;
    int maxY;
};
