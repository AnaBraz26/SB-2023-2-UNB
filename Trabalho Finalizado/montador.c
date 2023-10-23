// Software Básico - Montador 2023/2
// Ana Caroline da Rocha Braz
// Andreza Rodrigues Costa

//------------------------------------------------------------------------------------
// AVISO IMPORTANTE
// LEMBRE-SE SEMPRE DE DEIXAR O ARQUIVO NA MESMA PASTA DO PROGRAMA

// ---------------------------------------------------------------------------------
// PARA RODAR UTILIZE
// gcc montador.c -o montador
// ./montando [nome_do_arquivo].asm

//--------------------------------------------------------------------------------------
// Não foi implementado o pre-processamento para as macros

//--------------------------------------------------------------------------------------
//Bibliotecas
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//---------------------------------------------------------------------------------
// Conta a quantidade de linhas do arquivo
int counter(FILE *file){
    printf("\nFAZENDO A CONTAGEM DE LINHAS\n");
    char caractere;
    int contagem_linhas = 0;    

    while ((caractere = fgetc(file)) != EOF) {
        if (caractere == '\n') {
            contagem_linhas++;
        }
    }
    
    if (caractere != '\n') {
        contagem_linhas++;
    }

    return contagem_linhas;
}

//-----------------------------------------------------------------------------------------------
// Tamanho de cada operacao de acordo com a tabela dada em sala de aula
int tamanho_op(char *opcode){

    if ((strcmp(opcode, "COPY") == 0) || (strcmp(opcode, "copy") == 0)){
        return 3; 
    }

    if( (strcmp(opcode, "SECAO") == 0) || (strcmp(opcode, "secao") == 0)){
        return 0; 
    }

    if ((strcmp(opcode, "STOP") == 0)  || (strcmp(opcode, "stop") == 0) || 
       (strcmp(opcode, "SPACE") == 0) || (strcmp(opcode, "space") == 0)|| 
       (strcmp(opcode, "CONST") == 0) || (strcmp(opcode, "const") == 0))
    {
        return 1; 
    }

    return (2); // o resto das operações só retorna 2. 

}

//-------------------------------------------------------------------------------------------
// Procura se existe label
int acha_label(char *termo){

    int i; 
    i = 0; 

    while(i < 30){
        if(termo[i] == ':'){ 
            return 1; 
        }
        if(termo[i] == '\0'){
            return 0;
        }
        i++; 
    }
}

//--------------------------------------------------------------------------------------
// Define o código do operando de acordo com a tabela passad a em sala de aula
int define_op_code(char *opcode){

    if((strcmp(opcode, "ADD") == 0) || (strcmp(opcode, "add") == 0)){
        return 1; 
    }

    if( (strcmp(opcode, "SUB") == 0) || (strcmp(opcode, "sub") == 0)){
        return 2; 
    }

    if((strcmp(opcode, "MUL") == 0) || (strcmp(opcode, "mult") == 0)){
        return 3; 
    }

    if((strcmp(opcode, "DIV") == 0) || (strcmp(opcode, "div") == 0)){
        return 4; 
    }
    
    if((strcmp(opcode, "JMP") == 0) || (strcmp(opcode, "jmp") == 0)){
        return 5; 
    }
    
    if((strcmp(opcode, "JMPN") == 0) || (strcmp(opcode, "jmpn") == 0)){
        return 6; 
    }

    if((strcmp(opcode, "JMPP") == 0) || (strcmp(opcode, "jmpp") == 0)){
        return 7; 
    }

    if((strcmp(opcode, "JMPZ") == 0) || (strcmp(opcode, "jmpz") == 0)){
        return 8; 
    }

    if((strcmp(opcode, "COPY") == 0) || (strcmp(opcode, "copy") == 0)){
        return 9; 
    }

    if((strcmp(opcode, "LOAD") == 0) || (strcmp(opcode, "load") == 0)){
        return 10; 
    }

    if((strcmp(opcode, "STORE") == 0) || (strcmp(opcode, "store") == 0)){
        return 11; 
    }

    if((strcmp(opcode, "INPUT") == 0) || (strcmp(opcode, "input") == 0)){
        return 12; 
    }

    if((strcmp(opcode, "OUTPUT") == 0) || (strcmp(opcode, "output") == 0)){
        return 13; 
    }

    if((strcmp(opcode, "STOP") == 0) || (strcmp(opcode, "stop") == 0)){
        return 14; 
    }

    return 0;
}
// ---------------------------------------------------------------------------------------
// MAIN

int main(int argc, char *argv[]){
    printf("\nINICIANDO MONTADOR!\n\n");

    //----------------------------------------------------
    // Variaveis úteis
    int tamanho;
    int qtd_linhas;
    int i,j,x,loop, erro = 0;
    int contagem_linha = 0;
    int somador_op = 0;
    int tem_label = 0;
    int simbolos_existente = 0;
    int simbolo_encontrado = 0;
    int acc_end = 0;
    int op;
    int numero = 0;
    char end_em_string[12];
    char op_em_string[12];
    int secao_text = 0;
    int secao_data = 0;
    int stop = 0;
    char *leitura_feita;
  
    //----------------------------------------------------------------------
    // Tokens
    int limite_tam_termo = 30; 
    char delimitador[] = ",\n ";
    char *token;
    char *termo1;
    char *termo2;
    char *termo3;
    char *termo4;
    char *linha;

    termo1 = (char *) malloc(limite_tam_termo * sizeof(char)); 
    termo2 = (char *) malloc(limite_tam_termo * sizeof(char));
    termo3 = (char *) malloc(limite_tam_termo * sizeof(char));
    termo4 = (char *) malloc(limite_tam_termo * sizeof(char));

    linha = (char *) malloc((4*limite_tam_termo) * sizeof(char));

    //---------------------------------------------------
    // Leitura do arquivo 
    FILE *arquivo_asm;
    FILE *arquivo_obj;
    char *nome_obj;
    int tam_nome_arq = strlen(argv[1]);

    arquivo_asm = fopen(argv[1], "r");

    if(arquivo_asm == NULL){
        printf("Problemas na abertura do arquivo\n");
        return 0;
    }
    
    strcpy(nome_obj, argv[1]);
    nome_obj[tam_nome_arq-1] = 'j';
    nome_obj[tam_nome_arq-2] = 'b';
    nome_obj[tam_nome_arq-3] = 'o';
    
    qtd_linhas = counter(arquivo_asm); // Contar quantas linhas tem o arquivo
    fclose(arquivo_asm);
    
    //------------------------------------------------------------------------
    // VERIFICAÇÃO DE QUAL SECAO VEM PRIMEIRO - TERMINANDO

    FILE *temp;
    char *novo_asm = "arquivo_temporario.asm";

    arquivo_asm = fopen(argv[1], "r");
    temp = fopen(novo_asm, "a");

    // SE O ARQUIVO INICIAR COM DATA, VAI JOGAR PRA OUTRO ARQUIVO E FAZER A INVERSÃO 
    while(fgets(linha, 200, arquivo_asm) != NULL){   
        if(strcmp(linha, "SECAO TEXT\n") == 0){
            fprintf(temp, "%s", linha);
            while(fgets(linha, 200, arquivo_asm) != NULL){
                if (strcmp(linha, "SECAO DATA\n") != 0)
                {                   
                    fprintf(temp, "%s", linha);       
                }
                else{
                    break;
                }                         
            }
        }
    }    

    fclose(arquivo_asm);
    fclose(temp);
    
    arquivo_asm = fopen(argv[1], "r");
    temp = fopen(novo_asm, "a");
    
    while(fgets(linha, 200, arquivo_asm) != NULL){   
        if(strcmp(linha, "SECAO DATA\n") == 0){
            fprintf(temp, "%s", linha);
            while(fgets(linha, 200, arquivo_asm) != NULL){
                if(strcmp(linha, "SECAO TEXT\n") != 0){
                    fprintf(temp, "%s", linha);
                }
                else{
                    break;
                }   
            }
        }
    }          

    fclose(arquivo_asm);
    fclose(temp);

    //----------------------------------------------------------------
    // PRIMEIRA PASSAGEM
    // Detecção de Erros e gração da Tabela de simbolos
    printf("-------------------------------------------------------------------------\n");
    printf("COMECO DA PRIMEIRA PASSAGEM\n\n");
    // Tabela de Simbolos 

    arquivo_asm = fopen(novo_asm, "r");

    char (*tabela_de_simbolos) [2][30] = malloc(sizeof(char) * qtd_linhas * 2 * 30);
    // tabela_de_simbolos[x][0] = simbolo
    // tabela_de_simbolos[x][1] = endereço

    // Preenchendo a tabela de simbolos
    for(i = 0; i < qtd_linhas; i++){
        for(j = 0; j < 2; j++){
            strcpy(tabela_de_simbolos[i][j], "\0");
        }
    }

    i = 0;
    j = 0;

    // -----------------------------------------------------------------
    // Verificação das linhas do arquivos
    while (fgets(linha, 200, arquivo_asm ) != NULL) {
        
        printf("Linha %d: %s", contagem_linha, linha);

        if(strcmp(linha, "\n") == 0){
            continue;
        }
        else{

        sprintf(end_em_string, "%d", contagem_linha);

        // Verifica os tokens e armazena em cada termo da sequencia
        token = strtok(linha, delimitador);       
        i = 1;

        do{
            switch(i){

                case 1 : strcpy(termo1, token);
                case 2 : strcpy(termo2, token);
                case 3 : strcpy(termo3, token);
                case 4 : strcpy(termo4, token); 
            
            }
            i++;  
            token = strtok(NULL, delimitador);             
        }while(token != NULL);
        

        i--;

        // Verificação de cada linha e seus elementos
        // if(i==1){
        //     printf("Termos na linha: %s\nQuantidade: %d\n", termo1, i);
        // }
        // if(i==2){
        //     printf("Termos na linha: %s %s\nQuantidade: %d\n", termo1, termo2, i);
        // }
        // if(i==3){
        //     printf("Termos na linha: %s %s %s\nQuantidade: %d\n", termo1, termo2, termo3, i);
        // }
        // if(i==4){
        //     printf("Termos na linha: %s %s %s %s\nQuantidade: %d\n", termo1, termo2, termo3, termo4, i);
        // }     
      
       printf("\nCOMECANDO A VERIFICAR ERROS\n\n");

       somador_op = tamanho_op(termo1);

       tem_label = acha_label(termo1);
    

       if (tem_label == 1){           
            //Tratando Labels

            if(i != 1){
               
                somador_op = tamanho_op(termo2); 
                acc_end = acc_end + somador_op;
                
            }
            else{
                somador_op = 0; 
            }

            // Eliminação do : para a tabela de simbolos
            for(x = 0; x < 30; x++){
                if(termo1[x] == ':'){     
                    termo1[x] = '\0';                 
                }
            } 

            // Procura simbolo na tabela atual para redefinar
            for(x = 0; x < simbolos_existente ; x++){
                if(strcmp(termo1, tabela_de_simbolos[x][0]) == 0 ){
                    printf("Erro na linha %d: simbolo sendo redefinido! -- Erro semantico.\n", contagem_linha);  
                    erro++;
                } 
            }     

            if(secao_data == 0){
                // Só se pode declarar estas variáveis depois de um SECAO DATA
                if ((strcmp(termo2, "CONST") == 0) || (strcmp(termo2, "SPACE") == 0) || 
                    (strcmp(termo2, "const") == 0) || (strcmp(termo2, "space") == 0) ){
                        printf("Erro na linha %d: esta diretiva e para ocorrer depois do SECAO DATA! -- Erro semantico.\n", contagem_linha); 
                        erro++;
                }    
            }      

            if(secao_text == 0){
                // Só se pode declarar estas variáveis depois de um SECAO TEXT
                if ((strcmp(termo2, "ADD") == 0) || (strcmp(termo2, "add") == 0)||(strcmp(termo2, "SUB") == 0) || (strcmp(termo2, "sub") == 0)||
                    (strcmp(termo2, "MULT") == 0) || (strcmp(termo2, "mult") == 0)||(strcmp(termo2, "DIV") == 0) || (strcmp(termo2, "div") == 0)||
                    (strcmp(termo2, "JMP") == 0) || (strcmp(termo2, "jmp") == 0)||(strcmp(termo2, "JMPN") == 0) || (strcmp(termo2, "jmpn") == 0)||
                    (strcmp(termo2, "JMPP") == 0) || (strcmp(termo2, "jmpp") == 0)||(strcmp(termo2, "JMPZ") == 0) || (strcmp(termo2, "jmpz") == 0)||
                    (strcmp(termo2, "COPY") == 0) || (strcmp(termo2, "copy") == 0)||(strcmp(termo2, "LOAD") == 0) || (strcmp(termo2, "load") == 0)||
                    (strcmp(termo2, "STORE") == 0) || (strcmp(termo2, "store") == 0)||(strcmp(termo2, "INPUT") == 0) || (strcmp(termo2, "input") == 0)||
                    (strcmp(termo2, "OUTPUT") == 0) || (strcmp(termo2, "output") == 0)||(strcmp(termo2, "STOP") == 0) || (strcmp(termo2, "stop") == 0)){
                        
                        printf("Erro na linha %d: esta diretiva e para ocorrer depois do SECAO TEXT! -- Erro semantico.\n", contagem_linha);
                        erro++;
                }    
            }  

            strcpy(tabela_de_simbolos[simbolos_existente][0], termo1); 
            strcpy(tabela_de_simbolos[simbolos_existente][1], end_em_string);

            if(simbolos_existente < qtd_linhas){
                simbolos_existente++;
            }            

       }
        else{

            //Tratando Seções
        
            if((strcmp(termo1, "SECAO") == 0) || (strcmp(termo1, "secao") == 0)){      
                acc_end = acc_end + somador_op;
            }   

            if((strcmp(termo2, "TEXT") == 0) || (strcmp(termo2, "text") == 0)){
                secao_text = 1; 
            }

            if((strcmp(termo2, "DATA") == 0) || (strcmp(termo2, "data") == 0)){
                secao_data = 1;
            }
        }       

        if(tem_label == 1){
            if(tamanho_op(termo2) == 1){
                contagem_linha ++;
            }
            else if(tamanho_op(termo2) == 2)
            {
                contagem_linha += 2;
            }
            else if(tamanho_op(termo2) == 3){
                contagem_linha += 3;
            }
            else{
                contagem_linha = contagem_linha;
            }
        }
        else{
            if(tamanho_op(termo1) == 1){
                contagem_linha++;
            }
            else if(tamanho_op(termo1) == 2)
            {
                contagem_linha += 2;
            }
            else if(tamanho_op(termo1) == 3){
                contagem_linha += 3;
            }
            else{
                contagem_linha = contagem_linha;
            }
        }
            

        printf("FIM DA VERIFICACAO, TUDO CERTO!\n");
        printf("-------------------------------------------------------------\n");
    }}

    fclose(arquivo_asm );

    printf("\nTabela de simbolos resultante da Primeira Passagem!\n");

    tabela_de_simbolos = realloc( tabela_de_simbolos, sizeof(char) * simbolos_existente * 2 * 30); 

    printf("\n");
    printf("-----------------------\n");
    for(x = 0; x < simbolos_existente ; x++){
        for(int y = 0 ; y < 2 ; y++){
            printf("%s ", tabela_de_simbolos[x][y]);    
        }
        printf("\n"); 
    }
    printf("----------------------\n\n");

    // FIM DA PRIMEIRA PASSAGEM
    printf("FIM DA PRIMEIRA PASSAGEM\n");
    printf("----------------------------------------------------\n");
    printf("\nCOMECO DA SEGUNDA PASSAGEM\n");

    // ---------------------------------------------------------------------
    // SEGUNDA PASSAGEM
    // Criação do arquivo obj 

    arquivo_asm = fopen(novo_asm, "r");

    secao_data = 0;
    secao_text = 0;

    // Abertura do arquivo .obj para escrita
    arquivo_obj = fopen(nome_obj, "w");    
    printf("\nARQUIVO %s CRIADO\n", nome_obj); 

    if(arquivo_obj == NULL){
        printf("Problemas na abertura do arquivo\n");
        return 0;
    }

    contagem_linha = 0;

    while (fgets(linha, 200, arquivo_asm ) != NULL) {
        
        if(strcmp(linha, "\n") == 0){
            continue;;
        }
        else{
        // Verifica os tokens e armazena em cada termo da sequencia
        token = strtok(linha, delimitador);       
        i = 1;

        do{
            switch(i){

                case 1 : strcpy(termo1, token);
                case 2 : strcpy(termo2, token);
                case 3 : strcpy(termo3, token);
                case 4 : strcpy(termo4, token); 
            
            }
            i++;  
            token = strtok(NULL, delimitador);             
        }while(token != NULL);
        

        i--;

       // Verificação de cada linha e seus elementos
        // if(i==1){
        //     printf("Termos na linha: %s\nQuantidade: %d\n\n", termo1, i);
        // }
        // if(i==2){
        //     printf("Termos na linha: %s %s\nQuantidade: %d\n\n", termo1, termo2, i);
        // }
        // if(i==3){
        //     printf("Termos na linha: %s %s %s\nQuantidade: %d\n\n", termo1, termo2, termo3, i);
        // }
        // if(i==4){
        //     printf("Termos na linha: %s %s %s %s\nQuantidade: %d\n\n", termo1, termo2, termo3, termo4, i);
        // }       

        printf("\nCOMECANDO A VERIFICAR ERROS\n\n");

        if((strcmp(termo1, "STOP") == 0) || (strcmp(termo1, "stop") == 0) ){
            stop = 1;
        }

        tem_label = acha_label(termo1);

        if(tem_label == 1)
        {
            //Tratando Labels

            // Duas labels na mesma linha
            if((acha_label(termo2)) && (i != 1)){         
                printf("\nErro na linha %d: duas labels definidas na mesma linha! -- Erro sintatico.\n", contagem_linha);
                erro++;
            }

            if(i == 3){
                // Duas labels na mesma linha
                if(acha_label(termo3)){
                    printf("\nErro na linha %d: duas labels definidas na mesma linha! -- Erro sintatico.\n", contagem_linha);
                    erro++;
                }
            }
            
            if(i == 4){
                // Duas labels na mesma linha
                if (acha_label(termo3)){                    
                    printf("\nErro na linha %d: duas labels definidas na mesma linha! -- Erro sintatico.\n", contagem_linha);
                    erro++;
                }
                if(acha_label(termo4)){                    
                    printf("\nErro na linha %d: duas labels definidas na mesma linha! -- Erro sintatico.\n", contagem_linha);
                    erro++;
                }
                
            }            
            
            // Coloca 00 no arquivo objeto
            if((strcmp(termo2, "SPACE") == 0) || (strcmp(termo2, "space") == 0)){                
                fprintf(arquivo_obj, "00 ");
            }

            if((strcmp(termo2, "CONST") == 0) || (strcmp(termo2, "const") == 0)){

                for(x = 0; x < (strlen(termo3)); x++){
                    // checagem de digitos 
                    if( !((termo3[x] >= '0') && (termo3[x] <= '9'))  ){  
                        printf("\nErro na linha %d: o CONST deveria receber um número, e nao uma letra! -- Erro sintatico.\n", contagem_linha); 
                        erro++;
                    }
                }

                fprintf(arquivo_obj, "%s ", termo3);                
            }
            else{
                if(i != 1){
                    op = define_op_code(termo2);

                    if(op == 0){
                        if(!(strcmp(termo2, "CONST") == 0) && !(strcmp(termo2, "SPACE") == 0) && 
                           !(strcmp(termo2, "const") == 0) && !(strcmp(termo2, "space") == 0)){                            
                            printf("\nErro na linha %d: diretiva invalida invalida criacao de label! -- Erro sintatico.\n", contagem_linha);
                            erro++;
                        }
                    }
                    else{
                        fprintf(arquivo_obj, "%d ", op);

                        if(i == 3){
                            for(x = 0; x < simbolos_existente; x++){
                                if (strcmp(termo3, tabela_de_simbolos[x][0]) == 0 ){
                                    fprintf(arquivo_obj, "%s ", tabela_de_simbolos[x][1]);
                                    simbolo_encontrado = 1;
                                }
                            }

                            if(simbolo_encontrado == 1){
                                simbolo_encontrado = 0;
                            }
                            else{
                                for(x = 0; x < strlen(termo3) ; x++ ){
                                    if(((termo3[x] >= '0') && (termo3[x] <= '9'))  ){
                                        // apenas numeros
                                        numero = 1; 
                                    }
                                }

                                if (numero == 1){
                                    fprintf(arquivo_obj,"%s ", termo3);
                                    numero = 0;
                                    simbolo_encontrado = 0;
                                }
                                else{
                                    printf("\nErro na linha %d: %s simbolo nao encontrado! -- Erro lexico.\n", contagem_linha,termo3);
                                    erro++;
                                    numero = 0;
                                }
                            }
                        }

                        if(i == 4){
                            for (x = 0; x < simbolos_existente; x++){
                                if( strcmp(termo3, tabela_de_simbolos[x][0]) == 0 ){
                                    // achamos o simbolo                                    
                                    fprintf(arquivo_obj, "%s ", tabela_de_simbolos[x][1]);   
                                    simbolo_encontrado = 1;
                                }             
                            }
                            if(simbolo_encontrado == 1){
                                simbolo_encontrado = 0;
                            }
                            else{
                                for(x = 0; x < strlen(termo3) ; x++ ){
                                    if(  ((termo3[x] >= '0') && (termo3[x] <= '9'))  ){
                                        // apenas numeros
                                        numero = 1; 
                                    }
                                }

                                if(numero == 1){
                                    fprintf(arquivo_obj, "%s ", termo3);
                                    arquivo_obj = 0; 
                                    simbolo_encontrado = 0;
                                }
                                else{
                                    printf("\nErro na linha %d: %s simbolo nao encontrado! -- Erro lexico.\n", contagem_linha,termo3);
                                    erro++;
                                    numero = 0;
                                    simbolo_encontrado = 0;
                                
                                }
                            }

                            for(x=0; x<simbolos_existente ; x++){
                                if( strcmp(termo4, tabela_de_simbolos[x][0]) == 0 ){
                                    // achamos o simbolo                                    
                                    fprintf(arquivo_obj, "%s ", tabela_de_simbolos[x][1]);
                                    simbolo_encontrado = 1;
                                }
                            }

                            if(simbolo_encontrado == 1){
                                simbolo_encontrado = 0;
                            }
                            else{
                                for(x=0; x< strlen(termo4) ; x++ ){
                                    if(  ((termo4[x] >= '0') && (termo4[x] <= '9'))  ){
                                        // apenas numeros
                                        numero = 1; 
                                    }
                                }

                                if(numero == 1){
                                    fprintf(arquivo_obj, "%s ", termo4);
                                    numero = 0; 
                                    simbolo_encontrado = 0;
                                }
                                else{
                                    printf("\nErro na linha %d: %s simbolo nao encontrado! -- Erro lexico.\n", contagem_linha,termo4);
                                    erro++;
                                    numero = 0;
                                }
                            } 
                        }
                    }
                }

                if(i == 1){

                    // se esta passando por uma label sozinha na linha sem nada a fazer
                    op = define_op_code(termo1);
                    
                    if(op == 0){
                        for(x = 0; x < simbolos_existente ; x++){
                            if( strcmp(termo1, tabela_de_simbolos[x][0]) == 0 ){
                                // achamos o simbolo
                                fprintf(arquivo_obj, "%s ", tabela_de_simbolos[x][1]);  
                                simbolo_encontrado = 1;
                            }
                        }

                        if(simbolo_encontrado == 1){
                            simbolo_encontrado = 0;
                        }
                    }
                    else{
                        fprintf(arquivo_obj, "%d ", op);
                    }
                }
            }            
        }
        else{
            if( !(strcmp(termo1, "SECAO") == 0) && !(strcmp(termo1, "secao") == 0)){
                somador_op = tamanho_op(termo1);

                if(i == somador_op){
                    op = define_op_code(termo1);

                    if(op == 0){                        
                        printf("\nErro na linha %d: operacao invalida! -- Erro lexico.\n", contagem_linha); 
                        erro++;
                    }
                    fprintf(arquivo_obj, "%d ", op);

                    if(i == 2){
                        for(x=0; x<simbolos_existente ; x++){
                           
                            if( strcmp(termo2, tabela_de_simbolos[x][0]) == 0 ){
                                // achamos o simbolo                                
                                fprintf(arquivo_obj, "%s ", tabela_de_simbolos[x][1]); 
                                simbolo_encontrado = 1;
                                
                            } 
                        }

                        if(simbolo_encontrado == 1){
                            simbolo_encontrado = 0;
                        }
                        else{
                            for(x=0; x< strlen(termo2) ; x++ ){
                                if(  ((termo2[x] >= '0') && (termo2[x] <= '9'))  ){
                                    // apenas numeros
                                    numero = 1; 
                                }
                            }

                            if(numero == 1){
                                fprintf(arquivo_obj, "%s ", termo2);
                                numero = 0; 
                                simbolo_encontrado = 0;
                            }
                            else{
                                printf("\nErro na linha %d: %s simbolo nao encontrado! -- Erro lexico.\n", contagem_linha,termo2);
                                erro++;
                                numero = 0;
                            }
                        }
                    }

                    if(i == 3){
                        for(x=0; x<simbolos_existente ; x++){
                            if( strcmp(termo2, tabela_de_simbolos[x][0]) == 0 ){
                                // achamos o simbolo
                                fprintf(arquivo_obj, "%s ", tabela_de_simbolos[x][1]);
                                simbolo_encontrado = 1;
                            } 
                        }
                        
                        if(simbolo_encontrado == 1){
                            simbolo_encontrado = 0;
                        }
                        else{
                            for(x=0; x< strlen(termo2) ; x++ ){
                                if(  ((termo2[x] >= '0') && (termo2[x] <= '9'))  ){
                                    // apenas numeros
                                    numero = 1; 
                                }
                            }

                            if(numero == 1){
                                fprintf(arquivo_obj, "%s ", termo2);
                                numero = 0; 
                                simbolo_encontrado = 0;
                            }
                            else{
                                printf("\nErro na linha %d: %s simbolo nao encontrado -- Erro lexico.\n", contagem_linha, termo2);
                                erro++;
                                numero = 0;
                            }
                        }

                        for(x=0; x<simbolos_existente ; x++){
                            if( strcmp(termo3, tabela_de_simbolos[x][0]) == 0 ){
                                // achamos o simbolo                                 
                                fprintf(arquivo_obj, "%s ", tabela_de_simbolos[x][1]);
                                simbolo_encontrado = 1;

                            } 
                        }

                        if(simbolo_encontrado == 1){
                            simbolo_encontrado = 0;
                        }
                        else{
                            for(x=0; x< strlen(termo2) ; x++ ){
                                if(  ((termo2[x] >= '0') && (termo2[x] <= '9'))  ){
                                    // apenas numeros
                                    numero = 1; 
                                }
                            }

                            if(numero == 1){
                                fprintf(arquivo_obj, "%s ", termo2);
                                numero = 0; 
                                simbolo_encontrado = 0;
                            }
                            else{
                                printf("\nErro na linha %d: simbolo nao encontrado -- Erro lexico.\n", contagem_linha, termo2);
                                erro++;
                                numero = 0;
                            }
                        }
                    }
                }
                else{
                    printf("\nErro na linha %d: a quantidade de argumentos nao está correta para esta operacao! -- Erro sintatico.\n", contagem_linha); 
                    erro++;
                }
            }
            else{
                // é um secao

                if((strcmp(termo2, "TEXT") == 0) || (strcmp(termo2, "text") == 0)){
                    // código passando por um secao text 
                    secao_text = 1; 
                }
                else{
                    if((strcmp(termo2, "DATA") == 0) || (strcmp(termo2, "data") == 0)){
                        // código passando por um secao data
                        secao_data = 1; 
                    }
                    else{
                        // secao nao existe
                        printf("\nErro na linha %d: esse tipo de SECAO nao existe! -- Erro sintatico.\n", contagem_linha); 
                        erro++;
                    }
                }
            }
        }

        contagem_linha++;    

        printf("FIM DA VERIFICACAO\n");
        printf("-------------------------------------------------------------\n");   
    }}

    printf("\nFIM DA SEGUNDA PASSAGEM!\n");

    if(stop == 0){

        // faltou um STOP no programa
        
        printf("\nErro: esse programa nao teve um STOP -- Erro semantico.\n"); 
        fclose(arquivo_obj); 
        remove(nome_obj);
        exit(0);

    }

    if(secao_text == 0){
        
        // faltou um SECAO TEXT no programa
        
        printf("\nErro: esse programa nao teve um SECAO TEXT -- Erro semantico.\n"); 
        fclose(arquivo_obj); 
        remove(nome_obj);
        exit(0);
    }
    else{
        if(secao_data == 0){
        
            // faltou um SECAO DATA no programa
        
            printf("\nErro: esse programa nao teve um SECAO DATA -- Erro semantico.\n"); 
            fclose(arquivo_obj); 
            remove(nome_obj);
            exit(0);
        }
    } 

    
    if(erro > 0){
        printf("\nHouveram um total de %d. Nao sera gerado o arquivo obj\n", erro);
        fclose(arquivo_obj); 
        remove(novo_asm);
        remove(nome_obj);       
        exit(0);

    }
    
    

    // Fechar o arquivo após a leitura e libera a memória
    fclose(arquivo_asm );
    fclose(arquivo_obj);
    
    free(termo1);
    free(termo2);
    free(termo3);
    free(termo4); 

    free(tabela_de_simbolos);
    free(nome_obj);
    remove(novo_asm);

    printf("\nArquivo .obj criado com sucesso\n\n");
    printf("Montador finalizado\n\n");

    return 0;
}
