#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
int calculation_or();

// 括弧がある場合の処理
int analyze_parentheses(void) {
    int load_result;
    if (input_number_info[load_point] == '(') {
        load_point++;
        load_result = calculation_or();
        if (input_number_info[load_point] == ')') {
            load_point++;
        }
    } else {
        load_result = analyze_numbers();
    }
    return load_result;
}

/*
    演算子が重なっていないこと,演算子の後ろが空白では無いことを確認
    正常：0
    異常：1
*/
void double_operator_error_check(void) {
    int next_point = load_point;
    if (!isdigit((unsigned char)input_number_info[next_point]) && input_number_info[load_point] != '-') {
        printf("\n計算式が不正です\n");
        printf("処理を終了します\n");
        exit(1);
    }
}

/*
    数字または演算子以外の文字列が含まれていないことを確認
    正常：0
    異常：1
*/
void invalid_value_error_check(void) {
    // チェック対象外とする演算子
    int allowed_value[256];
    allowed_value['+'] = 1;
    allowed_value['-'] = 1;
    allowed_value['*'] = 1;
    allowed_value['/'] = 1;
    allowed_value['%'] = 1;
    allowed_value['&'] = 1;
    allowed_value['|'] = 1;
    allowed_value['^'] = 1;
    allowed_value['('] = 1;

    int i = 0;
    while (input_number_info[i] != '\0') {
        if (!isdigit(input_number_info[i]) 
        && input_number_info[i] != '\n'
        && !allowed_value[(unsigned char)input_number_info[i]]) {
            printf("\n数字以外の文字列が含まれています\n");
            printf("処理を終了します\n");
            exit(1);
        }
        i++;
    }
}

// 掛け算と割り算
int calculation_mul_div(void) {
    int load_result;
    load_result = analyze_parentheses();
    while(input_number_info[load_point] == '*' || input_number_info[load_point] == '/' || input_number_info[load_point] == '%') {
        if(input_number_info[load_point] == '*') {
            load_point++;
            double_operator_error_check();
            load_result *= analyze_parentheses();
        } else if (input_number_info[load_point] == '/') {
            load_point++;
            double_operator_error_check();
            load_result /= analyze_parentheses();
        } else if (input_number_info[load_point] == '%') {
            load_point++;
            double_operator_error_check();
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
            double_operator_error_check();
            load_result += calculation_mul_div();
        } else if (input_number_info[load_point] == '-') {
            load_point++;
            double_operator_error_check();
            load_result -= calculation_mul_div();
        }
    }
    return load_result;
}

// ビット演算（AND）
int calculation_and(void) {
    int load_result;
    load_result = calculation_add_sub();
    while(input_number_info[load_point] == '&') {
        load_point++;
        double_operator_error_check();
        int temp_result = calculation_add_sub();
        load_result = load_result & temp_result;
    }
    return load_result;
}

// ビット演算（XOR）
int calculation_xor(void) {
    int load_result;
    load_result = calculation_and();
    while(input_number_info[load_point] == '^') {
        load_point++;
        double_operator_error_check();
        int temp_result = calculation_and();
        load_result = load_result ^ temp_result;
    }
    return load_result;
}

// ビット演算（or）
int calculation_or(void) {
    int load_result;
    load_result = calculation_xor();
    while(input_number_info[load_point] == '|') {
        load_point++;
        double_operator_error_check();
        int temp_result = calculation_xor();
        load_result = load_result | temp_result;
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
            invalid_value_error_check();
            int load_result = calculation_or();
            printf("入力された値は：%d\n", load_result);
            load_point = 0;
        } else {
            printf("計算を終了します\n");
            break;
        }
    }
}