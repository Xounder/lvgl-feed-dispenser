## 06. Alimentação

[voltar ao índice](../08-target-hardware.md)

---

### 26. Alimentação

O projeto prevê uma alimentação de:

```text
5 V
```

A distribuição da alimentação precisa considerar que diferentes componentes podem exigir características diferentes.

Conceitualmente:

```text
Fonte
  │
  ├── ESP32-S3 / placa
  ├── Display
  ├── Servo
  └── demais componentes
```

As tensões e correntes exatas deverão ser verificadas de acordo com as placas e módulos efetivamente adquiridos.

---

### 27. Atenção ao servo

O servo pode gerar picos de corrente, especialmente durante movimento ou quando encontra resistência mecânica.

Por isso, não se deve assumir que:

```text
"qualquer fonte USB de 5 V"
```

será automaticamente adequada para todo o sistema.

A alimentação real deve ser dimensionada considerando:

* consumo do ESP32-S3;
* display;
* touch;
* servo;
* periféricos;
* picos de corrente;
* estabilidade da tensão.

---

### 28. Separação de alimentação

Dependendo dos testes, pode ser necessário organizar a alimentação de forma que o servo não cause perturbações no ESP32-S3.

Conceitualmente:

```text
             Fonte
               │
        ┌──────┴──────┐
        │             │
        ▼             ▼
     ESP32-S3       Servo
        │
        ▼
      lógica
```

Os componentes devem compartilhar uma referência elétrica adequada, especialmente quando houver sinais de controle entre eles.

A topologia final deverá ser definida após conhecer as placas e fontes concretas.

---

### 29. MB102

O projeto considera o uso de um módulo:

```text
MB102
```

para facilitar a distribuição e ajuste da alimentação durante a prototipagem.

Ele pode ser utilizado como ferramenta de bancada/protoboard.

Entretanto, ele não deve ser automaticamente considerado parte da arquitetura elétrica final.

A solução definitiva de alimentação poderá ser diferente.

---

### 30. Protoboard

A prototipagem inicial pode utilizar:

```text
protoboard
```

para facilitar:

* testes;
* alterações de conexão;
* experimentação;
* medição;
* validação do circuito.

Durante a montagem final, a organização elétrica poderá ser modificada para aumentar:

* confiabilidade;
* segurança;
* estabilidade;
* resistência mecânica.

---

[voltar ao índice](../08-target-hardware.md)