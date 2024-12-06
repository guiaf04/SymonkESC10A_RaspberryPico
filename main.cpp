//
// Created by guilherme on 03/12/24.
//
// #include <pico/stdio.h>
// #include <pico/stdio.h>


#include <cstdio>

#include "FreeRTOS.h"
#include "motor.hpp"
#include "task.h"

auto motor = Motor(300, 15);

void motor_task(void *pvParameters) {
	while (true) {
		int speed = 0;
		for (int i = 0; i < 2; i++) {
			printf(" Digite a velocidade: \n");
			const char speedChar = getchar();

			const int ascii_to_num = speedChar % 48;

			if (i == 0) {
				speed += ascii_to_num * 10;
			} else {
				speed += ascii_to_num;
			}
		}

		printf("Speed: %d\n", speed);

		motor.setSpeed(speed);

		vTaskDelay(2000);
	}
}

void teste_motor_task(void *pvParams) {
	while (true) {
		printf("Começando task motor, setando 0%%\n");
		motor.setSpeed(0);

		vTaskDelay(7000);

		printf("Colocando 30%% para iniciar, espere 3segundos...\n");
		motor.setSpeed(30);

		vTaskDelay(3000);

		printf("Colocando 40%%, o motor deve estar funcionando\n");
		motor.setSpeed(40);

		vTaskDelay(3000);

		printf("Colocando 60%%, o motor esta acelerando\n");
		motor.setSpeed(60);

		vTaskDelay(15000);
		printf("Tentando novamente...\n");
	}
}


int main() {
	printf("motor rodando?");

	motor.vInitMotors();
	// xTaskCreate(motor_task, "motorTask", 1024, NULL, 1, NULL);
	xTaskCreate(teste_motor_task, "clear_motorTask", 1024, nullptr, 1, nullptr);

	vTaskStartScheduler();

	while (true);
}
