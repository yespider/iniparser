#include <time.h>
#include <string>

#include "iniparser.h"

class Accumulover
{
private:

  /* data */
  static FILE       *fp_;
  static dictionary *ini_;
  static uint32_t updated_motion_s_, updated_rcs_s_;
  static uint32_t updated_odometer_m_;
  static time_t  rcs_time_begin_;
public:
  Accumulover(const std::string& file_path);
  ~Accumulover();
  void UpdateTime(uint32_t motion_s, uint32_t rcs_s = 0, bool is_absolutely = true);
  void UpdateOdometer(uint32_t odometer_m, bool is_absolutely = true);
  void Dump(bool do_display = false);
};
