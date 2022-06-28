/**
 * @file input.c
 *
 * @author
 *
 * @date
 *
 * @brief
 */

//=============================================================================
// [Inclusions] ===============================================================

#include "input.h"
#include "output.h"
#include "semaphore.h"

//=============================================================================

//=============================================================================
// [Private Defines] ==========================================================
#define INPUT_GPIO GPIO_NUM_4

//=============================================================================

//=============================================================================
// [Local Typedef] ============================================================

//=============================================================================

//=============================================================================
// [External Data Definitions] ================================================

// Const ---------------------------------------------
//----------------------------------------------------

// Vars ----------------------------------------------
extern xSemaphoreHandle GlobalKey;
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
static uint8_t multiplier = 1;
static uint8_t input_state = 0;
static uint8_t first_press = 0;

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
static void input_read(void);

/**
 * @brief
 *
 * @param  None
 *
 * @return None
 *
 */
static void configure_input(void);

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
void input_task(void *pvParameters)
{

	// Local data

	// Infinity loop of task
	while (1)
	{
		input_read();

		if (first_press == 0 && input_state == 1)
		{
			ESP_LOGI("[input_task]", "Cambio el estado de first_press");
			xSemaphoreGive(GlobalKey);
			first_press = 1;
			multiplier = 0;
		}

		if (input_state == 1)
		{
			multiplier++;
			if (multiplier == 5)
			{
				multiplier = 1;
			}

			ESP_LOGI("[input_task]", "El multiplicador ha pasado a %i", multiplier);

			vTaskDelay(400 / portTICK_RATE_MS);
		}

		vTaskDelay(100 / portTICK_RATE_MS);
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
static void input_read(void)
{
	input_state = gpio_get_level(INPUT_GPIO);
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
static void configure_input(void)
{
	gpio_reset_pin(INPUT_GPIO);
	gpio_set_direction(INPUT_GPIO, GPIO_MODE_INPUT);
	gpio_set_pull_mode(INPUT_GPIO, GPIO_PULLDOWN_ONLY);
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
extern void input_init(void)
{
	configure_input();

	// Create FreeRTOS Task
	xTaskCreate(&input_task, "[input_task]", 1024 * 2, NULL, tskIDLE_PRIORITY + 1, NULL);
}
//---------------------------------------------------------------------------//
/* End */

/**
 * @details
 *
 * @param[in]
 *
 * @return
 */
extern uint8_t get_multiplier(void)
{
	return multiplier;
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