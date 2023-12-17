char* bs_expand_tilde(char *path) {
    if (path == NULL || strlen(path) == 0) {
        return NULL;
    }

    if (path[0] == '~') {
        char *home = getenv("HOME");
        if (home != NULL) {
            size_t home_len = strlen(home);
            size_t path_len = strlen(path);
            char *expanded_path = malloc(home_len + path_len);
            if (expanded_path != NULL) {
                strcpy(expanded_path, home);
                strcat(expanded_path, path + 1);  // Skip the ~
                return expanded_path;
            } else {
                perror("malloc");
                return NULL;
            }
        } else {
            perror("getenv");
            return NULL;
        }
    } else {
        return strdup(path);
    }
}
void bs_set_env_variable(char *variable, char *value) {
    if (variable == NULL || value == NULL) {
        fprintf(stderr, "bs: NULL value put into bs_set_env_variable\n");
        exit(EXIT_FAILURE);
    }

    // Construct a new string for the environment variable
    size_t size = strlen(variable) + strlen(value) + 2; // +2 for '=' and '\0'
    char *env_variable = malloc(size);
    if (env_variable == NULL) {
        fprintf(stderr, "bs: allocation error\n");
        free(env_variable);
        exit(EXIT_FAILURE);
    }

    // Copy the variable and value into the new string
    snprintf(env_variable, size, "%s=%s", variable, value);

    // Set the environment variable
    if (putenv(env_variable) != 0) {
        perror("putenv");
        free(env_variable);
        exit(EXIT_FAILURE);
    }
    free(env_variable);
}
char **bs_split_line(char *line)
{
  int bufsize = BS_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "bs: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, BS_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += BS_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "bs: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, BS_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}
char *bs_read_line(void)
{
  int bufsize = BS_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "bs: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += BS_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "bs: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}
char *bs_read_line_from_script(FILE* file)
{
  int bufsize = BS_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "bs: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = fgetc(file);

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      if(c == EOF && position == 0){
          free(buffer);
          return NULL;
      }
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += BS_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "bs: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}
int bs_launch(char **args){
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("bs");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("bs");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}