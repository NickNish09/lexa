# Analisador Léxico
Para compilar o programa siga os seguintes passos:
- Rode o comando flex no arquivo lexAnalyser.l

```flex lexAnalyser.l```

- Compile com gcc o arquivo .yy.c gerado:

```gcc lex.yy.c -ll  ```

- Rode o programa a.out gerado passando como argumento um dos arquivos de teste (.cstar):

```./a.out teste.cstar```

Os arquivos de teste que tem um _error são os que possuem erros, conforme descritos no relatório.

Para executar estes passos é preciso ter instalado em sua máquina o flex.