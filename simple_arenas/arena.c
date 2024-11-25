#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void* buffer;
    size_t buffer_size;
    size_t offset;
} Arena;

// Создаёт арену, получив блок и его размер. Возвращает саму арену, а не указатель на неё.
Arena arena_manual_init(void* buffer, size_t size) {
    Arena arena = {buffer, size, 0};
    return arena;
}

// Создаёт арену при помощи malloc. Принимает размер необходимой области.
// Возвращает адрес указатель на структуру арена.
// Если память выделить не удалось, возвращает NULL.
Arena* arena_malloc_init(size_t size) {
    Arena* arena  = (Arena *) malloc(sizeof(Arena));
    void*  buffer = malloc(size);

    if (arena  == NULL) return NULL;
    if (buffer == NULL) return NULL;

    *arena = arena_manual_init(buffer, size);

    return arena;
}

// Функция принимает арену и размер памяти, которую на ней нужно выделить.
// Проверяет, что память может быть выделена. Если нет, возвращает NULL.
// Если да, возвращает указатель на начало выделенной памяти.
void* arena_alloc(Arena* arena, size_t size) {
    size_t free_memory_size = (arena->buffer_size) - (arena->offset); // Может быть, из-за вычисления этого размера мы храним смещения, а не адреса?

    if (free_memory_size < size) return NULL;

    void* address = (arena->buffer) + (arena->offset);
    arena->offset += size;

    return address;
}

// Освобождает всю арену целиком
void arena_free_all(Arena* arena) {
    arena->offset = 0;
}

void arena_malloc_deinit(Arena* arena) {
    free(arena->buffer);
    free(arena);
}

int main() {
    Arena* arena = arena_malloc_init(1024);
    if (arena == NULL) return 1;
    char *string_buffer = (char *) arena_alloc(arena, 256);

    string_buffer[0] = 'H';
    string_buffer[1] = 'i';

    printf("Hello!\nIn string_buffer we hawe:\n%s", string_buffer);

    arena_malloc_deinit(arena);
    return 0;
}
