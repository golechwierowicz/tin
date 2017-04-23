# System czujników CZUJNIX

## testowanie

### Clion

Jeśli korzystasz z Clion aby uruchomić zestaw testów wybierz `run>run` a następnie konfigurację np. `core_test`.

### Shell

**TODO**: Skonfigurować cmake aby wszystkie wyniki działania znalazły się w folderze `build/`.

**UWAGA!** Cmake nie jest skonfigurowany aby zrobić *out of source build*. Dlatego budowanie w sposób zaprezentowany
poniżej nie jest wskazane, jako że zaśmieca projekt.
```
$ cmake .
$ make
$ ./src/core/core_test
```