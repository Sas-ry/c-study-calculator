#include <stdio.h>
#include <string.h>

int load_point = 0;
char in_num_char[256];

int analyze_number(void) {
    int load_result = 0;
    for(load_point = 0; '0' <= in_num_char[load_point] && '9' >= in_num_char[load_point]; load_point++) {
        load_result = load_result * 10 + in_num_char[load_point] - '0'; 
    }
    
    return load_result;
}

int main(void) {
    printf("数字を入れて下さい\n");
    scanf("%255s", &in_num_char);
    int load_result = analyze_number();
    printf("入力された値は：%d\n", load_result);

}