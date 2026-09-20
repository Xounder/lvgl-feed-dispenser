## 04. Sensor de peso, dispenser e física

[voltar ao índice](../11-migration-pc-to-esp32.md)

---

### 16. WeightSensor

A abstração:

```c
WeightSensor
```

foi criada justamente para permitir essa migração.

No PC:

```text
WeightSensor
      ↓
simulated_weight_sensor
```

No ESP32:

```text
WeightSensor
      ↓
real_weight_sensor
      ↓
HX711
      ↓
load cell
```

A intenção é que o `DosingController` continue trabalhando com a capacidade de obter o peso, e não com os detalhes do HX711.

A interface, responsabilidades e implementações do `WeightSensor` são detalhadas em [05-hardware-abstraction.md](../05-hardware-abstraction.md).

---

### 17. O que muda no sensor

A implementação simulada atualmente possui comportamento simples:

```text
read_grams()
add_grams()
reset()
```

O hardware real provavelmente precisará de operações adicionais ou de uma camada de calibração.

Por exemplo:

```text
inicialização
tara
leitura bruta
conversão
filtragem
validação
peso em gramas
```

Isso não significa que todas essas responsabilidades devam entrar diretamente no controller.

A cadeia `HX711 → leitura bruta → calibração/filtragem → gramas → WeightSensor` é própria da abstração de hardware — ver [05-hardware-abstraction.md](../05-hardware-abstraction.md) e [08-target-hardware.md](../08-target-hardware.md).

---

### 18. HX711

No hardware real:

```text
Load Cell
    ↓
HX711
    ↓
ESP32-S3
```

O HX711 fará a interface com a célula de carga.

O objetivo da camada de hardware será transformar a leitura física em algo que a aplicação possa consumir como:

```text
peso em gramas
```

Detalhes do HX711 e da montagem da load cell: [08-target-hardware.md](../08-target-hardware.md).

---

### 19. Calibração

A simulação não precisa de calibração.

O hardware real precisará.

O processo provavelmente envolverá:

```text
1. Tara
2. Leitura sem carga
3. Colocação de peso conhecido
4. Ajuste do fator de escala
5. Verificação
```

A calibração pertence à implementação do sensor/hardware, não à regra de negócio da dosagem.

O processo de calibração físico é detalhado em [08-target-hardware.md](../08-target-hardware.md).

---

### 20. Ruído do sensor

No simulador:

```text
peso = 100
```

pode ser exatamente:

```text
100
100
100
```

No hardware real, poderá ocorrer:

```text
99
100
101
100
99
```

Por isso, a implementação real do `WeightSensor` provavelmente precisará lidar com:

* ruído;
* filtragem;
* estabilidade;
* média;
* tara;
* leituras inválidas.

O controller deve receber uma informação suficientemente confiável para tomar decisões.

A representação de ruído na simulação é tratada em [06-simulation-strategy.md](../06-simulation-strategy.md); a filtragem física em [08-target-hardware.md](../08-target-hardware.md).

---

### 21. Dispenser

A abstração:

```c
Dispenser
```

também foi criada para permitir a substituição.

No PC:

```text
Dispenser
    ↓
simulated_dispenser
```

No hardware:

```text
Dispenser
    ↓
real_dispenser
    ↓
SG90 / mecanismo
```

A interface, responsabilidades e implementações do `Dispenser` são detalhadas em [05-hardware-abstraction.md](../05-hardware-abstraction.md).

---

### 22. SG90

O SG90 é um componente físico de atuação.

A aplicação não deveria precisar conhecer detalhes como:

```text
GPIO 17
ângulo 45°
PWM 50 Hz
pulso X µs
```

Esses detalhes pertencem à implementação física.

O domínio deve pensar em algo conceitualmente mais próximo de:

```text
dispenser.start()
dispenser.stop()
```

O detalhamento do servo está em [05-hardware-abstraction.md](../05-hardware-abstraction.md) e [08-target-hardware.md](../08-target-hardware.md).

---

### 23. Diferença entre servo e dispenser

É importante não confundir:

```text
SG90
```

com:

```text
Dispenser
```

O SG90 é o mecanismo físico.

`Dispenser` é uma capacidade da aplicação.

Pode ser que futuramente o mecanismo seja alterado para:

* outro servo;
* motor DC;
* motor de passo;
* solenóide;
* outro mecanismo.

A aplicação não deveria precisar ser completamente reescrita por causa disso.

A distinção servo versus dispenser e o possível mecanismo são detalhados em [08-target-hardware.md](../08-target-hardware.md).

---

### 24. Overshoot físico

Essa é uma das maiores diferenças entre simulação e hardware.

No simulador:

```text
98
100
→ stop
```

No hardware:

```text
98
100
101
103
```

pode acontecer porque existe ração em movimento.

A estratégia de parada deverá considerar:

```text
vazão
latência
inércia
quantidade de ração
mecânica
tempo de resposta
```

Essa é uma responsabilidade que será descoberta e ajustada durante os testes físicos.

A inércia e o overshoot no hardware são detalhados em [08-target-hardware.md](../08-target-hardware.md); a simulação do overshoot em [06-simulation-strategy.md](../06-simulation-strategy.md).

---

### 25. Como a simulação deverá evoluir

A implementação simulada não precisa ser descartada quando o hardware chegar.

Ela pode continuar sendo utilizada para testar situações que o hardware não é conveniente reproduzir, e a evolução da simulação (níveis, falhas, ruído, overshoot e cenários de regressão) é canônica em [06-simulation-strategy.md](../06-simulation-strategy.md).

Conceitualmente, a simulação pode evoluir para algo como:

```text
SimulatedWeightSensor
        │
        ├── normal
        ├── stopped
        ├── noisy
        └── overshoot
```

Isso pode tornar a simulação uma ferramenta de regressão.