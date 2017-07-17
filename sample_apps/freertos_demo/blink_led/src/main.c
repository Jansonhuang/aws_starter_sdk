/*
 *  Copyright (C) 2008-2016, Marvell International Ltd.
 *  All Rights Reserved.
 */

#include <wmstdio.h>
#include <wm_os.h>
#include <mdev_gpio.h>
#include <mdev_pinmux.h>
#include <board.h>

#include "FreeRTOS.h"

/*
 * Simple Application which uses GPIO driver.
 *
 * Summary:
 * This application turns ON/OFF a LED every one second
 *
 * Description:
 *
 * This application is written using APIs exposed by MDEV
 * layer of GPIO driver and Pinmux Driver.
 *
 * The application configures led_2 to blink every one second
 */

/*------------------Macro Definitions ------------------*/

#define GPIO_LED_FN  PINMUX_FUNCTION_0

#define BLINK_LED_STACK_SIZE    (configMINIMAL_STACK_SIZE)
#define BLINK_LED_TASK_PRIORITY     (3)
/*------------------Global Variable Definitions ---------*/

/* This holds  LED gpio pin number */
static unsigned int gpio_led;

/* This function turns on the LED*/
static void gpio_led_on(void)
{
    mdev_t *gpio_dev = gpio_drv_open("MDEV_GPIO");
    /* Turn on LED by writing  0 in GPIO register */
    gpio_drv_write(gpio_dev, gpio_led, 0);
    gpio_drv_close(gpio_dev);
}

/* This function turns off the LED*/
static void gpio_led_off(void)
{
    mdev_t *gpio_dev = gpio_drv_open("MDEV_GPIO");
    /* Turn off LED by writing  1 in GPIO register */
    gpio_drv_write(gpio_dev, gpio_led, 1);
    gpio_drv_close(gpio_dev);
}

/* Configure GPIO pins to be used as LED and push button */
static void configure_gpios()
{
    mdev_t *pinmux_dev, *gpio_dev;

    /* Initialize  pinmux driver */
    pinmux_drv_init();

    /* Open pinmux driver */
    pinmux_dev = pinmux_drv_open("MDEV_PINMUX");

    /* Initialize GPIO driver */
    gpio_drv_init();

    /* Open GPIO driver */
    gpio_dev = gpio_drv_open("MDEV_GPIO");

    /* Configure GPIO pin function for GPIO connected to LED */
    pinmux_drv_setfunc(pinmux_dev, gpio_led, GPIO_LED_FN);

    /* Configure GPIO pin direction as Output */
    gpio_drv_setdir(gpio_dev, gpio_led, GPIO_OUTPUT);
    /* Keep initial state of LED: ON */
    gpio_drv_write(gpio_dev, gpio_led, 0);

    /* Close drivers */
    pinmux_drv_close(pinmux_dev);
    gpio_drv_close(gpio_dev);
}

static void prvBlinkLedTask( void *pvParameters )
{
    while (1)
    {
        gpio_led_on();
        os_thread_sleep(os_msec_to_ticks(1000));
        gpio_led_off();
        os_thread_sleep(os_msec_to_ticks(1000));
    }
}

/* This is an entry point for the application.
   All application specific initialization is performed here. */
int main(void)
{
    int ret = 0;

    /* Initializes console on UART0 */
    ret = wmstdio_init(UART0_ID, 0);
    if (ret == -WM_FAIL)
    {
        wmprintf("Failed to initialize console on uart0\r\n");
        return -1;
    }

    wmprintf(" LED demo application started\r\n");
    wmprintf(" This application demonstrates the"
                  " use of blinking led\r\n");

    gpio_led = (board_led_2()).gpio;

    wmprintf(" LED Pin : %d\r\n", gpio_led);

    configure_gpios();

    xTaskCreate(prvBlinkLedTask, "Blink led", ( uint16_t ) BLINK_LED_STACK_SIZE, NULL, ( ( UBaseType_t ) BLINK_LED_TASK_PRIORITY ) | portPRIVILEGE_BIT, NULL);
    while (1)
    {
        os_thread_sleep(os_msec_to_ticks(1000));
    }
    return 0;
}

