#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define HASH_OF_CORRECT_PASSWORD 0x4b665b0

void winner_handler()
{
    printf("\033[32mCongraz! You complete the task!\033[0m\n");
    exit(0);
}

unsigned int getHash(const char *str, unsigned int len)
{
    const unsigned int bitsOfUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int threeQuarters = (unsigned int)((bitsOfUnsignedInt * 3) / 4);
    const unsigned int halfQuarter = (unsigned int)(bitsOfUnsignedInt / 8);
    const unsigned int highBits = (unsigned int)(0xFFFFFFFF << (bitsOfUnsignedInt - halfQuarter));

    unsigned int hash = 0;
    unsigned int test = 0;

    for (int i = 0; i < len; ++i)
    {
        hash = (hash << halfQuarter) + (*str++);

        if ((test = hash & highBits) != 0)
        {
            hash = ((hash ^ (test >> threeQuarters)) & (~highBits));
        }
    }

    return hash;
}

void setup()
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

unsigned int get_file_size(const char *file)
{
    struct stat statbuf;
    stat(file, &statbuf);
    int size = statbuf.st_size;
    return size;
}

int main()
{
    setup();

    char user_name[48] = {0};
    char password[48] = {0};
    char flags[48] = {0};

    int i = 0;
    char c;
    int user_name_fd = open("./user_name.txt", O_RDWR);
    if (user_name_fd == -1)
    {
        perror("open user_name.txt");
        exit(-1);
    }
    while (read(user_name_fd, &c, 1) != -1)
    {

        if (c == '\n')
            break;
        if (i > 47)
        {
            exit(-1);
        }
        user_name[i] = c;
        i++;
    }
    printf("Read %d bytes from user_name.txt\n", i);

    int password_fd = open("./password.txt", O_RDWR);
    if (password_fd == -1)
    {
        perror("open password.txt");
        exit(-1);
    }
    i = 0;
    while (read(password_fd, &c, 1) != -1)
    {
        if (c == '\n')
            break;
        if (i > 47)
        {
            exit(-1);
        }
        password[i] = c;
        i++;
    }
    close(user_name_fd);
    close(password_fd);
    printf("Read %d bytes from password.txt\n", i);
    printf("user_name ==> %s\n", user_name);
    printf("password ==> %s\n", password);

    if (strncmp(user_name, "We1s0n", 6) == 0 &&
        getHash(password, 32) == HASH_OF_CORRECT_PASSWORD)
    {
        strncpy(flags, "AcCes50k", 8);
    }

    if (strncmp(flags, "AcCes50k", 8) == 0)
    {
        printf("[+] Login Success!\n");
        winner_handler();
    }
    else
    {
        printf("[-] Wrong user name or password\n");
    }
}

//  gcc -g ./race_condition.c -o ./race_condition