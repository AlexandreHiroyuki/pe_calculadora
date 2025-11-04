#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>
#include <stdint.h>

// Estrutura do nó da lista encadeada
typedef struct ListNode {
  uint32_t value;          // Valor armazenado (base 2^32)
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

// Adiciona um valor no final da lista
void linkedlist_append(LinkedList* list, uint32_t value);

// Adiciona um valor no início da lista
void linkedlist_prepend(LinkedList* list, uint32_t value);

// Obtém o valor no índice especificado (0 é o primeiro elemento)
uint32_t linkedlist_get(LinkedList* list, size_t index);

// Define o valor no índice especificado
void linkedlist_set(LinkedList* list, size_t index, uint32_t value);

// Retorna o comprimento da lista
size_t linkedlist_length(LinkedList* list);

// Cria uma cópia da lista
LinkedList* linkedlist_copy(LinkedList* list);

// Remove todos os elementos da lista
void linkedlist_clear(LinkedList* list);

// Remove zeros à esquerda (do final da lista, já que LSB está primeiro)
void linkedlist_remove_leading_zeros(LinkedList* list);

#endif // LINKEDLIST_H

