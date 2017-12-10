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
  int *bad = (int *)-1;
  if(size == bad){
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

void write_story(char *line) {
  //lseek(fileno(fp), 0, SEEK_END);
  //write(fileno(fp), line, strlen(line));
  FILE *fp = fopen("storytext.txt", "a");
  fprintf(fp, "%s", line);
  fclose(fp);
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

  fclose(fp);
  // Write to file
  //write_story(fp, line);
  write_story(line);
  printf("\nTESTING\n");
  // Size of new line
  int *new_size = (int *)strlen(line);
  size = new_size;
  printf("\nTESTING2\n");
  shmdt(size);
  printf("\nTESTING3\n");
  // UP(semaphore)
  sbuf.sem_op = 1;
  printf("\nTESTING4\n");
  semop(sem_id, &sbuf, 1);
  printf("\nTESTING5\n");

  return 0;

}
