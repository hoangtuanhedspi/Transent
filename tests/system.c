#include <stdio.h>
#include <stdlib.h>

int main()
{
  FILE* ptr = fopen("./clientdb/test.pdf","rb");
  if(ptr) printf("opened\n");
  char payload[1024];
  int rlen = fread(payload,sizeof(char),1024,ptr);
  printf("Pass:%d\n",rlen);
  fclose(ptr);
}