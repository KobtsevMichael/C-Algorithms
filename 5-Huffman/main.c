#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "operations.h"
#include "tree.h"

#define CODE_LEN (32)
#define BUF_LEN (320)
#define ZERO ("0\0")
#define ONE ("1\0")

#define IN_FILE fopen("in.txt", "rb")
#define OUT_FILE fopen("out.txt", "wb")
#define u_char unsigned char


int printCompressedText(const char *string, int len, FILE* f_out) {

    u_char *output = malloc(len * sizeof(u_char));

    int last_byte = 0, index = 0;
    char bin[9] = {0};

    for (int i=0; i < len; i+=8) {
        if (i+8 > len) {
            substring(string, bin, i, len);
            for (int j=len; j < i+8; ++j) {
                strcat(bin, ZERO);
                last_byte++ ;
            }
        }
        else {
            substring(string, bin, i, i+8);
        }
        output[index++] = convertToChar(bin);
    }

    fwrite(output, sizeof(u_char), index, f_out);
    free(output);

    return last_byte;
}

void generateTextCode(const u_char* input, char* out_code, int len,
                      char codes_arr[256][CODE_LEN]) {

    for (int i=0; i < len; ++i) {
        strcat(out_code, codes_arr[(int) input[i]]);
    }
}

void generateTreeCode(tree* node, char *code) {

    if (node == NULL) return;

    if (node->is_leaf) {
        strcat(code, ONE);
        strcat(code, convertToBin(node->symbol));
    }
    else {
        strcat(code, ZERO);
    }

    generateTreeCode(node->left, code);
    generateTreeCode(node->right, code);
}

void createCodesArr(tree *node, char *code, int iter,
                    char mass[256][CODE_LEN]) {

    if (node->is_leaf) {
        if (iter == 0)
            strcpy(mass[(int) node->symbol], ONE);
        else
            strncpy(mass[(int) node->symbol], code, iter);
    }
    else {
        code[iter] = '0';
        createCodesArr(node->left, code, iter+1, mass);

        code[iter] = '1';
        createCodesArr(node->right, code, iter+1, mass);
    }
}

void compress(FILE* f_in, FILE* f_out) {

    // Инициализация
    int res, idx, last_byte = 0;
    u_char buf[BUF_LEN];
    char buf_code[CODE_LEN*BUF_LEN + 8];
    char codes_arr[256][CODE_LEN];
    char empty_code[CODE_LEN];

    // Заполнение нулями
    for (int i=0; i < 256; ++i) {
        memset(codes_arr[i], 0, sizeof(codes_arr[i]));
    }
    buf_code[0] = 0;

    // Создание дерева
    tree* nodes = initializeTree();
    if (!(res = fread(buf, sizeof(u_char), BUF_LEN, f_in))) {
        exit(0);
    }
    while (res) {
        addLeaves(nodes, buf, res);
        res = fread(buf, sizeof(u_char), BUF_LEN, f_in);
    }
    tree* head = buildTree(nodes, 256);

    // Создание массива с кодами символов
    createCodesArr(head, empty_code, 0, codes_arr);

    // Кодирование дерева и вывод
    generateTreeCode(head, buf_code);
    idx = strlen(buf_code) - strlen(buf_code)%8;
    printCompressedText(buf_code, idx, f_out);

    // Копирование последних символов из кода дерева
    copyChar(buf_code, &buf_code[idx]);

    // Кодирование текста и вывод
    fseek(f_in, 3, SEEK_SET);
    while ( (res = fread(buf, sizeof(u_char), BUF_LEN, f_in)) ) {

        generateTextCode(buf, buf_code, res, codes_arr);

        if (feof(f_in)) {
            idx = strlen(buf_code);
            last_byte = printCompressedText(buf_code, idx, f_out);
        }
        else {
            idx = strlen(buf_code) - strlen(buf_code)%8;
            printCompressedText(buf_code, idx, f_out);
            copyChar(buf_code, &buf_code[idx]);
        }
    }

    // Вывод последнего символа
    u_char last_symb[2] = {last_byte+'0', 0};
    fwrite(last_symb, sizeof(u_char), 1, f_out);

    free(nodes);
}

int decode(tree* nodes, const char* input, u_char* output, int len,
           int index, int last_index, int *iter) {

    tree* node = &nodes[0];
    last_index = index;

    while (!node->is_leaf) {
        if      (index == len)        return last_index;
        else if (input[index] == '0') node = node->left;
        else if (input[index] == '1') node = node->right;
        index++;
    }
    // Если вершина - это лист
    if (nodes[0].is_leaf) {
        if ((index++) == len) return last_index;
    }

    output[(*iter)++] = node->symbol;
    return decode(nodes, input, output, len, index, last_index, iter);
}

int readLastBuf(u_char* input, char* bin_code, int size) {

    for (int i=0; i < size-1; ++i) {
        strcat(bin_code, convertToBin(input[i]));
    }

    return input[size-1]-'0';
}

void readCompressedText(u_char* input, char* bin_code, int size) {

    for (int i=0; i < size; ++i) {
        strcat(bin_code, convertToBin(input[i]));
    }
}

void decompress(FILE* f_in, FILE* f_out) {

    // Инициализация
    int res, idx = 0, free_idx = 1;
    int code_len, last_byte, out_size = 0;
    u_char buf[BUF_LEN];
    char buf_code[CODE_LEN*BUF_LEN + 8];
    u_char output[CODE_LEN*BUF_LEN + 8];

    // Чтение кода дерева
    if (!(res = fread(buf, sizeof(u_char), BUF_LEN, f_in))) {
        exit(0);
    }
    else if (feof(f_in)) {
        buf_code[0] = 0;
        last_byte = readLastBuf(buf, buf_code, res);
        code_len = strlen(buf_code) - last_byte;
    }
    else {
        buf_code[0] = 0;
        readCompressedText(buf, buf_code, res);
        code_len = strlen(buf_code);
    }

    // Воссоздание дерева
    tree* nodes = malloc(511 * sizeof(tree));
    buildRestoredTree(nodes, nodes, buf_code, &idx, &free_idx);

    // Декодирование и вывод текста в буфере после дерева
    idx = decode(nodes, buf_code, output, code_len, idx, 0, &out_size);
    fwrite(output, sizeof(u_char), out_size, f_out);

    // Декодирование остальной части текста
    while (!feof(f_in)) {
        // Очистка буфера вывода и копирование последних
        // символов текста
        copyChar(buf_code, &buf_code[idx]);
        memset(output, 0, sizeof(output));
        // Чтение
        res = fread(buf, sizeof(u_char), BUF_LEN, f_in);
        // Перевод в строку из 0 и 1
        if (feof(f_in)) {
            last_byte = readLastBuf(buf, buf_code, res);
            code_len = strlen(buf_code) - last_byte;
        }
        else {
            readCompressedText(buf, buf_code, res);
            code_len = strlen(buf_code);
        }
        // Декодирование и вывод
        out_size = 0;
        idx = decode(nodes, buf_code, output, code_len, 0, 0, &out_size);
        fwrite(output, sizeof(u_char), out_size, f_out);
    }

    free(nodes);
}

int main(void) {

    FILE* in_file = IN_FILE;
    FILE* out_file = OUT_FILE;
    u_char mode[3];

    if (!fread(mode, sizeof(u_char), 3, in_file)) {
        exit(0);
    }

    if (mode[0] == 'c') compress(in_file, out_file);
    if (mode[0] == 'd') decompress(in_file, out_file);

    fclose(in_file);
    fclose(out_file);

    return 0;
}
