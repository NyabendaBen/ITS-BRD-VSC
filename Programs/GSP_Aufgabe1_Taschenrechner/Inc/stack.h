#ifndef STACK_H
#define STACK_H

#include "errorhandler.h"

#define STACK_SIZE 8

// Legt einen Wert auf den Stack
ErrorCode stack_push(int value);

// Nimmt den obersten Wert vom Stack
ErrorCode stack_pop(int *value);

// Liest einen Wert ohne ihn zu entfernen
ErrorCode stack_peek(int depth, int *value);

// Leert den Stack
void stack_clear(void);

// Gibt die aktuelle Anzahl der Elemente zurück
int stack_size(void);

#endif // STACK_H