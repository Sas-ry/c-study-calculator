#include <stdio.h>
#include <string.h>

// 読み込み位置
int load_point = 0;
// 入力情報
char input_number_info[256];

// 数字の解析
int analyze_numbers(void) {
    int load_result = 0;
    // 数字の先頭に「-」が付いている場合
    if (input_number_info[load_point] == '-') {
        load_point++;
        for(int i = 0; '0' <= input_number_info[load_point] && '9' >= input_number_info[load_point]; load_point++) {
            load_result = load_result * 10 + input_number_info[load_point] - '0'; 
        }
        load_result *= -1;
    // 数字の先頭に「+」が付いている場合
    } else if(input_number_info[load_point] == '+') {
        load_point++;
        for(int i = 0; '0' <= input_number_info[load_point] && '9' >= input_number_info[load_point]; load_point++) {
            load_result = load_result * 10 + input_number_info[load_point] - '0'; 
        }
    // 数字の先頭に何も付いていない場合
    } else {
        for(int i = 0; '0' <= input_number_info[load_point] && '9' >= input_number_info[load_point]; load_point++) {
            load_result = load_result * 10 + input_number_info[load_point] - '0'; 
        }
    }
    return load_result;
}

// 未定義によるコンパイルエラー回避
int calculation_add_sub();

// 括弧がある場合の処理
int analyze_parentheses(void) {
    int load_result;
    if (input_number_info[load_point] == '(') {
        load_point++;
        load_result = calculation_add_sub();
        if (input_number_info[load_point] == ')') {
            load_point++;
        }
    } else {
        load_result = analyze_numbers();
    }
    return load_result;
}

// 掛け算と割り算
int calculation_mul_div(void) {
    int load_result;
    load_result = analyze_parentheses();
    while(input_number_info[load_point] == '*' || input_number_info[load_point] == '/' || input_number_info[load_point] == '%') {
        if(input_number_info[load_point] == '*') {
            load_point++;
            load_result *= analyze_parentheses();
        } else if (input_number_info[load_point] == '/') {
            load_point++;
            load_result /= analyze_parentheses();
        } else if (input_number_info[load_point] == '%') {
            load_point++;
            load_result %= analyze_parentheses();
        }
    }
    return load_result;
}

// 足し算と引き算
int calculation_add_sub(void) {
    int load_result;
    load_result = calculation_mul_div();
    while(input_number_info[load_point] == '+' || input_number_info[load_point] == '-') {
        if(input_number_info[load_point] == '+') {
            load_point++;
            load_result += calculation_mul_div();
        } else if (input_number_info[load_point] == '-') {
            load_point++;
            load_result -= calculation_mul_div();
        }
    }
    return load_result;
}

int exit_check(void) {
    if (strcmp(input_number_info, "quit") == 0 || strcmp(input_number_info, "exit") == 0) {
        return 1;
    } else {
        return 0;
    }
}

int main(void) {
    for(;;) {
        printf("\n数字を入れて下さい\n");
        scanf("%255s", &input_number_info);
        int exit_check_res = exit_check();
        if (exit_check_res == 0) {
            int load_result = calculation_add_sub();
            printf("入力された値は：%d\n", load_result);
            load_point = 0;
        } else {
            printf("計算を終了します\n");
            break;
        }
    }
}