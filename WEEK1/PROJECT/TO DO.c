/*
 * CLI To-Do Manager (C)
 * Author: Frank Manu (your name on GitHub)
 * Description: Add, list, mark done, delete tasks. Persists to tasks.txt.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 200
#define MAX_DESC  200
#define DATA_FILE "tasks.txt"

typedef struct {
    int  done;                 // 0 = not done, 1 = done
    char desc[MAX_DESC];       // task description
} Task;

/* ---------- Utilities ---------- */

static void read_line(const char *prompt, char *buffer, size_t size) {
    if (prompt) printf("%s", prompt);
    if (!fgets(buffer, (int)size, stdin)) {
        // EOF or error
        buffer[0] = '\0';
        return;
    }
    // strip trailing newline if present
    size_t n = strcspn(buffer, "\n");
    buffer[n] = '\0';
}

static int read_int_in_range(const char *prompt, int min, int max) {
    char line[64];
    char *end;
    long val;

    for (;;) {
        if (prompt) printf("%s", prompt);
        if (!fgets(line, sizeof line, stdin)) {
            puts("Input error. Try again.");
            clearerr(stdin);
            continue;
        }
        val = strtol(line, &end, 10);
        if (end == line || (*end != '\n' && *end != '\0')) {
            puts("Please enter a valid number.");
            continue;
        }
        if (val < min || val > max) {
            printf("Enter a number between %d and %d.\n", min, max);
            continue;
        }
        return (int)val;
    }
}

/* ---------- Persistence ---------- */

static int load_tasks(Task tasks[], int max_tasks) {
    FILE *f = fopen(DATA_FILE, "r");
    if (!f) return 0; // first run: no file yet

    char line[512];
    int count = 0;

    while (count < max_tasks && fgets(line, sizeof line, f)) {
        // format: status|description
        size_t n = strcspn(line, "\n");
        line[n] = '\0';

        char *sep = strchr(line, '|');
        if (!sep) continue; // ignore malformed line

        *sep = '\0';
        int done = atoi(line);
        const char *desc = sep + 1;

        tasks[count].done = done ? 1 : 0;
        strncpy(tasks[count].desc, desc, MAX_DESC - 1);
        tasks[count].desc[MAX_DESC - 1] = '\0';
        count++;
    }

    fclose(f);
    return count;
}

static void save_tasks(const Task tasks[], int count) {
    FILE *f = fopen(DATA_FILE, "w");
    if (!f) {
        perror("Could not save tasks");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d|%s\n", tasks[i].done ? 1 : 0, tasks[i].desc);
    }
    fclose(f);
}

/* ---------- Actions ---------- */

static void list_tasks(const Task tasks[], int count) {
    if (count == 0) {
        puts("\nNo tasks yet. Add one!");
        return;
    }
    puts("\nYour Tasks:");
    for (int i = 0; i < count; i++) {
        printf("%2d. [%c] %s\n", i + 1, tasks[i].done ? 'x' : ' ', tasks[i].desc);
    }
}

static void add_task(Task tasks[], int *count) {
    if (*count >= MAX_TASKS) {
        puts("Task list is full.");
        return;
    }
    char desc[MAX_DESC];
    read_line("Enter task description: ", desc, sizeof desc);

    if (desc[0] == '\0') {
        puts("Empty description ignored.");
        return;
    }

    tasks[*count].done = 0;
    strncpy(tasks[*count].desc, desc, MAX_DESC - 1);
    tasks[*count].desc[MAX_DESC - 1] = '\0';
    (*count)++;

    save_tasks(tasks, *count);
    puts("Added ✅");
}

static void mark_done(Task tasks[], int count) {
    if (count == 0) {
        puts("No tasks to mark.");
        return;
    }
    list_tasks(tasks, count);
    int idx = read_int_in_range("Mark which task as done? (number): ", 1, count) - 1;
    tasks[idx].done = 1;
    save_tasks(tasks, count);
    puts("Marked as done ✅");
}

static void delete_task(Task tasks[], int *count) {
    if (*count == 0) {
        puts("No tasks to delete.");
        return;
    }
    list_tasks(tasks, *count);
    int idx = read_int_in_range("Delete which task? (number): ", 1, *count) - 1;

    for (int i = idx; i < *count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    (*count)--;
    save_tasks(tasks, *count);
    puts("Deleted 🗑️");
}

static void clear_all(Task tasks[], int *count) {
    (void)tasks; // unused parameter (we just reset count and overwrite file)
    char ans[8];
    read_line("Are you sure you want to clear ALL tasks? (y/N): ", ans, sizeof ans);
    if (ans[0] == 'y' || ans[0] == 'Y') {
        *count = 0;
        save_tasks(tasks, *count);
        puts("All tasks cleared.");
    } else {
        puts("Canceled.");
    }
}

/* ---------- UI ---------- */

static void show_menu(void) {
    puts("\n=== CLI To-Do Manager ===");
    puts("1) List tasks");
    puts("2) Add a task");
    puts("3) Mark a task as done");
    puts("4) Delete a task");
    puts("5) Clear all tasks");
    puts("6) Exit");
}

int main(void) {
    Task tasks[MAX_TASKS];
    int count = load_tasks(tasks, MAX_TASKS);

    printf("Loaded %d task(s). Data file: %s\n", count, DATA_FILE);

    for (;;) {
        show_menu();
        int choice = read_int_in_range("Choose 1-6: ", 1, 6);

        switch (choice) {
            case 1: list_tasks(tasks, count); break;
            case 2: add_task(tasks, &count); break;
            case 3: mark_done(tasks, count); break;
            case 4: delete_task(tasks, &count); break;
            case 5: clear_all(tasks, &count); break;
            case 6: puts("Goodbye!"); return 0;
            default: break;
        }
    }
}