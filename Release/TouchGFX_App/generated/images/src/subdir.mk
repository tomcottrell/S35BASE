################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX_App/generated/images/src/BitmapDatabase.cpp \
../TouchGFX_App/generated/images/src/SVGDatabase.cpp \
../TouchGFX_App/generated/images/src/image_BottomButton_active_eno.cpp \
../TouchGFX_App/generated/images/src/image_BottomButton_inactive.cpp \
../TouchGFX_App/generated/images/src/image_BottomButton_pressed_eno.cpp \
../TouchGFX_App/generated/images/src/image_EnovationControlsLogo_280x84.cpp \
../TouchGFX_App/generated/images/src/image_EnovationControlsLogo_Vector-Jan24.cpp \
../TouchGFX_App/generated/images/src/image_Home.cpp \
../TouchGFX_App/generated/images/src/image_brightness.cpp \
../TouchGFX_App/generated/images/src/image_oil_pressure.cpp \
../TouchGFX_App/generated/images/src/image_oil_temperature.cpp \
../TouchGFX_App/generated/images/src/image_rpm.cpp \
../TouchGFX_App/generated/images/src/image_status_leds.cpp \
../TouchGFX_App/generated/images/src/image_temperature.cpp 

OBJS += \
./TouchGFX_App/generated/images/src/BitmapDatabase.o \
./TouchGFX_App/generated/images/src/SVGDatabase.o \
./TouchGFX_App/generated/images/src/image_BottomButton_active_eno.o \
./TouchGFX_App/generated/images/src/image_BottomButton_inactive.o \
./TouchGFX_App/generated/images/src/image_BottomButton_pressed_eno.o \
./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_280x84.o \
./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_Vector-Jan24.o \
./TouchGFX_App/generated/images/src/image_Home.o \
./TouchGFX_App/generated/images/src/image_brightness.o \
./TouchGFX_App/generated/images/src/image_oil_pressure.o \
./TouchGFX_App/generated/images/src/image_oil_temperature.o \
./TouchGFX_App/generated/images/src/image_rpm.o \
./TouchGFX_App/generated/images/src/image_status_leds.o \
./TouchGFX_App/generated/images/src/image_temperature.o 

CPP_DEPS += \
./TouchGFX_App/generated/images/src/BitmapDatabase.d \
./TouchGFX_App/generated/images/src/SVGDatabase.d \
./TouchGFX_App/generated/images/src/image_BottomButton_active_eno.d \
./TouchGFX_App/generated/images/src/image_BottomButton_inactive.d \
./TouchGFX_App/generated/images/src/image_BottomButton_pressed_eno.d \
./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_280x84.d \
./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_Vector-Jan24.d \
./TouchGFX_App/generated/images/src/image_Home.d \
./TouchGFX_App/generated/images/src/image_brightness.d \
./TouchGFX_App/generated/images/src/image_oil_pressure.d \
./TouchGFX_App/generated/images/src/image_oil_temperature.d \
./TouchGFX_App/generated/images/src/image_rpm.d \
./TouchGFX_App/generated/images/src/image_status_leds.d \
./TouchGFX_App/generated/images/src/image_temperature.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX_App/generated/images/src/%.o TouchGFX_App/generated/images/src/%.su TouchGFX_App/generated/images/src/%.cyclo: ../TouchGFX_App/generated/images/src/%.cpp TouchGFX_App/generated/images/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM_VERSION -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../TouchGFX_App/config -I../TouchGFX_App/generated -I../TouchGFX_App/gui -I../TouchGFX_App/generated/fonts/include -I../TouchGFX_App/gui/include -I../TouchGFX_App/generated/gui_generated/include -I../TouchGFX_App/generated/images/include -I../TouchGFX_App/generated/texts/include -I../TouchGfx_App/touchgfx/framework/include -I../TouchGfx_App/touchgfx/3rdparty/libjpeg/include -I../TouchGFX_App/touchgfx/framework/include -I../BSP/Inc -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TouchGFX_App-2f-generated-2f-images-2f-src

clean-TouchGFX_App-2f-generated-2f-images-2f-src:
	-$(RM) ./TouchGFX_App/generated/images/src/BitmapDatabase.cyclo ./TouchGFX_App/generated/images/src/BitmapDatabase.d ./TouchGFX_App/generated/images/src/BitmapDatabase.o ./TouchGFX_App/generated/images/src/BitmapDatabase.su ./TouchGFX_App/generated/images/src/SVGDatabase.cyclo ./TouchGFX_App/generated/images/src/SVGDatabase.d ./TouchGFX_App/generated/images/src/SVGDatabase.o ./TouchGFX_App/generated/images/src/SVGDatabase.su ./TouchGFX_App/generated/images/src/image_BottomButton_active_eno.cyclo ./TouchGFX_App/generated/images/src/image_BottomButton_active_eno.d ./TouchGFX_App/generated/images/src/image_BottomButton_active_eno.o ./TouchGFX_App/generated/images/src/image_BottomButton_active_eno.su ./TouchGFX_App/generated/images/src/image_BottomButton_inactive.cyclo ./TouchGFX_App/generated/images/src/image_BottomButton_inactive.d ./TouchGFX_App/generated/images/src/image_BottomButton_inactive.o ./TouchGFX_App/generated/images/src/image_BottomButton_inactive.su ./TouchGFX_App/generated/images/src/image_BottomButton_pressed_eno.cyclo ./TouchGFX_App/generated/images/src/image_BottomButton_pressed_eno.d ./TouchGFX_App/generated/images/src/image_BottomButton_pressed_eno.o ./TouchGFX_App/generated/images/src/image_BottomButton_pressed_eno.su ./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_280x84.cyclo ./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_280x84.d ./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_280x84.o ./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_280x84.su ./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_Vector-Jan24.cyclo ./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_Vector-Jan24.d ./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_Vector-Jan24.o ./TouchGFX_App/generated/images/src/image_EnovationControlsLogo_Vector-Jan24.su ./TouchGFX_App/generated/images/src/image_Home.cyclo ./TouchGFX_App/generated/images/src/image_Home.d ./TouchGFX_App/generated/images/src/image_Home.o ./TouchGFX_App/generated/images/src/image_Home.su ./TouchGFX_App/generated/images/src/image_brightness.cyclo ./TouchGFX_App/generated/images/src/image_brightness.d ./TouchGFX_App/generated/images/src/image_brightness.o ./TouchGFX_App/generated/images/src/image_brightness.su ./TouchGFX_App/generated/images/src/image_oil_pressure.cyclo ./TouchGFX_App/generated/images/src/image_oil_pressure.d ./TouchGFX_App/generated/images/src/image_oil_pressure.o ./TouchGFX_App/generated/images/src/image_oil_pressure.su ./TouchGFX_App/generated/images/src/image_oil_temperature.cyclo ./TouchGFX_App/generated/images/src/image_oil_temperature.d ./TouchGFX_App/generated/images/src/image_oil_temperature.o ./TouchGFX_App/generated/images/src/image_oil_temperature.su ./TouchGFX_App/generated/images/src/image_rpm.cyclo ./TouchGFX_App/generated/images/src/image_rpm.d ./TouchGFX_App/generated/images/src/image_rpm.o ./TouchGFX_App/generated/images/src/image_rpm.su ./TouchGFX_App/generated/images/src/image_status_leds.cyclo ./TouchGFX_App/generated/images/src/image_status_leds.d ./TouchGFX_App/generated/images/src/image_status_leds.o ./TouchGFX_App/generated/images/src/image_status_leds.su ./TouchGFX_App/generated/images/src/image_temperature.cyclo ./TouchGFX_App/generated/images/src/image_temperature.d ./TouchGFX_App/generated/images/src/image_temperature.o ./TouchGFX_App/generated/images/src/image_temperature.su

.PHONY: clean-TouchGFX_App-2f-generated-2f-images-2f-src

