# Sync

## Descrição
Simulação de uma estação de metrô com threads. Um vagão chega com `count`
assentos livres e passageiros (threads) esperam por ele. O vagão só parte quando
todos os passageiros que pegam assento confirmaram o embarque e o vagão
está cheio ou não há mais ninguém esperando. A solução usa um único mutex e
duas variáveis de condição, sem semáforos e sem busy-wait.

## Ambiente
- Sistema operacional: Linux (Omarchy)
- GCC: 16.2.1
- GNU Make: 4.4.1

## Arquivos
- `sync_car.c`: implementação da estação (init, load_car, wait_for_car, on_board).
- `sync_car.h`: definição de `struct station` e protótipos.
- `sync_car-runner.c`: testador fornecido pelo professor (não modificado); contém o `main`.
- `Makefile`: compilação e limpeza.

## Como compilar
    make

## Como executar
    ./sync_car

## Como limpar
    make clean

## O que esperar
A saída termina com `Looks good!`e o progama retorna 0.

## Como testar 
Primeiramente, execute make clean. Em seguida, compile o código utilizando make e, por fim, execute o programa com ./sync_car. A saída esperada deve exibir a mensagem Looks good!, sem apresentar nenhum erro.

## Decisões de projeto
- Um único mutex protege todos os contadores (`waiting`, `capacity`,
  `seats_taken`, `boarded`).
- Duas variáveis de condição: `car_arrived` (passageiros esperam vagão/assento)
  e `passenger_boarded` (vagão espera as confirmações de embarque).
- `pthread_cond_broadcast` na chegada do vagão, para permitir embarque simultâneo.
- `pthread_cond_signal` em `station_on_board`, pois só o vagão espera nessa condição.
- `struct station` fica em `sync_car.h`, porque o runner declara
  `struct station station;` na pilha e precisa da definição completa.
