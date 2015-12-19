#include <Game.hpp>

#if _MSC_VER == 1900
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
  return _iob;
}
#endif

int main(int argc, char *argv[])
{
  Game game;

  if (game.start())
  {
    game.mainLoop();

    game.quit();
  }

  return EXIT_SUCCESS;
}
