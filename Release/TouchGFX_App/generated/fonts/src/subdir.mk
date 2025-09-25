################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX_App/generated/fonts/src/ApplicationFontProvider.cpp \
../TouchGFX_App/generated/fonts/src/CachedFont.cpp \
../TouchGFX_App/generated/fonts/src/FontCache.cpp \
../TouchGFX_App/generated/fonts/src/Font_verdana_10_4bpp_0.cpp \
../TouchGFX_App/generated/fonts/src/Font_verdana_16_4bpp_0.cpp \
../TouchGFX_App/generated/fonts/src/Font_verdana_20_4bpp_0.cpp \
../TouchGFX_App/generated/fonts/src/Font_verdana_40_4bpp_0.cpp \
../TouchGFX_App/generated/fonts/src/GeneratedFont.cpp \
../TouchGFX_App/generated/fonts/src/Kerning_verdana_10_4bpp.cpp \
../TouchGFX_App/generated/fonts/src/Kerning_verdana_16_4bpp.cpp \
../TouchGFX_App/generated/fonts/src/Kerning_verdana_20_4bpp.cpp \
../TouchGFX_App/generated/fonts/src/Kerning_verdana_40_4bpp.cpp \
../TouchGFX_App/generated/fonts/src/Table_verdana_10_4bpp.cpp \
../TouchGFX_App/generated/fonts/src/Table_verdana_16_4bpp.cpp \
../TouchGFX_App/generated/fonts/src/Table_verdana_20_4bpp.cpp \
../TouchGFX_App/generated/fonts/src/Table_verdana_40_4bpp.cpp \
../TouchGFX_App/generated/fonts/src/UnmappedDataFont.cpp 

OBJS += \
./TouchGFX_App/generated/fonts/src/ApplicationFontProvider.o \
./TouchGFX_App/generated/fonts/src/CachedFont.o \
./TouchGFX_App/generated/fonts/src/FontCache.o \
./TouchGFX_App/generated/fonts/src/Font_verdana_10_4bpp_0.o \
./TouchGFX_App/generated/fonts/src/Font_verdana_16_4bpp_0.o \
./TouchGFX_App/generated/fonts/src/Font_verdana_20_4bpp_0.o \
./TouchGFX_App/generated/fonts/src/Font_verdana_40_4bpp_0.o \
./TouchGFX_App/generated/fonts/src/GeneratedFont.o \
./TouchGFX_App/generated/fonts/src/Kerning_verdana_10_4bpp.o \
./TouchGFX_App/generated/fonts/src/Kerning_verdana_16_4bpp.o \
./TouchGFX_App/generated/fonts/src/Kerning_verdana_20_4bpp.o \
./TouchGFX_App/generated/fonts/src/Kerning_verdana_40_4bpp.o \
./TouchGFX_App/generated/fonts/src/Table_verdana_10_4bpp.o \
./TouchGFX_App/generated/fonts/src/Table_verdana_16_4bpp.o \
./TouchGFX_App/generated/fonts/src/Table_verdana_20_4bpp.o \
./TouchGFX_App/generated/fonts/src/Table_verdana_40_4bpp.o \
./TouchGFX_App/generated/fonts/src/UnmappedDataFont.o 

CPP_DEPS += \
./TouchGFX_App/generated/fonts/src/ApplicationFontProvider.d \
./TouchGFX_App/generated/fonts/src/CachedFont.d \
./TouchGFX_App/generated/fonts/src/FontCache.d \
./TouchGFX_App/generated/fonts/src/Font_verdana_10_4bpp_0.d \
./TouchGFX_App/generated/fonts/src/Font_verdana_16_4bpp_0.d \
./TouchGFX_App/generated/fonts/src/Font_verdana_20_4bpp_0.d \
./TouchGFX_App/generated/fonts/src/Font_verdana_40_4bpp_0.d \
./TouchGFX_App/generated/fonts/src/GeneratedFont.d \
./TouchGFX_App/generated/fonts/src/Kerning_verdana_10_4bpp.d \
./TouchGFX_App/generated/fonts/src/Kerning_verdana_16_4bpp.d \
./TouchGFX_App/generated/fonts/src/Kerning_verdana_20_4bpp.d \
./TouchGFX_App/generated/fonts/src/Kerning_verdana_40_4bpp.d \
./TouchGFX_App/generated/fonts/src/Table_verdana_10_4bpp.d \
./TouchGFX_App/generated/fonts/src/Table_verdana_16_4bpp.d \
./TouchGFX_App/generated/fonts/src/Table_verdana_20_4bpp.d \
./TouchGFX_App/generated/fonts/src/Table_verdana_40_4bpp.d \
./TouchGFX_App/generated/fonts/src/UnmappedDataFont.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX_App/generated/fonts/src/%.o TouchGFX_App/generated/fonts/src/%.su TouchGFX_App/generated/fonts/src/%.cyclo: ../TouchGFX_App/generated/fonts/src/%.cpp TouchGFX_App/generated/fonts/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM_VERSION -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../TouchGFX_App/config -I../TouchGFX_App/generated -I../TouchGFX_App/gui -I../TouchGFX_App/generated/fonts/include -I../TouchGFX_App/gui/include -I../TouchGFX_App/generated/gui_generated/include -I../TouchGFX_App/generated/images/include -I../TouchGFX_App/generated/texts/include -I../TouchGfx_App/touchgfx/framework/include -I../TouchGfx_App/touchgfx/3rdparty/libjpeg/include -I../TouchGFX_App/touchgfx/framework/include -I../BSP/Inc -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TouchGFX_App-2f-generated-2f-fonts-2f-src

clean-TouchGFX_App-2f-generated-2f-fonts-2f-src:
	-$(RM) ./TouchGFX_App/generated/fonts/src/ApplicationFontProvider.cyclo ./TouchGFX_App/generated/fonts/src/ApplicationFontProvider.d ./TouchGFX_App/generated/fonts/src/ApplicationFontProvider.o ./TouchGFX_App/generated/fonts/src/ApplicationFontProvider.su ./TouchGFX_App/generated/fonts/src/CachedFont.cyclo ./TouchGFX_App/generated/fonts/src/CachedFont.d ./TouchGFX_App/generated/fonts/src/CachedFont.o ./TouchGFX_App/generated/fonts/src/CachedFont.su ./TouchGFX_App/generated/fonts/src/FontCache.cyclo ./TouchGFX_App/generated/fonts/src/FontCache.d ./TouchGFX_App/generated/fonts/src/FontCache.o ./TouchGFX_App/generated/fonts/src/FontCache.su ./TouchGFX_App/generated/fonts/src/Font_verdana_10_4bpp_0.cyclo ./TouchGFX_App/generated/fonts/src/Font_verdana_10_4bpp_0.d ./TouchGFX_App/generated/fonts/src/Font_verdana_10_4bpp_0.o ./TouchGFX_App/generated/fonts/src/Font_verdana_10_4bpp_0.su ./TouchGFX_App/generated/fonts/src/Font_verdana_16_4bpp_0.cyclo ./TouchGFX_App/generated/fonts/src/Font_verdana_16_4bpp_0.d ./TouchGFX_App/generated/fonts/src/Font_verdana_16_4bpp_0.o ./TouchGFX_App/generated/fonts/src/Font_verdana_16_4bpp_0.su ./TouchGFX_App/generated/fonts/src/Font_verdana_20_4bpp_0.cyclo ./TouchGFX_App/generated/fonts/src/Font_verdana_20_4bpp_0.d ./TouchGFX_App/generated/fonts/src/Font_verdana_20_4bpp_0.o ./TouchGFX_App/generated/fonts/src/Font_verdana_20_4bpp_0.su ./TouchGFX_App/generated/fonts/src/Font_verdana_40_4bpp_0.cyclo ./TouchGFX_App/generated/fonts/src/Font_verdana_40_4bpp_0.d ./TouchGFX_App/generated/fonts/src/Font_verdana_40_4bpp_0.o ./TouchGFX_App/generated/fonts/src/Font_verdana_40_4bpp_0.su ./TouchGFX_App/generated/fonts/src/GeneratedFont.cyclo ./TouchGFX_App/generated/fonts/src/GeneratedFont.d ./TouchGFX_App/generated/fonts/src/GeneratedFont.o ./TouchGFX_App/generated/fonts/src/GeneratedFont.su ./TouchGFX_App/generated/fonts/src/Kerning_verdana_10_4bpp.cyclo ./TouchGFX_App/generated/fonts/src/Kerning_verdana_10_4bpp.d ./TouchGFX_App/generated/fonts/src/Kerning_verdana_10_4bpp.o ./TouchGFX_App/generated/fonts/src/Kerning_verdana_10_4bpp.su ./TouchGFX_App/generated/fonts/src/Kerning_verdana_16_4bpp.cyclo ./TouchGFX_App/generated/fonts/src/Kerning_verdana_16_4bpp.d ./TouchGFX_App/generated/fonts/src/Kerning_verdana_16_4bpp.o ./TouchGFX_App/generated/fonts/src/Kerning_verdana_16_4bpp.su ./TouchGFX_App/generated/fonts/src/Kerning_verdana_20_4bpp.cyclo ./TouchGFX_App/generated/fonts/src/Kerning_verdana_20_4bpp.d ./TouchGFX_App/generated/fonts/src/Kerning_verdana_20_4bpp.o ./TouchGFX_App/generated/fonts/src/Kerning_verdana_20_4bpp.su ./TouchGFX_App/generated/fonts/src/Kerning_verdana_40_4bpp.cyclo ./TouchGFX_App/generated/fonts/src/Kerning_verdana_40_4bpp.d ./TouchGFX_App/generated/fonts/src/Kerning_verdana_40_4bpp.o ./TouchGFX_App/generated/fonts/src/Kerning_verdana_40_4bpp.su ./TouchGFX_App/generated/fonts/src/Table_verdana_10_4bpp.cyclo ./TouchGFX_App/generated/fonts/src/Table_verdana_10_4bpp.d ./TouchGFX_App/generated/fonts/src/Table_verdana_10_4bpp.o ./TouchGFX_App/generated/fonts/src/Table_verdana_10_4bpp.su ./TouchGFX_App/generated/fonts/src/Table_verdana_16_4bpp.cyclo ./TouchGFX_App/generated/fonts/src/Table_verdana_16_4bpp.d ./TouchGFX_App/generated/fonts/src/Table_verdana_16_4bpp.o ./TouchGFX_App/generated/fonts/src/Table_verdana_16_4bpp.su ./TouchGFX_App/generated/fonts/src/Table_verdana_20_4bpp.cyclo ./TouchGFX_App/generated/fonts/src/Table_verdana_20_4bpp.d ./TouchGFX_App/generated/fonts/src/Table_verdana_20_4bpp.o ./TouchGFX_App/generated/fonts/src/Table_verdana_20_4bpp.su ./TouchGFX_App/generated/fonts/src/Table_verdana_40_4bpp.cyclo ./TouchGFX_App/generated/fonts/src/Table_verdana_40_4bpp.d ./TouchGFX_App/generated/fonts/src/Table_verdana_40_4bpp.o ./TouchGFX_App/generated/fonts/src/Table_verdana_40_4bpp.su ./TouchGFX_App/generated/fonts/src/UnmappedDataFont.cyclo ./TouchGFX_App/generated/fonts/src/UnmappedDataFont.d ./TouchGFX_App/generated/fonts/src/UnmappedDataFont.o ./TouchGFX_App/generated/fonts/src/UnmappedDataFont.su

.PHONY: clean-TouchGFX_App-2f-generated-2f-fonts-2f-src

