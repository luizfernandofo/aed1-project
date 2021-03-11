#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

bool sair = false;
FILE* p;
FILE* aux;

struct funcionarios{
	char nome[200];
	int codigo;
	char cargo[20];
	float salario;
}f;

void abrirArquivo(){
    p=fopen("t.txt", "a+b");
    aux=fopen("auxiliar.txt", "wb");
    if(p==NULL || aux == NULL){
        printf("Erro na abertura do arquivo\n");
        system("pause");
        exit(1);
    }
}

void cadastrar(){
    long int tell; //teste da posição
	char c;
    abrirArquivo();
    do{
        tell = ftell(p);
        printf("tell: %ld\n%d\n", tell,sizeof(f));
        system("pause");

        system("cls");
        printf("\n\n------------- CADASTRO DE FUNCIONARIOS -------------\n\n");
        printf("Codigo: ");
        scanf("%d",&f.codigo);
        printf("Nome: ");
        fflush(stdin);
        gets(f.nome);
        printf("Cargo: ");
        fflush(stdin);
        gets(f.cargo);
        printf("Salario: ");
        fflush(stdin);
        scanf("%f",&f.salario);
        fseek(p,SEEK_END, 1); //rever
        fflush(stdin);
        fwrite(&f,sizeof(f), 1, p);
        printf("\nCadastro realizado com sucesso...\n\n");
        printf("DESEJA CADASTRAR MAIS FUNCIONARIOS (s/n)?\n");
        scanf("%c", &c);
    }while(c == 'S' || c == 's');
    fclose(p);
    fclose(aux);
    system("pause");
}

void consultar(){
    system("cls");
    abrirArquivo();
    int cod;
    int a=0;
    printf("\n\n------------- CONSULTA DE FUNCIONARIOS -------------\n\n");
    printf("Informe o codigo a ser pesquisado: ");
    scanf("%d",&cod);
    rewind(p);
    //fseek(p, 0, SEEK_SET); //mudei
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
    system("pause");

}

void listar(){
    system("cls");
    abrirArquivo();
    rewind(p);
    //fseek(p, SEEK_SET, 1);
    fread(&f, sizeof(f), 1, p);
    printf("\n\n------------- LISTA DE FUNCIONARIOS -------------\n\n");
    while(!feof(p)){
        printf(" Nome: %s, Cargo: %s, Salario: R$ %.2f\n", f.nome, f.cargo, f.salario);
        fread(&f, sizeof(f), 1, p);
    }
    printf("\n");
    fclose(p);
    fclose(aux);
    system("pause");
}

void alterarSalario(){
    system("cls");
    abrirArquivo();
    int cod;
    int a=0;
    printf("\n\n------------- ALTERAR SALARIO DE FUNCIONARIOS -------------\n\n");
    printf("Informe o funcionario a ter o salario alterado: ");
    scanf("%d",&cod);
    //fseek(p, SEEK_SET, 1);
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
    system("pause");
}

void alterarCargo(){
    system("cls");
    abrirArquivo();
    int cod;
    int a=0;
    printf("\n\n------------- ALTERAR CARGO DE FUNCIONARIOS -------------\n\n");
    printf("Informe o funcionario a ter o cargo alterado: ");
    scanf("%d",&cod);
    fseek(p, SEEK_SET, 1);
    fread(&f, sizeof(f), 1, p);
    while(!feof(p)){
        if(f.codigo == cod){
            a++;
            printf("Informe o novo cargo: ");
            fflush(stdin);
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
    system("pause");
}

void demitir(){
    system("cls");
    abrirArquivo();
    int cod;
    int a=0;
    printf("\n\n------------- DEMISSAO DE FUNCIONARIOS -------------\n\n");
    printf("Informe o funcionario a ser demitido: ");
    scanf("%d",&cod);
    fseek(p, SEEK_SET, 1);
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
    system("pause");
}

void menu(){
    int op;
    int c;
    system("cls");
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

int main(){
    /*p = fopen("t.txt", "a+b");
    if(p==NULL){
        printf("Erro na abertura do arquivo\n");
        system("pause");
        exit(1);
    }
    fclose(p);*/
    while(!sair){
        menu();
    }
    return 0;
}
