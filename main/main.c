/**
 * @file main.c
 *
 * @author
 *
 * @date
 *
 * @brief
 */

//=============================================================================
// [Inclusions] ===============================================================

#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "sdkconfig.h"

#include "output.h"
#include "input.h"
#include "gps.h"
#include "motion.h"

//=============================================================================

//=============================================================================
// [Private Defines] ==========================================================

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
//----------------------------------------------------

// Task Handlers -------------------------------------
TaskHandle_t h2TestTask = NULL;
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

//=============================================================================

//=============================================================================
// [FreeRTOS Task Definitions] ================================================

/**
 * @brief      FreeRTOS Task to test gps appy layer iplementation
 *
 * @param      void *pvParameters
 *
 * @returns    None
 *
 */
void test_task(void *pvParameters)
{
	// Local data declaration
	const char *TAG = "[main_test_task]";
	gps_t gps_data = {0};

	// Local data definition


	// Wait until get GPS valid data
	ESP_LOGI(TAG, "Waiting to get GPS valid data...");
	xEventGroupWaitBits(gps_event_group, GPS_DATA_VALID, false, false, portMAX_DELAY);

	// Infinity loop of task
	while (1)
	{
		gps_data = gps_get_data();

		if(gps_data.valid)
		{
			ESP_LOGI(TAG, 	"\r\n\r\n"
							"\tfix		= %d\r\n"
							"\tvalid		= %s\r\n"
							"\tutc date	= %02d/%02d/%02d\r\n"
							"\tutc time	= %02d:%02d:%02d\r\n"
							"\tlatitude    	= %.05f %s\r\n"
							"\tlongtitude  	= %.05f %s\r\n"
							"\taltitude    	= %.02f [m]\r\n"
							"\tspeed      	= %f [m/s]\r\n"
							"\tSat in view 	= %d\r\n"
							"\tSat in use  	= %d\r\n",
							gps_data.fix,
							(gps_data.valid ? "true":"false"),
							gps_data.date.day, gps_data.date.month, gps_data.date.year + YEAR_BASE,
							// gps_data.tim.hour - TIME_ZONE, gps_data.tim.minute, gps_data.tim.second,
							gps_data.tim.hour, gps_data.tim.minute, gps_data.tim.second,
							gps_data.latitude, (gps_data.latitude > 0 ? "°N":"°S"),
							gps_data.longitude, (gps_data.longitude > 0 ? "°E":"°W"), gps_data.altitude, gps_data.speed,
							gps_data.sats_in_view, gps_data.sats_in_use);
		}
		else
		{
			ESP_LOGW(TAG, "GPS Invalid Data");
		}


		vTaskDelay(5000/portTICK_RATE_MS);
	}
	// End while(1)
	vTaskDelete(&h2TestTask);
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

//=============================================================================

//=============================================================================
// [External Function Definition] =============================================

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

//=============================================================================
//================================[Main App]===================================

/**
 * @brief  Main fuction on app
 *
 * @param  None
 *
 * @return None
 *
 */
void app_main(void)
{

	// Init outputs
	// output_init();

	// Init inputs
	// input_init();

	// Init gps
	gps_init();

	// Init motion logic
	motion_init();

	/**
     * FreeRTOS Task Creation
     */
    // xTaskCreate(&test_task, "[main_test_task]", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, &h2TestTask);
}

//=============================[End app_main]==================================

//=============================[End Document]==================================