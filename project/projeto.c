#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include"projeto.h"

#define BUFF_PAGE 10 //tamanho do vetor que vai armazenar os funcionarios que serão exibidos na página 

/*TO DO:

    *Incluir uma opção de confirmação de operação nas funções.

*/


//Variáveis globais
bool sair = false;
FILE* employees=NULL;
FILE* aux=NULL;
FILE* availableCodes=NULL;
FILE* fired=NULL;
funcionarios f, fpage[BUFF_PAGE];


struct tm *timeinfo;
char date[10+1]; // dd/mm/aaaa


//main()
int main(int argc, char *argv[]){

    memset(fpage, 0, sizeof(fpage));

    while(!sair){
        menu();
    }
    system(CLS);
    //remove("auxiliar.txt"); 
    return 0;
}


//FUNÇÕES 
/*void abrirArquivo(){
    p=fopen("funcionarios.txt", "r+b");
    aux=fopen("auxiliar.txt", "wb");
    availableCodes=fopen("availableCodes.txt", "r+b");
    fired=fopen("fired.txt", "r+b");

    if(p == NULL) p=fopen("funcionarios.txt", "w+b"); //caso o arquivo n exista ele vai retornar NULL, então IF verifica e cria o arquivo funcionarios.txt
    if(availableCodes == NULL) availableCodes=fopen("availableCodes.txt", "w+b");//
    if(fired == NULL) fired=fopen("fired.txt", "w+b"); //caso o arquivo n exista ele vai retornar NULL, então IF verifica e cria o arquivo funcionarios.txt

    if(p==NULL || aux == NULL || availableCodes == NULL || fired==NULL){
        printf("Erro na abertura do arquivo\n");
        fecharArquivo();
        pause();
        exit(1);
    }

    return;
}*/

FILE * abrirArquivo(const char *filename, const char *mode){

    FILE *temp = NULL;

    temp = fopen(filename, mode);

    if(temp == NULL && (strcmp(mode, "r+b") == 0 || strcmp(mode, "rb") == 0)) temp = fopen(filename, "w+b");     

    if(temp == NULL){
        printf("Erro na abertura do arquivo: %s\n", filename);
        exit(1);
    }

    return temp;
}

void cadastrar(){
    
    int avCode=0; //serve para armazenar f.codigo dos func deletados
    long int tempCode; 
	char c;

    

    do{

        system(CLS);
        printf("\n\n------------- CADASTRO DE FUNCIONARIOS -------------\n\n");

        avCode=0; //setado 0 para comparar lá embaixo caso não haja números disponíveis

        employees = abrirArquivo("funcionarios.bin", "r+b");
        availableCodes = abrirArquivo("availableCodes.bin", "r+b");

        fread(&avCode, sizeof(avCode), 1, availableCodes);
        fclose(availableCodes);

        if(avCode <= 0){ //caso não haja nenhum codigo de funcionario deletado pendente 

            fseek(employees, 0, SEEK_END); 
            tempCode = ftell(employees);
            
            if(tempCode == 0) f.codigo = 1;
            else{
                tempCode /= sizeof(f);
                f.codigo = tempCode+1;
            }

        }else f.codigo = avCode; //Se houver algum codigo de funcionario deletado pendente f.codigo recebe      
        
        fclose(employees);

        printf("Codigo: %d\n", f.codigo);
        printf("Nome: ");
        setbuf(stdin, NULL);
        gets(f.nome);
        printf("Cargo: ");
        setbuf(stdin, NULL);
        gets(f.cargo);
        printf("Salario: R$ ");
        setbuf(stdin, NULL);
        scanf("%f",&f.salario);
        setbuf(stdin, NULL);
        time(&(f.rawtime)); //pega a data do cadastro

        printf("\nCONFIRMA O CADASTRO DO FUNCIONARIO (s/n)? ");
        setbuf(stdin, NULL);
        scanf("%c", &c);

        if(c == 'n' || c == 'N'){ //caso não confirma os arquivos são simplesmente fechados

            printf("\nCadastro nao realizado!\n\n");
        
        }else{ //caso confirme: se o codigo do "func" tiver origem de um deletado, avCode vai ser diferente de 0. Caso o arquivo de funcionários disponíveis esteja vazio avCode=0;

            if(avCode > 0){ //o código utilizado do "available.txt" é apagado

                employees = abrirArquivo("funcionarios.bin", "r+b");

                fseek(employees, sizeof(f)*(avCode-1), SEEK_SET); //o cursor do arquivo principal é setado para a posição onde, no arquivo principal, o índice avCode está zerado

                fwrite(&f,sizeof(f), 1, employees); //é feita a escrita do funcionario cadastrado no arquivo.

                fclose(employees);

                //=================================

                availableCodes = abrirArquivo("availableCodes.bin", "r+b");
                aux = abrirArquivo("auxiliar.bin", "wb");

                fread(&avCode, sizeof(avCode), 1, availableCodes);
                fread(&avCode, sizeof(avCode), 1, availableCodes);// a leitura é feito duas vezes para consumir o primeiro valor que é que será excluído
                while(!feof(availableCodes)){
                    fwrite(&avCode, sizeof(avCode), 1, aux);
                    fread(&avCode, sizeof(avCode), 1, availableCodes);
                }

                fclose(aux);
                fclose(availableCodes);
                remove("availableCodes.bin");
                rename("auxiliar.bin","availableCodes.bin");

            }else{

                employees = abrirArquivo("funcionarios.bin", "r+b");

                fseek(employees, 0, SEEK_END);

                fwrite(&f,sizeof(f), 1, employees); //é feita a escrita do funcionario cadastrado no arquivo.

                fclose(employees);

            }
            
            printf("\nCadastro realizado com sucesso!\n\n");

        }
        
        printf("DESEJA CADASTRAR MAIS FUNCIONARIOS (s/n)? ");
        setbuf(stdin, NULL);
        scanf("%c", &c);

    }while(c == 'S' || c == 's');
    
    pause();

    return;
}

void consultar(int cod){

    if(cod<=0) return;

    employees = abrirArquivo("funcionarios.bin", "r+b");

    fseek(employees, sizeof(f)*(cod-1), SEEK_SET);

    fread(&f, sizeof(f), 1, employees);

    fclose(employees);

    system(CLS);
    printf("\n\n------------- CONSULTA DE FUNCIONARIOS -------------\n\n");
    
    if(f.codigo == cod){

        printf("\nFuncionario encontrado!\n\n");
        printf("Nome: %s\n", f.nome);
        printf("Cargo: %s\n", f.cargo);
        printf("Salario: R$ %.2f\n", f.salario);
        printf("Data de admissao: %s\n\n", tmTOstring());

    }else printf("\nFuncionario nao encontrado!\n\n");
    
    pause();

    return;

}

void listar(){

    int i;
    int op;
    int pgNumber = 1, pgQt;
    bool sairListar = false;

    while(!sairListar){

        pgQt = loadFuncBuff(pgNumber);

        system(CLS);
        printf("\n\n------------------------- LISTA DE FUNCIONARIOS ------------------------- Page: %d/%d\n\n", pgNumber, pgQt);
        printf("     Codigo  |  Nome\n");

        for(i=0; i<BUFF_PAGE; i++) if(fpage[i].codigo!=0) printf(" %10d  |  %s\n", fpage[i].codigo, fpage[i].nome);
          
        printf("\n ------------------------- OPCOES -------------------------");
        printf("\n |                                                        |");
        printf("\n |  1 - Ir para pagina:                                   |");
        printf("\n |  2 - Consultar funcionario                             |");
        printf("\n |  3 - Alterar salario de funcionario                    |");
        printf("\n |  4 - Alterar cargo de funcionario                      |");
        printf("\n |  5 - Demitir funcionario                               |");
        printf("\n |  0 - Voltar                                            |");
        printf("\n ---------------------------------------------------------\n");
        printf("\n ESCOLHA UMA OPCAO: ");

        setbuf(stdin, NULL);
        scanf("%d",&op);

        switch(op){
            case 1:

                printf(" Pagina: ");
                setbuf(stdin, NULL);
                scanf("%d", &pgNumber);

                if(pgNumber < 1 || pgNumber > pgQt){
                    printf(" Pagina informada nao existe!\n");
                    pgNumber = 1;
                    pause();
                }
                break;

            case 2: consultar( readFCode() ); break;
            case 3: alterarSalario( readFCode() ); break;
            case 4: alterarCargo( readFCode() ); break;
            case 5: demitir( readFCode() ); break;
            case 0: sairListar = true; break;
            default:
                printf("\nOpcao invalida!\n");
                pause();
            break;
        }

    }

    return; 
}

void alterarSalario(int cod){
    
    char opcao;

    if(cod<=0) return;

    employees = abrirArquivo("funcionarios.bin", "r+b");

    fseek(employees, sizeof(f)*(cod-1), SEEK_SET);

    fread(&f, sizeof(f), 1, employees);

    fclose(employees);

    system(CLS);
    printf("\n\n------------- ALTERAR SALARIO DO FUNCIONARIO -------------\n\n");


    if(f.codigo == cod){

        setbuf(stdin, NULL);
        printf("\n Codigo: %d\n Nome: %s\n Cargo: %s\n Salario: R$ %.2f\n", f.codigo, f.nome, f.cargo, f.salario);
        printf("\nInforme o novo salario: R$ ");
        scanf("%f",&f.salario);

        setbuf(stdin, NULL);
        printf("Confirma alteracao? (S/N)\n");
        scanf("%c", &opcao);

        if(opcao == 'S' || opcao == 's'){

            employees = abrirArquivo("funcionarios.bin", "r+b");

            fseek(employees, sizeof(f)*(cod-1), SEEK_SET);

            fwrite(&f, sizeof(f), 1, employees);

            fclose(employees);

            printf("\nSalario alterado com sucesso!\n\n");

        }else printf("\nSalario nao alterado!\n\n");     
        
    }else printf("\nFuncionario nao encontrado!\n\n"); 

    pause();

    return;
}

void alterarCargo(int cod){
    
    char opcao;

    if(cod<=0) return;

    employees = abrirArquivo("funcionarios.bin", "r+b");

    fseek(employees, sizeof(f)*(cod-1), SEEK_SET);

    fread(&f, sizeof(f), 1, employees);

    fclose(employees);

    system(CLS);
    printf("\n\n------------- ALTERAR CARGO DO FUNCIONARIO -------------\n\n");
    
    
    if(f.codigo == cod){
        
        printf("\n Codigo: %d\n Nome: %s\n Cargo: %s\n Salario: R$ %.2f\n", f.codigo, f.nome, f.cargo, f.salario);
        printf("\nInforme o novo cargo: ");
        setbuf(stdin, NULL);
        gets(f.cargo);
        
        setbuf(stdin, NULL);
        printf("Confirma alteracao? (S/N)\n");
        scanf("%c", &opcao);

        if(opcao == 'S' || opcao == 's'){

            employees = abrirArquivo("funcionarios.bin", "r+b");

            fseek(employees, sizeof(f)*(cod-1), SEEK_SET);

            fwrite(&f, sizeof(f), 1, employees);

            fclose(employees);

            printf("\nCargo alterado com sucesso!\n\n");

        }else printf("\nCargo nao alterado!\n\n");
        
    }else printf("\nFuncionario nao encontrado!\n\n");
    
    pause();

    return;
}

void demitir(int cod){
    
    char opcao;

    if(cod<=0) return;

    system(CLS);
    printf("\n\n------------- DEMISSAO DE FUNCIONARIOS -------------\n\n");
    

    employees = abrirArquivo("funcionarios.bin", "r+b");

    fseek(employees, sizeof(f)*(cod-1), SEEK_SET);

    fread(&f, sizeof(f), 1, employees);

    fclose(employees);

    if(f.codigo == cod){
        
        setbuf(stdin, NULL);
        printf("Confirma demissao? (S/N)\n");
        scanf("%c", &opcao);

        if(opcao == 'S' || opcao == 's'){

            availableCodes = abrirArquivo("availableCodes.bin", "ab");
            
            fwrite(&f.codigo, sizeof(f.codigo), 1, availableCodes); //o código do func deletado é escrito no arquivo available.txt

            fclose(availableCodes);

            //===========================================

            fired = abrirArquivo("fired.bin", "ab");

            time(&(f.rawtime)); // é armazenada a data da demissão do funcionário

            fwrite(&f, sizeof(f), 1, fired);//o funcionario é escrito no arquivo fired.bin

            fclose(fired);

            //===========================================

            memset(&f, 0, sizeof(f)); // f é zerado

            employees = abrirArquivo("funcionarios.bin", "r+b");

            fseek(employees, sizeof(f)*(cod-1), SEEK_SET);
            
            fwrite(&f, sizeof(f),1,employees); //e sobrescreve no arquivo principal

            fclose(employees);

            printf("\nFuncionario demitido com sucesso!\n\n");

        }else printf("\nFuncionario nao demitido!\n\n");

    }else printf("\nFuncionario nao encontrado!\n\n");
        
    sortAvailableCodes();
    pause();

    return;
}

void menu(){

    int op;

    setbuf(stdin, NULL);
    system(CLS);
    printf("\n ------------- GERENCIAMENTO DE FUNCIONARIOS -------------");
    printf("\n |                                                       |");
    printf("\n |  1 - Cadastrar funcionario                            |");
    printf("\n |  2 - Listar funcionarios                              |");
    printf("\n |  3 - Listar funcionarios demitidos                    |");
    printf("\n |  4 - Limpar lista de funcionarios demitidos           |");
    printf("\n |  0 - Sair                                             |");
    printf("\n ---------------------------------------------------------\n");
    printf("\n\n ESCOLHA UMA OPCAO: ");
    scanf("%d",&op);

    switch(op){
        case 1: cadastrar(); break;
        case 2: listar(); break;
        case 3: listar_demitidos(); break;
        case 4: cleanFiredList(); break;
        case 0: sair = true; break;
        default:
            printf("\nOpcao invalida!\n");
            pause();
        break;
    }

    return;
}

void pause(){

    setbuf(stdin, NULL);
    printf("\n Pressione ENTER para continuar..."); //não pode ter "lixo" no stdin para ela funcionar
    getchar();

    return;
}

char *tmTOstring(){

    timeinfo = localtime(&(f.rawtime));
    sprintf(date, "%d/%d/%d", timeinfo->tm_mday, ((timeinfo->tm_mon)+1), ((timeinfo->tm_year)+1900));
    timeinfo = NULL;
    
  return date;
}

void listar_demitidos(){

    system(CLS);
    printf("\n\n------------------------- LISTA DE FUNCIONARIOS DEMITIDOS-------------------------\n\n");

    fired = abrirArquivo("fired.bin", "rb");
    fread(&f, sizeof(f), 1, fired);
    while(!feof(fired)){
        printf(" Codigo: %d, Nome: %s, Cargo: %s, Salario: R$ %.2f, Demissao: %s\n", f.codigo, f.nome, f.cargo, f.salario, tmTOstring());
        fread(&f, sizeof(f), 1, fired);
    }
    printf("\n\n");
    
    fclose(fired);

    //==========================================

    pause(); 

    return;
}

void cleanFiredList(){

    char opcao;

    system(CLS);
    printf("\n\n------------------------- LIMPEZA DA LISTA DE FUNCIONARIOS DEMITIDOS -------------------------\n\n");

    printf("Deseja limpar a lista de funcionarios demitidos? (S/N)\n\n");
    setbuf(stdin, NULL);
    scanf("%c", &opcao);

    if(opcao == 'S' || opcao == 's'){
        remove("fired.bin");
        printf("Lista de funcionarios demitidos limpa com sucesso!\n\n");

    }

    pause();

    return;

}

void sortAvailableCodes(){

    int *av=NULL; //vetor para armazenar os valores: ordenação interna
    int i, j; //contadores
    long int qtAvailable; //pega o retorno do ftell para saber a qtd de elementos no arquivo
    int temp;

    availableCodes=fopen("availableCodes.txt", "r+b");
    if(availableCodes == NULL) return;
    
    fseek(availableCodes, 0, SEEK_END);     //reseta a posição do arquivo para o fim
    qtAvailable = ftell(availableCodes);    //pega a quantidade de bytes no arquivo
    qtAvailable /= sizeof(f.codigo);        //divide pelo tamanho de f.codigo e acha a quantidade de f.codigos

    if(qtAvailable > 1){

        av = (int *) malloc(sizeof(f.codigo)*qtAvailable); //aloca mem para todos os elementos a serem ordenados
        if(av == NULL){
            printf("Falha ao alocar memoria para ordenacao do arquivo de codigos de funcionarios disponiveis.\n");

            fclose(availableCodes);

            exit(EXIT_FAILURE);
        }

        fseek(availableCodes, 0, SEEK_SET); //reseta a posição do arquivo para o começo
        fread(av, sizeof(f.codigo), qtAvailable, availableCodes); //lê todos os elementos do arquivo

        fclose(availableCodes); //fecha o arquivo para prevenir que ele seja corrompido caso haja algum "crash" durante a ordenação

        for(i=0; i<qtAvailable; i++)            //ordenação
            for(j=i+1; j<qtAvailable; j++){     //
                if(av[j] < av[i]){              //
                    temp = av[j];               //
                    av[j] = av[i];              //
                    av[i] = temp;               //
                }
            }

        availableCodes=fopen("availableCodes.txt", "r+b"); //abre o arquivo novamente
        if(availableCodes == NULL){
            printf("Falha na abertura do arquivo 'availableCodes.txt'\n");

            free(av);

            exit(EXIT_FAILURE);
        }

        fwrite(av, sizeof(f.codigo), qtAvailable, availableCodes); //sobrescreve todos os elementos do arquivo pelos do vetor já ordenados

    }
    
    free(av); 
    fclose(availableCodes);

    return;
}

int loadFuncBuff(int pgNumber){

    FILE *tempF=NULL;
    int qtAvailable; //pega o retorno do ftell para saber a qtd de elementos no arquivo
    int i=0;

    float pgQt; 

    memset(fpage, 0, sizeof(fpage)); //zera o buffer de listagem de funcionários 

    tempF=fopen("funcionarios.bin", "r+b");
    if(tempF == NULL) return 1;

    fseek(tempF, 0, SEEK_END);
    qtAvailable = ftell(tempF);

    if(qtAvailable == 0) return 1; // se não houver funcionários retorna 1 como número de páginas

    fseek(tempF, (pgNumber-1)*BUFF_PAGE*sizeof(f), SEEK_SET); //reposiciona o cursor para uma casa antes do início da página para a leitura

    while(i<BUFF_PAGE && !feof(tempF)){
        fread(&fpage[i], sizeof(f), 1, tempF);
        if(fpage[i].codigo != 0) i++;
    } //alimenta o buffer somente com funcionários existentes. Os que estão zerados não entram

    fclose(tempF);

    tempF=fopen("availableCodes.bin", "r+b");
    if(tempF != NULL){

        fseek(tempF, 0, SEEK_END);
        qtAvailable -= (ftell(tempF)/sizeof(f.codigo))*sizeof(f);
        fclose(tempF);

    } //calcula o numero de funcionários zerados no arquivo principal para calcular a quantidade de páginas corretamente

    pgQt = (double)qtAvailable/(BUFF_PAGE*sizeof(f));

    pgQt = ceil(pgQt);

    return (int) pgQt;
}

int readFCode(){

    int fCode;

    printf(" Codigo do funcionario ou digite 0 para cancelar: ");

    setbuf(stdin, NULL);

    scanf("%d", &fCode);
    
    if(fCode < 1) return 0;

    return fCode;

}