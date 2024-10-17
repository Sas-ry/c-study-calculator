#include <stdio.h>
#include <string.h>

int main(void) {
    printf("数字を入れて下さい\n");
    char in_num_char[256];
    scanf("%255s", &in_num_char);
    int in_num = in_num_char[0] - '0'; 
    printf("入力された値は：%d\n", in_num);

}