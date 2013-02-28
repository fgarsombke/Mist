#pragma once

namespace Perlin {
// Lots of different options for noise generation.
// This one looks really cool: http://libnoise.sourceforge.net/downloads/index.html

// Code taken from 
//    http://stackoverflow.com/questions/4753055/perlin-noise-generation-for-terrain
class PerlinModified
{
public:

   // Constructor
   PerlinModified();
   PerlinModified(double persistence, double frequency, double amplitude, int octaves, int randomseed);

  // Get Height
  double GetHeight(double x, double y) const;

  // Get
  double persistence() const { return persistence_; }
  double frequency()   const { return frequency_;   }
  double amplitude()   const { return amplitude_;   }
  int    octaves()     const { return octaves_;     }
  int    randomSeed()  const { return random_seed_;  }

  // Sethttp://stackoverflow.com/questions/4753055/perlin-noise-generation-for-terrain
  void Set(double persistence, double frequency, double amplitude, int octaves, int randomseed);

  void SetPersistence(double persistence) { persistence_ = persistence; }
  void SetFrequency(  double frequency)   { frequency_ = frequency;     }
  void SetAmplitude(  double amplitude)   { amplitude_ = amplitude;     }
  void SetOctaves(    int    octaves)     { octaves_ = octaves;         }
  void SetRandomSeed( int    randomSeed)  { random_seed_ = randomSeed;   }

private:

    double Total(double i, double j) const;
    static double GetValue(double x, double y);
    static double Interpolate(double x, double y, double a);
    static double Noise(int x, int y);

    double persistence_;
    double frequency_;
    double amplitude_;
    int octaves_;
    int random_seed_;
};

}