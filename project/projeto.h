#ifndef PROJETO_H
#define PROJETO_H


#ifdef _WIN32 // Includes both 32 bit and 64 bit
    #define CLS "cls"
    	    
#elif __linux__
    #define CLS "clear"

#else
	#define CLS "clear"

#endif

//===============================================================================
//strucs
//===============================================================================

typedef struct{

	char nome[70];
	int codigo;
	char cargo[70];
	float salario;
	long rawtime;

}funcionarios;

//===============================================================================
//Declaração das funções
//===============================================================================

/**
 * @brief Função que contém o menu principal.
 * 
 */
void menu();

/**
 * @brief Abre ou cria o arquivo e verifica se fopen() não retornou NULL. Caso o arquivo for aberto como leitura
 * e não existir, ele será criado.
 * 
 * @param filename Nome do arquivo.
 * @param mode Modo de abertura do arquivo.
 * @return Será retornado um FILE* com o endereço do arquivo aberto.
 */
FILE * abrirArquivo(const char *filename, const char *mode);

/**
 * @brief Função que realiza a operação de cadastro dos funcionários.
 * 
 */
void cadastrar();

/**
 * @brief Função que realiza a listagem dos funcionários.
 * 
 */
void listar();

/**
 * @brief Função que busca e exibe os dados do funcionário informado caso ele exista.
 * 
 * @param cod Código do funcionário a ser procurado nos registros.
 */
void consultar(int cod);

/**
 * @brief Função para alterar o salário de um funcionário já cadastrado.
 * 
 * @param cod Código do funcionário a ser procurado nos registros.
 */
void alterarSalario(int cod);

/**
 * @brief Função para alterar o salário de um funcionário já cadastrado.
 * 
 * @param cod Código do funcionário a ser procurado nos registros.
 */
void alterarCargo(int cod);

/**
 * @brief Função para realizar a demissão de um funcionário cadastrado.
 * 
 * @param cod Código do funcionário a ser procurado nos registros.
 */
void demitir(int cod);

/**
 * @brief Função que realiza uma pausa no terminal.
 * 
 */
void pause();

/**
 * @brief Função que converte (struct tm) da lib "time.h" para (time_t) e retorna a data em string.
 * 
 * @return Retorna um ponteiro char* com a data no formato: dd/mm/yyyy. String de tamanho 10+1.
 */
char *tmTOstring();

/**
 * @brief Função que realiza a listagem dos funcionários demitidos.
 * 
 */
void listar_demitidos();

/**
 * @brief Função para realizar a limpeza do arquivo de funcionários demitidos.
 * 
 */
void cleanFiredList();

/**
 * @brief Função que realiza a ordenação dos códigos dos funcionários que foram demitidos
 * para estarem disponíveis para serem usados novamente na ordem crescente.
 * 
 */
void sortAvailableCodes();

/**
 * @brief Função que calcula a quantidade de páginas de acordo com o tamanho do BUFF_PAGE
 * e alimenta um buffer com os funcionários da página escolhida.
 * 
 * @param pgNumber Número da página a ser exibida.
 * @return Retorna a (int) quantidade de páginas existentes.
 */
int loadFuncBuff(int pgNumber);

/**
 * @brief Função que lê um int.
 * 
 * @return Retorna o int lido.
 */
int readFCode();

#endif
