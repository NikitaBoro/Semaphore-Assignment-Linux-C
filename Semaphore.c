#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define NUMOFSEM 5

int semid;
int status;
struct sembuf sops[NUMOFSEM];

union semun {
    int val;
    struct semid_ds* buf;
    unsigned short* array;
    struct seminfo* __buf;
};

void main() {
    union semun semarg;
    int i, j, k;

    semid = semget(IPC_PRIVATE, NUMOFSEM, 0600); // Set of 5 semaphores

    for (i = 0; i < NUMOFSEM; i++) {
        semarg.val = NUMOFSEM - i - 1;
        semctl(semid, i, SETVAL, semarg);
        int semValue = semctl(semid, i, GETVAL, semarg);
        printf("-- Semaphore %d value is %d\n", i, semValue);
    }

    sops->sem_flg = 0; 

    for (i = 0; i < NUMOFSEM; i++) {
        int pid = fork();
        if (pid == 0) {
            for (j = i + 1; j <= 100; j += NUMOFSEM) {
                sops[0].sem_num = i; // Current semaphore
                sops[0].sem_op = -NUMOFSEM + 1; // Wait for semaphore
                semop(semid, sops, 1);

                printf("-%d", j);
                fflush(stdout);

                for (k = 0; k < NUMOFSEM - 1; k++) {
                    sops[k].sem_num = (i + k + 1) % NUMOFSEM; // Indexes to run
                    sops[k].sem_op = 1; // Signal
                }

                semop(semid, sops, NUMOFSEM - 1); // Size of array currently
            }
            exit(0);
        }
    }

    for (i = 0; i < NUMOFSEM; i++) {
        wait(&status); // Wait for children to die
    }

    printf("\nfather finished\n");
    semctl(semid, 0, IPC_RMID, semarg); // Delete semaphores
}
