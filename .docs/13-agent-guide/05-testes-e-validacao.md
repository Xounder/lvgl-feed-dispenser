## 05. Testes e validação

[voltar ao índice](../13-agent-guide.md)

---

## 44. Teste antes de considerar concluído

Depois de uma alteração, pelo menos:

```text
1. compilar
2. executar
3. testar o fluxo afetado
```

Se a alteração envolver domínio:

```text
4. testar também estados relacionados
```

Se envolver hardware:

```text
5. verificar impacto na implementação simulada
6. verificar impacto esperado no hardware real
```

---

## 45. Regra de regressão

Uma mudança pequena pode quebrar uma parte distante do sistema.

Por isso, depois de alterações relevantes, testar o fluxo completo:

```text
Home
 ↓
Mode
 ↓
Config
 ↓
Dosing
 ↓
Completed
 ↓
Nova dosagem
```

E, quando aplicável:

```text
Cancelar
 ↓
Home
```

---

## 46. Testes de erro devem ser reproduzíveis

Quando o simulador evoluir para suportar falhas, preferir cenários determinísticos.

Exemplo:

```text
SIMULATION_MODE_NO_PROGRESS
```

pode permitir reproduzir:

```text
dispenser ativo
peso = 0
timeout
ERROR
```

Isso é melhor para testes do que depender de uma falha aleatória.

---

## 47. Não depender do hardware para testar tudo

Sempre que uma regra puder ser testada no PC:

```text
testar no PC primeiro.
```

Exemplos:

```text
✓ estado
✓ configuração
✓ timeout
✓ cancelamento
✓ conclusão
✓ sensor parado
✓ erro
```

O hardware deve ser necessário para validar principalmente aquilo que depende de:

```text
física
eletrônica
mecânica
ruído
tempo real
```

---

## 48. Hardware físico é fonte de informação

Quando o hardware chegar, não assumir que o simulador estava "errado".

O simulador representa uma hipótese controlada.

O hardware poderá revelar:

```text
atrasos
ruído
overshoot
instabilidade
limitações
```

Essas descobertas devem ser incorporadas à arquitetura.

---

## 49. Migração para ESP32-S3

Durante a migração, preservar sempre que possível:

```text
DosingConfig
DosingController
regras de negócio
interfaces
fluxo da aplicação
```

Substituir principalmente:

```text
SDL2
SDL HAL
hardware simulado (src/hardware/simulated/)
```

por implementações adequadas ao ESP32.

---

## 50. O que não deve ser levado para o firmware

Não levar para o ESP32:

```text
SDL2
Windows
MSVC
vcpkg específico do PC
DLLs
código exclusivo do simulador
```

Essas são dependências da plataforma desktop.

---

## 51. O que deve continuar existindo

A migração deve preservar conceitualmente:

```text
LVGL
UI
DosingConfig
DosingController
WeightSensor
Dispenser
state machine
fluxo de dosagem
```

As implementações concretas podem mudar.