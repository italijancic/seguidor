/**
 * @file gps.c
 *
 * @author itlijancic@outlook.com
 *
 * @date 27/06/2022
 *
 * @brief Apply layer of gps driver
 */

//=============================================================================
// [Inclusions] ===============================================================

#include "gps.h"

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
gps_t gps_data = {0};
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
TaskHandle_t h2GpsTask = NULL;
//----------------------------------------------------

// Queue Handlers ------------------------------------
QueueHandle_t xGPSQueue;
//----------------------------------------------------

// Event Group Handlers ------------------------------

EventGroupHandle_t gps_event_group = NULL;

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


//=============================================================================

//====================================================================
// [Event Handlers Definition] =======================================

/**
 * @brief GPS Event Handler
 *
 * @param event_handler_arg handler specific arguments
 * @param event_base event base, here is fixed to ESP_NMEA_EVENT
 * @param event_id event id
 * @param event_data event specific arguments
 */
static void gps_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data){

    /* Local data declaration */
    gps_t *gps;

    /* Local data definition */
    gps = NULL;

    switch (event_id) {

        case GPS_UPDATE:

			// Cast data to gps_t type
            gps = (gps_t *)event_data;
			// Load new data into global var
			gps_data = *gps;
			if(gps->valid && (gps->fix != GPS_FIX_INVALID))
			{
				xEventGroupSetBits(gps_event_group, GPS_DATA_VALID);
			}

			// Send data to apply task over freeRTOS queue
            // xQueueSend(xGPSQueue, gps, portMAX_DELAY);
            break;

        case GPS_UNKNOWN:
            /* print unknown statements */
            ESP_LOGW("[gps_event_handler]", "Unknown statement:%s", (char *)event_data);
            break;

        /* Otherwise */
        default:
            break;

    }
}
//------------------------------------------------------------------//
/* End  */

/**
 * @ name:
 * @ param:			None
 * @ return:		None
 * @ description:
 * */

//------------------------------------------------------------------//
/* End  */

//====================================================================

//=============================================================================
// [FreeRTOS Task Definitions] ================================================

/**
 * @ name:
 * @ param:			None
 * @ return:		None
 * @ description:
 * */
void gps_task( void *pvParameters ){

	/* Local data declaration */
	const char *TAG = "[gps_task]";
    gps_t gps;

	/* Local data definition */
    bzero(&gps, sizeof(gps_t));

    ESP_LOGI(TAG,"Initialization, waiting GPS data arrived...");

	/* while(1) */
	while(1){

        while( xQueueReceive(xGPSQueue, &gps, portMAX_DELAY) != 0){
            //ESP_LOGI("[Test_Task]","Frame Update");
            /* print information parsed from GPS statements */
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
							gps.fix,
							(gps.valid ? "true":"false"),
							gps.date.day, gps.date.month, gps.date.year + YEAR_BASE,
							// gps.tim.hour - TIME_ZONE, gps.tim.minute, gps.tim.second,
							gps.tim.hour, gps.tim.minute, gps.tim.second,
							gps.latitude, (gps.latitude > 0 ? "°N":"°S"),
							gps.longitude, (gps.longitude > 0 ? "°E":"°W"), gps.altitude, gps.speed,
							gps.sats_in_view, gps.sats_in_use);

            vTaskDelay(1000 / portTICK_RATE_MS);
        }

	}
	/* End while(1) */
	vTaskDelete(&h2GpsTask);
}

//------------------------------------------------------------------//
/* End void gps_task( void *pvParameters ) */

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
extern void gps_init(void)
{
	// Local data declaration
	const char *TAG = "[gps_init]";

	/* NMEA parser configuration */
    nmea_parser_config_t config = NMEA_PARSER_CONFIG_DEFAULT();
	// Set ESP32 (Rx Pin) <---> GPS
	config.uart.rx_pin = CONFIG_NMEA_PARSER_UART_RXD;

    /* init NMEA parser library */
    nmea_parser_handle_t nmea_hdl = nmea_parser_init(&config);
    /* register event handler for NMEA parser library */
    nmea_parser_add_handler(nmea_hdl, gps_event_handler, NULL);

	/**
	 * FreeRTOS Event Group Creation
	 * **/
	gps_event_group = xEventGroupCreate();
	if(gps_event_group == 0 )
		ESP_LOGE(TAG, "gps_event_group was not created!");
    /**
     * FreeRTOS Queues Creation
     */
    // xGPSQueue = xQueueCreate(10, sizeof(gps_t));
    // if( xGPSQueue == 0)
    //     ESP_LOGE(TAG,"xGPSQueue was not created!");

    /**
     * FreeRTOS Task Creation
     */
    // xTaskCreate(&gps_task, "[gps_task]", 4096 ,NULL, configMINIMAL_STACK_SIZE + 1, &h2GpsTask);
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
extern gps_t gps_get_data(void)
{
	// return gps data
	return gps_data;
}

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