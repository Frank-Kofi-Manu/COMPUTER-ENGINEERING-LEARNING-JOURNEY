# CLI To‑Do Manager (C)

A simple, cross‑platform command‑line To‑Do app written in C.  
Add, list, mark done, delete tasks — all saved to `tasks.txt`.

## Build
- Windows (MinGW): `gcc todo.c -o todo && todo`
- macOS/Linux: `gcc todo.c -o todo && ./todo`

## Features
- Add/List/Mark/Delete/Clear tasks
- Persistent file storage (`tasks.txt`)
- Clean input handling with `fgets` and range‑checked menus

## File Format
Each line in `tasks.txt` is:
