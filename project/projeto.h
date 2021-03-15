#ifndef PROJETO_H
#define PROJETO_H

#ifdef _WIN32 // Includes both 32 bit and 64 bit
    #define CLS "cls"
    	    
#elif __linux__
    #define CLS "clear"

#endif

//strucs
typedef struct{

	char nome[200];
	int codigo;
	char cargo[20];
	float salario;
	time_t rawtime;

}funcionarios;


//Declaração das funções
void menu();

void fecharArquivo();

void abrirArquivo();

void cadastrar();

void listar();

void consultar();

void alteraSalario();

void alterarCargo();

void demitir();

void pause();

/**
 * @brief Função que converte (struct tm) da lib "time.h" para (time_t) e retorna a data em string.
 * 
 * @return Retorna um ponteiro char* com a data no formato: dd/mm/yyyy. String de tamanho 10+1.
 */
char *tmTOstring();

void listar_demitidos();

#endif
