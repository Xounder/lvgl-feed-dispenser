## 06. Explicacao - comparacao direta e abstracoes seguidas

[voltar ao índice](../agent-propose-implementation-plan.md)

---

"""

Dado o que foi comentado lá no começo do nosso chat (o que está entre """""") seguimos as abordagens que vc tinha mencionado anteriormente? Se não pq não seguimos e foi a melhor escolha não seguir? Ou se seguimos, me diga como


## Agent Explanation after atual implementation in `src`

Sim. Pelo estado atual do projeto, **seguimos praticamente toda a arquitetura recomendada naquele texto — e, em alguns pontos, evoluímos para uma versão mais adequada ao C/LVGL do que a proposta original em C++**.

A principal diferença é que o texto inicial era uma proposta de arquitetura. Depois, durante a implementação, fomos adaptando-a ao stack real que você escolheu: **C + LVGL + SDL2 + CMake**, em vez de C++/PlatformIO.

### Comparação direta

| Proposta inicial                        | O que fizemos                                   | Situação   |
| --------------------------------------- | ----------------------------------------------- | ---------- |
| Desenvolver primeiro no PC              | LVGL rodando em Windows com SDL2                | ✅          |
| Tela simulada 800×480                   | `sdl_hal_init(800, 480)`                        | ✅          |
| Separar UI da lógica                    | `ui/` + `domain/`                               | ✅          |
| Máquina de estados                      | `DosingState` + fluxo de dosagem                | 🟡 Parcial |
| `IWeightSensor` / abstração do peso     | `WeightSensor` com function pointers            | ✅          |
| Sensor simulado                         | `simulated_weight_sensor`                       | ✅          |
| Sensor real futuro                      | Interface preparada para implementação futura   | ✅          |
| `IDispenser` / abstração do dispensador | `Dispenser` com function pointers               | ✅          |
| Servo simulado                          | `simulated_dispenser`                           | ✅          |
| Servo real futuro                       | Interface preparada                             | ✅          |
| Peso aumentando enquanto dispensa       | `add_grams(2)` a cada atualização               | ✅          |
| Parar ao atingir meta                   | `current_weight >= target_grams`                | ✅          |
| Cancelamento seguro                     | `stop()` + retorno ao estado inicial            | ✅          |
| Tela de erro                            | Planejada                                       | ⏳          |
| Falha de sensor/servo                   | Planejada                                       | ⏳          |
| Simulação de erros                      | Documentada como próxima evolução               | ⏳          |
| Testes automatizados                    | Estratégia documentada, ainda não implementados | ⏳          |
| Wokwi                                   | Não usamos ainda                                | ⏳          |
| ESP32-S3 real                           | Ainda não                                       | ⏳          |
| HX711 real                              | Ainda não                                       | ⏳          |
| Servo real                              | Ainda não                                       | ⏳          |

Então a resposta curta é:

> **Sim, a estratégia central foi seguida. O que mudou foi a implementação concreta e a ordem de algumas etapas.**

---

# 1. A maior decisão que tomamos: C em vez de C++

No texto original aparecia algo como:

```cpp
class IWeightSensor {
public:
    virtual float getWeight() = 0;
};
```

E depois:

```cpp
class SimulatedWeightSensor : public IWeightSensor
```

Nós **não seguimos isso literalmente**.

O projeto acabou sendo feito em **C**, utilizando:

```c
typedef struct {
    int (*read_grams)(void);
    void (*add_grams)(int grams);
    void (*reset)(void);
} WeightSensor;
```

Isso foi uma boa adaptação porque o destino final é um **ESP32-S3 + LVGL**, e C é perfeitamente adequado para esse tipo de firmware.

Em vez de:

```text
interface C++
      ↓
implementação
```

ficamos com:

```text
struct + function pointers
        ↓
implementação
```

Conceitualmente é a mesma ideia:

```text
                 WeightSensor
                      │
              ┌───────┴───────┐
              ▼               ▼
       Simulated          HX711 futuro
```

E o mesmo para o dispensador:

```text
                  Dispenser
                     │
             ┌───────┴───────┐
             ▼               ▼
       Simulated         Servo futuro
```

**Não perdemos a abstração. Só mudamos a forma idiomática de implementá-la em C.**

---

# 2. A ideia de "simular primeiro" foi seguida exatamente

Esse talvez seja o ponto em que mais seguimos a proposta original.

No começo a ideia era:

```text
PC
 │
 ├── LVGL
 ├── sensor fake
 └── servo fake
```

E é exatamente o que temos hoje.

Atualmente:

```text
             PC / Windows
                  │
                  ▼
             LVGL + SDL2
                  │
                  ▼
          Screen Manager
                  │
                  ▼
         Dosing Controller
             │          │
             ▼          ▼
       WeightSensor   Dispenser
             │          │
             ▼          ▼
          SIMULADO    SIMULADO
```

O hardware físico simplesmente ainda não está nessa árvore.

---

# 3. O HX711 simulado virou algo ainda mais simples

No texto inicial a simulação era conceitualmente:

```text
Servo ligado
     ↓
peso aumenta
```

Nós implementamos isso diretamente no controller:

```c
if (simulated_dispenser.is_active()) {
    simulated_weight_sensor.add_grams(2);
}
```

Então:

```text
Dispenser ativo
       ↓
+2 g
       ↓
próxima atualização
       ↓
+2 g
       ↓
...
```

Com o timer de aproximadamente 300 ms:

```text
2 g / 300 ms
≈ 6,7 g/s
```

E quando:

```c
current_weight >= config->target_grams
```

faz:

```c
simulated_dispenser.stop();
state = DOSING_STATE_COMPLETED;
```

Ou seja, a ideia fundamental do texto foi implementada.

---
