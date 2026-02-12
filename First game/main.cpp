#include<iostream>
#include <raylib.h>
using namespace std;

Color Green = Color{165, 220, 200, 255};
Color Dark_Green = Color{170, 255, 195, 255};
Color Light_Green = Color{135, 205, 185, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int cpu_score = 0;

class Ball
{
public:
float x,y;
int speed_x , speed_y;
int radiuse;

void Draw()
{
    DrawCircle(x, y, radiuse, Yellow);
}
void Update()
{
    x += speed_x;
    y += speed_y;

    if(y + radiuse >= GetScreenHeight() || y - radiuse <= 0)
    {
        speed_y *= -1;
    }
    if(x + radiuse >= GetScreenWidth() )
    {
         cpu_score++;
         ResetBall();
    }
    
    if(x - radiuse <= 0)
    {
        player_score++;
        ResetBall();
    }
}

void ResetBall()
{
    x = GetScreenWidth()/2;
    y = GetScreenHeight()/2;

    int speed_choices[2]= {-1,1};
    speed_x *= speed_choices[GetRandomValue(0,1)]; 
    speed_y *= speed_choices[GetRandomValue(0,1)]; 
}
};

class Paddle 
{
protected:
void LimitMovement()
{
        if (y <=0)
    {
        y =0;
    }
    if (y + height >= GetScreenHeight())
    {
        y = GetScreenHeight() - height;
    }
}
public:
float x,y;
float width, height;
int speed;

void Draw()
{
DrawRectangleRounded( Rectangle{x,y,width,height}  ,0.8, 0, WHITE);
}
void Update()
{
    if(IsKeyDown(KEY_UP))
    {
        y = y - speed;
    }
    if(IsKeyDown(KEY_DOWN))
    {
        y = y + speed;
    }
    LimitMovement();
}
};

class CpuPaddle : public Paddle
{
public:
void Update(int ball_y)
{
    if(y + height/2 > ball_y)
    {
        y = y - speed + 3 ;
    }
    if(y + height/2 <= ball_y)
    {
        y = y + speed - 3;
    }
    LimitMovement();
}

};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main()
{

    cout<< "Starting the game" <<endl;

    const int screen_width = 800;
    const int screen_height = 500;
    InitWindow(screen_width, screen_height, "Check out my game niggers !");
    SetTargetFPS(60);

    ball.radiuse = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x =8;
    ball.speed_y =8;

    player.width =20;
    player.height =100;
    player.x = 10;
    player.y = (screen_height / 2) - (player.height / 2);
    player.speed =6;

cpu.width = 20;
cpu.height = 100;
cpu.x = screen_width - cpu.width - 10;
cpu.y = (screen_height / 2) - (cpu.height / 2);
cpu.speed = 6;

    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        //Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        //Checking for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radiuse, Rectangle{player.x, player.y, player.width, player.height} ))
        {
            ball.speed_x *= -1;
        }
         if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radiuse, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height} ))
        {
            ball.speed_x *= -1;
        }

        //Drawing 
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width/2,0, screen_width/2 ,screen_height, Green);
        DrawCircle(screen_width/2, screen_height/2, 100, Light_Green);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i",cpu_score), screen_width/4 -20, 20, 80, WHITE);
        DrawText(TextFormat("%i",player_score), 3 * screen_width/4 -20, 20, 80, WHITE);
        EndDrawing();

    }


    CloseWindow();
    return 0;
}