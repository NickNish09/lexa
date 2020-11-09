# Analisador Léxico
Para compilar o programa siga os seguintes passos:
- Rode o comando do make file:

```make```

- Rode o programa program.out gerado passando como argumento um dos arquivos de teste (.cstar):

```./program.out tests/sintaticos/teste_sintatico.cstar```

Testes Certos:
teste_sintatico.cstar
teste_sintatico_aritm.cstar
teste_sintatico2.cstar
teste_sintatico_condicoes.cstar
teste_sintatico_laco.cstar
teste_sintatico_tuple.cstar
dummy_test.cstar

Testes Errados:
teste_sintatico_error.cstar
teste_sintatico_error2.cstar
teste_sintatico_error3.cstar
todos os testes da pasta semanticos/

Os arquivos de teste que tem um _error são os que possuem erros, conforme descritos no relatório.

## Especificações
Para executar estes passos é preciso ter instalado em sua máquina o flex versão 2.5.35, o bison versão 3.7 e um sistema baseado em unix
como MacOS ou Ubuntu