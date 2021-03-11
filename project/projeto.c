#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"projeto.h"

/*TO DO:
    *verificar a portabilidade do fflush para limpar o stdin;

    *system("pause") nao funcionando no linux.
        **Possivel soluçao: 
            printf("Pressione ENTER para continuar...\n");
            getchar();
*/


//Variáveis globais
bool sair = false;
FILE* p;
FILE* aux;
funcionarios f;


//main()
int main(int argc, char *argv[]){

    while(!sair){
        menu();
    }
    system(CLS);
    return 0;
}


//FUNÇÕES 
void abrirArquivo(){
    p=fopen("t.txt", "a+b");
    aux=fopen("auxiliar.txt", "wb");
    if(p==NULL || aux == NULL){
        printf("Erro na abertura do arquivo\n");
        pause();
        exit(1);
    }
}

void cadastrar(){
    //long int tell; //teste da posição
	char c;
    abrirArquivo();
    do{
        /*tell = ftell(p);
        printf("tell: %ld\n%d\n", tell,sizeof(f));     estava testando o retorno de ftell para usar o posicionador no arquivo
        pause();*/

        system(CLS);
        printf("\n\n------------- CADASTRO DE FUNCIONARIOS -------------\n\n");
        printf("Codigo: ");
        scanf("%d",&f.codigo);
        printf("Nome: ");
        setbuf(stdin, NULL);
        gets(f.nome);
        printf("Cargo: ");
        setbuf(stdin, NULL);
        gets(f.cargo);
        printf("Salario: ");
        setbuf(stdin, NULL);
        scanf("%f",&f.salario);
        setbuf(stdin, NULL);
        fwrite(&f,sizeof(f), 1, p);
        printf("\nCadastro realizado com sucesso...\n\n");
        printf("DESEJA CADASTRAR MAIS FUNCIONARIOS (s/n)?\n");
        setbuf(stdin, NULL);
        scanf("%c", &c);
    }while(c == 'S' || c == 's');
    fclose(p);
    fclose(aux);
    remove("auxiliar.txt");
    pause();
}

void consultar(){
    system(CLS);
    abrirArquivo();
    int cod;
    int a=0;
    printf("\n\n------------- CONSULTA DE FUNCIONARIOS -------------\n\n");
    printf("Informe o codigo a ser pesquisado: ");
    scanf("%d",&cod);
    //rewind(p);             //
    //fseek(p, 0, SEEK_SET); //ambos desempenham a mesma função, teoricamente.
    fread(&f, sizeof(f), 1, p);
    while(!feof(p)){
        if(f.codigo == cod){
            a++;
            printf("\nFuncionario encontrado!\n\n");
            printf("Nome: %s\n", f.nome);
            printf("Cargo: %s\n", f.cargo);
            printf("Salario: R$ %.2f\n\n", f.salario);
        }
        fread(&f, sizeof(f), 1, p);
    }
    if(a==0) printf("\nFuncionario nao encontrado!\n\n");
    fclose(p);
    fclose(aux);
    remove("auxiliar.txt");
    pause();

}

void listar(){
    system(CLS);
    abrirArquivo();
    fread(&f, sizeof(f), 1, p);
    printf("\n\n------------- LISTA DE FUNCIONARIOS -------------\n\n");
    while(!feof(p)){
        printf(" Codigo: %d, Nome: %s, Cargo: %s, Salario: R$ %.2f\n", f.codigo, f.nome, f.cargo, f.salario);
        fread(&f, sizeof(f), 1, p);
    }
    printf("\n");
    fclose(p);
    fclose(aux);
    remove("auxiliar.txt");
    pause(); 
}

void alterarSalario(){
    system(CLS);
    abrirArquivo();
    int cod;
    int a=0;
    printf("\n\n------------- ALTERAR SALARIO DE FUNCIONARIOS -------------\n\n");
    printf("Informe o funcionario a ter o salario alterado: ");
    scanf("%d",&cod);
    fread(&f, sizeof(f), 1, p);
    while(!feof(p)){
        if(f.codigo == cod){
            a++;
            printf("\nInforme o novo salario: ");
            scanf("%f",&f.salario);
        }
        fwrite(&f, sizeof(f),1,aux);
        fread(&f, sizeof(f),1,p);
    }
    if(a==0) printf("\nFuncionario nao encontrado!\n\n");
    else printf("\nSalario alterado com sucesso!\n\n");

    fclose(p);
    remove("t.txt");
    fclose(aux);
    rename("auxiliar.txt","t.txt");
    remove("auxiliar.txt");
    pause();
}

void alterarCargo(){
    system(CLS);
    abrirArquivo();
    int cod;
    int a=0;
    printf("\n\n------------- ALTERAR CARGO DE FUNCIONARIOS -------------\n\n");
    printf("Informe o funcionario a ter o cargo alterado: ");
    scanf("%d",&cod);
    fread(&f, sizeof(f), 1, p);
    while(!feof(p)){
        if(f.codigo == cod){
            a++;
            printf("Informe o novo cargo: ");
            setbuf(stdin, NULL);
            gets(f.cargo);
        }
        fwrite(&f, sizeof(f),1,aux);
        fread(&f, sizeof(f),1,p);
    }
    if(a==0) printf("\nFuncionario nao encontrado!\n\n");
    else printf("\nCargo alterado com sucesso!\n\n");

    fclose(p);
    remove("t.txt");
    fclose(aux);
    rename("auxiliar.txt","t.txt");
    remove("auxiliar.txt");
    pause();
}

void demitir(){
    system(CLS);
    abrirArquivo();
    int cod;
    int a=0;
    printf("\n\n------------- DEMISSAO DE FUNCIONARIOS -------------\n\n");
    printf("Informe o funcionario a ser demitido: ");
    scanf("%d",&cod);
    fread(&f, sizeof(f), 1, p);
    while(!feof(p)){
        if(f.codigo == cod){
            a++;
        }
        else{
            fwrite(&f, sizeof(f),1,aux);
        }
        fread(&f, sizeof(f),1,p);
    }
    if(a==0) printf("\nFuncionario nao encontrado!\n\n");
    else printf("\nFuncionario demitido com sucesso!\n\n");
    fclose(p);
    remove("t.txt");
    fclose(aux);
    rename("auxiliar.txt","t.txt");
    remove("auxiliar.txt");
    pause();
}

void menu(){
    int op;
    int c;
    system(CLS);
    printf("\n ------------- GERENCIAMENTO DE FUNCIONARIOS -------------");
    printf("\n |                                                       |");
    printf("\n |  1 - Cadastrar funcionario                            |");
    printf("\n |  2 - Listar funcionarios                              |");
    printf("\n |  3 - Consultar funcionario                            |");
    printf("\n |  4 - Alterar salario de funcionario                   |");
    printf("\n |  5 - Alterar cargo de funcionario                     |");
    printf("\n |  6 - Demitir funcionario                              |");
    printf("\n |  0 - Sair                                             |");
    printf("\n ---------------------------------------------------------\n");
    printf("\n\n ESCOLHA UMA OPCAO: ");
    scanf("%i",&op);

    switch(op){
        case 1: cadastrar(); break;
        case 2: listar(); break;
        case 3: consultar(); break;
        case 4: alterarSalario(); break;
        case 5: alterarCargo(); break;
        case 6: demitir(); break;
        case 0: sair = true; break;
        default:
            printf("\nOpcao invalida! Pressione ENTER...");
            scanf("%*c");
        break;
    }
}

void pause(){
    setbuf(stdin, NULL);
    printf("\nPressione ENTER para continuar...\n"); //não pode ter "lixo" no stdin para ela funcionar
    getchar();
}
