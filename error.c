

void error_redirect(char *err) {
    int file_fd = open("success_error.txt", O_WRONLY | O_CREAT | O_APPEND);
    if (file_fd == -1) {
        perror("\nFile opening failed");
        exit(EXIT_FAILURE);
    }
    if (dup2(file_fd, STDERR_FILENO) == -1) {
        perror("\ndup2() failed");
        exit(EXIT_FAILURE);
    }
    perror(err);
    close(file_fd);
    close(STDERR_FILENO);
}

void success_redirect(char *success) {
    int file_fd = open("success_error.txt", O_WRONLY | O_CREAT | O_APPEND);
    if (file_fd == -1) {
        perror("\nFile opening failed");
        exit(EXIT_FAILURE);
    }
    if (dup2(file_fd, STDOUT_FILENO) == -1) {
        perror("\ndup2() failed");
        exit(EXIT_FAILURE);
    }
    printf("\n%s", success);
    close(file_fd);
}