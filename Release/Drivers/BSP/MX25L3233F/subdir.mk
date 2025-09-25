################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/MX25L3233F/STM32L4R7_mx25l3233f.c \
../Drivers/BSP/MX25L3233F/mx25l3233f.c 

C_DEPS += \
./Drivers/BSP/MX25L3233F/STM32L4R7_mx25l3233f.d \
./Drivers/BSP/MX25L3233F/mx25l3233f.d 

OBJS += \
./Drivers/BSP/MX25L3233F/STM32L4R7_mx25l3233f.o \
./Drivers/BSP/MX25L3233F/mx25l3233f.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/MX25L3233F/%.o Drivers/BSP/MX25L3233F/%.su Drivers/BSP/MX25L3233F/%.cyclo: ../Drivers/BSP/MX25L3233F/%.c Drivers/BSP/MX25L3233F/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM_VERSION -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP -I../BSP/Inc -I../Middlewares/Third_Party/LittleFs -I../App/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../TouchGFX_App/config -I../TouchGFX_App/generated -I../TouchGFX_App/gui -I../TouchGFX_App/generated/fonts/include -I../TouchGFX_App/gui/include -I../TouchGFX_App/generated/gui_generated/include -I../TouchGFX_App/generated/images/include -I../TouchGFX_App/generated/texts/include -I../TouchGfx_App/touchgfx/framework/include -I../TouchGfx_App/touchgfx/3rdparty/libjpeg/include -I../TouchGFX_App/touchgfx/framework/include -I../TouchGFX/target/generated -I../TouchGFX/target -I../Middlewares/Third_Party/FatFs/src -I../Test/Inc -I../TouchGFX/App -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-MX25L3233F

clean-Drivers-2f-BSP-2f-MX25L3233F:
	-$(RM) ./Drivers/BSP/MX25L3233F/STM32L4R7_mx25l3233f.cyclo ./Drivers/BSP/MX25L3233F/STM32L4R7_mx25l3233f.d ./Drivers/BSP/MX25L3233F/STM32L4R7_mx25l3233f.o ./Drivers/BSP/MX25L3233F/STM32L4R7_mx25l3233f.su ./Drivers/BSP/MX25L3233F/mx25l3233f.cyclo ./Drivers/BSP/MX25L3233F/mx25l3233f.d ./Drivers/BSP/MX25L3233F/mx25l3233f.o ./Drivers/BSP/MX25L3233F/mx25l3233f.su

.PHONY: clean-Drivers-2f-BSP-2f-MX25L3233F

