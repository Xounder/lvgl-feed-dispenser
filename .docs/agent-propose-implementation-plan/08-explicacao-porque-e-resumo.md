## 08. Explicacao - por que nao 100 e resumo

[voltar ao índice](../agent-propose-implementation-plan.md)

---

# Então, por que não seguimos 100% o texto original?

Principalmente por **evolução incremental**.

O texto original descrevia o projeto ideal antes de termos implementado tudo.

Depois descobrimos na prática que:

### 1. C fazia mais sentido que C++

Então:

```cpp
interface
```

virou:

```c
struct + function pointers
```

### 2. Não precisávamos implementar tudo de uma vez

Em vez de começar com:

```text
ERROR
timeouts
falha HX711
falha servo
overshoot
calibração
```

começamos pelo caminho feliz:

```text
configurar
 ↓
dosar
 ↓
atingir meta
 ↓
parar
 ↓
concluir
```

Isso permitiu validar a arquitetura primeiro.

### 3. O simulador precisava ficar funcional rapidamente

Por isso a simulação atualmente é propositalmente simples:

```text
dispenser ativo
      ↓
+2 g
```

e não uma simulação física sofisticada.

A sofisticação pode vir depois.

### 4. Algumas abstrações podem ser fortalecidas quando houver necessidade real

Por exemplo, hoje:

```text
DosingController
      ↓
simulated_weight_sensor
```

Funciona.

Quando aparecer:

```text
simulated
real
test mock
```

a necessidade de injeção de dependência fica muito mais concreta.

Não havia necessidade de introduzir toda essa complexidade antes de termos o primeiro fluxo funcionando.

---

# E olhando para o projeto HOJE

Eu diria que estamos aproximadamente aqui:

```text
                 PROJETO
                    │
       ┌────────────┴────────────┐
       │                         │
   SOFTWARE                   HARDWARE
       │                         │
       ▼                         ▼
   ┌────────┐               ESP32-S3
   │  LVGL  │                   │
   │  SDL2  │               ainda futuro
   └────┬───┘
        │
        ▼
      UI ✓
        │
        ▼
   ScreenManager ✓
        │
        ▼
 DosingController ✓
        │
   ┌────┴─────┐
   ▼          ▼
WeightSensor Dispenser
   │          │
   ▼          ▼
 SIMULADO ✓  SIMULADO ✓
```

E o próximo nível é:

```text
SIMULAÇÃO
    │
    ├── caminho normal ✓
    ├── cancelamento ✓
    ├── conclusão ✓
    │
    └── robustez
         ├── ERROR
         ├── timeout
         ├── peso travado
         ├── sensor inválido
         ├── overshoot
         └── validação
```

Depois:

```text
                    ESP32-S3
                       │
              ┌────────┴────────┐
              ▼                 ▼
           HX711              Servo
              │                 │
        Load Cell          Mecanismo
```

---

## Em resumo

**Sim, seguimos a abordagem original.** E as principais decisões que permaneceram foram justamente as mais importantes:

```text
PC primeiro
   ↓
LVGL no PC
   ↓
UI independente
   ↓
domínio independente
   ↓
hardware abstraído
   ↓
hardware simulado
   ↓
validar comportamento
   ↓
só depois integrar ESP32-S3
   ↓
trocar simulações por hardware real
```

O que **não** seguimos literalmente foi principalmente a implementação em C++/classes e algumas funcionalidades avançadas que ficaram para depois. Isso aconteceu porque adotamos **C + LVGL + SDL2** e priorizamos uma evolução incremental.

E, olhando para o que já foi construído, **não vejo uma decisão arquitetural inicial importante que tenha sido abandonada**. Na verdade, o projeto atual é a materialização daquela proposta, só que mais concreta e documentada.

A única ressalva arquitetural que eu manteria no radar é esta:

```text
ATUAL:
DosingController
       ↓
simulated_*


FUTURO:
DosingController
       ↓
interfaces
       ↓
┌──────┴──────┐
simulado      real
```

Esse é provavelmente o próximo refinamento arquitetural relevante **quando começarmos a preparar a implementação real do ESP32-S3**.
