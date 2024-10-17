#include <stdio.h>
#include <string.h>

int main(void) {
    printf("数字を入れて下さい\n");
    char in_num_char[256];
    scanf("%255s", &in_num_char);
    int i = 0;
    int in_num = 0;
    for(i = 0; '0' <= in_num_char[i] && '9' >= in_num_char[i]; i++) {
        in_num = in_num * 10 + in_num_char[i] - '0'; 
    }
    printf("入力された値は：%d\n", in_num);

}