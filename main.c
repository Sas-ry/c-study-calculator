#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// 変数リスト
typedef struct VarList {
    char key[26];
    char value[26];
} VarList;

VarList variable_list;

// 読み込み位置
int load_point = 0;
// 入力情報
char input_number_info[256];

// 変数代入フラグ
int var_def_flag = 0;

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
    allowed_value[' '] = 1;

    int i = load_point;
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


// 変数フラグの有効化確認
void variable_flag_check(void) {
    // 計算結果を変数に入れる必要があるか確認
    if ((input_number_info[0] >= 'a' && input_number_info[0] <= 'z') && input_number_info[1] == '=') {
        // 変数フラグを有効化
        var_def_flag = 1;
        // 配列のポイントを代入用の「=」の後ろまで進める
        load_point = 2;
    }
}

// 式の中にある変数を置き換える
void variable_conversion(void) {
    // 現時点での読み込み位置から式の解析をするため変数にload_pointを代入
    int i = load_point;
    for (i; i < 256; i++) {
        // 入力された文字列の中に変数が含まれているか確認
        if (input_number_info[i] >= 'a' && input_number_info[i] <= 'z') {
            // 変数が2文字以上ではないことを確認
            if (input_number_info[i + 1] >= 'a' && input_number_info[i + 1] <= 'z') {
                printf("変数名は1文字である必要があります");
                exit(1);
            }

            for (int j = 0; j < 26; j++) {
                // 式の中にある変数に該当する値が存在しているかを確認
                if (input_number_info[i] == variable_list.key[j]) {
                    // 変数を置き換え
                    input_number_info[i] = variable_list.value[j];
                }
            }
        }
    }
}

// 式の実行結果を変数に代入
void variable_value_set(int load_result) {
    // 変数フラグ初期化
    var_def_flag = 0;
    for (int i = 0; i < 26; i++) {
        // 変数リスト内から空の場所を探す
        if (variable_list.key[i] == '\0') {
            // 変数を代入
            variable_list.key[i] = input_number_info[0];
            variable_list.value[i] = load_result + '0';
            break;
        }
    }
}

void remove_spaces(void) {
    int j = 0;
    for (int i = 0; i < 256; i++) {
        if (input_number_info[i] != ' ') {
            input_number_info[j] = input_number_info[i];
            j++;
        }
    }
    //input_number_info[j] = '\0'; // 文字列の終端を設定
}

int main(void) {
    for(;;) {
        printf("\n数字を入れて下さい\n");
        scanf("%255[^\n]%*c", &input_number_info);
        int exit_check_res = exit_check();
        if (exit_check_res == 0) {
            remove_spaces();
            variable_flag_check();
            variable_conversion();
            invalid_value_error_check();
            int load_result = calculation_or();
            if (var_def_flag == 1) {
                variable_value_set(load_result);
            }
            printf("入力された値は：%d\n", load_result);
            load_point = 0;
        } else {
            printf("計算を終了します\n");
            break;
        }
    }
}