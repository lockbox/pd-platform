# Defining Types
These are the flags needed to load `pd_api.h` into binary ninja.
```
-mthumb
-mcpu=cortex-m7
-mfloat-abi=hard
-mfpu=fpv5-sp-d16
-D__FPU_USED=1
-DTARGET_EXTENSION=1
-DTARGET_PLAYDATE=1
-fomit-frame-pointer
-falign-functions=16
-isystem/usr/lib/gcc/x86_64-linux-gnu/10/include
-isystem/usr/local/include
-isystem/usr/lib/llvm-11/lib/clang/11.0.1/include
-isystem/usr/include/x86_64-linux-gnu
-isystem/usr/include
-isystem/usr/lib/gcc/arm-non-eabi/8.3.1/include
-isystem/usr/lib/gcc/arm-non-eabi/8.3.1/include-fixed
-isystem/usr/lib/gcc/arm-non-eabi/8.3.1/../../../arm-none-eabi/include
-I/home/lockbox/d/o/play.date/sdk-1.12.3/C_API
```

`PlaydateSimulatorFullTypes.h` is a single header file that contains all of the
debug information from `PlaydateSimulator.debug` that is distributed with the
sdk.