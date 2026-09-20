## 10. Validação incremental

[voltar ao índice](../08-target-hardware.md)

---

### 44. Primeira integração física

A integração não precisa acontecer de uma vez.

Uma sequência recomendada é:

```text
1. ESP32-S3 funcionando
        ↓
2. Display
        ↓
3. Touch
        ↓
4. HX711
        ↓
5. Load Cell
        ↓
6. Calibração
        ↓
7. Servo
        ↓
8. Mecanismo
        ↓
9. Dosagem completa
```

Essa ordem facilita a identificação de problemas.

---

### 45. Validação do display

Primeiro deve ser possível confirmar:

```text
ESP32-S3
   ↓
display
   ↓
LVGL
   ↓
telas funcionando
```

Antes de introduzir a complexidade do sensor e do atuador.

---

### 46. Validação do sensor

Depois:

```text
ESP32-S3
   ↓
HX711
   ↓
Load Cell
```

deve ser validado isoladamente.

Testes iniciais:

* leitura sem carga;
* tara;
* peso conhecido;
* repetição;
* estabilidade;
* aumento gradual de peso.

---

### 47. Validação do servo

O servo deve ser testado independentemente.

Primeiro:

```text
posição fechada
```

depois:

```text
posição aberta
```

e então:

```text
abertura
↓
fechamento
↓
repetição
```

Antes de conectá-lo ao mecanismo completo.

---

### 48. Validação do mecanismo

Depois do servo funcionar:

```text
SG90
  ↓
mecanismo
```

deve ser testado com o reservatório.

Primeiro sem ração, depois com pequenas quantidades.

O objetivo inicial é verificar:

```text
abre
fecha
não trava
```

e somente depois:

```text
fluxo de ração
```

---

### 49. Primeira dosagem física

A primeira dosagem real deve ser tratada como um teste de integração.

Fluxo:

```text
tara
 ↓
objetivo pequeno
 ↓
abrir dispenser
 ↓
observar peso
 ↓
parar
 ↓
verificar peso final
```

Não se deve começar imediatamente com grandes quantidades.

---

### 50. Observação do overshoot

Durante os primeiros testes, deve ser registrado:

```text
peso no momento do comando de stop
```

e:

```text
peso final estabilizado
```

Por exemplo:

```text
objetivo = 100 g

stop enviado em:
98 g

peso estabilizado:
104 g
```

Esse comportamento ajudará a ajustar:

* mecanismo;
* fluxo;
* timing;
* estratégia de parada.

---

### 51. Relação com a simulação

Os dados físicos podem ser utilizados para melhorar a simulação.

Por exemplo, se os testes mostrarem:

```text
fluxo médio ≈ 8 g/s
overshoot médio ≈ 4 g
```

esses valores podem orientar um cenário do simulador.

Assim:

```text
SIMULAÇÃO
    ↓
hardware
    ↓
medições
    ↓
SIMULAÇÃO melhorada
```

O simulador e o hardware podem evoluir em conjunto.

---

[voltar ao índice](../08-target-hardware.md)