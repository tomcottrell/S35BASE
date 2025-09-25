################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_backgrounds_light_filled.cpp \
../TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_needles_rough.cpp \
../TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_home_30_30_E8F6FB.svg.cpp \
../TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_settings_24_24_E8F6FB.svg.cpp \
../TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_av_playlist_add_check_30_30_E8F6FB.svg.cpp \
../TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_analytics_24_24_E8F6FB.svg.cpp \
../TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_E8F6FB.svg.cpp \
../TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_FFFFFF.svg.cpp \
../TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_down_40_40_E8F6FB.svg.cpp \
../TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_up_40_40_E8F6FB.svg.cpp 

OBJS += \
./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_backgrounds_light_filled.o \
./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_needles_rough.o \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_home_30_30_E8F6FB.svg.o \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_settings_24_24_E8F6FB.svg.o \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_av_playlist_add_check_30_30_E8F6FB.svg.o \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_analytics_24_24_E8F6FB.svg.o \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_E8F6FB.svg.o \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_FFFFFF.svg.o \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_down_40_40_E8F6FB.svg.o \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_up_40_40_E8F6FB.svg.o 

CPP_DEPS += \
./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_backgrounds_light_filled.d \
./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_needles_rough.d \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_home_30_30_E8F6FB.svg.d \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_settings_24_24_E8F6FB.svg.d \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_av_playlist_add_check_30_30_E8F6FB.svg.d \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_analytics_24_24_E8F6FB.svg.d \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_E8F6FB.svg.d \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_FFFFFF.svg.d \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_down_40_40_E8F6FB.svg.d \
./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_up_40_40_E8F6FB.svg.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX_App/generated/images/src/__generated/%.o TouchGFX_App/generated/images/src/__generated/%.su TouchGFX_App/generated/images/src/__generated/%.cyclo: ../TouchGFX_App/generated/images/src/__generated/%.cpp TouchGFX_App/generated/images/src/__generated/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L4R7xx -DSTM_VERSION -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../TouchGFX_App/config -I../TouchGFX_App/generated -I../TouchGFX_App/gui -I../TouchGFX_App/generated/fonts/include -I../TouchGFX_App/gui/include -I../TouchGFX_App/generated/gui_generated/include -I../TouchGFX_App/generated/images/include -I../TouchGFX_App/generated/texts/include -I../TouchGfx_App/touchgfx/framework/include -I../TouchGfx_App/touchgfx/3rdparty/libjpeg/include -I../TouchGFX_App/touchgfx/framework/include -I../BSP/Inc -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TouchGFX_App-2f-generated-2f-images-2f-src-2f-__generated

clean-TouchGFX_App-2f-generated-2f-images-2f-src-2f-__generated:
	-$(RM) ./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_backgrounds_light_filled.cyclo ./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_backgrounds_light_filled.d ./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_backgrounds_light_filled.o ./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_backgrounds_light_filled.su ./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_needles_rough.cyclo ./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_needles_rough.d ./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_needles_rough.o ./TouchGFX_App/generated/images/src/__generated/image_alternate_theme_images_widgets_gauge_tiny_needles_rough.su ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_home_30_30_E8F6FB.svg.cyclo ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_home_30_30_E8F6FB.svg.d ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_home_30_30_E8F6FB.svg.o ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_home_30_30_E8F6FB.svg.su ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_settings_24_24_E8F6FB.svg.cyclo ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_settings_24_24_E8F6FB.svg.d ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_settings_24_24_E8F6FB.svg.o ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_action_settings_24_24_E8F6FB.svg.su ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_av_playlist_add_check_30_30_E8F6FB.svg.cyclo ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_av_playlist_add_check_30_30_E8F6FB.svg.d ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_av_playlist_add_check_30_30_E8F6FB.svg.o ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_av_playlist_add_check_30_30_E8F6FB.svg.su ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_analytics_24_24_E8F6FB.svg.cyclo ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_analytics_24_24_E8F6FB.svg.d ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_analytics_24_24_E8F6FB.svg.o ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_analytics_24_24_E8F6FB.svg.su ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_E8F6FB.svg.cyclo ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_E8F6FB.svg.d ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_E8F6FB.svg.o ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_E8F6FB.svg.su ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_FFFFFF.svg.cyclo ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_FFFFFF.svg.d ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_FFFFFF.svg.o ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_google_dashboard_48_w_layer_24_24_FFFFFF.svg.su ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_down_40_40_E8F6FB.svg.cyclo ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_down_40_40_E8F6FB.svg.d ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_down_40_40_E8F6FB.svg.o ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_down_40_40_E8F6FB.svg.su ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_up_40_40_E8F6FB.svg.cyclo ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_up_40_40_E8F6FB.svg.d ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_up_40_40_E8F6FB.svg.o ./TouchGFX_App/generated/images/src/__generated/image_icon_theme_images_hardware_keyboard_arrow_up_40_40_E8F6FB.svg.su

.PHONY: clean-TouchGFX_App-2f-generated-2f-images-2f-src-2f-__generated

