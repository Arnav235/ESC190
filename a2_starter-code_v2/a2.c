#include "a2.h"

int bitwise_xor(int value){
    return value^KEY;
}

char *xor_encrypt(char c){
    int xor_res = bitwise_xor(c);
    char* bit_char_res = (char*)malloc(sizeof(char) * 8);
    for (int i = 0; i < 7; i++){
        if (xor_res & (1 << i)){
            bit_char_res[6-i] = '1';
        } else {
            bit_char_res[6-i] = '0';
        }
    }
    bit_char_res[7] = '\0';
    return bit_char_res;
}

char xor_decrypt(char *s){
    int res = 0;
    for (int i = 0; i < 7; i++){
        if (s[6-i] == '1'){
            res += pow(2, i);
        }
    }
    return (char)bitwise_xor(res);
}

// returns 1 if the number num is in the array arr, else returns 0
int find_num(int num, int arr[], int size){
    for (int i = 0; i < size; i++){
        if (arr[i] == num){return 1;}
    }
    return 0;
}

char *gen_code(char *msg){
    char* sc_code = (char*)malloc(sizeof(char) * 256);
    memset(sc_code, '0', 256*sizeof(char));
    char* binary_char;
    int sc_idx = 0;
    int iterator;
    int zero_idx[] = {17, 18, 19, 28, 29, 30, 33, 35, 44, 46, 49, 50, 51, 
                    60, 61, 62, 193, 194, 195, 209, 211, 225, 226, 227};
    int one_idx[] = {0, 1, 2, 3, 4, 11, 12, 13, 14, 15, 16, 20, 27, 31, 32, 
                    34, 36, 43, 45, 47, 48, 52, 60, 64, 65, 66, 67, 68, 69, 
                    76, 77, 78, 79, 80, 176, 177, 178, 179, 180, 192, 196, 208,
                    210, 212, 224, 228, 240, 241, 242, 243, 244, 255};
    // Setting the relevant template region equal to the zero character
    for (int i = 0; i < sizeof(zero_idx)/sizeof(zero_idx[0]); i++){
        sc_code[zero_idx[i]] = '0';
    }
    // Setting the relevant template region equal to the one character
    for (int i = 0; i < sizeof(one_idx)/sizeof(one_idx[0]); i++){
        sc_code[one_idx[i]] = '1';
    }
    // Adding the binary representation of the message to the SC
    for (int char_idx = 0; char_idx < strlen(msg); char_idx++){
        binary_char = xor_encrypt(msg[char_idx]);
        iterator = 0;
        while (iterator < 7){
            // if the particular index is apart of the template region, then we don't put any message bits there
            if (find_num(sc_idx, zero_idx, sizeof(zero_idx)/sizeof(zero_idx[0])) || find_num(sc_idx, one_idx, sizeof(one_idx)/sizeof(one_idx[0]))){
                sc_idx++;
                continue;
            } else {
                sc_code[sc_idx] = binary_char[iterator];
                sc_idx++;
                iterator++;
            }
        }
        free(binary_char);
    }
    return sc_code;
}

char *read_code(char *code){
    int zero_idx[] = {17, 18, 19, 28, 29, 30, 33, 35, 44, 46, 49, 50, 51, 
                    60, 61, 62, 193, 194, 195, 209, 211, 225, 226, 227};
    int one_idx[] = {0, 1, 2, 3, 4, 11, 12, 13, 14, 15, 16, 20, 27, 31, 32, 
                    34, 36, 43, 45, 47, 48, 52, 60, 64, 65, 66, 67, 68, 69, 
                    76, 77, 78, 79, 80, 176, 177, 178, 179, 180, 192, 196, 208,
                    210, 212, 224, 228, 240, 241, 242, 243, 244, 255};
    char* ret_msg = (char*)malloc(sizeof(char) * 26); // the message that we want to return
    int msg_idx = 0;
    int sc_idx = 0;
    int char_idx = 0;
    char* temp_str = (char*)malloc(sizeof(char) * 8);
    temp_str[7] = '\0';
    while (sc_idx < 255){
        if (find_num(sc_idx, zero_idx, sizeof(zero_idx)/sizeof(int)) || find_num(sc_idx, one_idx, sizeof(one_idx)/sizeof(one_idx[0]))) {
            sc_idx++;
            continue;
        }
        if (char_idx == 7){
            ret_msg[msg_idx] = xor_decrypt(temp_str);
            if (ret_msg[msg_idx] == KEY) {
                ret_msg[msg_idx] = '\0';
                break;
            }
            msg_idx++;
            char_idx = 0;
        } else {
            temp_str[char_idx] = code[sc_idx];
            char_idx++;
            sc_idx++;
        }
    }
    free(temp_str);
    return ret_msg;
}

char *compress(char *code){
    char* ret_code = (char*)malloc(sizeof(char) * 65); // the hexadecimal string that we're returning
    ret_code[64] = '\0';
    int code_idx = 0; // the index that we're at in the return code
    int hex_num = 0; // the hexadecimal number that we're constructing
    int hex_counter = 0; // the power of 2 that a particular binary digit coresponds to
    int sc_code_idx = 0; // the index that we're at in the SC code
    while (sc_code_idx <= 256){
        if (hex_counter == 4){
            ret_code[code_idx] = (hex_num < 10) ? (char)(hex_num + 48) : (char)(hex_num + 55);
            code_idx++;
            hex_counter = 0;
            hex_num = 0;
            if (sc_code_idx == 256){break;}
            continue;
        }
        if (code[sc_code_idx] == '1'){
            hex_num += pow(2, 3-hex_counter);
        }
        hex_counter++;
        sc_code_idx++;
    }
    return ret_code;
}

char *decompress(char *code){
    char* ret_sc_code = (char*)malloc(sizeof(char) * 256); // the SC code that we're returning
    int sc_code_idx = 0;
    int hex_num;
    for (int i = 0; i < 64; i++){
        hex_num = (code[i] > 64) ? (code[i]-55) : (code[i]-48);
        for (int p = 0; p < 4; p++){
            ret_sc_code[sc_code_idx] = (hex_num / (int)pow(2, 3-p) == 1) ? '1' : '0';
            hex_num -= (ret_sc_code[sc_code_idx] - 48) * pow(2, 3-p);
            sc_code_idx++;
        }
    }
    return ret_sc_code;
}

// this function return the minimum of a and b
int min(int a, int b){
    return (a <= b) ? a : b;
}

int calc_ld(char *sandy, char *cima){
    // creating the array that will store our LDs
    int **arr = (int**)malloc(sizeof(int*) * (strlen(sandy) +1));
    for (int i = 0; i < strlen(sandy)+1; i++){
        arr[i] = (int*)malloc(sizeof(int) * (strlen(cima) +1));
    }

    // setting the values of the first row and column equal to a linearly ascending sequence
    for (int i = 0; i < strlen(sandy) +1; i++){
        arr[i][0] = i;
    }
    for (int i = 1; i < strlen(cima) +1; i++){
        arr[0][i] = i;
    }

    int m = 0, n = 0;

    for (int m = 1; m < strlen(sandy)+1; m++){
        for (int n = 1; n < strlen(cima)+1; n++){
            if (sandy[m] == cima[n]){
                arr[m][n] = arr[m-1][n-1];
            } else {
                arr[m][n] = 1 + min(arr[m-1][n-1], min(arr[m-1][n], arr[m][n-1]) );
            }
        }
    }
    int res = arr[strlen(sandy)][strlen(cima)]; // saving the result of the computation
    
    // freeing the array that we allocated
    for (int i = 0; i < strlen(sandy)+1; i++){
        free(arr[i]);
    }
    free(arr);
    return res;
}