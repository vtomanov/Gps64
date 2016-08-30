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
// FIXED POINT LIB TEST

#include "Gps64.h"

void setup()
{

  Serial.begin(115200);
  for (; !Serial;) {};

  Serial.println("Gps64 TEST");
}


void loop()
{
  // in GPS_DATA raw format the second number(minSec) always need to have 9 digits
  // BUT if it start with zero(s) they need to be ommited as else they will be treated as oct number
  GPS_DATA gps_data_lat_start(40, 748610000);
  GPS_DATA gps_data_lng_start(-73, 986410000);

  GPS_DATA gps_data_lat_end(40, 748650000);
  GPS_DATA gps_data_lng_end(-73, 986450000);

  ///////////////////////////////////////////////////////
  // disto (FAST)

  double d_disto;

  {
    uint32_t so = millis();

    d_disto = disto(gps_data_lat_start,
                    gps_data_lng_start,

                    gps_data_lat_end,
                    gps_data_lng_end);

    uint32_t eo = millis();

    Serial.println("Disto " + String(d_disto, 2) + " (Fast) [ " + (eo - so) + " ] millis");

  }

  ///////////////////////////////////////////////////////
  // dist

  double d_dist;

  {

    uint32_t s = millis();

    d_dist = dist(gps_data_lat_start,
                  gps_data_lng_start,

                  gps_data_lat_end,
                  gps_data_lng_end);

    uint32_t e = millis();

    Serial.println("Dist " + String(d_dist, 2) + " [ " + (e - s) + " ] millis");

  }

  ///////////////////////////////////////////////////////
  // bearingo (FAST)

  double d_bearingo;

  {

    uint32_t s_bo = millis();

    d_bearingo = bearingo(gps_data_lat_start,
                          gps_data_lng_start,

                          gps_data_lat_end,
                          gps_data_lng_end);

    uint32_t e_bo = millis();

    Serial.println("Bearingo " + String(d_bearingo, 2) + " (Fast) [ " + (e_bo - s_bo) + " ] millis");
  }


  ///////////////////////////////////////////////////////
  // bearing

  double d_bearing;

  {

    uint32_t s_b = millis();

    d_bearing = bearing(gps_data_lat_start,
                        gps_data_lng_start,

                        gps_data_lat_end,
                        gps_data_lng_end);

    uint32_t e_b = millis();

    Serial.println("Bearing " + String(d_bearing, 2) + " [ " + (e_b - s_b) + " ] millis");
  }

  ///////////////////////////////////////////////////////
  // desto - Destination point (FAST)
  {

    GPS_DATA gps_data_lat_dest_o;
    GPS_DATA gps_data_lng_dest_o;

    uint32_t s_do = millis();

    desto(gps_data_lat_start,
          gps_data_lng_start,

          d_bearingo,
          d_disto,

          gps_data_lat_dest_o,
          gps_data_lng_dest_o);

    uint32_t e_do = millis();

    Serial.print("Desto Lat: " + String(gps_data_lat_dest_o.deg) + "." + String(gps_data_lat_dest_o.minSec));
    Serial.print("Desto Lng: " + String(gps_data_lng_dest_o.deg) + "." + String(gps_data_lng_dest_o.minSec));
    Serial.println("Desto Time: [ " + String(e_do - s_do) + " ] millis");
  }

  ///////////////////////////////////////////////////////
  // dest - Destination point
  {

    GPS_DATA gps_data_lat_dest;
    GPS_DATA gps_data_lng_dest;

    uint32_t s_d = millis();

    dest(gps_data_lat_start,
         gps_data_lng_start,

         d_bearing,
         d_dist,

         gps_data_lat_dest,
         gps_data_lng_dest);

    uint32_t e_d = millis();

    Serial.print("Dest Lat: " + String(gps_data_lat_dest.deg) + "." + String(gps_data_lat_dest.minSec));
    Serial.print("Dest Lng: " + String(gps_data_lng_dest.deg) + "." + String(gps_data_lng_dest.minSec));
    Serial.println("Dest Time: [ " + String(e_d - s_d) + " ] millis");
  }

  Serial.println();

  delay(50000);
}
