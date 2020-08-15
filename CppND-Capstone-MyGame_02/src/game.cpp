#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height) 
  : snake(grid_width, grid_height)
{
  std::cout << "Game : Initializing" << std::endl;
  // Snake snake( (int)grid_width, (int)grid_height);
  std::mt19937 engine(dev());
  std::uniform_int_distribution<int> random_w(0, static_cast<int>(grid_width ));
  std::uniform_int_distribution<int> random_h(0, static_cast<int>(grid_height));   
  // Ball ball;     // this way calls copy-constructor, probably
  // Racket racket;
}

void Game::Run(Controller const &controller, Renderer &renderer, std::size_t target_frame_duration) 
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) 
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake, racket);
    Update();
    renderer.Render(snake, ball, racket, blocks, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) 
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) 
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}


void Game::Update() {
  if (!snake.alive) return;

  snake.Update();
  ball.UpdatePosition();
  racket.UpdatePosition();

  if (checker.checkBallvsRacket(ball, racket))
  {
    // std::cout << "Collision to Racket" << std::endl;
  }

  if (checker.checkBallvsBlocks(ball, blocks))
  {
    score++;
    // std::cout << "Collision to Block" << std::endl;
  }

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }