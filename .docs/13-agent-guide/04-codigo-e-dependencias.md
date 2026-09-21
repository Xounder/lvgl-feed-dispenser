## 04. Código e dependências

[voltar ao índice](../13-agent-guide.md)

---

## 29. Não refatorar sem necessidade

Uma tarefa como:

> "Adicionar um botão"

não deve automaticamente resultar em:

```text
refatoração completa da UI
mudança da arquitetura
renomeação de arquivos
migração para outra biblioteca
mudança do sistema de build
```

Primeiro resolver o problema solicitado.

Depois, se uma mudança estrutural for realmente necessária, explicar a razão.

---

## 30. Não introduzir abstrações prematuras

O projeto pretende possuir uma boa arquitetura, mas isso não significa criar dezenas de interfaces.

Antes de criar uma abstração, verificar:

```text
Existe mais de uma implementação?
Existe uma necessidade clara de isolamento?
A abstração ajuda na migração?
A abstração reduz acoplamento?
```

Se a resposta for não, talvez seja melhor manter a implementação simples.

---

## 31. CMake

O projeto usa CMake.

Ao adicionar um novo arquivo `.c`, verificar se ele está incluído no:

```text
MAIN_SOURCES
```

atualmente definido no `CMakeLists.txt`.

Não assumir que o CMake descobrirá automaticamente qualquer novo arquivo.

---

## 32. Dependências externas

O projeto utiliza:

```text
LVGL
SDL2
vcpkg
```

Algumas dependências são externas ao repositório e estão no `.gitignore`.

Não adicionar:

```text
build/
vcpkg_installed/
lvgl/
DLLs
```

ao Git sem uma razão explícita.

---

## 33. SDL2

SDL2 é usado como camada de plataforma do simulador PC.

O ambiente atual utiliza:

```text
SDL2 x64-windows
```

A DLL de debug é necessária para execução do build de debug.

Isso é uma particularidade do ambiente PC.

Não levar essa dependência para o firmware ESP32.

---

## 34. LVGL

LVGL é uma dependência central do projeto e continuará sendo importante na migração.

Atualmente o simulador utiliza:

```text
LVGL + SDL2
```

No ESP32, a plataforma será diferente, mas a camada de UI deverá permanecer conceitualmente a mesma.

---

## 35. ThorVG

A configuração atual do LVGL utiliza recursos que exigem:

```text
LV_USE_THORVG = 1
```

quando:

```text
LV_USE_VECTOR_GRAPHIC = 1
```

está habilitado.

Ao alterar `lv_conf.h`, verificar dependências entre as opções.

Não assumir que uma opção do LVGL é independente das demais.

---

## 36. main.c

O `main.c` atual é específico do simulador.

Ele:

```text
inicializa LVGL
 ↓
inicializa SDL HAL
 ↓
inicializa UI
 ↓
executa lv_timer_handler()
```

Não transformar `main.c` em um local para colocar regras de negócio.

O loop principal deve permanecer pequeno.

---

## 37. HAL

O diretório:

```text
src/hal/
```

representa o suporte de plataforma do simulador.

Essa camada pode conter detalhes necessários para:

```text
SDL2
janela
display
input
cursor
```

mas não deve conter regras de dosagem.

---

## 38. UI atual

As telas existentes são:

```text
Home
Mode
Config
Dosing
Completed
Interrupted
```

Fluxo:

```text
Home
 ↓
Mode (Massa / Valor R$)
 ↓
Config
 ↓
Dosing
 ├──→ Completed
 └──→ Interrupted
```

---

## 39. Respeitar a navegação existente

Ao alterar uma tela, preservar os caminhos existentes, salvo se a tarefa pedir explicitamente uma mudança.

Atualmente:

```text
Home → Mode
Mode → Config
Config → Dosing
Dosing → Completed
Dosing → Interrupted
```

Também existem caminhos de retorno:

```text
Mode → Home
Config → Home
Completed → Home
Completed → Mode
Interrupted → Home
Interrupted → Mode
```

---

## 40. Textos da interface

A fonte padrão atual do LVGL não possui todos os caracteres acentuados necessários.

Por isso existem textos sem acentos, por exemplo:

```text
Dosador de Racao
Liberacao manual
Dosagem concluida
Dosagem interrompida
```

Não considerar automaticamente isso como erro de lógica.

Se for necessário adicionar suporte completo a português, isso deve ser tratado como uma decisão de fonte/glyphs do LVGL.

---

## 41. Não adicionar acentos sem verificar a fonte

Antes de trocar:

```text
Racao
```

por:

```text
Ração
```

verificar se a fonte utilizada contém:

```text
ç
ã
```

Caso contrário, a interface poderá apresentar glyphs ausentes.

---

## 42. Timer da tela de dosagem

A tela de dosagem utiliza um timer de aproximadamente:

```text
300 ms
```

Esse timer:

```text
chama dosing_controller_update()
 ↓
lê peso
 ↓
atualiza interface
 ↓
verifica estado
```

Ao concluir, o timer deve deixar de atualizar a operação.

---

## 43. Não misturar timer de UI com temporização física sem pensar

No simulador, o timer de LVGL também serve como mecanismo conveniente de atualização da simulação.

No hardware real, a temporização poderá precisar de outra estratégia.

Não assumir que:

```text
lv_timer
```

é automaticamente o mecanismo correto para todo controle físico.