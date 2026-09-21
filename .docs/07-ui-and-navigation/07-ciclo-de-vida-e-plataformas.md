# Ciclo de vida e plataformas

## 47. Contexto da tela de configuração

A tela de configuração utiliza um contexto próprio para saber:

```text
qual modo foi selecionado
```

e:

```text
quais controles devem ser apresentados.
```

Atualmente esse contexto é criado dinamicamente durante a construção da tela.

Essa implementação funciona, mas deve ser observada em futuras evoluções porque o ciclo de vida desse contexto precisa acompanhar corretamente o ciclo de vida da tela.

---

## 48. Ciclo de vida das telas

Cada tela é criada quando solicitada pelo `Screen Manager`.

Conceitualmente:

```text
show SCREEN_HOME
    ↓
home_screen_create()
    ↓
LVGL screen
```

Depois:

```text
show SCREEN_CONFIG
    ↓
config_screen_create()
    ↓
LVGL screen
```

e assim por diante.

Isso mantém a construção específica de cada tela no próprio módulo.

---

## 49. Responsabilidade dos módulos

### `home_screen.c`

Responsável por:

```text
Home (inclui seleção de modo MASSA / VALOR e liberação manual)
```

### `config_screen.c`

Responsável por:

```text
configuração
```

### `manual_release_widget.c`

Responsável por:

```text
widget de liberação manual (LED + botão), reutilizado em Home,
Concluído e Interrompido
```

### `screen_chrome.c`

Responsável por:

```text
barra de status, título, blocos de estado, barra inferior e cards
(cores e layout compartilhados)
```

### `dosing_screen.c`

Responsável por:

```text
acompanhamento da dosagem
```

### `completed_screen.c`

Responsável por:

```text
resultado da dosagem
```

### `screen_manager.c`

Responsável por:

```text
coordenação entre telas
```

---

## 50. Navegação não deve conter regras de negócio

Evitar lógica como:

```text
se peso >= objetivo
    mudar tela
```

diretamente baseada em uma leitura feita pela UI.

O fluxo atual correto é:

```text
controller
    ↓
state = COMPLETED
    ↓
UI detecta state
    ↓
muda tela
```

A diferença é importante.

O domínio determina:

```text
"dosagem terminou"
```

e a UI decide:

```text
"qual tela representa esse resultado?"
```

---

## 51. Relação com o simulador

No PC, a arquitetura é:

```text
SDL2
  ↓
LVGL
  ↓
UI
  ↓
Screen Manager
  ↓
Dosing Controller
  ↓
Simulated Hardware
```

O SDL2 fornece o ambiente de execução.

O comportamento da aplicação não deve depender de recursos específicos do SDL2.

A estratégia de simulação é documentada em [06-simulation-strategy.md](../06-simulation-strategy.md).

---

## 52. Relação com o ESP32-S3

No hardware final, a ideia é:

```text
Display / Touch
       ↓
      LVGL
       ↓
       UI
       ↓
Screen Manager
       ↓
Dosing Controller
       ↓
Hardware Abstraction
       ↓
HX711 / SG90
```

A principal mudança estará na camada de plataforma e hardware.

A navegação conceitual deve permanecer.

O hardware alvo é descrito em [08-target-hardware.md](../08-target-hardware.md).

---

## 53. Evolução visual

A interface atual é funcional e serve principalmente para validar:

```text
fluxo
```

e:

```text
comportamento
```

A aparência visual ainda pode evoluir.

Possíveis melhorias futuras:

* tema;
* tipografia;
* ícones;
* espaçamento;
* indicadores visuais;
* feedback de interação;
* animações;
* suporte adequado a acentuação;
* layouts específicos para 800×480;
* melhor aproveitamento do touchscreen.

Essas melhorias não devem alterar desnecessariamente o domínio.

---

## 54. Suporte a caracteres

Atualmente alguns textos foram escritos sem acentos devido à fonte padrão utilizada pelo LVGL.

Exemplo:

```text
Racao
Porcoes
Inicio
concluida
```

Em uma futura versão, uma fonte com suporte aos caracteres necessários poderá ser incorporada.

Isso é uma preocupação de apresentação e não deve modificar a lógica da aplicação.

---

## 55. Interface de 800×480

O display alvo possui:

```text
800 × 480
```

A UI deve ser desenvolvida considerando essa resolução.

No simulador, a inicialização atual utiliza:

```c
sdl_hal_init(800, 480);
```

Isso permite validar antecipadamente:

* posicionamento;
* tamanhos;
* botões;
* barras;
* textos;
* espaçamento.

---

## 56. Touchscreen

O hardware final utilizará uma tela capacitiva.

Portanto, a UI deve evitar depender exclusivamente de interações impossíveis ou inadequadas para touchscreen.

Os controles principais devem ser:

```text
botões grandes
áreas de toque claras
feedback visual
```

O simulador deve continuar sendo utilizado para validar a navegação antes da integração física.