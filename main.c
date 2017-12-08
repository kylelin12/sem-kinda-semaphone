#include "main.h"

int main(){
  int v = sg_value();
  if(v){
    printf("Resources: %d\n", v);
  }
  else{
    printf("Resources: All are currently busy. Please standby.\n");
  }
  return 0;
}
