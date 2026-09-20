## 5. `WeightSensor`

_Voltar ao índice: [`../05-hardware-abstraction.md`](../05-hardware-abstraction.md)._

---

O `WeightSensor` representa a capacidade de obter informações sobre o peso da ração.

Interface atual:

```c
#ifndef WEIGHT_SENSOR_H
#define WEIGHT_SENSOR_H

typedef struct {
    int (*read_grams)(void);
    void (*add_grams)(int grams);
    void (*reset)(void);
} WeightSensor;

extern WeightSensor simulated_weight_sensor;

#endif
```

---

## 6. Responsabilidade do `WeightSensor`

A responsabilidade conceitual do sensor é:

```text
fornecer o peso atual
```

A função mais importante é:

```c
read_grams()
```

que representa:

```text
"Qual é o peso atual em gramas?"
```

---

## 7. `read_grams()`

A operação:

```c
int (*read_grams)(void);
```

deve retornar o peso atual em uma unidade compreensível pelo domínio.

Atualmente a unidade escolhida é:

```text
gramas (g)
```

Isso significa que o domínio não deve precisar interpretar diretamente:

* contagens ADC;
* valores brutos do HX711;
* fatores de calibração;
* tensão;
* unidades internas do driver.

A implementação do sensor deve fazer essa conversão.

---

## 8. `reset()`

A interface atual possui:

```c
void (*reset)(void);
```

No simulador, essa operação significa:

```text
peso = 0
```

Ela é utilizada quando uma nova dosagem começa (ver máquina de estados no [`../04-domain-and-state-machine.md`](../04-domain-and-state-machine.md)).

No hardware real, o conceito de `reset` provavelmente precisará evoluir para uma operação semanticamente mais adequada, como:

```text
tare()
```

ou:

```text
zero()
```

porque uma célula de carga real possui um peso físico e precisa estabelecer uma referência.

Essa alteração deve ser feita quando a integração real for implementada.

---

## 9. `add_grams()`

A interface atual possui:

```c
void (*add_grams)(int grams);
```

Essa função existe especificamente para facilitar a simulação.

Ela permite representar artificialmente:

```text
0 g
 ↓
2 g
 ↓
4 g
 ↓
6 g
```

Ela não representa uma operação que faça sentido em um sensor físico real.

Portanto:

> `add_grams()` é uma capacidade da implementação simulada, não uma obrigação conceitual de um `WeightSensor` físico.

À medida que a abstração amadurecer, essa função pode deixar de fazer parte da interface pública comum e ficar restrita à simulação.

---

## 10. Implementação simulada do sensor

Arquivo:

```text
src/hardware/simulated_weight_sensor.c
```

Implementação atual:

```c
static int simulated_weight = 0;
```

A leitura retorna esse valor:

```c
static int simulated_read_grams(void)
{
    return simulated_weight;
}
```

A simulação pode incrementar o valor:

```c
static void simulated_add_grams(int grams)
{
    simulated_weight += grams;
}
```

E reiniciar:

```c
static void simulated_reset(void)
{
    simulated_weight = 0;
}
```

---

## 11. Objetivo do sensor simulado

O sensor simulado não tenta reproduzir ainda a física da célula de carga.

Ele existe para permitir testar:

```text
controller
   ↓
leitura de peso
   ↓
comparação com objetivo
   ↓
conclusão
```

Isso permite validar a lógica antes da integração física.

---

## 12. Evolução da simulação do sensor

A simulação poderá evoluir posteriormente.

Atualmente:

```text
peso += 2 g
```

Uma simulação mais realista poderia considerar:

```text
tempo
+
taxa de alimentação
+
variação
+
overshoot
+
atraso
+
ruído
```

Por exemplo:

```text
dispenser ativo
       ↓
taxa de fluxo
       ↓
variação do peso
       ↓
leitura simulada
```

O objetivo é permitir testar comportamentos que seriam difíceis de reproduzir manualmente.

---

## 13. Futuro `WeightSensor` baseado em HX711

No hardware real, a cadeia será aproximadamente:

```text
Load Cell
    │
    ▼
  HX711
    │
    ▼
driver / implementação
    │
    ▼
WeightSensor
    │
    ▼
DosingController
```

A célula de carga produz um sinal elétrico relacionado ao peso.

O HX711 realiza a aquisição/amplificação desse sinal.

A implementação do sensor será responsável por transformar a leitura em uma representação adequada ao domínio.

---

## 14. Responsabilidades da implementação HX711

A implementação real poderá ser responsável por:

* inicializar o HX711;
* realizar leituras;
* obter valores brutos;
* aplicar tara;
* aplicar calibração;
* converter para gramas;
* filtrar leituras;
* detectar leituras inválidas;
* reportar falhas.

O domínio não deve precisar conhecer esses detalhes.

---

## 15. Fluxo do sensor real

Conceitualmente:

```text
HX711
  │
  ▼
raw reading
  │
  ▼
calibração
  │
  ▼
filtragem
  │
  ▼
peso em gramas
  │
  ▼
WeightSensor
  │
  ▼
DosingController
```

---

## 16. Calibração

A calibração do sensor será uma etapa importante da integração física.

A relação entre:

```text
leitura bruta
```

e:

```text
gramas
```

não deve ser assumida sem testes.

A implementação real poderá utilizar um fator de calibração determinado experimentalmente.

Esse fator pertence à implementação do sensor, e não à regra de negócio.

---

## 17. Tara

Uma célula de carga normalmente precisa estabelecer uma referência antes da dosagem.

A intenção é que o sistema consiga distinguir:

```text
peso da estrutura/recipiente
```

de:

```text
peso da ração
```

Portanto, o conceito atual de:

```text
reset()
```

provavelmente deverá evoluir para uma operação de tara no hardware real.

A decisão exata dependerá da montagem física.

---

## 18. Filtragem

Uma célula de carga real não necessariamente fornecerá uma leitura perfeitamente estável.

Podem existir:

* ruído;
* pequenas oscilações;
* vibração;
* movimento da ração;
* interferência mecânica;
* variação da leitura.

A implementação do sensor poderá aplicar filtragem antes de entregar o valor ao domínio.

Por exemplo:

```text
leituras brutas
   ↓
filtro
   ↓
peso estável
   ↓
controller
```

O filtro deve ser escolhido com base nos testes do hardware real.

Não deve ser introduzido um algoritmo complexo apenas por antecipação.