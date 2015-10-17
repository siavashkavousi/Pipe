#include <iostream>
#include <unistd.h>

using namespace std;

#define READ 0
#define WRITE 1

int fib(int n) {
    int lo = 0;
    int hi = 1;
    for (int i = 0; i < n; i++) {
        hi = lo + hi;
        lo = hi - lo;
    }
    return lo;
}

int main() {
    int input = 6, result = 0;
    ssize_t size;

    int fd_0[2], fd_1[2];
    pid_t pid;

    if (pipe(fd_0) == -1) {
        printf("Pipe failed");
        return 1;
    }

    if (pipe(fd_1) == -1) {
        printf("Pipe failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        printf("Fork failed");
        return 1;
    }

    if (pid > 0) {
        close(fd_0[READ]);
        close(fd_1[WRITE]);

        scanf("%d", &input);
        size = write(fd_0[WRITE], &input, sizeof(input));
        if (size < 0) return 1;

        size = read(fd_1[READ], &result, sizeof(result));
        if (size < 0) return 1;
        else printf("Parent : The result is %d\n", result);

        close(fd_0[WRITE]);
        close(fd_1[READ]);
    } else {
        close(fd_1[READ]);
        close(fd_0[WRITE]);

        size = read(fd_0[READ], &input, sizeof(input));
        if (size < 0) return 1;
        else {
            printf("Child : The input number : %d\n", input);
            result = fib(input);
            size = write(fd_1[WRITE], &result, sizeof(result));
            if (size < 0) return 1;
        }

        close(fd_0[READ]);
        close(fd_1[WRITE]);
    }

    getchar();
    return 0;
}