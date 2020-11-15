load *libpng* and *png++* and place it near the main.cpp.

# libpng
```bash
cd libpng
cp scripts/makefile.msys .
make -fmakefile.msys

# optional
cp png.h pngconf.h /usr/local/include
cp libpng.a libpng.dll.a /usr/local/lib
cp libpng14.dll pngtest-stat.exe pngtest.exe /usr/local/bin
```

# png++
replace `error.hpp:108` with `return std::string("");`

# Использование

Передать первым аргументом PNG файл (или перетащить на файл программы).
