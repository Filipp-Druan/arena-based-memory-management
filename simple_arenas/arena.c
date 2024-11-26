#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void* buffer;
    size_t buffer_size;
    size_t offset;
    size_t last_allocation_offset;
} Arena;

// Создаёт арену, получив блок и его размер. Возвращает саму арену, а не указатель на неё.
Arena arena_manual_init(void* buffer, size_t buffer_size) {
    Arena arena = {buffer, buffer_size, 0, 0};
    return arena;
}

// Выделяет память сразу и под структуру арены и под буффер.
// Принимает размер буффера.
// В начале выделенной памяти располагает структуру,
// а буффер в оставшейся памяти.
Arena* arena_heap_init(size_t buffer_size) {
    size_t struct_size = sizeof(Arena);
    size_t full_size   = struct_size + buffer_size;

    void* memory = malloc(full_size);
    if (memory == NULL) return NULL;

    void* buffer = memory + struct_size;
    // Буффер начинается сразу после структуры
    Arena* arena = (Arena *)memory;
    // Располагаем в начале памяти структуру арены

    *arena = arena_manual_init(buffer, buffer_size);
    return arena;
}

void arena_heap_deinit(Arena* arena_pointer) {
    free(arena_pointer); // Так можно сделать потому,
    // что мы выделяем область памяти и под структуру и под буффер
    // одним вызовом malloc, и структура находится в самом начале
    // этой области.
}

// Вычисляет, сколько свободного места осталось в буффере арены.
size_t arena_free_memory_size(Arena* arena) {
    return (arena->buffer_size) - (arena->offset);
}

// Функция принимает арену и размер памяти, которую на ней нужно выделить.
// Проверяет, что память может быть выделена. Если нет, возвращает NULL.
// Если да, возвращает указатель на начало выделенной памяти.
void* arena_alloc(Arena* arena, size_t size) {
    size_t free_memory_size = arena_free_memory_size(arena);
    // Может быть, из-за вычисления этого размера мы храним смещения, а не адреса?

    if (free_memory_size < size) return NULL;

    void* address = (arena->buffer) + (arena->offset);
    arena->last_allocation_offset = arena->offset;
    arena->offset += size;

    return address;
}

// Увеличивает размер области, выделенной на арени под последний объект.
// Если увеличить область не удалось, возвращает NULL.
void* arena_realloc_last(Arena* arena, size_t size) {
    if (arena_free_memory_size(arena) < size) return NULL;

    size_t new_offset = arena->last_allocation_offset + size;

    size_t size_delta = new_offset - arena->offset;
    // Находим, насколько изменился размер

    void* new_area = arena->buffer + arena->offset;
    // Находим адрес области, которая раньше не принадлежала
    // последнему выделенному объекту, а теперь принадлежит.
    memset(new_area, 0, size_delta);
    // Заполняем новую выделенную область нулями.

    arena->offset = new_offset;

    return arena->buffer + arena->last_allocation_offset;
}

// Освобождает всю арену целиком, но не удаляет её.
void arena_free_all(Arena* arena) {
    arena->offset                 = 0;
    arena->last_allocation_offset = 0; // Нужно ли обнулять это смещение? Или не нужно? Пока буду обнулять.
}

int test_arena_heap_init_deinit() {
    srand(1);
    for (size_t i = 50000000; i != 0; i--) {
        Arena* arena = arena_heap_init(rand() / 1000 + 2048);
        if (arena == NULL) printf("Не удалось выделеть память.");

        arena_heap_deinit(arena);
        //printf("Ok\n");
    }
    return 0;
}


int main() {
    test_arena_heap_init_deinit();
}
