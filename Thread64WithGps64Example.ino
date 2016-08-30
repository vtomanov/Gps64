/**
   USE OF THIS SOFTWARE IS GOVERNED BY THE TERMS AND CONDITIONS
   OF THE LICENSE STATEMENT AND LIMITED WARRANTY FURNISHED WITH
   THE PRODUCT.
   <p/>
   IN PARTICULAR, YOU WILL INDEMNIFY AND HOLD ITS AUTHOR, ITS
   RELATED ENTITIES AND ITS SUPPLIERS, HARMLESS FROM AND AGAINST ANY
   CLAIMS OR LIABILITIES ARISING OUT OF THE USE, REPRODUCTION, OR
   DISTRIBUTION OF YOUR PROGRAMS, INCLUDING ANY CLAIMS OR LIABILITIES
   ARISING OUT OF OR RESULTING FROM THE USE, MODIFICATION, OR
   DISTRIBUTION OF PROGRAMS OR FILES CREATED FROM, BASED ON, AND/OR
   DERIVED FROM THIS SOURCE CODE FILE.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Background thread example
//
// This library is designed to be used in scenarios whee one foreground and one background thread is required
// e.g. when part of the calculations are heavy and need to be done in background based on asynchronous fashion without
// affecting the foreground processing working with gauges and communication


//////////////////////////////////////////////////////////////////////////////////////
// MAIN

#include "Gps64.h"
#include "Thread64.h"

// pre-recorded GPS data
#define GPS_DATA_COUNT 43

GPS_DATA lat[GPS_DATA_COUNT] = {
  GPS_DATA(  42, 260096 ),
  GPS_DATA( 42, 260285 ),
  GPS_DATA( 42, 260521 ),
  GPS_DATA( 42, 260529 ),
  GPS_DATA( 42, 260468 ),
  GPS_DATA( 42, 260427 ),
  GPS_DATA( 42, 26022  ),
  GPS_DATA( 42, 259997 ),
  GPS_DATA( 42, 25975  ),
  GPS_DATA( 42, 259503 ),
  GPS_DATA( 42, 259256 ),
  GPS_DATA( 42, 259008 ),
  GPS_DATA( 42, 258853 ),
  GPS_DATA( 42, 258718 ),
  GPS_DATA( 42, 258593 ),
  GPS_DATA( 42, 258522 ),
  GPS_DATA( 42, 258316 ),
  GPS_DATA( 42, 258065 ),
  GPS_DATA( 42, 257813 ),
  GPS_DATA( 42, 257562 ),
  GPS_DATA( 42, 25741  ),
  GPS_DATA( 42, 257447 ),
  GPS_DATA( 42, 257551 ),
  GPS_DATA( 42, 257691 ),
  GPS_DATA( 42, 257674 ),
  GPS_DATA( 42, 2579 ),
  GPS_DATA( 42, 258086 ),
  GPS_DATA( 42, 258254 ),
  GPS_DATA( 42, 258336 ),
  GPS_DATA( 42, 258462 ),
  GPS_DATA( 42, 258605 ),
  GPS_DATA( 42, 258715 ),
  GPS_DATA( 42, 258957 ),
  GPS_DATA( 42, 266766 ),
  GPS_DATA( 42, 273308 ),
  GPS_DATA( 42, 268417 ),
  GPS_DATA( 42, 258  ),
  GPS_DATA( 42, 243896 ),
  GPS_DATA( 42, 234174 ),
  GPS_DATA( 42, 232775 ),
  GPS_DATA( 42, 239575 ),
  GPS_DATA( 42, 246564 ),
  GPS_DATA( 42, 240083 )
};

GPS_DATA lng[GPS_DATA_COUNT] = {
  GPS_DATA(  3, 183401  ),
  GPS_DATA( 3, 183478  ),
  GPS_DATA( 3, 183574  ),
  GPS_DATA( 3, 183755  ),
  GPS_DATA( 3, 184096  ),
  GPS_DATA( 3, 184514  ),
  GPS_DATA( 3, 184654  ),
  GPS_DATA( 3, 184764  ),
  GPS_DATA( 3, 184844  ),
  GPS_DATA( 3, 184925  ),
  GPS_DATA( 3, 185005  ),
  GPS_DATA( 3, 185086  ),
  GPS_DATA( 3, 184992  ),
  GPS_DATA( 3, 184635  ),
  GPS_DATA( 3, 18404 ),
  GPS_DATA( 3, 183439  ),
  GPS_DATA( 3, 183396  ),
  GPS_DATA( 3, 18326 ),
  GPS_DATA( 3, 183125  ),
  GPS_DATA( 3, 18299 ),
  GPS_DATA( 3, 182912  ),
  GPS_DATA( 3, 182753  ),
  GPS_DATA( 3, 182407  ),
  GPS_DATA( 3, 181958  ),
  GPS_DATA( 3, 181392  ),
  GPS_DATA( 3, 181318  ),
  GPS_DATA( 3, 181266  ),
  GPS_DATA( 3, 181325  ),
  GPS_DATA( 3, 181724  ),
  GPS_DATA( 3, 182351  ),
  GPS_DATA( 3, 183069  ),
  GPS_DATA( 3, 183931  ),
  GPS_DATA( 3, 184808  ),
  GPS_DATA( 3, 195648  ),
  GPS_DATA( 3, 233328  ),
  GPS_DATA( 3, 272038  ),
  GPS_DATA( 3, 29607 ),
  GPS_DATA( 3, 30019 ),
  GPS_DATA( 3, 275385  ),
  GPS_DATA( 3, 237019  ),
  GPS_DATA( 3, 198309  ),
  GPS_DATA( 3, 149815  ),
  GPS_DATA( 3, 103981  )
};


GPS_DATA  def_start_lat(lat[0]);
GPS_DATA  def_start_lng(lng[0]);

// GPS current point which will be processed with default value
T64_DEF_G64( data, def_start_lat, def_start_lng );

////////////////////////////////////////////////////////////////////////////////
//foreground generate new GPS ( simulate moving ) - executes maximum once every 3 second
int32_t gps_read_lastExecute;
// 3 sec in 10's of millis
#define gps_read_timeout  (300L)


uint8_t gps_counter = 1;

void gps_read()
{
  if (!T64_DO_EXECUTE(T64_TIMER_GET(), gps_read_lastExecute, gps_read_timeout))
  {
    return;
  }
  gps_read_lastExecute = T64_TIMER_GET();

  // get next point
  if (gps_counter > GPS_DATA_COUNT)
  {
    gps_counter = 0;
  }

  GPS_DATA lat_tmp(lat[gps_counter]);
  GPS_DATA lng_tmp(lng[gps_counter]);


  if (T64_TRY_TO_G64( lat_tmp, lng_tmp, data ))
  {
    ++gps_counter;
    Serial.println("GPS(new position aquired)[" + String(lat_tmp.deg) + "." + String(lat_tmp.minSec) + " : " + String(lng_tmp.deg) + "." + String(lng_tmp.minSec) + "]");
  }
}


////////////////////////////////////////////////////////////////////////////////
//foreground gps display - executes every 100 millis and try to consume data ready from
//background
int32_t display_lastExecute;
// 100 millis  in 10's of millis
#define display_timeout  (10L)

int32_t last_data_received_at;
int32_t start_at;

void display()
{

  if (!T64_DO_EXECUTE(T64_TIMER_GET(), display_lastExecute, display_timeout))
  {
    return;
  }

  display_lastExecute = T64_TIMER_GET();

  // define variable to store the data from the processor
  double bearing_start;
  double bearing_current;
  double distance_start;
  double distance_current;
  // try receive data from the background processor if available
  if (T64_TRY_FROM_G64( bearing_start, bearing_current, distance_start, distance_current, data ))
  {
    // data is received
    int32_t received_at_tmp = last_data_received_at;
    int32_t now_tmp = T64_TIMER_GET();
    last_data_received_at = T64_TIMER_GET();

    double nm_current = distance_current / 1852.0d;
    double nm_start = distance_start / 1852.0d;
    double hr_current = (((double)now_tmp) - ((double)received_at_tmp)) / 360000.0d;
    double hr_start = (((double)now_tmp) - ((double)start_at)) / 360000.0d;

    double speed_current = nm_current / hr_current;
    double speed_start = nm_start / hr_start;

    Serial.println("Display:  Distance from start[" + String(distance_start, 2) + "] Bearing from start[" + String(bearing_start, 2) + "]  Average speed from start["  + String(speed_start, 2) + "]");
    Serial.println("Display:  Distance from last[" + String(distance_current, 2) + "] Bearing [" + String(bearing_current, 2) + "] Speed[" + String(speed_current, 2) + "]");
    Serial.println();
  }

}


////////////////////////////////////////////////////////////////////////////////
// background gps calculations - executes every 10 millis and try processing the data if available
//
int32_t calc_lastExecute;
// 10 millis  in 10's of millis
#define calc_timeout  (1L)

void  calc()
{

  if (!T64_DO_EXECUTE(T64_TIMER_GET(), calc_lastExecute, calc_timeout))
  {
    return;
  }
  calc_lastExecute = T64_TIMER_GET();

  // temporary storage
  GPS_DATA current_lat;
  GPS_DATA current_lng;
  GPS_DATA start_lat;
  GPS_DATA start_lng;
  GPS_DATA last_lat;
  GPS_DATA last_lng;

  // try get data for processing if available
  if (T64_TRY_GET_G64( current_lat, current_lng, start_lat, start_lng, last_lat, last_lng, data ))
  {
    double bearing_start = bearingo(start_lat,
                                    start_lng,

                                    current_lat,
                                    current_lng);
    double bearing_current = bearingo(last_lat,
                                      last_lng,

                                      current_lat,
                                      current_lng);
    double distance_start = disto(start_lat,
                                  start_lng,

                                  current_lat,
                                  current_lng);
    double distance_current = disto(last_lat,
                                    last_lng,

                                    current_lat,
                                    current_lng);

    if (T64_TRY_SET_G64( bearing_start, bearing_current, distance_start, distance_current, data ))
    {
      // data has been actually processed and left for consumer
    }
  }

}

////////////////////////////////////////////////////////////////////////////////
//foreground set start to last known good current position - executes maximum once every 60 sec
int32_t reset_start_lastExecute;
// 60 sec  in 10's of millis
#define reset_start_timeout  (6000L)

void reset_start()
{
  if (!T64_DO_EXECUTE(T64_TIMER_GET(), reset_start_lastExecute, reset_start_timeout))
  {
    return;
  }
  reset_start_lastExecute = T64_TIMER_GET();

  // set start to lst job here
  GPS_DATA lat_tmp;
  GPS_DATA lng_tmp;

  start_at = T64_TIMER_GET();

  T64_GET_LAST_G64( lat_tmp, lng_tmp, data );
  T64_SET_START_G64( lat_tmp, lng_tmp, data );

  Serial.println();
  Serial.println("Start position changed to [" + String(lat_tmp.deg) + "." + String(lat_tmp.minSec) + " : " + String(lng_tmp.deg) + "." + String(lng_tmp.minSec) + "]");
  Serial.println();
}


void setup()
{
  // Init Serial
  Serial.begin(115200);
  for (; !Serial;) {};

  Serial.println("Thread64/Gps64 Example");

  // Thread64 ( background function & background stack size )
  T64_INIT(calc, 1280);

  // make sure timer has started
  for (; T64_TIMER_GET() != 0;) {};

  // set now forstarting gps point
  last_data_received_at = T64_TIMER_GET();
  start_at = T64_TIMER_GET();


}

void loop()
{

  gps_read();

  display();

  // background calculation is processor calc()

  reset_start();


  delay(1);
}

