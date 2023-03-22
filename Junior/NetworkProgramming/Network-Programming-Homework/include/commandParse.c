#define MAX_COMMANDS_SIZE 5000

/**
 * @brief split the command with space
 * 
 * @param command command to split
 * @param args the array to store the splited command
 * @return the number of the split commands
 */
int command_parse(char *command, char **args) {
  int argc = 0;
  char *arg = malloc(MAX_COMMANDS_SIZE * sizeof(char));
  char *delim = " \n";
  arg = strtok(command, delim);
  while (arg != NULL) {
    args[argc++] = arg;
    arg = strtok(NULL, delim);
  }
  args[argc] = NULL;
  return argc;
}