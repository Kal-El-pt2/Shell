#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <vector>

enum validCommands
{
  echo,
  cd,
  exit0,
  type,
  Builtin,
  Executable,
  invalid,
};


struct FullCommandType
{
  validCommands type;
  std::string executable_path;
};

std::vector<std::string> parse_command_to_string_vector(std::string command);
FullCommandType command_to_full_command_type(std::string command);
std::string find_command_executable_path(std::string command);
std::string find_command_in_path(std::string command, std::string path);


std::string get_path(std::string command)
{
  std::string path_env = std::getenv("PATH");

  std::stringstream ss(path_env);
  std::string path;
  while (!ss.eof())
  {
    getline(ss, path, ':');
    std::string abs_path = path + '/' + command;
    if (std::filesystem::exists(abs_path))
    {
      return abs_path;
    }
  }
  return "";
}

validCommands isValid(std::string command)
{
  command = command.substr(0, command.find(" "));

  if (command == "echo")
    return validCommands::echo;
  if (command == "cd")
    return validCommands::cd;
  if (command == "exit")
    return validCommands::exit0;
  if (command == "type")
    return validCommands::type;

  return invalid;
}

std::string valid[4] = {"echo", "cd", "exit 0", "type"};

int main()
{

  bool exit = false;

  while (!exit)
  {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> command_vector = parse_command_to_string_vector(input);
    if (command_vector.size() == 0)
    {
      continue;
    }
    FullCommandType fct = command_to_full_command_type(command_vector[0]);
    std::string command_name = command_vector[1];
    FullCommandType command_type = command_to_full_command_type(command_name);

    switch (isValid(input))
    {
    case cd:

      break;
    case echo:
      input.erase(0, input.find(" ") + 1);
      std::cout << input << "\n";
      break;
    case exit0:
      exit = true;
      break;
    case type:
      input.erase(0, input.find(" ") + 1);
      if (isValid(input) != invalid)
      {
        std::cout << input << " is a shell builtin\n";
      }
      else
      {
        std::string path = get_path(input);

        if (path.empty())
        {
          std::cout << input << ": not found\n";
        }
        else
        {
          std::cout << input << " is " << path << "\n";
        }
      }

      break;
      case Builtin:
          std::cout << input << " is a shell builtin\n";
          break;
        case Executable:
          std::cout << input << " is " << command_type.executable_path << "\n";
          break;
    default:
      std::cout << input << ": command not found\n";
      break;
    }
    if (fct.type == Executable)
    {
      std::string command_with_full_path = fct.executable_path;
      for (int argn = 1; argn < command_vector.size(); argn++)
      {
        command_with_full_path += " ";
        command_with_full_path += command_vector[argn];
      }
      const char *command_ptr = command_with_full_path.c_str();
      system(command_ptr);
      continue;
    }
  }
  return 0;
}
