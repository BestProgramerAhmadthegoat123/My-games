#include<iostream>
#include<raylib.h>
#include<deque>
#include<fstream>
#include<raymath.h>
using namespace std;
Color green = {173, 204, 96, 255};
Color DarkGreen = {43, 51, 24, 255};
Color LightOrange = {255, 204, 128, 255};
Color DarkOrange = {255, 140, 0, 255};
Color Gray = {128, 128, 128, 255};

int score = 0;
int bestScore =0;
int cellSize =22;
int cellCount =25;
int offset = 55;
double lastUdateTime =0;

bool ElementInDeque(Vector2 element, const deque<Vector2>& dq)
{
    for (unsigned int i = 0; i < dq.size(); i++)
    {
        if (Vector2Equals(dq[i], element))
            return true;
    }
    return false;
}

bool eventTrigged(double interval)
{
   double currentTime = GetTime();
   if (currentTime - lastUdateTime >= interval)
   {
      lastUdateTime = currentTime;
      return true;
   }
   return false;
}

class Snake
{
public:
    deque<Vector2> body ={Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
    Vector2 direction = {1,0};
    bool addSegment = false;
    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++ )
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset+ x*cellSize, offset + y*cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 6, BLACK);
        }
    }
    void Update()
    {
        body.push_front(Vector2Add(body[0] , direction));
        if(addSegment == true)
        {
            
            addSegment = false;
        }
        else
        {
        body.pop_back();
        
        }
        
    }
    void Reset()
    {
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1,0};
       
    }
};
class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody)
    {
      Image image = LoadImage("Graphics/food1.png");
      texture = LoadTextureFromImage(image);
      UnloadImage(image);
      position = GenreateRandomPos(snakeBody);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
       DrawTexture(texture, offset + position.x * cellSize, offset+ position.y * cellSize, WHITE);
    }
    Vector2 GenerateRandomeCell()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x,y};
    }
    Vector2 GenreateRandomPos(deque<Vector2> snakeBody)
    {
        Vector2 position = GenerateRandomeCell();
        while(ElementInDeque(position, snakeBody))
        {
            position = GenerateRandomeCell();
        }
        return position;

       
    }
};

class Game
{
public:
  Snake snake = Snake();
  Food food = Food(snake.body);
  bool running =true;
  Sound eatSound ;
  Sound wallSound ;

  Game()
  {
    InitAudioDevice();
    eatSound = LoadSound("Sounds/eat.mp3");
    wallSound = LoadSound("Sounds/wall.mp3.wav");
  }

  ~Game()
  {
    UnloadSound(eatSound);
    UnloadSound(wallSound);
    CloseAudioDevice();
  }
  void Draw()
  {
    food.Draw();
    snake.Draw();
  }
  void Update()
  {
    if(running){
    snake.Update();
    CheckCollisonWithFood();
    CheckCollisionWithhEdges();
    CheckCollisionWithTail();
    bestScore = LoadBestScore();
    }
  }
  void CheckCollisonWithFood()
  {
      if(Vector2Equals(snake.body[0], food.position))
      {
        food.position = food.GenreateRandomPos(snake.body);
        snake.addSegment = true;
        score++;
        if(score > bestScore && score > 0)
        {
            bestScore = score;
        }
        PlaySound(eatSound);
      }
       
  }
  void CheckCollisionWithhEdges()
  {
      if(snake.body[0].x == cellCount || snake.body[0].x == -1)
      {
          GameOver();
      }
      if(snake.body[0].y == cellCount || snake.body[0].y == -1)
      {
          GameOver();
      }
  }
  void GameOver()
  {
    snake.Reset();
    food.position = food.GenreateRandomPos(snake.body);
    running = false;
    if (score > bestScore)
    {
      bestScore = score;
      SaveBestScore(bestScore);
    }
    score=0;
    PlaySound(wallSound);
    
  }
  void CheckCollisionWithTail()
  {
      deque<Vector2> headlessBody = snake.body;
      headlessBody.pop_front();
      if(ElementInDeque(snake.body[0], headlessBody))
      {
          GameOver();
      }
  }
  void SaveBestScore(int bestScore)
  {
     ofstream best_score("BEST_SCORE.txt");
     if (best_score.is_open())
     {
        best_score << score;
        best_score.close();
     }
  }
int LoadBestScore()
{
    ifstream best_score("BEST_SCORE.txt");
    int score = 0;

    if (best_score.is_open())
    {
        best_score >> score;
        best_score.close();
    }

    return score;
}

};

int main()
{

    InitWindow(2*offset + cellSize*cellCount,2*offset + cellSize*cellCount, "Retor Snake");
    SetTargetFPS(60);

    Game game = Game();

    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        if(eventTrigged(0.2))
        {
            game.Update();
        }
        if(IsKeyPressed(KEY_W) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0,-1};
            game.running =true;
        }
        if(IsKeyPressed(KEY_S) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0,1};
            game.running =true;
        }
        if(IsKeyPressed(KEY_A) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1,0};
            game.running =true;
        }
        if(IsKeyPressed(KEY_D) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1,0};
            game.running =true;
        }
        //Drawing
        ClearBackground(LightOrange);
       
        DrawText( "Niggers Silmulator" ,offset+130 ,15 ,30 ,DarkGreen);
        DrawText(TextFormat("%i",score),offset-5 ,offset+cellSize*cellCount+10 ,30 ,DarkGreen);
        DrawCircle(320, 325, 50, Gray);
        DrawRectangle(270, 550, 100, 60, Gray);
        DrawRectangle(270, 50, 100, 60, Gray);
        DrawLine(offset,325,offset + cellSize * cellCount,325,Gray);
        DrawText( "BEST SCORE :" ,offset+379 ,619 ,21 ,DarkGreen);
        DrawText(TextFormat("%i",bestScore),offset+540 ,offset+cellSize*cellCount+10 ,30 ,DarkGreen);
         DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellSize*cellCount+10, (float)cellSize*cellCount+10} ,5, DarkGreen);
        game.Draw();
       
        DrawFPS(540,10);
        EndDrawing();
    }
  
 

    CloseWindow();
    return 0;
}