#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char **argv)
{
    char *words;
    char *test;
    
    test = malloc( 512 );
    printf("test addr = 0x%08x\n", test);
    
    words = malloc( 512 );
    printf("words addr = 0x%08x\n", words);
    
    words[0] = test[0]+1; // 这里简单模拟下一个内存管理,内存管理的前一段有管理头信息
    
    printf("words addr = 0x%08x\n", words);
    
    free(words);
    free(test);
    return 0;
}
