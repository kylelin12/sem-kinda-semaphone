#include "main.h"

static void sighandler(int signo){
  //user wants to exit
  if (signo == SIGINT){
    printf("\nlogout\n");
    exit(0);
  }
}

void last_line(FILE *fp, int *size) {
  printf("The last line entered was: \n");
  if(size == -1){
    printf("\n");
  }
  else{
    char *lline = (char *)calloc(1, *size + 1);
    lseek(fileno(fp), -1 * *size, SEEK_END);
    read(fileno(fp), lline, *size);
    printf("%s\n", lline);
    free(lline);
  }
}

void write_story(FILE *fp, char *line) {
  lseek(fileno(fp), 0, SEEK_END);
  write(fileno(fp), line, strlen(line));
}

int main(){
  FILE  *fp;
  int shm_id;
  int sem_id;
  int *size;

  signal(SIGINT, sighandler);

  printf("Welcome to Semaphone! A game based off of Telephone, utilizing semaphores!\n\n ***Note: you can exit at anytime by simply pressing ctrl+C.\n\n");
  printf("Rules of the game:\n1. Once in, we should display the last line added to the story\n2. We will prompt the use for the next line.\n");

  // Does a semaphore exist?
  if (sem_id = semget(SEM_KEY, 1, 0) == -1) {
    printf("No story exists. You need to make one!\n");
    exit(1);
  }

  // Give me access or else!
  printf("Waiting forever for the semaphone...\n");
  struct sembuf sbuf;
  sbuf.sem_num = 0;
  sbuf.sem_op = -1; // DOWN(semaphore);
  sbuf.sem_flg = SEM_UNDO; // Undo's when the process ends
  semop(sem_id, &sbuf, 1);
  printf("Semaphore: Okay all done. That took 2^64 seconds to complete. You can do whatever you want now.\n");

  fp = fopen("storytext.txt", "r");
  shm_id = shmget(SHM_KEY, sizeof(int), 0);
  size = shmat(shm_id, 0, 0);

  last_line(fp, size); // Prints latest line

  // User input
  printf("\nPlease input the next line:\n");
  char line[1024];
  fgets(line, sizeof(line), stdin);
  printf("You inputted: %s\n", line);

  // Write to file
  write_story(fp, line);

  // Size of new line
  *size = strlen(line);
  shmdt(size);

  // UP(semaphore)
  sbuf.sem_op = 1;
  semop(sem_id, &sbuf, 1);

  return 0;

}
