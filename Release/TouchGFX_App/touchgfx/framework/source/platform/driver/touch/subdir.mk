################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX_App/touchgfx/framework/source/platform/driver/touch/SDL2TouchController.cpp 

OBJS += \
./TouchGFX_App/touchgfx/framework/source/platform/driver/touch/SDL2TouchController.o 

CPP_DEPS += \
./TouchGFX_App/touchgfx/framework/source/platform/driver/touch/SDL2TouchController.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX_App/touchgfx/framework/source/platform/driver/touch/%.o TouchGFX_App/touchgfx/framework/source/platform/driver/touch/%.su TouchGFX_App/touchgfx/framework/source/platform/driver/touch/%.cyclo: ../TouchGFX_App/touchgfx/framework/source/platform/driver/touch/%.cpp TouchGFX_App/touchgfx/framework/source/platform/driver/touch/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM_VERSION -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../TouchGFX_App/config -I../TouchGFX_App/generated -I../TouchGFX_App/gui -I../TouchGFX_App/generated/fonts/include -I../TouchGFX_App/gui/include -I../TouchGFX_App/generated/gui_generated/include -I../TouchGFX_App/generated/images/include -I../TouchGFX_App/generated/texts/include -I../TouchGfx_App/touchgfx/framework/include -I../TouchGfx_App/touchgfx/3rdparty/libjpeg/include -I../TouchGFX_App/touchgfx/framework/include -I../BSP/Inc -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TouchGFX_App-2f-touchgfx-2f-framework-2f-source-2f-platform-2f-driver-2f-touch

clean-TouchGFX_App-2f-touchgfx-2f-framework-2f-source-2f-platform-2f-driver-2f-touch:
	-$(RM) ./TouchGFX_App/touchgfx/framework/source/platform/driver/touch/SDL2TouchController.cyclo ./TouchGFX_App/touchgfx/framework/source/platform/driver/touch/SDL2TouchController.d ./TouchGFX_App/touchgfx/framework/source/platform/driver/touch/SDL2TouchController.o ./TouchGFX_App/touchgfx/framework/source/platform/driver/touch/SDL2TouchController.su

.PHONY: clean-TouchGFX_App-2f-touchgfx-2f-framework-2f-source-2f-platform-2f-driver-2f-touch

