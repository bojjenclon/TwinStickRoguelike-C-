#include <iostream>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/DataPak.h>
#include <locale>
#include <algorithm>
#include <codecvt>

using namespace Awesomium;

std::string GetApplicationDir()
{
  auto hModule = GetModuleHandleW(nullptr);
  WCHAR wpath[MAX_PATH];

  GetModuleFileNameW(hModule, wpath, MAX_PATH);
  std::wstring wide(wpath);

  typedef std::codecvt_utf8<wchar_t> convert_type;
  std::wstring_convert<convert_type, wchar_t> converter;

  std::string path = converter.to_bytes(wide);
  std::replace(path.begin(), path.end(), '\\', '/');
  path = path.substr(0, path.find_last_of("\\/"));

  return path;
}

int main(int argc, char** argv)
{
  unsigned short filesWritten;
  auto success = WriteDataPak(WSLit((GetApplicationDir() + "/../assets/resources.pak").c_str()), WSLit((GetApplicationDir() + "/../html/dist").c_str()), WSLit(""), filesWritten);

  std::cout << (success ? "true" : "false") << ", " << filesWritten << std::endl;
}