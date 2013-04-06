#pragma once

namespace Mist {

class GeoLocale{
public:
   GeoLocale(double latitude, double longitude) 
      : latitude_(latitude), longitude_(longitude), elevation_(InitElevation())
   { 
   }

   GeoLocale() 
      : latitude_(0), longitude_(0), elevation_(0)
   {
   }

   inline double latitude()  const { return latitude_; }
   inline double longitude() const { return longitude_; }
   inline double elevation() const { return elevation_; }

   // Longitude of the centre of the local time zone
   double Lz() const 
   {
      // TODO: Implement
      return longitude_;
   }

// ------------------------------------------------------------------------------
private:
   double latitude_;
   double longitude_;
   
   // In meters
   double elevation_;

   double InitElevation() {
      // TODO: Implement
      return 0;
   }
};

}