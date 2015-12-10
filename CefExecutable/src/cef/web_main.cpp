#include "cef/WebSystem.hpp"

int main(int argc, char* argv[])
{
  //Run the process and capture the exit code.
  int exit_code = WebSystem::Main();

  //When we are finished running, exit with the captured exit code.
  if (exit_code >= 0)
  {
    return exit_code;
  }

  //It should not be possible to reach this location.

  //This program can also be written simply as:
  //return WebSystem::Main();
}