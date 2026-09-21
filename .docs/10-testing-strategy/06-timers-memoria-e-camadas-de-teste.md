# Timers, memória e camadas de teste

> _Voltar ao índice: [`../10-testing-strategy.md`](../10-testing-strategy.md)._

---

## 44. Teste de timers

O timer da tela de dosagem atualmente executa aproximadamente a cada:

```text
300 ms
```

Ao sair da tela, ele deve ser eliminado quando apropriado.

É necessário verificar que:

```text
Dosing
 ↓
Completed
```

não deixa um timer antigo atualizando objetos que já não existem.

---

## 45. Teste de navegação repetida

Executar repetidamente:

```text
Home
 ↓
Mode
 ↓
Config
 ↓
Home
 ↓
Mode
 ↓
Config
 ↓
...
```

O objetivo é verificar se a criação e troca das telas continuam estáveis.

Esse teste é particularmente importante porque as telas são criadas dinamicamente.

---

## 46. Teste de memória

Como o projeto (ainda com código em C, em migração para C++) opera com alocação manual, gerenciamento de memória deve ser observado.

Devem ser investigados:

* alocações com `lv_malloc`;
* objetos LVGL;
* timers;
* contextos de eventos;
* telas antigas;
* callbacks.

O objetivo é evitar:

```text
memory leak
use-after-free
double free
```

---

## 47. Contexto atual de configuração

A tela de configuração atualmente utiliza um contexto alocado dinamicamente.

A implementação funciona, mas existe uma preocupação de longo prazo com o ciclo de vida desse contexto.

Portanto, mudanças futuras nessa tela devem verificar:

```text
quem aloca?
quem utiliza?
quem libera?
em que momento?
```

Isso deve ser tratado antes que o número de telas e contextos aumente significativamente.

---

## 48. Testes de UI

Os testes de UI devem verificar comportamento, não apenas aparência.

Exemplos:

```text
botão Iniciar funciona
botão Voltar funciona
botão + altera valor
botão - respeita limite
Continuar inicia dosagem
Cancelar interrompe dosagem
Nova dosagem volta para seleção
```

A aparência visual pode ser refinada separadamente.

---

## 49. Testes do domínio

O `DosingController` deve eventualmente ser testável sem depender de uma janela gráfica.

Os principais casos são:

```text
start
update
cancel
completion
timeout
sensor failure
overshoot
invalid configuration
```

Isso reforça a importância da separação entre:

```text
UI
```

e:

```text
Domain
```

---

## 50. Testes da abstração de hardware

`WeightSensor` deve ser testado em cenários como:

```text
reset
read
increase
no change
invalid reading
```

`Dispenser` deve ser testado em:

```text
start
stop
is_active
```

O objetivo é garantir que o controller não dependa de detalhes da implementação concreta.