int bs_cd(char **args);
int bs_help(char **args);
int bs_exit(char **args);
int bs_export(char **args);
int bs_getenv(char **args);

char *bs_builtin_str[] = {
  "cd",
  "help",
  "exit",
  "export",
  "getenv"
};
int (*bs_builtin_func[]) (char **) = {
  &bs_cd,
  &bs_help,
  &bs_exit,
  &bs_export,
  &bs_getenv
};
int bs_num_builtins() {
  return sizeof(bs_builtin_str) / sizeof(char *);
}

int bs_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "bs: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("bs");
    }
  }
  return 1;
}
int bs_exit(char **args){
  exit(EXIT_SUCCESS);
}
int bs_export(char **args) {
    putenv(args[1]);
    return 1;
}
int bs_getenv(char **args){
    if (args[1] == NULL) {
        // Missing argument
        printf("Usage: getenv <variable>\n");
        return 1;
    }

    char *value = getenv(args[1]);
    if (value != NULL) {
        printf("%s=%s\n", args[1], value);
    } else {
        printf("%s not found in the environment.\n", args[1]);
    }

    return 1;
}
int bs_help(char **args){
  int i;
  printf("This is Bobmbshell!\n");
  printf("Copyright RoidsOS Foundation!\n");
  printf("Builtin commands:\n");

  for (i = 0; i < bs_num_builtins(); i++) {
    printf("  %s\n", bs_builtin_str[i]);
  }

  printf("Use the \"man\" command for information on other programs.\n");
  return 1;
}