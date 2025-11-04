#include "LinkedList.h"
#include <stdlib.h>

// Cria uma nova lista encadeada vazia
LinkedList *linkedlist_create(void) {
  LinkedList *list = malloc(sizeof(LinkedList));
  if(list == NULL) {
    return NULL;
  }

  list->head   = NULL;
  list->tail   = NULL;
  list->length = 0;

  return list;
}

// Destrói uma lista encadeada e libera sua memória
void linkedlist_destroy(LinkedList *list) {
  if(list == NULL) {
    return;
  }

  ListNode *current = list->head;
  while(current != NULL) {
    ListNode *next = current->next;
    free(current);
    current = next;
  }

  free(list);
}

// Adiciona um dígito no final da lista
void linkedlist_append(LinkedList *list, char digit) {
  if(list == NULL) {
    return;
  }

  ListNode *new_node = malloc(sizeof(ListNode));
  if(new_node == NULL) {
    return;
  }

  new_node->digit = digit;
  new_node->next  = NULL;

  if(list->head == NULL) {
    // Lista vazia
    list->head = new_node;
    list->tail = new_node;
  } else {
    // Adiciona ao final
    list->tail->next = new_node;
    list->tail       = new_node;
  }

  list->length++;
}

// Adiciona um dígito no início da lista
void linkedlist_prepend(LinkedList *list, char digit) {
  if(list == NULL) {
    return;
  }

  ListNode *new_node = malloc(sizeof(ListNode));
  if(new_node == NULL) {
    return;
  }

  new_node->digit = digit;
  new_node->next  = list->head;

  if(list->head == NULL) {
    // Lista vazia
    list->tail = new_node;
  }

  list->head = new_node;
  list->length++;
}

// Obtém o dígito no índice especificado (0 é o primeiro elemento)
char linkedlist_get(LinkedList *list, size_t index) {
  if(list == NULL || index >= list->length) {
    return 0;
  }

  ListNode *current = list->head;
  for(size_t i = 0; i < index; i++) {
    current = current->next;
  }

  return current->digit;
}

// Define o dígito no índice especificado
void linkedlist_set(LinkedList *list, size_t index, char digit) {
  if(list == NULL || index >= list->length) {
    return;
  }

  ListNode *current = list->head;
  for(size_t i = 0; i < index; i++) {
    current = current->next;
  }

  current->digit = digit;
}

// Retorna o comprimento da lista
size_t linkedlist_length(LinkedList *list) {
  if(list == NULL) {
    return 0;
  }
  return list->length;
}

// Cria uma cópia da lista
LinkedList *linkedlist_copy(LinkedList *list) {
  if(list == NULL) {
    return NULL;
  }

  LinkedList *copy = linkedlist_create();
  if(copy == NULL) {
    return NULL;
  }

  ListNode *current = list->head;
  while(current != NULL) {
    linkedlist_append(copy, current->digit);
    current = current->next;
  }

  return copy;
}

// Remove todos os elementos da lista
void linkedlist_clear(LinkedList *list) {
  if(list == NULL) {
    return;
  }

  ListNode *current = list->head;
  while(current != NULL) {
    ListNode *next = current->next;
    free(current);
    current = next;
  }

  list->head   = NULL;
  list->tail   = NULL;
  list->length = 0;
}

// Remove zeros à esquerda (do final da lista, já que LSB está primeiro)
void linkedlist_remove_leading_zeros(LinkedList *list) {
  if(list == NULL || list->length == 0) {
    return;
  }

  // Remove zeros do final (que são zeros à esquerda na representação)
  // Mas mantém pelo menos um zero se a lista só tiver zeros
  while(list->length > 1 && list->tail != NULL && list->tail->digit == 0) {
    // Encontra o penúltimo nó
    if(list->head == list->tail) {
      // Só tem um elemento
      break;
    }

    ListNode *current = list->head;
    while(current->next != list->tail) {
      current = current->next;
    }

    // Remove o último nó
    free(list->tail);
    list->tail       = current;
    list->tail->next = NULL;
    list->length--;
  }
}
