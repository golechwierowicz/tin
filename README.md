# System czujników CZUJNIX

## Wymagania

Boost oraz libconfig++

Na debianowych: apt-get install libconfig++-dev

## Ręczne budowanie

```
mkdir build
cd build
cmake ..
make
```

## Testowanie

### Clion

Jeśli korzystasz z Clion aby uruchomić zestaw testów wybierz `run>run` a następnie konfigurację np. `core_test`.

### Shell

Po zbudowaniu zgodnie z instrukcją:
```
$ ./build/src/core/core_test
```
