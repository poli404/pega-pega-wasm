# Pega-Pega: The Game

Minigame 1v1 desenvolvido em **C**, usando a engine [WASM-4](https://wasm4.org) para jogos em WebAssembly.
Dois players se enfrentam em uma arena.

---

## Requisitos

* Linux
* [WASM-4 instalado](https://wasm4.org/docs/getting-started/setup)
* Compilador `clang` instalado (recomendado)
* SDK WASI (`wasi-sdk-25.0-x86_64-linux`) -> colocar link

--- 

## Instalação e execução

1. Baixe e descompacte o projeto:

```bash
tar zxvf jogo-wasm.tar
```

2. Exporte o caminho do WASI SDK
```bash
export WASI_SDK_PATH=$HOME/caminho/jogo-wasm/wasi-sdk-25.0-x86_64-linux
```

3. Acesse a pasta `c` e compile o jogo
```bash
cd c
make clean
make
```

4. Execute o jogo com:
```../w4 watch```
ou
```../w4 run ./build/cart.wasm```

---

## Controles
* Player 1 utiliza o Gamepad 1: setas para se mover + tecla . para atirar;
* Player 2 utiliza o Gamepad 2: teclas SDEF para se mover + tecla A para atirar;

---

## Implementação do jogo
* struct Player
* struct Shot
* update()
* move()
* fireShot()
* updateShot()
* drawBackground()
* drawPLayers()
* drawLives()
* endGame()
* verify_borders()

## Licença
