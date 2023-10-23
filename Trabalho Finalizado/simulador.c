//// Software Básico - Simulador 2023/2
// Ana Caroline da Rocha Braz
// Andreza Rodrigues Costa

//------------------------------------------------------------------------------------
// AVISO IMPORTANTE
// LEMBRE-SE SEMPRE DE DEIXAR O ARQUIVO NA MESMA PASTA DO PROGRAMA

// ---------------------------------------------------------------------------------
// PARA RODAR UTILIZE
// gcc simulador.c -o simulador
// ./simulador [nome_do_arquivo].obj

//----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 1000
#define MAX_LINE_LENGTH 100

int memory[MEMORY_SIZE];
int accumulator = 0;
int programCounter = 0;

// Função para executar uma instrução
int executeInstruction(char *opcode, char *operand1, char *operand2, char **vetor) {

    if (strcmp(opcode, "1") == 0) { //ADD
        int value1 = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO ADD \n");
        accumulator += atoi(vetor[value1]);
        programCounter += 2;
        return 0;
    } 
    else if (strcmp(opcode, "2") == 0) {//SUB
        int value1 = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO SUB \n");
        accumulator -= atoi(vetor[value1]);
        programCounter += 2;
        return 0;
    } 
    else if (strcmp(opcode, "3") == 0) {//MUL
        int value1 = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO MUL \n");
        accumulator *= atoi(vetor[value1]);
        programCounter += 2;
        return 0;
    } 
    else if (strcmp(opcode, "4") == 0) {//DIV
        int value1 = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO DIV \n");
        if (value1 == 0) {
            printf("Erro: Divisao por zero.\n");
            printf("SEGMENTATION FAULT \n", opcode);
            printf("Saida: %s ", vetor[value1]);
            printf("PC <- %d accumulator <- %d\n", programCounter, accumulator);
            exit(1);
        }        
        accumulator = accumulator/atoi(vetor[value1]);
        programCounter += 2;
        return 0;
    } 
    else if (strcmp(opcode, "5") == 0) {//JMP
        int targetAddress = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO JMP \n");
        programCounter = targetAddress;
        return targetAddress;
    } 
    else if (strcmp(opcode, "6") == 0) {//JMPN
        int targetAddress = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO JMPN \n");
        if (accumulator < 0) {
            programCounter = targetAddress;
            return targetAddress;
        } else {
            programCounter += 2;
            return 0;
        }
    } 
    else if (strcmp(opcode, "7") == 0) {//JMPP
        int targetAddress = atoi(operand1);
        printf("%d \n",accumulator);
        printf("DEPOIS DA INSTRUCAO JMPP \n");
        if (accumulator > 0) {
            programCounter = targetAddress;
            return targetAddress;
        } else {
            programCounter += 2;
            return 0;
        }
    } 
    else if (strcmp(opcode, "8") == 0) {//JMPZ
        int targetAddress = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO JMPZ \n");
        if (accumulator == 0) {
            programCounter = targetAddress;
            return targetAddress;
        } else {
            programCounter += 2;
            return 0;
        }
    } 
    else if (strcmp(opcode, "9") == 0) {//COPY
        int sourceAddress = atoi(operand1);
        int destAddress = atoi(operand2);
        printf("DEPOIS DA INSTRUCAO COPY \n");
        printf("Fazendo copia de %s para %s\n", vetor[sourceAddress], vetor[destAddress]);
        vetor[destAddress] = vetor[sourceAddress];       
        programCounter += 3;
        return 0;
    } 
    else if (strcmp(opcode, "10") == 0) {//LOAD
        int address = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO LOAD \n");
        accumulator = atoi(vetor[address]);
        programCounter += 2;
        return 0;
    } 
    else if (strcmp(opcode, "11") == 0) {//STORE
        int address = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO STORE \n");
        sprintf(vetor[address], "%d", accumulator);
        programCounter += 2;
        return 0;
    } 
    else if (strcmp(opcode, "12") == 0) {//INPUT
        int address = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO INPUT \n");
        printf("Informe um valor para a entrada: ");
        scanf("%d", &memory[address]);
        sprintf(vetor[address], "%d", memory[address]);
        programCounter += 2;
        return 0;
    } 
    else if (strcmp(opcode, "13") == 0) {//OUTPUT
        int address = atoi(operand1);
        printf("DEPOIS DA INSTRUCAO OUTPUT \n");
        printf("Saida: %s ", vetor[address]);
        programCounter += 2;
        return 0;
    } 
    else if (strcmp(opcode, "14") == 0) {//STOP
        printf("DEPOIS DA INSTRUCAO STOP \n");
        programCounter += 1;
        exit(0);
    } else {
        int address = atoi(operand1);
        printf("SEGMENTATION FAULT \n", opcode);
        printf("Saida: %s ", vetor[address]);
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
    char **vetor = NULL;
    int i = 0;

    // Inicialize a memória com zeros
    memset(memory, 0, sizeof(memory));

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " ");
        while (token != NULL) {
            i++;
            vetor = (char **)realloc(vetor, i * sizeof(char *));
            vetor[i - 1] = strdup(token);
            token = strtok(NULL, " ");            
        }
    }
    
    for(int j = 0; j < i; j++){
        int jump;

        if(strcmp(vetor[j], "9") != 0){
            printf("Elemento 1: %s, Elemento 2: %s\n", vetor[j],vetor[j+1]);
            jump = executeInstruction(vetor[j], vetor[j+1], "0", vetor);            
            if(jump == 0){
                j++;
            }
            else{
                j = jump - 1;
            }
        }
        else{
            printf("Elemento 1: %s, Elemento 2: %s, Elemento 3: %s\n", vetor[j],vetor[j+1], vetor[j+2]);
            jump = executeInstruction(vetor[j], vetor[j+1], vetor[j+2], vetor);
            if(jump == 0){
                j += 2;
            }
            else{
                j = jump - 1;
            }          
        }
        
        printf("PC <- %d accumulator <- %d\n", programCounter, accumulator);
        printf("Pressione ENTER para continuar...\n\n");
        getchar(); // Aguarda o usuário pressionar Enter

    }


    fclose(file);

    printf("Simulacao concluida.\n");
    return 0;
}
