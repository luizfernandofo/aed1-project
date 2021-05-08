#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include"projeto.h"

#define BUFF_PAGE 10 //tamanho do vetor que vai armazenar os funcionarios que serão exibidos na página 

/*TO DO:
    
    *Ver a possibilidade de um buffer, de tamanho a definir, para armazenar os dados para listar();

    *Incluir uma opção de confirmação de operação.

    *Criar função para alimentar o buffer page
*/


//Variáveis globais
bool sair = false;
FILE* p=NULL;
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
    remove("auxiliar.txt"); 
    return 0;
}


//FUNÇÕES 
void abrirArquivo(){
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
}

void fecharArquivo(){
    //caso criar um novo arquivo não esquecer de fechar ele aqui
    fclose(p);
    fclose(aux);
    fclose(availableCodes);
    fclose(fired);

    return;
}

void cadastrar(){
    
    int avCode=0; //serve para armazenar f.codigo dos func deletados
    long int tempCode; 
	char c;

    do{
        avCode=0; //setado 0 para comparar lá embaixo caso não haja números disponíveis

        abrirArquivo();
        system(CLS);
        printf("\n\n------------- CADASTRO DE FUNCIONARIOS -------------\n\n");

        fread(&avCode, sizeof(avCode), 1, availableCodes);
        if(avCode <= 0){ //caso não haja nenhum codigo de funcionario deletado pendente 

            fseek(p, 0, SEEK_END); 
            tempCode = ftell(p);
            
            if(tempCode == 0) f.codigo = 1;
            else{
                tempCode /= sizeof(f);
                f.codigo = tempCode+1;
            }

        }else{ //Se houver algum codigo de funcionario deletado pendente f.codigo recebe
            f.codigo = avCode;
            fseek(p, sizeof(f)*(avCode-1), SEEK_SET); //o cursor do arquivo principal é setado para a posição onde, no arquivo principal, o índice avCode está zerado
        }

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

            fecharArquivo();
            printf("\nCadastro nao realizado!\n\n");
        
        }else{ //caso confirme: se o codigo do "func" tiver origem de um deletado, avCode vai ser diferente de 0. Caso o arquivo de funcionários disponíveis esteja vazio avCode=0;

            if(avCode != 0){ //o código utilizado do "available.txt" é apagado

                fread(&avCode, sizeof(avCode), 1, availableCodes);
                while(!feof(availableCodes)){
                    fwrite(&avCode, sizeof(avCode), 1, aux);
                    fread(&avCode, sizeof(avCode), 1, availableCodes);
                }

            }

            fclose(aux);
            fclose(availableCodes);
            remove("availableCodes.txt");
            rename("auxiliar.txt","availableCodes.txt");
            //=====

            fwrite(&f,sizeof(f), 1, p); //é feita a escrita do funcionario cadastrado no arquivo.

            fecharArquivo(); //e os arquivos são fechados

            
            printf("\nCadastro realizado com sucesso!\n\n");

        }
        
        printf("DESEJA CADASTRAR MAIS FUNCIONARIOS (s/n)? ");
        setbuf(stdin, NULL);
        scanf("%c", &c);

    }while(c == 'S' || c == 's');
    
    fecharArquivo();
    remove("auxiliar.txt");
    pause();
}

void consultar(int cod){
    
    int a=0;

    if(cod==0) return;

    abrirArquivo();
    system(CLS);
    printf("\n\n------------- CONSULTA DE FUNCIONARIOS -------------\n\n");
    /*printf("Informe o codigo a ser pesquisado: ");
    scanf("%d",&cod);*/
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
    
    fecharArquivo();
    remove("auxiliar.txt");
    pause();

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
}

void alterarSalario(int cod){
    //int cod;
    int a=0;

    if(cod==0) return;

    abrirArquivo();

    system(CLS);
    printf("\n\n------------- ALTERAR SALARIO DE FUNCIONARIOS -------------\n\n");
    /*printf("Informe o funcionario a ter o salario alterado: ");
    scanf("%d",&cod);*/

    fread(&f, sizeof(f), 1, p);
    while(!feof(p)){
        if(f.codigo == cod){
            a++;

            fseek(p, sizeof(f)*(-1), SEEK_CUR); 

            setbuf(stdin, NULL);
            printf("\n Codigo: %d\n Nome: %s\n Cargo: %s\n Salario: R$ %.2f\n", f.codigo, f.nome, f.cargo, f.salario);
            printf("\nInforme o novo salario: R$ ");
            scanf("%f",&f.salario);

            fwrite(&f, sizeof(f),1,p);
            break;
        }
        fread(&f, sizeof(f),1,p);
    }
    if(a==0) printf("\nFuncionario nao encontrado!\n\n");
    else printf("\nSalario alterado com sucesso!\n\n");

    fecharArquivo();
    remove("auxiliar.txt");
    pause();
}

void alterarCargo(int cod){

    //int cod;
    int a=0;

    if(cod==0) return;

    abrirArquivo();

    system(CLS);
    printf("\n\n------------- ALTERAR CARGO DE FUNCIONARIOS -------------\n\n");
    /*printf("Informe o funcionario a ter o cargo alterado: ");
    scanf("%d",&cod);*/
    fread(&f, sizeof(f), 1, p);
    while(!feof(p)){
        if(f.codigo == cod){
            a++;
            fseek(p, sizeof(f)*(-1), SEEK_CUR);
            printf("\n Codigo: %d\n Nome: %s\n Cargo: %s\n Salario: R$ %.2f\n", f.codigo, f.nome, f.cargo, f.salario);
            printf("\nInforme o novo cargo: ");
            setbuf(stdin, NULL);
            gets(f.cargo);
            fwrite(&f, sizeof(f),1,p);
            break;
        }
        fread(&f, sizeof(f),1,p);
    }
    if(a==0) printf("\nFuncionario nao encontrado!\n\n");
    else printf("\nCargo alterado com sucesso!\n\n");

    fecharArquivo();
    remove("auxiliar.txt");
    pause();
}

void demitir(int cod){
    //int cod;
    int a=0;

    if(cod==0) return;

    system(CLS);

    abrirArquivo();
    fseek(availableCodes, 0, SEEK_END);
    fseek(fired, 0, SEEK_END);

    printf("\n\n------------- DEMISSAO DE FUNCIONARIOS -------------\n\n");
    /*printf("Informe o funcionario a ser demitido: ");
    scanf("%d",&cod);*/
    fread(&f, sizeof(f), 1, p);
    while(!feof(p)){
        if(f.codigo == cod){
            a++;

            
            fwrite(&f.codigo, sizeof(f.codigo), 1, availableCodes); //o código do func deletado é escrito no arquivo available.txt

            time(&(f.rawtime)); // é pego a data da demissao
            fwrite(&f, sizeof(f), 1, fired);//o funcionario é escrito no arquivo de fired.txt

            fseek(p, sizeof(f)*(-1), SEEK_CUR); //faz com que o cursor seja reposicionado para a sobrescrita, já que ele passou 1 casa devido a leitura anterior
            memset(&f, 0, sizeof(f)); // f é zerado
            fwrite(&f, sizeof(f),1,p); //e sobrescreve no arquivo principal
            
            break; //interrompe o laço para evitar continuar no loop de forma desnecessária
        }
        fread(&f, sizeof(f),1,p);
    }
    if(a==0) printf("\nFuncionario nao encontrado!\n\n");
    else printf("\nFuncionario demitido com sucesso!\n\n");

    fecharArquivo();
    sortAvailableCodes();
    pause();
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
    printf("\n |  0 - Sair                                             |");
    printf("\n ---------------------------------------------------------\n");
    printf("\n\n ESCOLHA UMA OPCAO: ");
    scanf("%d",&op);

    switch(op){
        case 1: cadastrar(); break;
        case 2: listar(); break;
        case 3: listar_demitidos(); break;
        case 0: sair = true; break;
        default:
            printf("\nOpcao invalida!\n");
            pause();
        break;
    }
}

void pause(){
    setbuf(stdin, NULL);
    printf("\n Pressione ENTER para continuar..."); //não pode ter "lixo" no stdin para ela funcionar
    getchar();
}

char *tmTOstring(){

    timeinfo = localtime(&(f.rawtime));
    sprintf(date, "%d/%d/%d", timeinfo->tm_mday, ((timeinfo->tm_mon)+1), ((timeinfo->tm_year)+1900));
    timeinfo = NULL;
    
  return date;
}

void listar_demitidos(){
 
    abrirArquivo();
    
    system(CLS);
    printf("\n\n------------------------- LISTA DE FUNCIONARIOS DEMITIDOS-------------------------\n\n");
    fread(&f, sizeof(f), 1, fired);
    while(!feof(fired)){
        printf(" Codigo: %d, Nome: %s, Cargo: %s, Salario: R$ %.2f, Demissao: %s\n", f.codigo, f.nome, f.cargo, f.salario, tmTOstring());
        fread(&f, sizeof(f), 1, fired);
    }
    printf("\n");
    
    fecharArquivo();
    remove("auxiliar.txt");
    pause(); 
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

int loadFuncBuff(int n){

    FILE *tempF=NULL;
    long int qtAvailable; //pega o retorno do ftell para saber a qtd de elementos no arquivo
    int i=0;

    float pgQt;

    memset(fpage, 0, sizeof(fpage));

    tempF=fopen("funcionarios.txt", "r+b");
    if(tempF == NULL) return 1;

    fseek(tempF, 0, SEEK_END);
    qtAvailable = ftell(tempF);

    if(qtAvailable == 0) return 1;

    fseek(tempF, (n-1)*BUFF_PAGE*sizeof(f), SEEK_SET);

    while(i<BUFF_PAGE && !feof(tempF)){
        fread(&fpage[i], sizeof(f), 1, tempF);
        if(fpage[i].codigo != 0) i++;
    }

    fclose(tempF);

    tempF=fopen("availableCodes.txt", "r+b");
    if(tempF != NULL){

        fseek(tempF, 0, SEEK_END);
        qtAvailable -= (ftell(tempF)/sizeof(f.codigo))*sizeof(f);
        fclose(tempF);

    }

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