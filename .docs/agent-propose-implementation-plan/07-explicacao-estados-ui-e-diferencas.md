## 07. Explicacao - estados, telas, diferencas e documentacao

[voltar ao índice](../agent-propose-implementation-plan.md)

---

# 4. A máquina de estados também foi seguida, mas ainda não está completa

Aqui temos uma diferença importante.

No texto original propusemos:

```text
IDLE
  ↓
SELECT_MODE
  ↓
CONFIGURING
  ↓
DOSING
  ↓
COMPLETED
  │
  └── ERROR
```

No código atual temos:

```c
typedef enum {
    DOSING_STATE_IDLE,
    DOSING_STATE_DOSING,
    DOSING_STATE_COMPLETED
} DosingState;
```

Portanto **a máquina de estados do domínio ainda está em uma versão inicial**.

Isso não foi um erro de arquitetura.

Foi uma escolha incremental.

A interface já possui:

```text
HOME
 ↓
MODE
 ↓
CONFIG
 ↓
DOSING
 ↓
COMPLETED
```

Mas esses estados de UI ainda não foram transformados em estados equivalentes do domínio.

E inclusive documentamos isso justamente para evitar confundir:

```text
Screen
```

com:

```text
Domain State
```

Isso é importante.

A tela:

```text
SCREEN_CONFIG
```

não necessariamente precisa significar que o domínio está em:

```text
CONFIGURING
```

São conceitos relacionados, mas não obrigatoriamente a mesma coisa.

---

# 5. A arquitetura de telas foi implementada

No texto inicial tínhamos:

```text
Home
 ↓
Selecionar modo
 ↓
Configurar
 ↓
Dosando
 ↓
Concluído
```

Hoje temos literalmente:

```text
home_screen
    ↓
mode_screen
    ↓
config_screen
    ↓
dosing_screen
    ↓
completed_screen
```

E um:

```c
screen_manager_show(...)
```

centralizando as transições.

Isso foi uma evolução importante em relação à ideia inicial, porque evitamos que cada tela fique navegando diretamente para qualquer outra tela.

Hoje a navegação passa pelo:

```text
Screen Manager
```

---

# 6. A separação UI → domínio foi seguida

Essa era uma das recomendações mais importantes do texto inicial.

E hoje temos:

```text
src/
├── ui/
├── domain/
└── hardware/
```

Isso corresponde praticamente diretamente ao conceito:

```text
UI
 ↓
Application/Domain
 ↓
Hardware abstraction
 ↓
Hardware implementation
```

Atualmente:

```text
ui/
   ↓
screen_manager
   ↓
dosing_controller
   ↓
hardware
```

Portanto, sim: **essa decisão foi seguida.**

---

# 7. Uma coisa que NÃO fizemos exatamente como a arquitetura ideal sugeria

Existe uma diferença importante no código atual.

Idealmente seria algo como:

```text
DosingController
       │
       ├── WeightSensor
       │
       └── Dispenser
```

recebendo as abstrações.

Mas atualmente o controller faz:

```c
#include "../hardware/weight_sensor.h"
#include "../hardware/dispenser.h"
```

e usa diretamente:

```c
simulated_weight_sensor
simulated_dispenser
```

Ou seja:

```text
DosingController
       │
       ├── SimulatedWeightSensor
       └── SimulatedDispenser
```

em vez de:

```text
DosingController
       │
       ├── WeightSensor
       └── Dispenser
             │
       ┌─────┴─────┐
       ▼           ▼
   simulated      real
```

**Isso é uma limitação da implementação atual, não uma mudança de arquitetura.**

E não corrigimos imediatamente porque o projeto estava avançando incrementalmente e a simulação já funcionava.

Quando formos fazer a integração real, esse será um dos pontos naturais para evoluir.

---

# 8. Também seguimos a ideia de não começar pelo hardware

Isso acabou sendo uma decisão particularmente boa para este projeto.

Hoje já conseguimos testar:

```text
✓ Navegação
✓ Configuração
✓ +/-
✓ Início
✓ Cancelamento
✓ Dosagem
✓ Peso simulado
✓ Dispensador simulado
✓ Meta
✓ Parada
✓ Conclusão
✓ Nova dosagem
✓ Retorno ao início
```

Sem:

```text
HX711
load cell
servo
ESP32
display físico
```

Ou seja, conseguimos validar uma parte significativa do comportamento antes de descobrir problemas específicos de hardware.

---

# 9. Wokwi: esse foi um ponto que decidimos NÃO seguir ainda

No texto original:

> "Você também pode usar o Wokwi como complemento."

Nós não começamos por ele.

E isso foi uma boa decisão para a etapa em que estávamos.

Porque nosso objetivo inicial era validar principalmente:

```text
UI
+
fluxo
+
domínio
+
simulação
```

O Wokwi seria mais interessante quando começarmos a validar coisas específicas do:

```text
ESP32-S3
GPIO
periféricos
timing
integração embarcada
```

Então a sequência ficou:

```text
PC + LVGL + SDL2
          ↓
domínio + simulação
          ↓
robustez
          ↓
ESP32-S3
          ↓
hardware real
```

em vez de introduzir Wokwi no começo.

---

# 10. Também acabamos criando uma documentação muito mais completa

Essa é uma diferença que não estava necessariamente no texto original.

Hoje temos uma documentação completa em `.docs/` (arquivos `00-project-story.md` a `13-agent-guide.md`, listados em [README.md](../README.md)).

Isso acabou formalizando exatamente a preocupação original de:

> "fazer um projeto que possa evoluir do PC para o hardware sem perder a arquitetura."

E o [`13-agent-guide.md`](../13-agent-guide.md), principalmente, serve para impedir que um agente futuro simplesmente veja o código e faça uma refatoração que quebre essa intenção.

---
