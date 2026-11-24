#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale>

struct reg
{
	char sabor[30];
	char tamanho[15];
	float preco;
	char status;
};

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void geraarqtxt(FILE *);
void excluir(FILE *);

int main()
{
	int op;
	FILE *arq;

	// Abre o arquivo de dados da pizzaria
	if ((arq = fopen("pizzas.dat", "rb+")) == NULL)
	{
		if ((arq = fopen("pizzas.dat", "wb+")) == NULL)
		{
			printf("Falha ao abrir o arquivo!\n");
			system("pause");
		}
	}

	do
	{
		system("CLS");
		printf("\n======= SISTEMA DA PIZZARIA ======= \n");
		printf("1. Cadastrar Pizza\n");
		printf("2. Consultar por codigo\n");
		printf("3. Gerar arquivo TXT\n");
		printf("4. Excluir registo\n");
		printf("5. Sair\n");
		printf("===========Pizzas cadastradas:%d=\n", tamanho(arq));
		printf("Opcao: ");
		scanf("%d", &op);
		
		switch (op)
		{
		case 1:
			cadastrar(arq);
			break;
		case 2:
			consultar(arq);
			break;
		case 3:
			geraarqtxt(arq);
			break;
		case 4:
			excluir(arq); 
			break;
		case 5: 
			fclose(arq);
			break;
		}
	} while (op != 5);
	
	return 0;
}

void cadastrar(FILE *arq)
{
	reg pizza;
	char confirma;
	pizza.status = ' ';
	fflush(stdin);

	printf("Cadastrando nova pizza:\n");
	printf("\nNumero do registo: %d\n", tamanho(arq) + 1);
	
	printf("Sabor........: ");
	scanf(" %[^\n]", pizza.sabor); // Lê string com espaços de forma segura
	
	printf("Tamanho......: ");
	scanf(" %[^\n]", pizza.tamanho);
	
	printf("Preco........: ");
	scanf("%f", &pizza.preco);

	printf("\nConfirma <s/n>: ");
	fflush(stdin);
	scanf(" %c", &confirma);

	if (toupper(confirma) == 'S')
	{
		printf("\ngravando...\n\n");
		fseek(arq, 0, SEEK_END);
		fwrite(&pizza, sizeof(reg), 1, arq);
	}
	system("pause");
}

void consultar(FILE *arq)
{
	reg pizza;
	int nr;
	printf("\nConsulta pelo codigo\n");
	printf("\nInforme o Codigo (Numero do registo)...: ");
	scanf("%d", &nr);
	
	if ((nr <= tamanho(arq)) && (nr > 0))
	{
		fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
		fread(&pizza, sizeof(reg), 1, arq);
		
		if (pizza.status == ' ')
		{
			printf("\nSabor.....: %s", pizza.sabor);
			printf("\nTamanho...: %s", pizza.tamanho);
			printf("\nPreco.....: R$ %.2f\n\n", pizza.preco);
		}
		else
		{
			printf("\nRegisto excluido! \n");
		}
	}
	else
	{
		printf("\nNumero de registo invalido!\n");
	}
	system("pause");
}

void geraarqtxt(FILE *arq)
{
	char nomearq[20];
	printf("Nome do arquivo texto (ex: cardapio): ");
	scanf("%s", nomearq);
	strcat(nomearq, ".txt");
	
	FILE *arqtxt = fopen(nomearq, "w");
	if (!arqtxt)
	{
		printf("Nao foi possivel criar esse arquivo!\n");
		system("pause");
		return;
	}
	
	fprintf(arqtxt, "Sabor                         Tamanho        Preco          Status\n");
	fprintf(arqtxt, "==================================================================\n");
	
	int nr;
	reg pizza;
	for (nr = 0; nr < tamanho(arq); nr++)
	{
		fseek(arq, nr * sizeof(reg), SEEK_SET);
		fread(&pizza, sizeof(reg), 1, arq);
		fprintf(arqtxt, "%-30s%-15sR$ %-10.2f- %c\n", pizza.sabor, pizza.tamanho, pizza.preco, pizza.status);
	}
	
	fprintf(arqtxt, "==================================================================\n");
	fclose(arqtxt);
	printf("Arquivo TXT gerado com sucesso!\n");
	system("pause");
}

void excluir(FILE *arq)
{
	reg pizza;
	char confirma;
	int nr;

	printf("\nInforme o codigo do registo para excluir: ");
	scanf("%d", &nr);
	
	if ((nr <= tamanho(arq)) && (nr > 0))
	{
		fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
		fread(&pizza, sizeof(reg), 1, arq);
		
		if (pizza.status == ' ')
		{
			printf("\nSabor.....: %s", pizza.sabor);
			printf("\nTamanho...: %s", pizza.tamanho);
			printf("\nPreco.....: R$ %.2f\n", pizza.preco);
			printf("\nConfirma a exclusao: <s/n>: ");
			
			fflush(stdin);
			scanf(" %c", &confirma);

			if (toupper(confirma) == 'S')
			{
				printf("\nexcluindo...\n\n");
				fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
				pizza.status = '*'; // Exclusão lógica
				fwrite(&pizza, sizeof(reg), 1, arq);
			}
		}
		else
		{
			printf("Registo inexistente ou ja excluido! \n");
		}
	}
	else
	{
		printf("\nNumero de registo invalido!\n");
	}
	system("pause");
}

int tamanho(FILE *arq)
{
	fseek(arq, 0, SEEK_END);
	return ftell(arq) / sizeof(reg);
}
