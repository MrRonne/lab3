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
        Snake(int maxBoardX, int maxBoardY) {
            int initialX = maxBoardX / 2;
            int initialY = maxBoardY / 2;
            
            position[0].x = initialX;
            position[0].y = initialY;
            
            position[1].x = initialX + 1;
            position[1].y = initialY;
            
            position[2].x = initialX + 2;
            position[2].y = initialY;
            
            size = 3;
            direction = LEFT;
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

        Point getPosition(int index) {
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
                direction == LEFT && newDirection == RIGHT)
            {
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
    FINISHED
};

class Game {
    public:
        Game(int maxBoardX, int maxBoardY): snake(maxBoardX, maxBoardY) {
            maxX = maxBoardX;
            maxY = maxBoardY;
            food = generateFood();
            state = RUNNING;
        }
        
        void update() {
            if (isRunning()) {
                if (snake.containsPoint(food)) {
                    snake.grow();
                    food = generateFood();
                }
                snake.update();
                if (snake.selfIntersects()) {
                    state = FINISHED;
                    return;
                }
            }
        }
        
        Snake &getSnake() {
            return snake;
        }
        
        void setSnakeDirection(Direction direction) {
            snake.setDirection(direction);
        }

        Point &getFood() {
            return food;
        }
        
        bool isRunning() {
            return state == RUNNING;
        }

    private:
        Point generateFood() {
            Point result = food;
            result.x = random(0, maxX);
            result.y = random(0, maxY);
            
            while (snake.containsPoint(result)) {
                result.x = (result.x + 1) % maxX;
                result.y = (result.y + 1) % maxY;
            }
            return result;
        }
  
        Snake snake;
        Point food;
        GameState state;
        int maxX;
        int maxY;
};