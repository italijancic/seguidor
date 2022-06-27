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
#define PROCESS_LED GPIO_NUM_2
#define STATUS_RED_LED GPIO_NUM_18
#define STATUS_GREEN_LED GPIO_NUM_19

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
static uint8_t PROCESS_LED_state = 0;
static uint8_t STATUS_RED_LED_state = 0;
static uint8_t STATUS_GREEN_LED_state = 0;

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
static void configure_leds(void);

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
		ESP_LOGI("[output_task]", "Turning the LED %s!", PROCESS_LED_state == true ? "ON" : "OFF");
		blink_led();
		/* Toggle the LED state */
		PROCESS_LED_state = !PROCESS_LED_state;
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
	gpio_set_level(PROCESS_LED, PROCESS_LED_state);
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
static void configure_leds(void)
{
	ESP_LOGI("[configure_leds]", "Configuring status and process LEDs");
	/* Reset pin mode to default */
	gpio_reset_pin(PROCESS_LED);
	gpio_reset_pin(STATUS_GREEN_LED);
	gpio_reset_pin(STATUS_RED_LED);
	/* Set pin mode */
	gpio_set_direction(PROCESS_LED, GPIO_MODE_OUTPUT);
	gpio_set_direction(STATUS_GREEN_LED, GPIO_MODE_OUTPUT);
	gpio_set_direction(STATUS_RED_LED, GPIO_MODE_OUTPUT);

	gpio_set_level(PROCESS_LED, PROCESS_LED_state);
	gpio_set_level(STATUS_GREEN_LED, STATUS_RED_LED_state);
	gpio_set_level(STATUS_RED_LED, STATUS_GREEN_LED_state);
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

	configure_leds();

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