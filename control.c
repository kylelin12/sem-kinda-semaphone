#include "main.h"

//views the contents of storytext.txt file
void viewstory(){
  char * line = NULL;
  size_t num = 0;
  ssize_t read;
  FILE *fp = fopen("storytext.txt", "r");
  if(fp == NULL){
    //story might not exist or something may have happened to path configs
    printf("Error in trying to access storytext.txt\n");
  }
  else{
    printf("Viewing story:\n\n");
    while ((read = getline(&line, &num, fp)) != -1) {
      printf("%s", line);
    }
    printf("\nEnd of story\n");
    fclose(fp);
  }
}

//creates a semaphore... or doesnt
void semaphore_create(){
  int id = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0664);
  // if -1, it existed, if not, semaphore didnt exist
  if (id != -1){
    // create a semaphore
    int semaphore;
    union semun su;
    su.val = 1;
    semctl(id, 0, SETVAL, su);
    printf("semaphore created: %d\n", id);
    printf("value set: %s\n", su);
  } 
  else{
    // semaphore exists
    printf("semaphore already exists\n");
  }
}

//creates a story... or doesnt.
void story_create(){
  //makes a new story
  //wb+: Truncate to zero length or create file for update
  FILE *fp = fopen("storytext.txt", "wb+");
  if(fp == NULL){
    //story might exist or something may have happened to path configs
    printf("Error in trying to create a new storytext.txt\n");
  }
  else{
    fclose(fp);
    printf("new storytext.txt created\n");
  }
}

//creates a shared memory... or doesnt.
void sm_create(){
  //create new shared memory
  int sm = shmget(SHM_KEY, 1000, IPC_CREAT | IPC_EXCL); 
  if (sm != -1){
    shmat(sm, 0, 0);
    printf("shared memory created: %d\n", SHM_KEY);
  }
  else{
    printf("Shared memory exists\n");
  }
}

//creates a semaphore, story, shared memory... or doesnt.
void all_create(){
  semaphore_create();
  story_create();
  sm_create();
}

//prints the story... or doesnt
void s_value(){
  viewstory();
}

//removes the semaphore... or doesnt
void semaphore_remove(){
  int id = semget(SEM_KEY, 0, 0);
  if(id == -1){
    printf("error: semaphore doesnt exist or no access to it\n");
  }
  else{
    printf("semaphore removed %d\n", semctl(id, 0, IPC_RMID));
  }
}

//removes the story... or doesnt
void story_remove(){
  viewstory();
  if(remove("storytext.txt") == 0){
    printf("Deleted story successfully\n");
  }
  else{
    printf("Unable to delete the story\n");
  }
}

//removes the shared memory... or doesnt
void sm_remove(){
  int sm = shmget(SHM_KEY, 0, 0);
  if(sm == -1){
    printf("error: shared doesnt exist or no access to it\n");
  }
  else{
    printf("shared memory removed: %d\n", shmctl(sm, 0, IPC_RMID));
  }
}

//removes the semaphore, story, shared memory... or doesnt
void all_remove(){
  semaphore_remove();
  story_remove();
  sm_remove();
}

int main(int argc, char *argv[]){
  if(argc != 2){
    printf("Invalid number of Arguments\n");
    return 1;
  }
  /*
    -c
    If the semaphore already exists, it should print a message saying so
  */
  if(!strcmp(argv[1],"-c")){
    all_create();
    return 0;
  }

  /*
    -v
    View the current value of the semaphore
  */
  if(!strcmp(argv[1],"-v")){
    s_value();
    return 0;
  }

  /*
    -r
    remove the semaphore
  */
  if(!strcmp(argv[1],"-r")){
    all_remove();
    return 0;
  }
  
  //Flag wasnt right
  printf("Invalid Flag\n");
  return 1;
}
