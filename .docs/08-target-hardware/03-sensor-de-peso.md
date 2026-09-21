## 03. Sensor de peso

[voltar ao índice](../08-target-hardware.md)

---

### 10. HX711

O HX711 será utilizado como interface para a célula de carga.

A cadeia será:

```text
Load Cell
    ↓
HX711
    ↓
ESP32-S3
    ↓
WeightSensor
    ↓
DosingController
```

O HX711 é responsável pela aquisição do sinal proveniente da célula de carga.

---

### 11. Load Cell

A plataforma de pesagem será apoiada em **4 células de carga de 5 kg**, uma em cada ponto de apoio (RP05, conforme Trabalho.md).

```text
ração
  ↓
recipiente / plataforma
  ↓
4 Load Cells de 5 kg (apoios)
  ↓
sinal elétrico
  ↓
HX711
```

As 4 células permitem distribuir a carga nos 4 pontos de apoio e obter o peso do produto efetivamente recebido.

A montagem mecânica das células será importante para que o peso seja transferido corretamente.

---

### 12. Função da Load Cell

A célula de carga não deve ser vista apenas como um componente eletrônico.

A montagem física influencia diretamente a leitura.

É necessário considerar:

* apoio;
* distribuição da carga;
* deformação;
* fixação;
* estabilidade;
* contato com a estrutura;
* peso do recipiente;
* vibração.

Por isso, a integração do sensor será tanto eletrônica quanto mecânica.

---

### 13. Peso do recipiente

O sistema precisará lidar com o peso da estrutura ou recipiente que fica sobre a célula.

Conceitualmente:

```text
peso total medido
=
peso do recipiente
+
peso da ração
```

Para obter:

```text
peso da ração
```

será necessário realizar uma tara/referência apropriada.

No simulador, isso é simplificado através do `reset()` do sensor.

No hardware real, essa operação deverá ser implementada como tara/calibração.

---

### 14. Calibração

A célula de carga não fornece diretamente:

```text
100 g
```

Ela fornece uma leitura elétrica que precisa ser convertida.

O processo será aproximadamente:

```text
leitura HX711
      ↓
valor bruto
      ↓
calibração
      ↓
peso
      ↓
gramas
```

A calibração deverá ser realizada experimentalmente com pesos conhecidos.

---

### 15. Precisão necessária

A precisão final necessária depende do objetivo do projeto.

Inicialmente, o sistema deve priorizar:

```text
leitura consistente
```

antes de buscar:

```text
precisão extrema
```

Será necessário avaliar fisicamente:

* erro médio;
* repetibilidade;
* estabilidade;
* tempo de estabilização;
* variação durante a queda da ração;
* overshoot.

Esses resultados poderão determinar ajustes no software e na mecânica.

---

### 16. Filtragem do peso

As leituras da célula podem apresentar pequenas oscilações.

Por isso, a implementação real poderá utilizar filtragem.

Conceitualmente:

```text
HX711
  ↓
leituras brutas
  ↓
filtro
  ↓
peso estável
  ↓
WeightSensor
```

O algoritmo específico ainda não está definido.

A escolha deverá ser baseada em testes reais.

---

[voltar ao índice](../08-target-hardware.md)