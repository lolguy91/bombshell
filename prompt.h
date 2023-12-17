void bs_set_prompt() {
    char *ps1 = getenv("PS1");
    if (ps1 != NULL) {
        // Parse ZSH-style escape sequences
        char *ptr = ps1;
        while (*ptr) {
            if(ptr[0] == '\\' && ptr[1] == '0' && ptr[2] == '3' && ptr[3] == '3'){
              printf("\033");
              ptr += 4;
            }else if(ptr[0] == '\\' && ptr[1] == 'n'){
              printf("\n");
              ptr += 2;
            }else if (*ptr == '%' && *(ptr + 1) != '\0') {
                switch (*(ptr + 1)) {
                    case 'n': {
                        // Username of the current user
                        char *user = getenv("USER");
                        if (user != NULL) {
                            printf("%s", user);
                        }
                        break;
                    }
                    case 'm': {
                        // Hostname
                        char hostname[256];
                        if (gethostname(hostname, sizeof(hostname)) == 0) {
                            printf("%s", hostname);
                        } else {
                            perror("gethostname");
                        }
                        break;
                    }
                    case 'p': {
                        // Current working directory, with home replaced by ~
                        char cwd[1024];
                        if (getcwd(cwd, sizeof(cwd)) != NULL) {
                            char *home = getenv("HOME");
                            if (home != NULL) {
                                size_t home_len = strlen(home);
                                if (strncmp(cwd, home, home_len) == 0) {
                                    printf("~%s", cwd + home_len);
                                } else {
                                    printf("%s", cwd);
                                }
                            } else {
                                printf("%s", cwd);
                            }
                        } else {
                            perror("getcwd");
                        }
                        break;
                    }
                    case 'd': {
                        // Current date in the format "Tue Sep 30"
                        time_t t;
                        struct tm *tm_info;
                        time(&t);
                        tm_info = localtime(&t);
                        char buffer[20];
                        strftime(buffer, 20, "%a %b %d", tm_info);
                        printf("%s", buffer);
                        break;
                    }
                    case 'T': {
                        // Current time in the format "23:01:01"
                        time_t t;
                        struct tm *tm_info;
                        time(&t);
                        tm_info = localtime(&t);
                        char buffer[9];
                        strftime(buffer, 9, "%T", tm_info);
                        printf("%s", buffer);
                        break;
                    }
                    case 'h': {
                        // Home directory
                        char *home = getenv("HOME");
                        if (home != NULL) {
                            printf("%s", home);
                        }
                        break;
                    }
                    case '#': {
                        // A # for root, % for normal users
                        if (getuid() == 0) {
                            putchar('#');
                        } else {
                            putchar('%');
                        }
                        break;
                    }
                    case 'B': {
                        // Bold text
                        printf("\033[1m");
                        break;
                    }
                    case 'f': {
                        // Reset foreground color
                        printf("\033[0m");
                        break;
                    }
                    case 'U': {
                        // Start underline
                        printf("\033[4m");
                        break;
                    }
                    case 'u': {
                        // Stop underline
                        printf("\033[24m");
                        break;
                    }
                    // Add more cases for other escape sequences as needed
                    default:
                        // Ignore unknown escape sequences
                        break;
                }
                ptr += 2; // Skip the escape sequence
            } else {
                putchar(*ptr);
                ptr++;
            }
        }
    } else {
        printf("Bombshell>> ");
    }
}