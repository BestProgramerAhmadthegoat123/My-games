#include<iostream>
#include<raylib.h>
using namespace std;

Color LightBlue = { 135, 206, 235, 255 };
Color PipeGreen = { 0, 180, 0, 255 };
Color FlappyYellow = { 255, 220, 0, 255 };

int score = 0;
bool gameOver = false;
struct Pipe
{
    float x;
    float gapY;
    bool passed = false;
};
class Player
{
public:
    Texture2D birdTexture;
    int BallY = 250;
    int BallX = 30;
    int BallSpeed = 40;

void Draw()
{

DrawTexture(
    birdTexture,
    BallX - birdTexture.width / 2, 
    BallY - birdTexture.height / 2,
    WHITE
);
}

void Updating()
{
    if (gameOver) return;
   if (IsKeyPressed(KEY_SPACE))
    {
        BallY -= BallSpeed;
    }
    MoveBlockDown();
    LimitMovement();

}
void MoveBlockDown()
{
    BallY +=  1;
    LimitMovement();
}

void LimitMovement()
{
    if (BallY <=0 )
    {
        BallY = GetScreenHeight() - 460;
    }
    if (BallY >= 460)
    {
        BallY = GetScreenHeight() - 40;
    }

}
void Run()
{
    Draw();
    Updating();
    MoveBlockDown();
    LimitMovement();
}
void Reset()
{
    BallY = 250;
}

};
Player player = Player();
class Obstacles
{
public:
    static const int pipeCount = 3;
    Pipe pipes[pipeCount];

    float speed = 2;

    void Init()
    {
        for (int i = 0; i < pipeCount; i++)
        {
            pipes[i].x = 600 + i * 300;
            pipes[i].gapY = GetRandomValue(150, 350);
        }
    }

void Update()
{
    if (gameOver) return;

    for (int i = 0; i < pipeCount; i++)
{
    pipes[i].x -= speed;

    if (!pipes[i].passed && pipes[i].x + 40 < player.BallX)
    {
        score++;
        pipes[i].passed = true; // منع العد مرة ثانية
    }

    if (pipes[i].x < -40)
    {
        pipes[i].x = 800;
        pipes[i].gapY = GetRandomValue(150, 350);
        pipes[i].passed = false; // إعادة الحالة للبايب الجديد
    }

    CheckWithPlayer(pipes[i]);
}
}


    void Draw()
    {
        for (int i = 0; i < pipeCount; i++)
        {
            Rectangle topRec = { pipes[i].x, 0, 40, pipes[i].gapY - 80 };
            Rectangle bottomRec = { pipes[i].x, pipes[i].gapY + 80, 40, 500 };
           DrawRectangleRounded(topRec, 0.2f, 10, PipeGreen);
           DrawRectangleRounded(bottomRec, 0.2f, 10, PipeGreen);
        }
    }

    void CheckWithPlayer(Pipe p)
{
    if (gameOver) return;

    Vector2 ballPos = { (float)player.BallX, (float)player.BallY };

    Rectangle topRec = { p.x, 0, 40, p.gapY - 80 };
    Rectangle bottomRec = { p.x, p.gapY + 80, 40, 500 };

    if (CheckCollisionCircleRec(ballPos, 30, topRec) ||
        CheckCollisionCircleRec(ballPos, 30, bottomRec))
    {
        gameOver = true;
    }
}
void Reset()
{
    gameOver = false;
    Init();
    score = 0;
}
};
void DrawGameOver()
{
    DrawText("GAME OVER", 250, 200, 50, RED);
    DrawText("Press R to Restart", 300, 270, 20, WHITE);
}


Obstacles ob = Obstacles(); 

int main()
{

    
    InitWindow(800, 500, "Flappy Bird by Ahmed");
    SetTargetFPS(60);
    InitAudioDevice();                    // تفعيل الصوت مرة واحدة 
    Music bgMusic = LoadMusicStream("Sound/Music.mp3");  
    PlayMusicStream(bgMusic);
    player.birdTexture = LoadTexture("Textcuer/Bird.png");
    ob.Init();
    while(WindowShouldClose()== false)
    {
    BeginDrawing();
    ClearBackground(LightBlue);
    player.Run();
    ob.Update();
    ob.Draw();
    UpdateMusicStream(bgMusic);
    DrawText(TextFormat("Score: %i", score), 10, 10, 20, WHITE);
    if (gameOver)
{
    DrawGameOver();

    if (IsKeyPressed(KEY_R))
    {
        ob.Reset();
        player.Reset();
    }
}
    EndDrawing();
    

}

    
    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
    UnloadTexture(player.birdTexture);
    CloseWindow();
    return 0;
}




