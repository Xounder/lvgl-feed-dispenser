## 06. Hardware

[voltar ao índice](../13-agent-guide.md)

---

## 52. Hardware futuro

O hardware alvo é aproximadamente:

```text
ESP32-S3 N16R8
        │
        ├── Display 4.3" 800×480
        ├── Touch capacitivo
        ├── HX711
        │     └── Load Cell
        ├── SG90
        ├── Botões físicos
        ├── Switch
        ├── LED/indicador
        └── alimentação
```

Detalhes como:

* pinagem;
* controlador do display;
* controlador do touch;
* mecanismo;
* alimentação definitiva;

ainda podem mudar.

Não inventar esses detalhes.

---

## 53. Ao trabalhar com hardware

Nunca assumir a pinagem sem confirmação.

Antes de escrever código específico para GPIO:

```text
verificar documentação do componente
verificar placa
verificar esquema
verificar datasheet
```

Se a informação não estiver disponível, registrar como pendência.

---

## 54. Segurança do atuador

Qualquer código que controle o dispenser físico deve considerar:

```text
estado inicial seguro
parada
timeout
erro
reinicialização
```

Evitar situações em que:

```text
ESP32 reinicia
 ↓
servo permanece acionado indefinidamente
```

O comportamento de boot deve ser analisado antes da integração física.

---

## 55. Alimentação

O servo pode gerar comportamento elétrico diferente do restante do sistema.

Não assumir que alimentar todos os componentes diretamente de uma única fonte ou regulador será adequado.

A arquitetura elétrica final deverá considerar:

```text
ESP32-S3
display
servo
HX711
```

e possíveis interferências.

Esse problema pertence à integração física e não deve ser mascarado por software.