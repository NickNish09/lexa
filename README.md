# Analisador Léxico

Para compilar o programa siga os seguintes passos:

- Rode o comando do make file:

`make`

- Rode o programa program.out gerado passando como argumento um dos arquivos de teste (.cstar):

`./program.out tests/certo1.cstar`

- Um arquivo .tac será gerado em /tests/tac/nome_do_arquivo.tac. Utilize ele para rodar no TAC.

Testes Certos:
tests/certo1.cstar (operações aritméticas e chamada de função)
tests/certo2.cstar (operações em tuplas e print tuplas)
tests/certo3.cstar (ordem de operações)
tests/certo4.cstar (condições)
tests/certo5.cstar (parenteses)
tests/certo6.cstar (laços)

Testes Errados:
tests/errado1.cstar
tests/errado2.cstar
tests/errado3.cstar
tests/errado4.cstar
tests/errado5.cstar

Os arquivos de teste que tem um \_error são os que possuem erros, conforme descritos no relatório. Há mais testes para mais casos dentro das pastas /tests/lexicos, /tests/semanticos e /tests/sintaticos, porém rode os testes da pasta /tests primeiro pois alguns desses outros testes podem estar desatualizados.

## Especificações

Para executar estes passos é preciso ter instalado em sua máquina o flex versão 2.5.35, o bison versão 3.7 e um sistema baseado em unix
como MacOS ou Ubuntu
