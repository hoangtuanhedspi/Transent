#include <stdio.h>
#include <stdlib.h>
#include <transent/interface.h>
int main(int argc, char* argv[])
{
  FILE* ptr = fopen("./clientdb/bitcoin.pdf","rb");
  FILE* pout = fopen("./tests/out.pdf","wb");
  if(ptr) printf("opened\n");
  char payload[PAY_LEN];
  char pay2[PAY_LEN];
  char buff[BUFF_SIZE];
  int len = 0,i = 0;
  int lenn = 0;
  while((len = fread(payload,1,PAY_LEN,ptr))>0){
    attach_payload(buff,payload,len);
    parse_packet(buff,pay2,&lenn);
    fwrite(pay2,1,len,pout);
    printf("%d-%d\n",i++,len);
  }
  fclose(pout);
  fclose(ptr);
}