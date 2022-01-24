
#include <stdint.h>
#include <SDL2/SDL.h>

enum KEYCODES
  {
    KEY_NONE = 0x0,
    KEY_F = SDL_SCANCODE_F,
    KEY_B = SDL_SCANCODE_B,
    KEY_N = SDL_SCANCODE_N,
    KEY_P = SDL_SCANCODE_P,
    KEY_SEMICOLON = SDL_SCANCODE_SEMICOLON,
    KEYS_LEN = SDL_NUM_SCANCODES
  };
typedef enum KEYCODES KEYCODES;

/* Common types */
/*   Integers */
typedef int32_t Int;
typedef int_least64_t BigInt;
typedef uint32_t Uint;
typedef uint_least64_t BigUint;

/*   Floating points */

typedef float SmallFloat;
typedef double Float;

/*   Bool */
typedef uint_fast8_t Bool;
#define true 0x1
#define false 0x0

/*   None */
typedef void None;

/*   Pointer */
typedef void* Ptr;

/*   NULL */
#define null 0x0

/* === */

typedef struct Vec2f
{
  Float x, y;
} Vec2f;

typedef struct Entity
{
  BigUint uid;
} Entity;

typedef struct Player
{
  Entity* ent;
  Float speed;
  Vec2f pos;
} Player;

typedef struct Camera
{
  Entity* ent;
  Float speed;
  Vec2f pos;
} Camera;

typedef struct Controller
{
  Bool state;
} Controller;

typedef struct Game
{
  Entity* ent;
  Player* player;
  Camera* camera;
  Controller* control;
} Game;


extern SDL_Window* sdlwin;
extern SDL_Renderer* sdlrender;
extern Bool is_running;
extern Bool input_keys[KEYS_LEN];
extern Game* glob_game;

extern Int render_init(None);
extern Int render_close(None);

extern Int game_init(None);
extern Int game_logic(None);
extern Int main(None);

SDL_Window* sdlwin = null;
SDL_Renderer* sdlrender = null;

Game* glob_game = null;

Bool is_running = 0x0;

/* Input manager's keys */
Bool input_keys[KEYS_LEN] = { 0x0 };

Int
render_init(None)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0x0)
    {
      fprintf(stderr, "Error, can't start SDL:\n  %s\n", SDL_GetError());
      return -0x1;
    }
  sdlwin = SDL_CreateWindow("Fun",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            0x800,
                            0x800,
                            SDL_WINDOW_RESIZABLE);
  if (sdlwin == NULL)
    {
      fprintf(stderr, "Error, can't create window:\n  %s\n", SDL_GetError());
      SDL_Quit();
      return -0x2;
    }
  
  sdlrender = SDL_CreateRenderer(sdlwin, -0x1, SDL_RENDERER_ACCELERATED);

  return 0x0;
}

Int
render_close(None)
{
  SDL_DestroyRenderer(sdlrender);
  SDL_DestroyWindow(sdlwin);
  SDL_Quit();
  return 0x0;
}

BigUint
uid_new(None)
{
  static BigUint last_uid = 0x0;
  return last_uid++;
}

Entity*
entity_new(None)
{
  Entity* ent;

  ent = (Entity*) malloc(sizeof(Entity));

  if (ent == null)
    {
      fprintf(stderr, "Error, can't allocate new entity (base class)\n");
      return null;
    }

  ent->uid = uid_new();

  return ent;
}

Player*
player_new(None)
{
  Player* plr;

  plr = (Player*) malloc(sizeof(Player));

  if (plr == null)
    {
      fprintf(stderr, "Error, can't allocate new player\n");
      return null;
    }

  plr->ent = entity_new();
  
  return plr;
}

Camera*
camera_new(None)
{
  Camera* cam;

  cam = (Camera*) malloc(sizeof(Camera));

  if (cam == null)
    {
      fprintf(stderr, "Error, can't allocate new camera\n");
      return null;
    }

  cam->ent = entity_new();

  return cam;
}

Game*
game_new(None)
{
  Game* game;

  game = (Game*) malloc(sizeof(Game));
  
  if (game == null)
    {
      fprintf(stderr, "Error, can't allocate `new` game (HOW?!)\n");
      return null;
    }

  game->ent = entity_new();
  
  return game;
}

Controller*
controller_new(None)
{
  Controller* control;

  control = (Controller*) malloc(sizeof(Controller));

  if (control == null)
    {
      fprintf(stderr, "Error, can't allocate new controller\n");
      return null;
    }

  return control;
}

Int
game_init(None)
{
  glob_game = game_new();
  /* Player config */
  glob_game->player = player_new();
  glob_game->player->speed = 0x0.01p0;
  /* Camera config */
  glob_game->camera = camera_new();
  glob_game->camera->speed = 0x0.08p0;
  /* Controller config */
  glob_game->control = controller_new();
  glob_game->control->state = true;
  
  is_running = 0x1;
  return 0x0;
}

Int
controller_process(None)
{
  Controller* cont;
  Vec2f* cpos;
  Float speed;

  /* TODO: Add safe check */
  cont = glob_game->control;
  

  cpos = cont->state?
    &(glob_game->camera->pos):
    &(glob_game->player->pos);

  speed = cont->state?
    glob_game->camera->speed:
    glob_game->player->speed;

  fprintf(stdout, "speed: %lf\n", speed);

  if (input_keys[KEY_F])
    cpos->x += speed;
  if (input_keys[KEY_B])
    cpos->x -= speed;
  if (input_keys[KEY_N])
    cpos->y += speed;
  if (input_keys[KEY_P])
    cpos->y -= speed;
  if (input_keys[KEY_SEMICOLON])
    cont->state = !cont->state;
  
  return 0x0;
}

Int
game_logic(None)
{
  controller_process();
  
  /* Draw player */
  SDL_Rect rect;
  rect = (SDL_Rect)
    {
      .x = glob_game->player->pos.x-glob_game->camera->pos.x,
      .y = glob_game->player->pos.y-glob_game->camera->pos.y,
      .w = 0x10,
      .h = 0x10,
    };
  SDL_SetRenderDrawColor(sdlrender, 0x00, 0xff, 0x00, 0xff);
  SDL_RenderDrawRect(sdlrender, &rect);

  fprintf(stdout, "State: %d\ncam: %lf %lf\nplr: %lf %lf\n",
          glob_game->control->state,
          glob_game->camera->pos.x,
          glob_game->camera->pos.y,
          glob_game->player->pos.x,
          glob_game->player->pos.y
          );

  return 0x0;
}

Int
main(None)
{
  if (render_init() != 0x0)
    {
      fprintf(stderr, "Error, can't init render\n");
      return -0x1;
    }

  game_init();

  while (is_running)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
        {
          switch (event.type)
            {
            case SDL_QUIT:
              is_running = 0x0;
              break;
            case SDL_KEYDOWN:
              input_keys[event.key.keysym.scancode] = 0x1;
              break;
            case SDL_KEYUP:
              input_keys[event.key.keysym.scancode] = 0x0;
              break;
            default:
              break;
            }
        }
      
      SDL_SetRenderDrawColor(sdlrender, 0x00, 0x00, 0x00, 0xff);
      SDL_RenderClear(sdlrender);

      game_logic();
      
      SDL_RenderPresent(sdlrender);
    }

  render_close();
  return 0x0;
}
