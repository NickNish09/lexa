# Analisador Léxico
Para compilar o programa siga os seguintes passos:
- Rode o comando flex no arquivo lexAnalyser.l

```flex lexAnalyser.l```

- Compile com gcc o arquivo .yy.c gerado?

```gcc lex.yy.c -ll  ```

- Rode o programa a.out gerado:

```./a.out```

Para executar estes passos é preciso ter instalado em sua máquina o flex.