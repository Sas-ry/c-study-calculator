#include <stdio.h>
#include <string.h>

int load_point = 0;
char in_num_char[256];

int analyze_number(void) {
    int load_result = 0;
    int i;
    for(i = 0; '0' <= in_num_char[load_point] && '9' >= in_num_char[load_point]; load_point++) {
        load_result = load_result * 10 + in_num_char[load_point] - '0'; 
    }
    
    return load_result;
}

int main(void) {
    printf("数字を入れて下さい\n");
    scanf("%255s", &in_num_char);
    int load_result;
    load_result = analyze_number();
    while(in_num_char[load_point] == '+' || in_num_char[load_point] == '-') {
        if(in_num_char[load_point] == '+') {
            load_point++;
            load_result += analyze_number();
        } else if (in_num_char[load_point] == '-') {
            load_point++;
            load_result -= analyze_number();
        }
    }
    printf("入力された値は：%d\n", load_result);

}