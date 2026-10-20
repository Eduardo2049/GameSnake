# Snake Game

Jogo da cobrinha feito em C com OpenGL/GLUT.

## Como jogar

| Tecla | Ação |
| --- | --- |
| Setas ou W A S D | Mover a cobra |
| R | Jogar novamente (na tela de game over) |
| ESC | Sair (na tela de game over) |

Cada peça vermelha comida vale 1 ponto e aumenta a cobra. A cada 5 pontos a velocidade aumenta. O jogo acaba ao bater na parede ou no próprio corpo.

## Como compilar (Windows + MinGW)

```sh
gcc -std=c99 -o snakegame.exe snakegame.c -lglu32 -lglut32 -lopengl32
./snakegame.exe
```

No VS Code, a task **Compilar e Rodar** (`Ctrl+Shift+B`) já faz as duas coisas.

O `glut32.dll` precisa estar na mesma pasta do executável.
