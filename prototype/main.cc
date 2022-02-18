
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <SDL2/SDL.h>

typedef int32_t Int;
typedef double Float;
typedef float SFloat;
typedef uint32_t UInt;
typedef int_fast8_t Bool;
#define true 0x1
#define false 0x0
#define null NULL

typedef void None;

typedef enum LogType : int_fast8_t
  {
    all = 0,
    debug,
    info,
    warning,
    error,
    critical,
    none,
    log_type_len
  } LogType;


class Logger
{
protected:
  Logger() = default;
  LogType current_type = error;
public:
  Logger(Logger const&) = delete;
  void operator=(Logger const&) = delete;
  static Logger& getInstance();

  Bool print(LogType type, const char* log, ...);
  None set_type(LogType type);
};

Logger&
Logger::getInstance()
{
  static Logger logger;
  return logger;
}
Logger& logger = Logger::getInstance();

Bool
Logger::print(LogType type, const char* log, ...)
{
  if (this->current_type > type)
    return false;

  va_list args;
  va_start(args, log);
  vfprintf(stdout, log, args);
  va_end(args);
  
  return true;
}

None
Logger::set_type(LogType type)
{
  this->current_type = type;
  return;
}

Int
main(None)
{
  logger.set_type(all);
  if (SDL_Init(SDL_INIT_VIDEO) != 0x0)
    {
      logger.print(critical,
                   "can't init sdl, logs: %s\n", SDL_GetError());
      return -0x1;
    }
  logger.print(debug, "test\n");
  logger.print(info, "test\n");
  logger.print(warning, "test\n");
  logger.print(error, "test\n");
  logger.print(critical, "test\n");
      
  return 0x0;
}

