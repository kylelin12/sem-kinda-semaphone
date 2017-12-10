#include "main.h"

//gets the contents of storytext.txt file
char *getstory(){
  struct stat sb;
  stat("storytext.txt", &sb);
  // If the story doesn't exist
  int fd;
  if ((fd = open("storytext.txt", O_RDONLY)) == -1) {
    printf("No story exists!\n");
    exit(1);
  } else {
    int size = sb.st_size;
    char *story = calloc(1, size + 1);
    read(fd, story, size);
    return story;
  }
}

//views the contents of storytext.txt file
void viewstory(){
  char *story = getstory();
  printf("Viewing story:\n%s\n", story);
  free(story);
}

//creates a semaphore... or doesnt
int semaphore_create(){
  int sem_id = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0664);
  int val;
  // Exists?
  if (sem_id != -1){
    // create a semaphore
    int semaphore;
    union semun su;
    su.val = 1;
    val = semctl(sem_id, 0, SETVAL, su);
    printf("semaphore created: %d\n", sem_id);
    printf("value set: %d\n", val);
  } 
  return sem_id;
}

//creates a story... or doesnt.
void story_create(){
  int fd;
  if( access("storytext.txt", F_OK) == -1 ){
    fd = open("storytext.txt", O_CREAT | O_TRUNC, 0644);
    printf("created storytext.txt\n");
    close(fd);
  }
  else{
    printf("story already exists!\n");
  }
}

//creates a shared memory... or doesnt.
void sm_create(){
  int sm_id = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0644); 
  if (sm_id != -1){
    printf("shared memory created: %d\n", SHM_KEY);
  }
  else{
    printf("Shared memory exists\n");
  }
}

//creates a semaphore, story, shared memory... or doesnt.
void all_create(){
  int sem_id = semaphore_create();
  if (sem_id != -1) {
    story_create(sem_id);
    sm_create();
  } else {
    printf("Cannot create. Already exists!\n");
  }
}

//prints the story... or doesnt
void s_value(){
  viewstory();
}

//removes the semaphore... or doesnt
void semaphore_remove(){
  int sem_id = semget(SEM_KEY, 0, 0);
  if(sem_id == -1){
    printf("error: semaphore doesnt exist or no access to it\n");
  }
  else{
    printf("semaphore removed %d\n", semctl(sem_id, 0, IPC_RMID));
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
  int sm_id = shmget(SHM_KEY, 0, 0);
  if(sm_id == -1){
    printf("error: shared doesnt exist or no access to it\n");
  }
  else{
    printf("shared memory removed: %d\n", shmctl(sm_id, 0, IPC_RMID));
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
