# Tests

Este directorio contiene pruebas unitarias basadas en **Qt Test**.

## Compilaci\u00f3n y ejecuci\u00f3n

1. Desde el directorio raiz del proyecto ejecute:

```bash
qmake tests/tests.pro -o Makefile.tests
make -f Makefile.tests
```

2. Ejecute las pruebas resultantes:

```bash
./tests
```

Tambien puede usar su entorno de desarrollo para ejecutar el proyecto `tests/tests.pro`.
