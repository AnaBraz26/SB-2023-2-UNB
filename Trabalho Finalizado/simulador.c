#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 1000
#define MAX_LINE_LENGTH 100

int memory[MEMORY_SIZE];
int accumulator = 0;
int programCounter = 0;

// Função para executar uma instrução
void executeInstruction(char *opcode, char *operand1, char *operand2) {
    if (strcmp(opcode, "1") == 0) { //ADD
        int value1 = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO ADD \n");
        accumulator += memory[value1];
        programCounter += 2;
    } else if (strcmp(opcode, "2") == 0) {//SUB
        int value1 = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO SUB \n");
        accumulator -= memory[value1];
        programCounter += 2;
    } else if (strcmp(opcode, "3") == 0) {//MUL
        int value1 = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO MUL \n");
        accumulator *= memory[value1];
        programCounter += 2;
    } else if (strcmp(opcode, "4") == 0) {//DIV
        int value1 = atoi(operand1);
        printf("VALOR DO DIV = %d\n\n", memory[value1]);
        printf("DEPOIS DA INSTRUCAO DIV \n");
        if (value1 == 0) {
            printf("Erro: Divisao por zero.\n");
            printf("SEGMENTATION FAULT \n", opcode);
            printf("Saida: %d ", memory[value1]);
            printf("PC <- %d accumulator <- %d\n", programCounter, accumulator);
            exit(1);
        }
        
        accumulator = accumulator/memory[value1];
        programCounter += 2;
    } else if (strcmp(opcode, "5") == 0) {//JMP
        int targetAddress = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO JMP \n");
        programCounter = targetAddress;
    } else if (strcmp(opcode, "6") == 0) {//JMPN
        int targetAddress = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO JMPN \n");
        if (accumulator < 0) {
            programCounter = targetAddress;
        } else {
            programCounter += 2;
        }
    } else if (strcmp(opcode, "7") == 0) {//JMPP
        int targetAddress = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO JMPP \n");
        if (accumulator > 0) {
            programCounter = targetAddress;
        } else {
            programCounter += 2;
        }
    } else if (strcmp(opcode, "8") == 0) {//JMPZ
        int targetAddress = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO JMPZ \n");
        if (accumulator == 0) {
            programCounter = targetAddress;
        } else {
            programCounter += 2;
        }
    } else if (strcmp(opcode, "9") == 0) {//COPY
        int sourceAddress = atoi(operand1);
        int destAddress = atoi(operand2);
        printf("DEPOIS DA INSTRUCAO COPY \n");
        memory[destAddress] = memory[sourceAddress];
        programCounter += 3;
    } else if (strcmp(opcode, "10") == 0) {//LOAD
        int address = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO LOAD \n");
        accumulator = memory[address];
        programCounter += 2;
    } else if (strcmp(opcode, "11") == 0) {//STORE
        int address = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO STORE \n");
        memory[address] = accumulator;
        programCounter += 2;
    } else if (strcmp(opcode, "12") == 0) {//INPUT
        int address = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO INPUT \n");
        printf("Informe um valor para a entrada: ");
        scanf("%d", &memory[address]);
        programCounter += 2;
    } else if (strcmp(opcode, "13") == 0) {//OUTPUT
        int address = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO OUTPUT \n");
        printf("Saida: %d ", memory[address]);
        programCounter += 2;
    } else if (strcmp(opcode, "14") == 0) {//STOP
        printf("DEPOIS DA INSTRUCAO STOP \n");
        programCounter += 1;
        exit(0);
    } else {
        int address = atoi(operand1);
        printf("SEGMENTATION FAULT \n", opcode);
        printf("Saida: %d ", memory[address]);
        printf("PC <- %d accumulator <- %d\n", programCounter, accumulator);
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.obj>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    char *opcode;
    char *operand1;
    char *operand2;

    // Inicialize a memória com zeros
    memset(memory, 0, sizeof(memory));

    // Loop para ler e executar as instruções
    while (fgets(line, sizeof(line), file) != NULL) {

        char *token;
        token = strtok(line, " ");

        while (token != NULL) {
            if(strcmp(token, "9") != 0){
                printf("\nElemento 1: %s, ", token);
                opcode = token;
                token = strtok(NULL, " ");
                if (token != NULL) {
                    printf("Elemento 2: %s\n", token);
                    operand1 = token;
                    token = strtok(NULL, " ");
                    
                }
                operand2 = 0;
            }
            else{
                printf("\nElemento 1: %s, ", token);
                opcode = token;
                token = strtok(NULL, " ");
                if (token != NULL) {
                    printf("Elemento 2: %s, ", token);
                    operand1 = token;
                    token = strtok(NULL, " ");
                    printf("Elemento 3: %s\n", token);
                    operand2 = token;
                    token = strtok(NULL, " ");
                }

            }
            executeInstruction(opcode, operand1, operand2);

            printf("PC <- %d accumulator <- %d\n", programCounter, accumulator);
            printf("Pressione ENTER para continuar...\n");
            getchar(); // Aguarda o usuário pressionar Enter

        }
    }

    fclose(file);

    printf("Simulacao concluida.\n");
    return 0;
}
