################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Gfx/Fonts/RobotoMono_Bold_6x14.c \
../Gfx/Fonts/Roboto_Regular_8x13.c \
../Gfx/Fonts/fonts.c 

C_DEPS += \
./Gfx/Fonts/RobotoMono_Bold_6x14.d \
./Gfx/Fonts/Roboto_Regular_8x13.d \
./Gfx/Fonts/fonts.d 

OBJS += \
./Gfx/Fonts/RobotoMono_Bold_6x14.o \
./Gfx/Fonts/Roboto_Regular_8x13.o \
./Gfx/Fonts/fonts.o 


# Each subdirectory must supply rules for building sources it contributes
Gfx/Fonts/%.o Gfx/Fonts/%.su Gfx/Fonts/%.cyclo: ../Gfx/Fonts/%.c Gfx/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM32_THREAD_SAFE_STRATEGY=4 -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../Gfx -I../Gfx/Fonts -I../Core/ThreadSafe -I../BSP/Inc -I../Drivers/BSP -I../Middlewares/Third_Party/LittleFs -I../App/Inc -I../Test/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Gfx-2f-Fonts

clean-Gfx-2f-Fonts:
	-$(RM) ./Gfx/Fonts/RobotoMono_Bold_6x14.cyclo ./Gfx/Fonts/RobotoMono_Bold_6x14.d ./Gfx/Fonts/RobotoMono_Bold_6x14.o ./Gfx/Fonts/RobotoMono_Bold_6x14.su ./Gfx/Fonts/Roboto_Regular_8x13.cyclo ./Gfx/Fonts/Roboto_Regular_8x13.d ./Gfx/Fonts/Roboto_Regular_8x13.o ./Gfx/Fonts/Roboto_Regular_8x13.su ./Gfx/Fonts/fonts.cyclo ./Gfx/Fonts/fonts.d ./Gfx/Fonts/fonts.o ./Gfx/Fonts/fonts.su

.PHONY: clean-Gfx-2f-Fonts

