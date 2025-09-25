################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BSP/Src/button_handler.cpp \
../BSP/Src/touchgfx_input_controller.cpp 

C_SRCS += \
../BSP/Src/adc_helper.c \
../BSP/Src/dma2d_operations.c \
../BSP/Src/fs_layer.c \
../BSP/Src/usb_helper.c 

C_DEPS += \
./BSP/Src/adc_helper.d \
./BSP/Src/dma2d_operations.d \
./BSP/Src/fs_layer.d \
./BSP/Src/usb_helper.d 

OBJS += \
./BSP/Src/adc_helper.o \
./BSP/Src/button_handler.o \
./BSP/Src/dma2d_operations.o \
./BSP/Src/fs_layer.o \
./BSP/Src/touchgfx_input_controller.o \
./BSP/Src/usb_helper.o 

CPP_DEPS += \
./BSP/Src/button_handler.d \
./BSP/Src/touchgfx_input_controller.d 


# Each subdirectory must supply rules for building sources it contributes
BSP/Src/%.o BSP/Src/%.su BSP/Src/%.cyclo: ../BSP/Src/%.c BSP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM_VERSION -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP -I../BSP/Inc -I../Middlewares/Third_Party/LittleFs -I../App/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../TouchGFX_App/config -I../TouchGFX_App/generated -I../TouchGFX_App/gui -I../TouchGFX_App/generated/fonts/include -I../TouchGFX_App/gui/include -I../TouchGFX_App/generated/gui_generated/include -I../TouchGFX_App/generated/images/include -I../TouchGFX_App/generated/texts/include -I../TouchGfx_App/touchgfx/framework/include -I../TouchGfx_App/touchgfx/3rdparty/libjpeg/include -I../TouchGFX_App/touchgfx/framework/include -I../TouchGFX/target/generated -I../TouchGFX/target -I../Middlewares/Third_Party/FatFs/src -I../Test/Inc -I../TouchGFX/App -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
BSP/Src/%.o BSP/Src/%.su BSP/Src/%.cyclo: ../BSP/Src/%.cpp BSP/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM_VERSION -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../TouchGFX_App/config -I../TouchGFX_App/generated -I../TouchGFX_App/gui -I../TouchGFX_App/generated/fonts/include -I../TouchGFX_App/gui/include -I../TouchGFX_App/generated/gui_generated/include -I../TouchGFX_App/generated/images/include -I../TouchGFX_App/generated/texts/include -I../TouchGfx_App/touchgfx/framework/include -I../TouchGfx_App/touchgfx/3rdparty/libjpeg/include -I../TouchGFX_App/touchgfx/framework/include -I../BSP/Inc -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-BSP-2f-Src

clean-BSP-2f-Src:
	-$(RM) ./BSP/Src/adc_helper.cyclo ./BSP/Src/adc_helper.d ./BSP/Src/adc_helper.o ./BSP/Src/adc_helper.su ./BSP/Src/button_handler.cyclo ./BSP/Src/button_handler.d ./BSP/Src/button_handler.o ./BSP/Src/button_handler.su ./BSP/Src/dma2d_operations.cyclo ./BSP/Src/dma2d_operations.d ./BSP/Src/dma2d_operations.o ./BSP/Src/dma2d_operations.su ./BSP/Src/fs_layer.cyclo ./BSP/Src/fs_layer.d ./BSP/Src/fs_layer.o ./BSP/Src/fs_layer.su ./BSP/Src/touchgfx_input_controller.cyclo ./BSP/Src/touchgfx_input_controller.d ./BSP/Src/touchgfx_input_controller.o ./BSP/Src/touchgfx_input_controller.su ./BSP/Src/usb_helper.cyclo ./BSP/Src/usb_helper.d ./BSP/Src/usb_helper.o ./BSP/Src/usb_helper.su

.PHONY: clean-BSP-2f-Src

