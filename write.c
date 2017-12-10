#include "main.h"

static void sighandler(int signo){
  //user wants to exit
  if (signo == SIGINT){
    printf("\nlogout\n");
    exit(0);
  }
}

int main(){
  printf("Welcome to Semaphone! A game based off of Telephone, utilizing semaphores!\n\n ***Note: you can exit at anytime by simply pressing ctrl+C.\n\n");
  printf("Rules of the game:\n1. Once in, we should display the last line added to the story\n2. We will prompt the use for the next line.\n");
  while (1) {
    signal(SIGINT, sighandler);
    printf("\n[testing] Please input the next line:\n");
    char line [100];
    fgets(line, sizeof(line), stdin);
    printf("\n%s\n",line);
  }
  return 0;
}
