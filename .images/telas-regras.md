# Estados do Sistema

## Aguardando
- Selecione o modo: Massa ou Valor
- Defina a quantidade desejada
- Liberação manual disponível

## Dosando
- Dosagem automática em andamento
- Exibe a massa atual e o progresso
- Liberação manual desabilitada
- Use "INTERROMPER DOSAGEM" para interromper

## Concluído
- Dosagem concluída com sucesso
- Liberação manual disponível
- Pronto para nova dosagem

## Interrompido
- Dosagem interrompida
- Mostra a massa parcial dosada
- Liberação manual disponível
- Pronto para nova dosagem

# Comportamento do sistema
- A liberação manual só está disponível nos estados: 'Aguardando', 'Concluido' e 'Interrompido'
- Durante o estado 'Dosando', o comando manual é desabilitado para garantir o controle automático pelo ESP32