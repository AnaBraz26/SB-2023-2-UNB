# Montador e Simulador
Ana Caroline da Rocha Braz - 212008482

Andreza Rodrigues Costa - 180074474

Para este trabalho deve-se implementar um montador e um simulador utilizando o assembly trabalho em sala de aula.
____
# MONTADOR
O montador de duas passagens implementado recebe um arquivo .asm e verificar se suas seções text e data estão trocadas. 
Em caso da seção data vir primeiro, é feita a inversão das seções. Caso contrário, é mantido.
Após essa verificação, é feita a primeira passagem a fim de criar a tabela de símbolos e identificar erros.
Na segunda passagem é feita a escrita no arquivo final .obj e também verifica erros léxicos, sintáticos e semânticos. 
Ao final é gerado o arquivo .obj que será utilizado no simulador.

Utilize apenas arquivos .asm para rodar o programa!

Para a compilação, faça:
```dif
gcc montador.c -o montador
````
Para rodar o programa, faça:
```dif
./montador [nome_do_arquivo].asm
````
____
# SIMULADOR
O simulador simula a operação gerada pelo montador no arquivo .obj.

Utilize apenas arquivos .obj para rodar o programa!

Para a compilação, faça:
```dif
gcc simulador.c -o simulador
````
Para rodar o programa, faça:
```dif
./simulador [nome_do_arquivo].obj
````
