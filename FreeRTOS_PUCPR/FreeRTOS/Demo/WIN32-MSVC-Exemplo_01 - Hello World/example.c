/*
* CIENCIA DA COMPUTACAO QUARTO PERIODO
* GLAUCIA BISPO SANTOS DE ARAUJO
* LORENA SCHAFFKA
* NIKOLLY GABRIELI DE OLIVEIRA DA SILVA
*/
#include "FreeRTOS.h"
#include "task.h"
#include "basic_io.h"
#include <string.h>
#include <stdio.h>
#include "semphr.h"

/*
 1 - O protótipo da função de uma tarefa deve sempre retornar void, ou seja, a tarefa não possui retorno.
 2 - A função de uma tarefa deve receber um parâmetro de ponteiro void.
*/

// Declaração das funções que serão utilizadas como tarefas
void arfagem(void* pvParameters);
void rolagem(void* pvParameters);
void guinada(void* pvParameters);

// Valores iniciais dos quatro motores do drone
int motor0 = 1000;
int motor1 = 1000;
int motor2 = 1000;
int motor3 = 1000;

// variavel do tipo SemaphoreHandle_t; 
SemaphoreHandle_t xBinarySemaphore;

SemaphoreHandle_t xMutex;


/*
 Denição da estrutura da função
*/
void arfagem(void* pvParameters)
{	
	// Converte o parâmetro recebido pela tarefa para uma string
	char* mover = (char*)pvParameters;

	// Mantém a estrutura padrão de repetição da tarefa
	for (;;) {

		
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
		// Verifica se o movimento solicitado é para frente
		if (strcmp(mover, "Frente") == 0) {

			// Altera a potência dos motores para realizar a arfagem para frente
			motor0 -= 25;
			motor1 -= 25;
			motor2 += 25;
			motor3 += 25;

			// Exibe qual movimento está sendo realizado
			vPrintString("Arfagem: Frente\n");

		}
		// Verifica se o movimento solicitado é para trás
		else if (strcmp(mover, "Trás") == 0) {

			// Altera a potência dos motores para realizar a arfagem para trás
			motor0 += 25;
			motor1 += 25;
			motor2 -= 25;
			motor3 -= 25;

			// Exibe qual movimento está sendo realizado
			vPrintString("Arfagem: Trás\n");

			
		}
	xSemaphoreGive(xBinarySemaphore); 
	}
	}

	// Exibe os valores atuais dos quatro motores
		printf("motor0=%d motor1=%d motor2=%d motor3=%d\n", motor0, motor1, motor2, motor3);

		// Suspende a tarefa por um determinado período
		vTaskDelay(40);
		// Exclui a própria tarefa após sua execução
		vTaskDelete(NULL);
	}


// Função responsável pelo movimento de rolagem do drone
void rolagem(void* pvParameters)
{
	// Converte o parâmetro recebido pela tarefa para uma string
	char* direção = (char*)pvParameters;

	// Mantém a estrutura padrão de repetição da tarefa
	for (;;) {

	if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
		// Verifica se o movimento solicitado é para a esquerda
		if (strcmp(direção, "Esquerda") == 0) {

			// Altera a potência dos motores para inclinar o drone para a esquerda
			motor0 -= 50;
			motor1 += 50;
			motor2 += 50;
			motor3 -= 50;

			// Exibe qual movimento está sendo realizado
			vPrintString("Rolagem: Esquerda\n");
		}

		// Verifica se o movimento solicitado é para a direita
		else if (strcmp(direção, "Direita") == 0) {

			// Altera a potência dos motores para inclinar o drone para a direita
			motor0 += 50;
			motor1 -= 50;
			motor2 -= 50;
			motor3 += 50;

			// Exibe qual movimento está sendo realizado
			vPrintString("Rolagem: Direita\n");
		}
		xSemaphoreGive(xBinarySemaphore); 
	}

		// Exibe os valores atuais dos quatro motores
		printf("motor0=%d motor1=%d motor2=%d motor3=%d\n", motor0, motor1, motor2, motor3);

		// Suspende a tarefa por um determinado período
		vTaskDelay(20);

		// Exclui a própria tarefa após sua execução
		vTaskDelete(NULL);
	}
}



// Função responsável pelo movimento de guinada do drone
void guinada(void* pvParameters)
{
	// Converte o parâmetro recebido pela tarefa para uma string
	char* sentido = (char*)pvParameters;

	// Mantém a estrutura padrão de repetição da tarefa
	for (;;) {

	if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
		// Verifica se a rotação será realizada no sentido horário
		if (strcmp(sentido, "Horario") == 0) {

			// Altera a potência dos motores para realizar a rotação
			motor0 += 100;
			motor1 -= 100;
			motor2 += 100;
			motor3 -= 100;

			// Exibe o sentido da guinada
			vPrintString("Guinada: Horario\n");
		}

		// Caso não seja horário, realiza a guinada no sentido anti-horário
		else {

			// Realiza a alteração inversa na potência dos motores
			motor0 -= 100;
			motor1 += 100;
			motor2 -= 100;
			motor3 += 100;

			// Exibe o sentido da guinada
			vPrintString("Guinada: Anti-Horario\n");
		}
		xSemaphoreGive(xBinarySemaphore); 
		}
		// Exibe os valores atuais dos quatro motores
		printf("motor0=%d motor1=%d motor2=%d motor3=%d\n", motor0, motor1, motor2, motor3);



		// Suspende a tarefa por um determinado período
		vTaskDelay(10);

		// Exclui a própria tarefa após sua execução
		vTaskDelete(NULL);
	}
}


// Função principal responsável por configurar e iniciar as tarefas
int main_(void)
{
	// Define o sentido que será utilizado pela tarefa de guinada
	char sentido[] = "Horario";

	// Define o movimento que será utilizado pela tarefa de arfagem
	char mover[] = "Frente";

	// Define a direção que será utilizada pela tarefa de rolagem
	char direção[] = "Esquerda";


	vSemaphoreCreateBinary(xBinarySemaphore);
 	if (xBinarySemaphore == NULL) {
        /* Falha na criação (ex: memória insuficiente no heap do FreeRTOS) */
        for (;;);
    }


	// Cria a tarefa responsável pela arfagem
	xTaskCreate(arfagem, "Task Arfagem", 1000, (void*)mover, 1, NULL);

	// Cria a tarefa responsável pela rolagem
	xTaskCreate(rolagem, "Task Rolagem", 1000, (void*)direção, 1, NULL);

	// Cria a tarefa responsável pela guinada
	xTaskCreate(guinada, "Task Guinada", 1000, (void*)sentido, 1, NULL);

	// Inicia o escalonador de tarefas
	vTaskStartScheduler();




	// Mantém o programa em execução caso o escalonador seja encerrado
	for (;; );

	return 0;
}