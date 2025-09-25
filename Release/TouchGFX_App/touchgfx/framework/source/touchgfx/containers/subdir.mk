################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/CacheableContainer.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Container.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ListLayout.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ModalWindow.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ScrollableContainer.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SlideMenu.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Slider.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SwipeContainer.cpp \
../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ZoomAnimationImage.cpp 

OBJS += \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/CacheableContainer.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Container.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ListLayout.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ModalWindow.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ScrollableContainer.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SlideMenu.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Slider.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SwipeContainer.o \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ZoomAnimationImage.o 

CPP_DEPS += \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/CacheableContainer.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Container.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ListLayout.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ModalWindow.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ScrollableContainer.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SlideMenu.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Slider.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SwipeContainer.d \
./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ZoomAnimationImage.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX_App/touchgfx/framework/source/touchgfx/containers/%.o TouchGFX_App/touchgfx/framework/source/touchgfx/containers/%.su TouchGFX_App/touchgfx/framework/source/touchgfx/containers/%.cyclo: ../TouchGFX_App/touchgfx/framework/source/touchgfx/containers/%.cpp TouchGFX_App/touchgfx/framework/source/touchgfx/containers/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM_VERSION -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../TouchGFX_App/config -I../TouchGFX_App/generated -I../TouchGFX_App/gui -I../TouchGFX_App/generated/fonts/include -I../TouchGFX_App/gui/include -I../TouchGFX_App/generated/gui_generated/include -I../TouchGFX_App/generated/images/include -I../TouchGFX_App/generated/texts/include -I../TouchGfx_App/touchgfx/framework/include -I../TouchGfx_App/touchgfx/3rdparty/libjpeg/include -I../TouchGFX_App/touchgfx/framework/include -I../BSP/Inc -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TouchGFX_App-2f-touchgfx-2f-framework-2f-source-2f-touchgfx-2f-containers

clean-TouchGFX_App-2f-touchgfx-2f-framework-2f-source-2f-touchgfx-2f-containers:
	-$(RM) ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/CacheableContainer.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/CacheableContainer.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/CacheableContainer.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/CacheableContainer.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Container.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Container.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Container.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Container.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ListLayout.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ListLayout.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ListLayout.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ListLayout.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ModalWindow.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ModalWindow.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ModalWindow.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ModalWindow.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ScrollableContainer.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ScrollableContainer.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ScrollableContainer.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ScrollableContainer.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SlideMenu.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SlideMenu.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SlideMenu.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SlideMenu.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Slider.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Slider.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Slider.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/Slider.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SwipeContainer.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SwipeContainer.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SwipeContainer.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/SwipeContainer.su ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ZoomAnimationImage.cyclo ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ZoomAnimationImage.d ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ZoomAnimationImage.o ./TouchGFX_App/touchgfx/framework/source/touchgfx/containers/ZoomAnimationImage.su

.PHONY: clean-TouchGFX_App-2f-touchgfx-2f-framework-2f-source-2f-touchgfx-2f-containers

