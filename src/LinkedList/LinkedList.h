#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>

// Estrutura do nó da lista encadeada
typedef struct ListNode {
  char digit;              // Dígito armazenado (0-9)
  struct ListNode* next;   // Ponteiro para o próximo nó
} ListNode;

// Estrutura da lista encadeada
typedef struct {
  ListNode* head;          // Ponteiro para o primeiro nó
  ListNode* tail;          // Ponteiro para o último nó
  size_t length;           // Quantidade de elementos na lista
} LinkedList;

// Cria uma nova lista encadeada vazia
LinkedList* linkedlist_create(void);

// Destrói uma lista encadeada e libera sua memória
void linkedlist_destroy(LinkedList* list);

// Adiciona um dígito no final da lista
void linkedlist_append(LinkedList* list, char digit);

// Adiciona um dígito no início da lista
void linkedlist_prepend(LinkedList* list, char digit);

// Obtém o dígito no índice especificado (0 é o primeiro elemento)
char linkedlist_get(LinkedList* list, size_t index);

// Define o dígito no índice especificado
void linkedlist_set(LinkedList* list, size_t index, char digit);

// Retorna o comprimento da lista
size_t linkedlist_length(LinkedList* list);

// Cria uma cópia da lista
LinkedList* linkedlist_copy(LinkedList* list);

// Remove todos os elementos da lista
void linkedlist_clear(LinkedList* list);

// Remove zeros à esquerda (do final da lista, já que LSB está primeiro)
void linkedlist_remove_leading_zeros(LinkedList* list);

#endif // LINKEDLIST_H

