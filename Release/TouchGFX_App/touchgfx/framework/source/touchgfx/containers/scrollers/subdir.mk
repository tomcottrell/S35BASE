################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/DrawableList.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollBase.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollList.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheel.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelBase.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelWithSelectionStyle.cpp 

OBJS += \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/DrawableList.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollBase.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollList.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheel.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelBase.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelWithSelectionStyle.o 

CPP_DEPS += \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/DrawableList.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollBase.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollList.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheel.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelBase.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelWithSelectionStyle.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/%.o TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/%.su TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/%.cyclo: ../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/%.cpp TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM_VERSION -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../TouchGFX_App/config -I../TouchGFX_App/generated -I../TouchGFX_App/gui -I../TouchGFX_App/generated/fonts/include -I../TouchGFX_App/gui/include -I../TouchGFX_App/generated/gui_generated/include -I../TouchGFX_App/generated/images/include -I../TouchGFX_App/generated/texts/include -I../TouchGfx_App/touchgfx/framework/include -I../TouchGfx_App/touchgfx/3rdparty/libjpeg/include -I../TouchGFX_App/touchgfx/framework/include -I../BSP/Inc -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TouchGFX_App-2f-touchgfx-2f-framework-2f-source-2f-touchgfx-2f-containers-2f-scrollers

clean-TouchGFX_App-2f-touchgfx-2f-framework-2f-source-2f-touchgfx-2f-containers-2f-scrollers:
	-$(RM) ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/DrawableList.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/DrawableList.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/DrawableList.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/DrawableList.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollBase.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollBase.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollBase.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollBase.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollList.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollList.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollList.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollList.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheel.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheel.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheel.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheel.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelBase.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelBase.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelBase.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelBase.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelWithSelectionStyle.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelWithSelectionStyle.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelWithSelectionStyle.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/ScrollWheelWithSelectionStyle.su

.PHONY: clean-TouchGFX_App-2f-touchgfx-2f-framework-2f-source-2f-touchgfx-2f-containers-2f-scrollers

