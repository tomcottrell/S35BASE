################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LittleFs/lfs.c \
../Middlewares/Third_Party/LittleFs/lfs_util.c 

C_DEPS += \
./Middlewares/Third_Party/LittleFs/lfs.d \
./Middlewares/Third_Party/LittleFs/lfs_util.d 

OBJS += \
./Middlewares/Third_Party/LittleFs/lfs.o \
./Middlewares/Third_Party/LittleFs/lfs_util.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LittleFs/%.o Middlewares/Third_Party/LittleFs/%.su Middlewares/Third_Party/LittleFs/%.cyclo: ../Middlewares/Third_Party/LittleFs/%.c Middlewares/Third_Party/LittleFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM_VERSION -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP -I../BSP/Inc -I../Middlewares/Third_Party/LittleFs -I../App/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../TouchGFX_App/config -I../TouchGFX_App/generated -I../TouchGFX_App/gui -I../TouchGFX_App/generated/fonts/include -I../TouchGFX_App/gui/include -I../TouchGFX_App/generated/gui_generated/include -I../TouchGFX_App/generated/images/include -I../TouchGFX_App/generated/texts/include -I../TouchGfx_App/touchgfx/framework/include -I../TouchGfx_App/touchgfx/3rdparty/libjpeg/include -I../TouchGFX_App/touchgfx/framework/include -I../TouchGFX/target/generated -I../TouchGFX/target -I../Middlewares/Third_Party/FatFs/src -I../Test/Inc -I../TouchGFX/App -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-LittleFs

clean-Middlewares-2f-Third_Party-2f-LittleFs:
	-$(RM) ./Middlewares/Third_Party/LittleFs/lfs.cyclo ./Middlewares/Third_Party/LittleFs/lfs.d ./Middlewares/Third_Party/LittleFs/lfs.o ./Middlewares/Third_Party/LittleFs/lfs.su ./Middlewares/Third_Party/LittleFs/lfs_util.cyclo ./Middlewares/Third_Party/LittleFs/lfs_util.d ./Middlewares/Third_Party/LittleFs/lfs_util.o ./Middlewares/Third_Party/LittleFs/lfs_util.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-LittleFs

