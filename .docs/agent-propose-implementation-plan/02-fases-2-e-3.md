## 02. Fases 2 e 3 - interface completa e HX711 simulado

[voltar ao índice](../agent-propose-implementation-plan.md)

---

# Fase 2 — Criar a interface completa

Eu criaria primeiro todas as telas.

Por exemplo:

```
```

```
TELA 1
INÍCIO
   │
   ├── Dosagem por massa
   │
   └── Dosagem por valor


TELA 2
CONFIGURAR MASSA
   │
   ├── 100 g
   ├── 250 g
   ├── 500 g
   └── Personalizado


TELA 3
DOSANDO
   │
   ├── Peso atual: 327 g
   ├── Meta: 500 g
   ├── Barra de progresso
   └── Cancelar


TELA 4
CONCLUÍDO
   │
   ├── Peso final: 500 g
   └── Nova dosagem


TELA 5
ERRO
   │
   ├── Erro na balança
   ├── Peso não detectado
   └── Cancelar
```

Tudo isso você pode testar agora.

---

# Fase 3 — Simular o HX711

Aqui está uma das partes mais importantes.

Em vez de seu código chamar diretamente o HX711:

```
```

```
peso = hx711.get_units();
```

Você cria uma interface para o sensor.

Por exemplo:

```
```

```
class IWeightSensor {
public:
    virtual float getWeight() = 0;
};
```

Depois criamos duas versões.

## Versão real

Quando você tiver o hardware:

```
```

```
class HX711WeightSensor : public IWeightSensor {
public:

    float getWeight() override {
        return hx711.get_units();
    }
};
```

---

## Versão simulada

Agora, sem hardware:

```
```

```
class SimulatedWeightSensor : public IWeightSensor {
private:
    float weight = 0;

public:

    float getWeight() override {
        return weight;
    }

    void addWeight(float value) {
        weight += value;
    }
};
```

Então você pode fazer algo assim:

```
```

```
Servo ligado
      ↓
Simulação adiciona peso
      ↓
0 g
10 g
25 g
47 g
80 g
120 g
...
```

Até chegar:

```
```

```
500 g
```

E então:

```
```

```
Servo desligado
      ↓
Dosagem concluída
```

---
