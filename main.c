#include "main.h"

int main(){
  int v = sg_value();
  if(v){
    printf("Resources: %d\n", v);
    printf("Last line: %s\n",  sg_line());
    printf("Enter next line:");
    char line[100];
    fgets(line, sizeof(line), stdin);
  }
  else{
    printf("Resources: All are currently busy. Please standby.\n");
  }
  return 0;
}
