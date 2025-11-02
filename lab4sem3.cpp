#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define MAX_LEVEL 1000 

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

int element_levels[MAX_LEVEL] = { 0 };

struct Node* ins_unq(struct Node* root, int data) {
    if (root == NULL) {
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (data < root->data) {
        root->left = ins_unq(root->left, data);
    }
    else if (data > root->data) {
        root->right = ins_unq(root->right, data);
    }
    return root;
}

int povtor(struct Node* root, int key) {
    if (root == NULL) return 0;

    if (root->data == key) return 1;
    else if (key < root->data)
        return povtor(root->left, key);
    else
        return povtor(root->right, key);
}

struct Node* ins_dup_limited(struct Node* root, int data, int current_level) {
    if (root == NULL) {
        if (element_levels[data] > 0 && current_level > 5) {
            printf("Дубликат %d не добавлен - превышена максимальная глубина 5 (текущий уровень: %d)\n", data, current_level);
            return NULL;
        }
        if (current_level > element_levels[data]) {
            element_levels[data] = current_level;
        }

        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;
        printf("Элемент %d добавлен на уровень %d\n", data, current_level);
        return newNode;
    }

    if (data < root->data) {
        root->left = ins_dup_limited(root->left, data, current_level + 1);
    }
    else {
        root->right = ins_dup_limited(root->right, data, current_level + 1);
    }
    return root;
}

int count_dup(struct Node* root, int key) {
    if (root == NULL) return 0;

    int count = 0;
    if (root->data == key) count = 1;

    count += count_dup(root->left, key);
    count += count_dup(root->right, key);

    return count;
}

void print_tree(struct Node* root, int level) {
    if (root == NULL) return;

    print_tree(root->right, level + 1);
    for (int i = 0; i < level; i++) printf("   ");
    printf("%d\n", root->data);
    print_tree(root->left, level + 1);
}

void print_all_levels(struct Node* root, int current_level) {
    if (root == NULL) return;

    printf("Элемент %d находится на уровне %d\n", root->data, current_level);
    print_all_levels(root->left, current_level + 1);
    print_all_levels(root->right, current_level + 1);
}

void find_elements_at_level(struct Node* root, int current_level, int target_level) {
    if (root == NULL) return;

    if (current_level == target_level) {
        printf("На уровне %d найден элемент: %d\n", target_level, root->data);
    }

    find_elements_at_level(root->left, current_level + 1, target_level);
    find_elements_at_level(root->right, current_level + 1, target_level);
}

void free_tree(struct Node* root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main() {
    setlocale(LC_ALL, "");
    struct Node* root = NULL;
    int choice, data;
    for (int i = 0; i < MAX_LEVEL; i++) {
        element_levels[i] = -1;
    }

    while (1) {
        printf("Выберите режим:\n");
        printf("1 - БЕЗ повторений\n");
        printf("2 - С повторениями (дубликаты только до 5 уровня)\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Режим: БЕЗ повторений\n");
            break;
        case 2:
            printf("Режим: С повторениями (дубликаты только до 5 уровня)\n");
            break;
        default:
            printf("Неверный выбор! Попробуйте снова.\n\n");
            continue;
        }
        break;
    }

    printf("\nВведите числа (0 для окончания ввода):\n");

    if (choice == 1) {
        while (1) {
            printf("Число: ");
            scanf("%d", &data);
            if (data == 0) break;
            if (data < 0) {
                printf("Отрицательные числа не допускаются!\n");
                continue;
            }
            root = ins_unq(root, data);
        }
    }
    else {
        while (1) {
            printf("Число: ");
            scanf("%d", &data);
            if (data == 0) break;
            if (data < 0) {
                printf("Отрицательные числа не допускаются!\n");
                continue;
            }
            if (data >= MAX_LEVEL) {
                printf("Число слишком большое! Максимум: %d\n", MAX_LEVEL - 1);
                continue;
            }
            struct Node* new_root = ins_dup_limited(root, data, 0);
            if (new_root != NULL) {
                root = new_root;
            }
        }
    }

    printf("\nДерево:\n");
    print_tree(root, 0);

    printf("\nВсе элементы с их уровнями:\n");
    print_all_levels(root, 0);

    int search_level;
    printf("\nВведите уровень для поиска элементов: ");
    scanf("%d", &search_level);
    printf("Элементы на уровне %d:\n", search_level);
    find_elements_at_level(root, 0, search_level);

    int search_key;
    printf("\nВведите число для поиска: ");
    scanf("%d", &search_key);

    int count;
    if (choice == 1) {
        count = povtor(root, search_key);
    }
    else {
        count = count_dup(root, search_key);
    }

    if (count > 0) {
        printf("Число %d найдено! Встречается %d раз(а)\n", search_key, count);
    }
    else {
        printf("Число %d не найдено\n", search_key);
    }

    free_tree(root);
    return 0;
}