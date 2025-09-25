################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Test/Src/fatfs_test.c \
../Test/Src/fs_layer_test.c \
../Test/Src/mtest.c \
../Test/Src/mx25l3233f_test.c 

C_DEPS += \
./Test/Src/fatfs_test.d \
./Test/Src/fs_layer_test.d \
./Test/Src/mtest.d \
./Test/Src/mx25l3233f_test.d 

OBJS += \
./Test/Src/fatfs_test.o \
./Test/Src/fs_layer_test.o \
./Test/Src/mtest.o \
./Test/Src/mx25l3233f_test.o 


# Each subdirectory must supply rules for building sources it contributes
Test/Src/%.o Test/Src/%.su Test/Src/%.cyclo: ../Test/Src/%.c Test/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM32_THREAD_SAFE_STRATEGY=4 -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../Gfx -I../Gfx/Fonts -I../Core/ThreadSafe -I../BSP/Inc -I../Drivers/BSP -I../Middlewares/Third_Party/LittleFs -I../App/Inc -I../Test/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Test-2f-Src

clean-Test-2f-Src:
	-$(RM) ./Test/Src/fatfs_test.cyclo ./Test/Src/fatfs_test.d ./Test/Src/fatfs_test.o ./Test/Src/fatfs_test.su ./Test/Src/fs_layer_test.cyclo ./Test/Src/fs_layer_test.d ./Test/Src/fs_layer_test.o ./Test/Src/fs_layer_test.su ./Test/Src/mtest.cyclo ./Test/Src/mtest.d ./Test/Src/mtest.o ./Test/Src/mtest.su ./Test/Src/mx25l3233f_test.cyclo ./Test/Src/mx25l3233f_test.d ./Test/Src/mx25l3233f_test.o ./Test/Src/mx25l3233f_test.su

.PHONY: clean-Test-2f-Src

