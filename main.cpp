#include <FreeRTOS.h>
#include <task.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdio.h>
#include <queue.h>
#include "motor.hpp"

// Definições de pinos e parâmetros
#define POT_PIN 26        // Pino do ADC conectado ao potenciômetro
#define MIN_DUTY 30   // Duty cycle mínimo em porcentagem
#define MAX_DUTY 50    // Duty cycle máximo em porcentagem
QueueHandle_t motorQueue;

auto motor = Motor(300, 15);

void potentiometerTask(void *pvParameters) {
	while (true) {
		// Lê o valor do ADC (0-4095)
		uint16_t potValue = adc_read();

		// Normaliza para a faixa de 30%-70%
		int normalizedValue = (potValue * (MAX_DUTY - MIN_DUTY) / 4095) + MIN_DUTY;


		// Envia o valor para a fila
		if (xQueueSend(motorQueue, &normalizedValue, portMAX_DELAY) != pdPASS) {
			printf("Erro ao enviar valor para a fila!\n");
		}
	printf("Pot value: %d \n", normalizedValue);
		// Pequeno atraso para suavizar leituras
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

// Task do motor (exemplo para processar os valores da fila)
void motorControlTask(void *pvParameters) {
	int dutyCycle;
	while (true) {
		// Aguarda por valores na fila
		if (xQueueReceive(motorQueue, &dutyCycle, portMAX_DELAY) == pdPASS) {
			// Ajusta a velocidade do motor usando a classe Motor
			printf("Velocidade ajustada: %d%%\n", dutyCycle);
			motor.setSpeed(dutyCycle);
		}
		vTaskDelay(50);
	}
}


int main() {
	printf("motor rodando?");

	motor.vInitMotors();
	adc_init();
	adc_gpio_init(POT_PIN);
	adc_select_input(0);  // Seleciona o canal 0 do ADC (correspondente ao pino 26)

	// Criação da fila para comunicação entre tasks
	motorQueue = xQueueCreate(5, sizeof(int));
	xTaskCreate(potentiometerTask, "PotTask", 256, NULL, 1, NULL);
	xTaskCreate(motorControlTask, "motorControl", 256, NULL, 1, NULL);

	vTaskStartScheduler();
	while (true);
}
