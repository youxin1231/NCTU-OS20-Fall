#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 123456

int *ptr;

int main()
{
    int id;

    /************************* TODO 1 *************************/
    // Attach the memory section, the return value is a pointer to
    // the shared memory section
    /************************* TODO 1 *************************/
    if ((id = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666)) < 0)
    {
        printf("shmget error!");
        exit(-1);
    }

    ptr = (int *)shmat(id, NULL, 0);

    printf("\033[1;33m[client] The value is %d\033[0m\n", ptr[0]);

    while (1)
    {
        int cmd;

        printf("\n");
        printf("1: Show the value\n");
        printf("2: Modify the value\n");
        printf("3: Exit\n");
        printf("Enter commands: ");
        scanf("%d", &cmd);

        if (cmd == 1)
            printf("\033[1;33m[client] The value is %d\033[0m\n", ptr[0]);
        else if (cmd == 2)
        {
            printf("Input new value: ");
            scanf("%d", &ptr[0]);
        }
        else
            break;
    }
}
