#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    float *words;
    char *test;
    unsigned int i;
    
    srand(time(NULL));
    
    while(1){
        printf("--->>\n");
        test = (char *)malloc( sizeof(char)*16 );
        if(test == NULL){
            printf("test malloc error.\n");
        }
        printf("test addr = 0x%08x, size = %d\n", test,sizeof(test[0]));
        
        words = (float *)malloc( sizeof(float)*16 );
        if(words == NULL){
            printf("words malloc error.\n");
        }
        printf("words addr = 0x%08x, size = %d\n", words,sizeof(words[0]));
#if 1
        for(i=0;i<16;i++)
            test[i] = rand();
#endif   
        for(i=0;i<16;i++){
            printf("test[%d] = %d, addr = 0x%08x\n",i,test[i],&test[i]);
            words[i] = sqrt(pow(test[i],2)+1); // 这里简单模拟下一个内存管理,内存管理的前一段有管理头信息
            printf("words[%d] = %d,addr = 0x%08x\n",i,words[i],&words[i]);
        }
        
        
        
        for(i=0;i<16;i++){
            printf("word[%d] = %f\n",i,words[i]);
        }
        printf("test addr = 0x%08x\n", test);
        printf("words addr = 0x%08x\n", words);
        
        free(words);
        free(test);
    }
    return 0;
}
