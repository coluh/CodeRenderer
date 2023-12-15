#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>
#define TIME_SLICE 10
int counterErr = 0;
void ControlFPS(clock_t *previousTime) {
	clock_t drawTime = clock() - *previousTime;
	clock_t waitTime = TIME_SLICE - drawTime;
	if (waitTime <= 0) {
		printf("\n________________\n\x1B[1;37;41m TOO BUSY \x1B[0m\n________________\n");
		counterErr++;
	}
	else {
		Sleep(waitTime);
	}
	*previousTime = clock();
}
int main(int argc, char* argv[]) {
	int counter = 0;
	printf("\033[?25l");
	clock_t previousTime = clock();
	while (true) {
		printf("\x1B[1;1H");
		printf("Hello, world!\n%d\nError: %d times", counter/100, counterErr);
		counter++;
		ControlFPS(&previousTime);
	}
	return 0;
}