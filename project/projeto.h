#ifndef PROJETO_H
#define PROJETO_H

#ifdef _WIN32 // Includes both 32 bit and 64 bit
    #define CLS "cls"
    	    
#elif __linux__
    #define CLS "clear"

#endif

void abrirArquivo();

void cadastrar();

void listar();

void consultar();

void alteraSalario();

void alterarCargo();

void demitir();

#endif
