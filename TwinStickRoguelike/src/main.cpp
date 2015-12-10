#include "Game.hpp"

#if _MSC_VER == 1900
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
  return _iob;
}
#endif

int main(int argc, char *argv[])
{
  WebSystem::SetSubprocess("CefExecutable.exe");

  int exit_code = WebSystem::Main();
  //If the exit code returned by main was anything other than -1, it means that this process is a child doing cef work for another process.
  //It also means that this child has closed, meaning we should exit main.
  if (exit_code >= 0)
  {
    return exit_code;
  }

  Game game;
  
  if (game.start())
  {
    game.mainLoop();

    game.quit();
  }

  return EXIT_SUCCESS;
}
