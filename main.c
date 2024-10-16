#include <stdio.h>

int main(void) {
    printf("数字を入れて下さい\n");
    int in_num;
    scanf("%d", &in_num);

    printf("入力された値は：%d\n", in_num);

    return 0;
}