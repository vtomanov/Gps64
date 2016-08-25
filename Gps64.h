/**
   USE OF THIS SOFTWARE IS GOVERNED BY THE TERMS AND CONDITIONS
   OF THE LICENSE STATEMENT AND LIMITED WARRANTY FURNISHED WITH
   THE PRODUCT.
   <p/>
   IN PARTICULAR, YOU WILL INDEMNIFY AND HOLD B2N LTD., ITS
   RELATED COMPANIES AND ITS SUPPLIERS, HARMLESS FROM AND AGAINST ANY
   CLAIMS OR LIABILITIES ARISING OUT OF THE USE, REPRODUCTION, OR
   DISTRIBUTION OF YOUR PROGRAMS, INCLUDING ANY CLAIMS OR LIABILITIES
   ARISING OUT OF OR RESULTING FROM THE USE, MODIFICATION, OR
   DISTRIBUTION OF PROGRAMS OR FILES CREATED FROM, BASED ON, AND/OR
   DERIVED FROM THIS SOURCE CODE FILE.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GPS coordinates based calculations using Float64/Math64

#include <Float64.h>
#include <Math64.h>

struct GPS_DATA
{
  GPS_DATA() {};
  GPS_DATA(const GPS_DATA & gps_data): deg(gps_data.deg), minSec(gps_data.minSec) {};
  GPS_DATA(const int16_t deg_, const uint32_t minSec_) : deg(deg_), minSec(minSec_) {};
  
  int16_t deg;
  uint32_t minSec;
};

//////////////////////////////////////////////////////////////////////////////////////
// ALL AVAILABLE FUNCTIONS ( .h )

//////////////////////////////////////////////////////////////////////////////////////
// FINAL PREFERRED FUNCTIONS

// Fast distance between two coordinates in meters ~150 msec
inline f64 dist64o(f64 & latStart,
                   f64 & lngStart,

                   f64 & latEnd,
                   f64 & lngEnd);

// Fast distance between two coordinates in meters ~150msec
inline double disto(GPS_DATA & latStart,
                    GPS_DATA & lngStart,

                    GPS_DATA & latEnd,
                    GPS_DATA & lngEnd);

// Bearing between two coordinates in degrees ~170msec
inline f64 bearing64o(f64 & latStart,
                      f64 & lngStart,

                      f64 & latEnd,
                      f64 & lngEnd);

// Bearing between two coordinates in degrees ~170msec
inline double bearingo(GPS_DATA & latStart,
                       GPS_DATA & lngStart,

                       GPS_DATA & latEnd,
                       GPS_DATA & lngEnd);

// Destination point based on start coordinates in dewgrees , bearing in degree and distance in meters ~380msec
// Use with caution - may be not exact to about 10-20 meters 
inline void dest64o(f64 & latStart,
                    f64 & lngStart,

                    f64 & bearing,
                    f64 & distance,
                   
                    f64 & latEnd,
                    f64 & lngEnd);
                   
// Destination point based on start coordinates in dewgrees , bearing in degree and distance in meters ~380msec
// Use with caution - may be not exact to about 10-20 meters
inline void desto(GPS_DATA & latStart,
                  GPS_DATA & lngStart,

                  double & bearing,
                  double & distance,
                 
                  GPS_DATA & latEnd,
                  GPS_DATA & lngEnd);                      

//////////////////////////////////////////////////////////////////////////////////////
// LOW PERF HI-ACCURANCY FUNCTIONS

// Distance between two coordinates in meters ~12sec 
inline f64 dist64(f64 & latStart,
                   f64 & lngStart,

                   f64 & latEnd,
                   f64 & lngEnd);

// Distance between two coordinates in meters ~12sec
inline double dist(GPS_DATA & latStart,
                    GPS_DATA & lngStart,

                    GPS_DATA & latEnd,
                    GPS_DATA & lngEnd);

// Bearing between two coordinates in degrees ~2sec
inline f64 bearing64(f64 & latStart,
                   f64 & lngStart,

                   f64 & latEnd,
                   f64 & lngEnd);

// Bearing between two coordinates in degrees ~2sec
inline double bearing(GPS_DATA & latStart,
                       GPS_DATA & lngStart,

                       GPS_DATA & latEnd,
                       GPS_DATA & lngEnd);

// Destination point based on start coordinates in dewgrees , bearing in degree and distance in meters ~ 5sec
inline void dest64(f64 & latStart,
                   f64 & lngStart,

                   f64 & bearing,
                   f64 & distance,
                   
                   f64 & latEnd,
                   f64 & lngEnd);
                   
// Destination point based on start coordinates in dewgrees , bearing in degree and distance in meters ~5sec
inline void dest(GPS_DATA & latStart,
                 GPS_DATA & lngStart,

                 double & bearing,
                 double & distance,
                 
                 GPS_DATA & latEnd,
                 GPS_DATA & lngEnd);
                   
//////////////////////////////////////////////////////////////////////////////////////
// GENERIC FUNCTIONS

// Convert between f64 and double to maximum posible precision
inline double f64tod(const f64 & r);
inline f64 dtof64(const double & d);

// Convert between raw GPS data to f64
inline GPS_DATA f64tog(const f64 & rhs);
inline f64 gtof64 (const GPS_DATA & gps_data);

// Convert between degree and radians
inline f64 dtor64(const f64 & d);
inline f64 rtod64(const f64 & r);

// Fast Min/Max for F64
inline f64 min64o(const f64 & x, const f64 & y);
inline f64 max64o(const f64 & x, const f64 & y);
inline f64 abs64o(const f64 & rhs);

// Fast sin/cos with possible with error less then 1.8e-7
// Do not use for low opf cosine distance as need more precision
// Designed for use in haversine formula
inline f64 sin64o(const f64 & a);
inline f64 cos64o(const f64 & a);

// Very fast sin error < 0.001 %
// Do not use for distance, bearing or destination calculations - not enough prec
inline f64 sin64oo(const f64 & rhs);
inline f64 cos64oo(const f64 & rhs);

// Fast and exact sqrt based on Babylonian Method
inline f64 sqrt64o(const f64 & rhs);

// Fast acos/asin  possible with error less then 1.8e-7
inline f64 acos64o(const f64 & rhs);
inline f64 asin64o(const f64 & rhs);
inline f64 atan264o(const f64 & y, const f64 & x);

//////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATIONS ( .cpp )

// Convert f64 to double to maximum posible precision
inline double f64tod(const f64 & r)
{
  double d;
  
  if (r > f64(16777216L))
  {
    d = 16777216.0d;
  }
  else
  {

    f64 tmp(r);

    if (r > f64(1000000L))
    {
      // tmp *= f64(1L);
      d =  static_cast<double>(tmp.ipart()); // / 1.0d
    }
    else if (r > f64(100000L))
    {
      tmp *= f64(10L);
      d = static_cast<double>(tmp.ipart()) / 10.0d;
    }
    else if (r > f64(10000L))
    {
      tmp *= f64(100L);
      d = static_cast<double>(tmp.ipart()) / 100.0d;
    }
    else if (r > f64(1000L))
    {
      tmp *= f64(1000L);
      d = static_cast<double>(tmp.ipart()) / 1000.0d;
    }
    else if (r > f64(100L))
    {
      tmp *= f64(10000L);
      d = static_cast<double>(tmp.ipart()) / 10000.0d;
    }
    else if (r > f64(10L))
    {
      tmp *= f64(100000L);
      d = static_cast<double>(tmp.ipart()) / 100000.0d;
    }
    else
      // if (r > f64(1L))
    {
      tmp *= f64(1000000L);
      d = static_cast<double>(tmp.ipart()) / 1000000.0d;
    }
  }

  return d;
}

// Convert double f64 to maximum posible precision
inline f64 dtof64(const double & d)
{
    f64 r;
     
    double tmp = d;
    if (d > 1000000.0d)
    {
      // tmp *= 1.0d;
      r =  f64(static_cast<int32_t>(tmp));  // / f64(1L)
    }
    else if (d > 100000.0d)
    {
      tmp *= 10.0d;
      r = f64(static_cast<int32_t>(tmp)) / f64(10L);
    }
    else if (d > 10000.0d)
    {
      tmp *= 100.0d;
      r = f64(static_cast<int32_t>(tmp)) / f64(100L);
    }
    else if (d > 1000.0d)
    {
      tmp *= 1000.0d;
      r = f64(static_cast<int32_t>(tmp)) / f64(1000L);
    }
    else if (d > 100.0d)
    {
      tmp *= 10000.0d;
      r = f64(static_cast<int32_t>(tmp)) / f64(10000L);
    }
    else if (d > 10.0d)
    {
      tmp *= 100000.0d;
      r = f64(static_cast<int32_t>(tmp)) / f64(100000L);
    }
    else
      // if (d > 1.0d)
    {
      tmp *= 1000000.0d;
      r = f64(static_cast<int32_t>(tmp)) / f64(1000000L);
    }

    return r;
}

// Convert raw GPS data to f64
inline f64 gtof64 (const GPS_DATA & gps_data)
{
  f64 rhs(gps_data.deg);
  f64 ret_frac(gps_data.minSec);
  ret_frac /= f64(1000000000L);
  if (gps_data.deg > 0)
  {
    rhs += ret_frac;
  }
  else
  {
    rhs -= ret_frac;
  }

  return rhs;
}

// Fast abs
inline f64 abs64o(const f64 & rhs)
{
   f64 x(rhs);
   return (x >= f64(0L) ? x : -x);
}

// Fast max
inline f64 max64o(const f64 & x, const f64 & y)
{
   if (x > y)
   {
      return f64(x);
   }

   return f64(y);
}

// Fast min
inline f64 min64o(const f64 & x, const f64 & y)
{
   if (y > x)
   {
      return f64(x);
   }

   return f64(y);
}

// Convert f64 to raw GPS data
inline GPS_DATA f64tog(const f64 & rhs)
{
  GPS_DATA gps_data;
  gps_data.deg = static_cast<int16_t>(rhs.ipart());
  int32_t iint = static_cast<int32_t>(rhs.ipart());
  f64 fint(iint);
  f64 frac(rhs - fint);
  frac *= f64(1000000000L);
  if (rhs.isNegative())
  {
    gps_data.minSec = static_cast<uint32_t>(abs64o(frac).ipart());
  }
  else
  {
    gps_data.minSec = static_cast<uint32_t>(frac.ipart());
  }

  return gps_data;
}

// Convert degree to radians
inline f64 dtor64(const f64 & d)
{
  f64 r(d);
  r *= f64(0x3FF921FB, 0x54442D18) / f64(90L);
  return r;
}

// Convert radians to degree
inline f64 rtod64(const f64 & r)
{
  f64 d(r);
  d /= f64(0x3FF921FB, 0x54442D18) / f64(90L);
  return d;
}

// Fast sin possible with error less then 1.8e-7
inline f64 sin64o(const f64 & a)
{
  /* C simulation gives a max absolute error of less than 1.8e-7 */
  f64 c0x(0L); // 0.0
  f64 c0y(f64(5L) / f64(10L)); // 0.5
  f64 c0z(1L); // 1.0
  f64 c0w(0L); // 0.0

  f64 c1x(f64(25L) / f64(100L)); // 0.25
  f64 c1y(-9L); // -9.0
  f64 c1z(f64(75L) / f64(100L)); // 0.75
  f64 c1w(((f64(154943091L) / f64(1000000000L)) + f64(159L)) / f64(1000L)); // 0.159154943091
    
  f64 c2x(((f64(808039603L) / f64(1000000000L)) + f64(249L)) / f64(10L)); // 24.9808039603
  f64 c2y(f64(0L) - c2x); // -24.9808039603
  f64 c2z(f64(0L) - (((f64(458091736L) / f64(1000000000L)) + f64(601L)) / f64(10L))); // -60.1458091736
  f64 c2w(f64(0L) - c2z); // 60.1458091736

  f64 c3x(((f64(537887573L) / f64(1000000000L)) + f64(854L)) / f64(10L)); // 85.4537887573  
  f64 c3y(f64(0L) - c3x); // -85.4537887573
  f64 c3z(f64(0L) - (((f64(393539429L) / f64(1000000000L)) + f64(649L)) / f64(10L))); // -64.9393539429
  f64 c3w(f64(0L) - c3z); //64.9393539429
    
  f64 c4x(((f64(392082214L) / f64(1000000000L)) + f64(197L)) / f64(10L)); // 19.7392082214
  f64 c4y(f64(0L) - c4x); // -19.7392082214
  f64 c4z(-1L);
  f64 c4w(1L);
       
  /* r0x = sin(a) */
  f64 r0x;
  f64 r0y;
  f64 r0z;

  f64 r1x;
  f64 r1y;
  f64 r1z;

  f64 r2x;
  f64 r2y;
  f64 r2z;

  r1x =  c1w * a - c1x;                                      // normalize input . only difference from cos!
  
  r1y  = f64( r1x - f64(static_cast<int32_t>(r1x.ipart()))); // and extract fraction
  
  r2x  = f64( (r1y < c1x) ? 1L : 0L );                       // range check: 0.0 to 0.25
  
  r2y =  f64( (r1y >= c1y) ? 1L : 0L);                       // range check: 0.75 to 1.0
  r2z =  f64( (r1y >= c1z) ? 1L : 0L);                       //
  
  r2y = r2x * c4z + r2y * c4w + r2z * c4z ;                  // range check: 0.25 to 0.75
  
  r0x = c0x - r1y;                                           // range centering
  r0y = c0y - r1y;                                           // 
  r0z = c0z - r1y;                                           //
  
  r0x = r0x * r0x;
  r0y = r0y * r0y;
  r0z = r0z * r0z;
  
  r1x = c2x * r0x + c2z;           // start power series
  r1y = c2y * r0y + c2w;           // 
  r1z = c2x * r0z + c2z;           // 
  
  r1x = r1x * r0x + c3x;
  r1y = r1y * r0y + c3y;
  r1z = r1z * r0z + c3x;
  
  r1x = r1x * r0x + c3z;
  r1y = r1y * r0y + c3w;
  r1z = r1z * r0z + c3z;
  
  r1x = r1x * r0x + c4x;
  r1y = r1y * r0y + c4y;
  r1z = r1z * r0z + c4x;
  
  r1x = r1x * r0x + c4z;
  r1y = r1y * r0y + c4w;
  r1z = r1z * r0z + c4z;
  
  r0x  = r1x * (f64(0L) - r2x) + r1y * (f64(0L) - r2y) + r1z * (f64(0L) - r2z);// range extract

  return r0x;
}

// Fast cos possible with error less then 1.8e-7
inline f64 cos64o(const f64 & a)
{
  /* C simulation gives a max absolute error of less than 1.8e-7 */
  f64 c0x(0L); // 0.0
  f64 c0y(f64(5L) / f64(10L)); // 0.5
  f64 c0z(1L); // 1.0
  f64 c0w(0L); // 0.0

  f64 c1x(f64(25L) / f64(100L)); // 0.25
  f64 c1y(-9L); // -9.0
  f64 c1z(f64(75L) / f64(100L)); // 0.75
  f64 c1w(((f64(154943091L) / f64(1000000000L)) + f64(159L)) / f64(1000L)); // 0.159154943091
    
  f64 c2x(((f64(808039603L) / f64(1000000000L)) + f64(249L)) / f64(10L)); // 24.9808039603
  f64 c2y(f64(0L) - c2x); // -24.9808039603
  f64 c2z(f64(0L) - (((f64(458091736L) / f64(1000000000L)) + f64(601L)) / f64(10L))); // -60.1458091736
  f64 c2w(f64(0L) - c2z); // 60.1458091736

  f64 c3x(((f64(537887573L) / f64(1000000000L)) + f64(854L)) / f64(10L)); // 85.4537887573  
  f64 c3y(f64(0L) - c3x); // -85.4537887573
  f64 c3z(f64(0L) - (((f64(393539429L) / f64(1000000000L)) + f64(649L)) / f64(10L))); // -64.9393539429
  f64 c3w(f64(0L) - c3z); //64.9393539429
    
  f64 c4x(((f64(392082214L) / f64(1000000000L)) + f64(197L)) / f64(10L)); // 19.7392082214
  f64 c4y(f64(0L) - c4x); // -19.7392082214
  f64 c4z(-1L);
  f64 c4w(1L);
       
  /* r0x = cos(a) */
  f64 r0x;
  f64 r0y;
  f64 r0z;

  f64 r1x;
  f64 r1y;
  f64 r1z;

  f64 r2x;
  f64 r2y;
  f64 r2z;

  r1x =  c1w * a;                                            // normalize input
  
  r1y  = f64( r1x - f64(static_cast<int32_t>(r1x.ipart()))); // and extract fraction
  
  r2x  = f64( (r1y < c1x) ? 1L : 0L );                       // range check: 0.0 to 0.25
  
  r2y =  f64( (r1y >= c1y) ? 1L : 0L);                       // range check: 0.75 to 1.0
  r2z =  f64( (r1y >= c1z) ? 1L : 0L);                       //
  
  r2y = r2x * c4z + r2y * c4w + r2z * c4z ;                  // range check: 0.25 to 0.75
  
  r0x = c0x - r1y;                                           // range centering
  r0y = c0y - r1y;                                           // 
  r0z = c0z - r1y;                                           //
  
  r0x = r0x * r0x;
  r0y = r0y * r0y;
  r0z = r0z * r0z;
  
  r1x = c2x * r0x + c2z;           // start power series
  r1y = c2y * r0y + c2w;           // 
  r1z = c2x * r0z + c2z;           // 
  
  r1x = r1x * r0x + c3x;
  r1y = r1y * r0y + c3y;
  r1z = r1z * r0z + c3x;
  
  r1x = r1x * r0x + c3z;
  r1y = r1y * r0y + c3w;
  r1z = r1z * r0z + c3z;
  
  r1x = r1x * r0x + c4x;
  r1y = r1y * r0y + c4y;
  r1z = r1z * r0z + c4x;
  
  r1x = r1x * r0x + c4z;
  r1y = r1y * r0y + c4w;
  r1z = r1z * r0z + c4z;
  
  r0x  = r1x * (f64(0L) - r2x) + r1y * (f64(0L) - r2y) + r1z * (f64(0L) - r2z);// range extract

  return r0x;
}

// Very fast sin error < 0.001 %
inline f64 sin64oo(const f64 & rhs)
{
    f64 B_(f64(4L)  / (f64(0x3FF921FB, 0x54442D18) * f64(2L)));
    f64 C_(f64(-4L) / ((f64(0x3FF921FB, 0x54442D18) * f64(2L)) * (f64(0x3FF921FB, 0x54442D18) * f64(2L))));
    f64 P_(f64(225L) / f64(1000L));

    f64 x(rhs);
    f64 y((B_ * x) + (C_ * x * abs64o(x)));
    y = (P_ * ((y * abs64o(y)) - y)) + y;
    
    return y;
}

// Very fast cos error < 0.001 %
inline f64 cos64oo(const f64 & rhs)
{ 
  f64 PI2_(0x3FF921FB, 0x54442D18);
  f64 PI_(PI2_ * f64(2L));
  f64 P2I_(PI_ * f64(2L));
  f64 x(rhs);
  
  x += PI2_;
  if(x > PI_)
  {
    x -= P2I_;   
  }

  return  sin64oo(x);
}

// Fast sqrt based on Babylonian Method
inline f64 sqrt64o(const f64 & rhs)
{  
 f64 x(rhs / f64(2L));
 f64 t(0L) ; 
 
 for(;x != t;)
 { 
    t = x;
    x = (x + rhs/x) / f64(2L); 
 }
 
 return x; 
}
  
// Fast acos  possible with error less then 1.8e-7
inline f64 acos64o(const f64 & rhs) 
{ 
  f64 negate = f64(rhs < f64(0L) ? 1L : 0L);
  f64 x(abs64o(rhs));
  
  f64 ret(f64(-187293L) / f64(10000000L));
  ret *= x;
  ret += f64(742610L) / f64(10000000L);
  ret *= x;
  ret -= f64(2121144L) / f64(10000000L);
  ret *= x;
  ret += f64(15707288L) / f64(10000000L);
  ret *= sqrt64o(f64(1L) - x);
  ret -= f64(2L) * negate * ret;
  ret += negate * f64(f64(0x3FF921FB, 0x54442D18) * f64(2L));
  return ret;
}

// Fast asin  possible with error less then 1.8e-7
inline f64 asin64o(const f64 & rhs) 
{
  f64 negate = f64(rhs < f64(0L) ? 1L : 0L);
  f64 x(abs64o(rhs));
  f64 ret(f64(-187293L) / f64(10000000L));
  ret *= x;
  ret += f64(742610L) / f64(10000000L);
  ret *= x;
  ret -= f64(2121144L) / f64(10000000L);
  ret *= x;
  ret += f64(15707288L) / f64(10000000L);
  ret = f64(0x3FF921FB, 0x54442D18) - sqrt64o(f64(1L) - x) * ret;
  return ret - f64(2L) * negate * ret;
}

// Fast astan possible with error less then 1.8e-7
inline f64 atan264o(const f64 & y, const f64 & x)
{

  f64 t3(abs64o(x));
  f64 t1(abs64o(y));
  f64 t0(max64o(t3, t1));
  t1 = min64o(t3, t1);
  t3 = f64(1L) / t0;
  t3 = t1 * t3;

  f64 t4(t3 * t3);
  t0 =         - (f64(13480470L) / f64(1000000000L));
  t0 = t0 * t4 + (f64(57477314L) / f64(1000000000L)) ;
  t0 = t0 * t4 - (f64(121239071L) / f64(1000000000L));
  t0 = t0 * t4 + (f64(195635925L) / f64(1000000000L));
  t0 = t0 * t4 - (f64(332994597L) / f64(1000000000L));
  t0 = t0 * t4 + (f64(999995630L) / f64(1000000000L));
  t3 = t0 * t3;

  t3 = (abs64o(y) > abs64o(x)) ? f64(0x3FF921FB, 0x54442D18) - t3 : t3;
  t3 = (x < f64(0L)) ?  (f64(0x3FF921FB, 0x54442D18) * f64(2L)) - t3 : t3;
  t3 = (y < f64(0L)) ? f64(0L) - t3 : t3;

  return t3;
}

// Fast distance between two coordinates in meters
inline f64 dist64o(f64 & latStart,
                   f64 & lngStart,

                   f64 & latEnd,
                   f64 & lngEnd)
{
  f64 _latStart(dtor64(latStart));
  f64 _lngStart(dtor64(lngStart));

  f64 _latEnd(dtor64(latEnd));
  f64 _lngEnd(dtor64(lngEnd));

  f64 dlon(_lngEnd - _lngStart);
  f64 dlat(_latEnd - _latStart);

  f64 sin64_dlat(sin64o(dlat/f64(2L)));
  f64 sin64_dlon(sin64o(dlon/f64(2L)));
  
  f64 a(sin64_dlat * sin64_dlat + cos64o(_latStart) * cos64o(_latEnd) * sin64_dlon * sin64_dlon);
  f64 c(f64(2L) * atan264o(sqrt64o(a), sqrt64o(f64(1L)-a))); 
  return f64(6371000L) * c;
}

// Distance between two coordinates in meters
inline f64 dist64(f64 & latStart,
                   f64 & lngStart,

                   f64 & latEnd,
                   f64 & lngEnd)
{
  
  f64 _latStart(dtor64(latStart));
  f64 _lngStart(dtor64(lngStart));

  f64 _latEnd(dtor64(latEnd));
  f64 _lngEnd(dtor64(lngEnd));
  
  return acos64( (sin64(_latStart) * sin64(_latEnd)) + (cos64(_latStart) * cos64(_latEnd) * cos64(_lngEnd - _lngStart)) ) * f64(6371000L);
 
}

// Distance between two coordinates in meters
inline double disto(GPS_DATA & latStart,
                    GPS_DATA & lngStart,

                    GPS_DATA & latEnd,
                    GPS_DATA & lngEnd)
{
  f64 _latStart(gtof64(latStart));
  f64 _lngStart(gtof64(lngStart));

  f64 _latEnd(gtof64(latEnd));
  f64 _lngEnd(gtof64(lngEnd));

  return f64tod(
    dist64o(_latStart,
           _lngStart,

           _latEnd,
           _lngEnd));
}

// Distance between two coordinates in meters
inline double dist(GPS_DATA & latStart,
                   GPS_DATA & lngStart,

                   GPS_DATA & latEnd,
                   GPS_DATA & lngEnd)
{
  f64 _latStart(gtof64(latStart));
  f64 _lngStart(gtof64(lngStart));

  f64 _latEnd(gtof64(latEnd));
  f64 _lngEnd(gtof64(lngEnd));

  return f64tod(
    dist64(_latStart,
           _lngStart,

           _latEnd,
           _lngEnd));
}

// Return bearing between two coordinates in dewgrees
inline f64 bearing64o(f64 & latStart,
                      f64 & lngStart,

                      f64 & latEnd,
                      f64 & lngEnd)
{
  
  f64 _latStart(dtor64(latStart));
  f64 _lngStart(dtor64(lngStart));

  f64 _latEnd(dtor64(latEnd));
  f64 _lngEnd(dtor64(lngEnd));

  //ATAN2(SIN(lon2-lon1)*COS(lat2), COS(lat1)*SIN(lat2)-SIN(lat1)*COS(lat2)*COS(lon2-lon1)) 
  f64 _bearing = atan264o(sin64o(_lngEnd-_lngStart) *cos64o(_latEnd), cos64o(_latStart) * sin64o(_latEnd) - sin64o(_latStart) * cos64o(_latEnd) * cos64o(_lngEnd-_lngStart)); 

  f64 bearing(rtod64(_bearing));
  for (;bearing < f64(0L); bearing += f64(360L)) {};
  return bearing;
}

// Return bearing between two coordinates in degrees
inline double bearingo(GPS_DATA & latStart,
                       GPS_DATA & lngStart,

                       GPS_DATA & latEnd,
                       GPS_DATA & lngEnd)
{
  f64 _latStart(gtof64(latStart));
  f64 _lngStart(gtof64(lngStart));

  f64 _latEnd(gtof64(latEnd));
  f64 _lngEnd(gtof64(lngEnd));

  return f64tod(
    bearing64o(_latStart,
               _lngStart,

               _latEnd,
               _lngEnd));
}

// Return bearing between two coordinates in dewgrees
inline f64 bearing64(f64 & latStart,
                   f64 & lngStart,

                   f64 & latEnd,
                   f64 & lngEnd)
{
  
  f64 _latStart(dtor64(latStart));
  f64 _lngStart(dtor64(lngStart));

  f64 _latEnd(dtor64(latEnd));
  f64 _lngEnd(dtor64(lngEnd));

  //ATAN2(SIN(lon2-lon1)*COS(lat2), COS(lat1)*SIN(lat2)-SIN(lat1)*COS(lat2)*COS(lon2-lon1)) 
  f64 _bearing = atan264(sin64(_lngEnd-_lngStart) *cos64(_latEnd), cos64(_latStart) * sin64(_latEnd) - sin64(_latStart) * cos64(_latEnd) * cos64(_lngEnd-_lngStart)); 
  
  f64 bearing(rtod64(_bearing));
  for (;bearing < f64(0L); bearing += f64(360L)) {};
  return bearing;
}

// Return bearing between two coordinates in degrees
inline double bearing(GPS_DATA & latStart,
                      GPS_DATA & lngStart,

                      GPS_DATA & latEnd,
                      GPS_DATA & lngEnd)
{
  f64 _latStart(gtof64(latStart));
  f64 _lngStart(gtof64(lngStart));

  f64 _latEnd(gtof64(latEnd));
  f64 _lngEnd(gtof64(lngEnd));

  return f64tod(
    bearing64(_latStart,
              _lngStart,

              _latEnd,
              _lngEnd));
}

// Destination point based on start coordinates in dewgrees , bearing in degree and distance in meters
inline void dest64o(f64 & latStart,
                    f64 & lngStart,

                    f64 & bearing,
                    f64 & distance,
                   
                    f64 & latEnd,
                    f64 & lngEnd)
{
   f64 _latStart(dtor64(latStart));
   f64 _lngStart(dtor64(lngStart));

   f64 _bearing(dtor64(bearing));
   
   //lat2: =ASIN(SIN(lat1)*COS(d/R) + COS(lat1)*SIN(d/R)*COS(brng))
   //lon2: =lon1 + ATAN2(SIN(brng)*SIN(d/R)*COS(lat1),COS(d/R)-SIN(lat1)*SIN(lat2)) 
   f64 d_r((distance / f64(1000L)) / f64(6371000L));
   f64 _latEnd(asin64o(sin64o(_latStart) * cos64o(d_r) + cos64o(_latStart) * sin64o(d_r) * cos64o(_bearing))); 
   f64 _lngEnd(_lngStart + atan264o(sin64(_bearing) * sin64o(d_r) * cos64o(_latStart), cos64o(d_r) - sin64o(_latStart) * sin64(_latEnd)));

   latEnd = rtod64(_latEnd);
   lngEnd = rtod64(_lngEnd);
}

// Destination point based on start coordinates in dewgrees , bearing in degree and distance in meters
inline void dest64(f64 & latStart,
                   f64 & lngStart,

                   f64 & bearing,
                   f64 & distance,
                   
                   f64 & latEnd,
                   f64 & lngEnd)
{
   f64 _latStart(dtor64(latStart));
   f64 _lngStart(dtor64(lngStart));

   f64 _bearing(dtor64(bearing));
   
   //lat2: =ASIN(SIN(lat1)*COS(d/R) + COS(lat1)*SIN(d/R)*COS(brng))
   //lon2: =lon1 + ATAN2(SIN(brng)*SIN(d/R)*COS(lat1),COS(d/R)-SIN(lat1)*SIN(lat2)) 
   f64 d_r((distance / f64(1000L)) / f64(6371000L));
   f64 _latEnd(asin64(sin64(_latStart) * cos64(d_r) + cos64(_latStart) * sin64(d_r) * cos64(_bearing))); 
   f64 _lngEnd(_lngStart + atan264(sin64(_bearing) * sin64(d_r) * cos64(_latStart), cos64(d_r) - sin64(_latStart) * sin64(_latEnd)));

   latEnd = rtod64(_latEnd);
   lngEnd = rtod64(_lngEnd);
}

// Destination point based on start coordinates in dewgrees , bearing in degree and distance in meters
inline void desto(GPS_DATA & latStart,
                  GPS_DATA & lngStart,

                  double & bearing,
                  double & distance,
                 
                  GPS_DATA & latEnd,
                  GPS_DATA & lngEnd)
{
  f64 _latStart(gtof64(latStart));
  f64 _lngStart(gtof64(lngStart));

  f64 _bearing(dtof64(bearing));
  f64 _distance(dtof64(distance));

  f64 _latEnd;
  f64 _lngEnd;
  
  dest64o(_latStart,
         _lngStart,

         _bearing,
         _distance, 
         
         _latEnd,
         _lngEnd);
         
 latEnd = f64tog(_latEnd);
 lngEnd = f64tog(_lngEnd);
}

// Destination point based on start coordinates in dewgrees , bearing in degree and distance in meters
inline void dest(GPS_DATA & latStart,
                 GPS_DATA & lngStart,

                 double & bearing,
                 double & distance,
                 
                 GPS_DATA & latEnd,
                 GPS_DATA & lngEnd)
{
  f64 _latStart(gtof64(latStart));
  f64 _lngStart(gtof64(lngStart));

  f64 _bearing(dtof64(bearing));
  f64 _distance(dtof64(distance));

  f64 _latEnd;
  f64 _lngEnd;
  
  dest64(_latStart,
         _lngStart,

         _bearing,
         _distance, 
         
         _latEnd,
         _lngEnd);
         
 latEnd = f64tog(_latEnd);
 lngEnd = f64tog(_lngEnd);
}

String gtos(GPS_DATA & gps_data)
{
  return String(String(gps_data.deg) + "." + String(gps_data.minSec));
}

