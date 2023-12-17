
char* parse_zsh_escape_sequences(const char* input) {
    char *result = (char *)malloc(strlen(input) * 2 + 1); // Allocate enough space for parsed string

    if (result != NULL) {
        result[0] = '\0'; // Initialize the result string

        const char *ptr_input = input;

        while (*ptr_input) {
            if (ptr_input[0] == '\\' && ptr_input[1] == '0' && ptr_input[2] == '3' && ptr_input[3] == '3') {
                // ZSH-style escape sequence '\033' for color
                strcat(result, "\033");
                ptr_input += 4;
            } else if (ptr_input[0] == '\\' && ptr_input[1] == 'n') {
                // ZSH-style escape sequence '\n' for newline
                strcat(result, "\n");
                ptr_input += 2;
            } else if (*ptr_input == '%' && *(ptr_input + 1) != '\0') {
                // Handle other escape sequences using strcat
                switch (*(ptr_input + 1)) {
                    case 'n':
                        strcat(result, getenv("USER"));
                        break;
                    case 'm': {
                        char hostname[256];
                        if (gethostname(hostname, sizeof(hostname)) == 0) {
                            strcat(result, hostname);
                        } else {
                            perror("gethostname");
                        }
                        break;
                    }
                    case 'p': {
                        char cwd[1024];
                        if (getcwd(cwd, sizeof(cwd)) != NULL) {
                            char *home = getenv("HOME");
                            if (home != NULL) {
                                size_t home_len = strlen(home);
                                if (strncmp(cwd, home, home_len) == 0) {
                                    strcat(result, "~");
                                    strcat(result, cwd + home_len);
                                } else {
                                    strcat(result, cwd);
                                }
                            } else {
                                strcat(result, cwd);
                            }
                        } else {
                            perror("getcwd");
                        }
                        break;
                    }
                    case 'd': {
                        time_t t;
                        struct tm *tm_info;
                        time(&t);
                        tm_info = localtime(&t);
                        char buffer[20];
                        strftime(buffer, 20, "%a %b %d", tm_info);
                        strcat(result, buffer);
                        break;
                    }
                    case 'T': {
                        time_t t;
                        struct tm *tm_info;
                        time(&t);
                        tm_info = localtime(&t);
                        char buffer[9];
                        strftime(buffer, 9, "%T", tm_info);
                        strcat(result, buffer);
                        break;
                    }
                    case 'h': {
                        char *home = getenv("HOME");
                        if (home != NULL) {
                            strcat(result, home);
                        }
                        break;
                    }
                    case '#': {
                        if (getuid() == 0) {
                            strcat(result, "#");
                        } else {
                            strcat(result, "%");
                        }
                        break;
                    }
                    case 'B': {
                        strcat(result, "\033[1m");
                        break;
                    }
                    case 'f': {
                        strcat(result, "\033[0m");
                        break;
                    }
                    case 'U': {
                        strcat(result, "\033[4m");
                        break;
                    }
                    case 'u': {
                        strcat(result, "\033[24m");
                        break;
                    }
                    // Add more cases for other escape sequences as needed
                    default:
                        // Ignore unknown escape sequences
                        break;
                }
                ptr_input += 2; // Skip the escape sequence
            } else {
                char temp[2] = {*ptr_input, '\0'};
                strcat(result, temp);
                ptr_input++;
            }
        }
    }

    return result;
}

void anchor_strings(char* left, char* right) {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    int terminalWidth = w.ws_col;
    int leftLength = strlen(left);
    int rightLength = strlen(right) + 4;

    int spaceCount = terminalWidth - rightLength;

    if (spaceCount < 0) {
        // Terminal too small to display both strings
        printf("Terminal too small!\n");
        return;
    }

    printf("%*s%*s%s\n", leftLength, left, spaceCount, "", right);
}

void bs_set_prompt() {
    char *ps1 = getenv("PS1");
    char *ps1_line_2 = getenv("PS1_LINE_2");
    char *ps1_right = getenv("PS1_RIGHT");

    if (ps1 != NULL && ps1_right != NULL) {
        // Parse PS1
        char *ps1_parsed = parse_zsh_escape_sequences(ps1);

        // Parse PS1_RIGHT
        char *ps1_right_parsed = parse_zsh_escape_sequences(ps1_right);

        anchor_strings(ps1_parsed,ps1_right_parsed);

        if(ps1_line_2 != NULL){
            // Parse PS1 ln 2
            char *ps1_l2_parsed = parse_zsh_escape_sequences(ps1_line_2);
            printf("%s",ps1_l2_parsed);
            free(ps1_l2_parsed);
        }

        // Free allocated memory
        free(ps1_parsed);
        free(ps1_right_parsed);
    } else {
        printf("Bombshell>> ");
    }
}