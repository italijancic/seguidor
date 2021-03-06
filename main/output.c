/**
 * @file output.c
 *
 * @author
 *
 * @date
 *
 * @brief
 */

//=============================================================================
// [Inclusions] ===============================================================

#include "output.h"

#include "input.h"

//=============================================================================

//=============================================================================
// [Private Defines] ==========================================================
#define BLINK_GPIO_1 GPIO_NUM_2
#define BLINK_GPIO_2 GPIO_NUM_5

//=============================================================================

//=============================================================================
// [Local Typedef] ============================================================

//=============================================================================

//=============================================================================
// [External Data Definitions] ================================================

// Const ---------------------------------------------
//----------------------------------------------------

// Vars ----------------------------------------------
//----------------------------------------------------

// Task Handlers -------------------------------------
//----------------------------------------------------

// Queue Handlers ------------------------------------
//----------------------------------------------------

// Event Group Handlers ------------------------------
//----------------------------------------------------

// Semaphore Handlers --------------------------------
//----------------------------------------------------

//=============================================================================

//=============================================================================
// [Local Data Declarations] ==================================================

// Const ---------------------------------------------
//----------------------------------------------------

// Vars ----------------------------------------------
static uint8_t s_led_state = 0;
//----------------------------------------------------

// Task Handlers -------------------------------------
//----------------------------------------------------

// Queue Handlers ------------------------------------
//----------------------------------------------------

// Event Group Handlers ------------------------------
//----------------------------------------------------

// Semaphore Handlers --------------------------------
//----------------------------------------------------

//=============================================================================

//=============================================================================
// [Local Function Declarations] ==============================================

/**
 * @brief
 *
 * @param  None
 *
 * @return None
 *
 */
static void blink_led(void);

/**
 * @brief
 *
 * @param  None
 *
 * @return None
 *
 */
static void configure_led(void);

//=============================================================================

//=============================================================================
// [FreeRTOS Task Definitions] ================================================

/**
 * @brief      FreeRTOS
 *
 * @param      void *pvParameters
 *
 * @returns    None
 *
 */
void output_task(void *pvParameters)
{

	// Local data
	uint8_t multiplier = 0;

	// Infinity loop of task
	while (1)
	{
		ESP_LOGI("[output_task]", "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
		blink_led();
		/* Toggle the LED state */
		s_led_state = !s_led_state;
		// Read multiplier
		multiplier = get_multiplier();
		ESP_LOGI("[output_task]", "delay %u ms!", multiplier * 500);
		vTaskDelay(multiplier * 500 / portTICK_RATE_MS);
	}
}
//---------------------------------------------------------------------------//
/* End */

/**
 * @brief      FreeRTOS
 *
 * @param      void *pvParameters
 *
 * @returns    None
 *
 */

//---------------------------------------------------------------------------//
/* End */

//=============================================================================

//=============================================================================
// [Local Function Definitions] ===============================================

/**
 * @brief static void blink_led(void)
 *
 * @param  None
 *
 * @return None
 *
 */
static void blink_led(void)
{
	/* Set the GPIO level according to the state (LOW or HIGH)*/
	gpio_set_level(BLINK_GPIO_1, s_led_state);
	gpio_set_level(BLINK_GPIO_2, !s_led_state);
}
//---------------------------------------------------------------------------//
/* End */

/**
 * @brief
 *
 * @param  None
 *
 * @return None
 *
 */
static void configure_led(void)
{
	ESP_LOGI("[configure_led]", "Example configured to blink GPIO LED!");
	gpio_reset_pin(BLINK_GPIO_1);
	gpio_reset_pin(BLINK_GPIO_2);
	/* Set the GPIO as a push/pull output */
	gpio_set_direction(BLINK_GPIO_1, GPIO_MODE_OUTPUT);
	gpio_set_direction(BLINK_GPIO_2, GPIO_MODE_OUTPUT);
}
//---------------------------------------------------------------------------//
/* End */

//=============================================================================

//=============================================================================
// [External Function Definition] =============================================

/**
 * @details
 *
 * @param
 *
 * @return
 */
extern void output_init(void)
{

	ESP_LOGI("[output_init]", "Example configured to blink GPIO LED!");

	configure_led();

	// Create FreeRTOS Task
	xTaskCreate(&output_task, "[output_task]", 1024 * 2, NULL, tskIDLE_PRIORITY + 1, NULL);
}
//---------------------------------------------------------------------------//
/* End */

/**
 * @brief
 *
 * @param  None
 *
 * @return None
 *
 */

//---------------------------------------------------------------------------//
/* End */

//=============================================================================

//=============================[End Document]==================================