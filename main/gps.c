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

#define TIME_ZONE (3)   	//Argentina Time
#define YEAR_BASE (2000) 	//date in GPS starts from 2000

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
TaskHandle_t h2GpsTask = NULL;
//----------------------------------------------------

// Queue Handlers ------------------------------------
QueueHandle_t xGPSQueue;
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
			// Send data to apply task over freeRTOS queue
            xQueueSend(xGPSQueue, gps, portMAX_DELAY);

            /* print information parsed from GPS statements */
            // ESP_LOGI("[gps_event_handler]", "%d/%d/%d %d:%d:%d => \r\n"
            //         "\t\t\t\t\t\tlatitude    = %.05f °N\r\n"
            //         "\t\t\t\t\t\tlongtitude  = %.05f °E\r\n"
            //         "\t\t\t\t\t\taltitude    = %.02f [m]\r\n"
            //         "\t\t\t\t\t\tspeed       = %f [m/s]\r\n"
            //         "\t\t\t\t\t\tSat in view = %d\r\n"
            //         "\t\t\t\t\t\tSat in use  = %d\r\n",
            //         gps->date.year + YEAR_BASE, gps->date.month, gps->date.day,
            //         gps->tim.hour + TIME_ZONE, gps->tim.minute, gps->tim.second,
            //         gps->latitude, gps->longitude, gps->altitude, gps->speed,
            //         gps->sats_in_view, gps->sats_in_use);

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
            ESP_LOGI(TAG, "%d/%d/%d %d:%d:%d => \r\n"
                    "\t\t\t\t\t\tlatitude    = %.05f °N\r\n"
                    "\t\t\t\t\t\tlongtitude  = %.05f °E\r\n"
                    "\t\t\t\t\t\taltitude    = %.02f [m]\r\n"
                    "\t\t\t\t\t\tspeed       = %f [m/s]\r\n"
                    "\t\t\t\t\t\tSat in view = %d\r\n"
                    "\t\t\t\t\t\tSat in use  = %d\r\n",
                     gps.date.day, gps.date.month, gps.date.year + YEAR_BASE,
                    //gps.tim.hour + TIME_ZONE, gps.tim.minute, gps.tim.second,
                    gps.tim.hour, gps.tim.minute, gps.tim.second,
                    gps.latitude, gps.longitude, gps.altitude, gps.speed,
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
	/* NMEA parser configuration */
    nmea_parser_config_t config = NMEA_PARSER_CONFIG_DEFAULT();
	// Set ESP32 (Rx Pin) <---> GPS
	config.uart.rx_pin = CONFIG_NMEA_PARSER_UART_RXD;

    /* init NMEA parser library */
    nmea_parser_handle_t nmea_hdl = nmea_parser_init(&config);
    /* register event handler for NMEA parser library */
    nmea_parser_add_handler(nmea_hdl, gps_event_handler, NULL);

    /**
     * FreeRTOS Queus Creation
     */
    xGPSQueue = xQueueCreate(10, sizeof(gps_t));
    if( xGPSQueue == 0)
        ESP_LOGE("[app_main]","xGPSQueue was not created!");

    /**
     * FreeRTOS Task Creation
     */
    xTaskCreate(&gps_task, "[gps_task]", 4096 ,NULL, configMINIMAL_STACK_SIZE + 1, &h2GpsTask);
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