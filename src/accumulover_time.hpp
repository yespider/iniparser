#include <time.h>
#include <string>

#include "iniparser.h"

#define INI_PATH "./time.ini"

class AccumuloverTime
{
private:
  AccumuloverTime(const std::string& file_path);

  /* data */
  static AccumuloverTime* accumulover_time_;
  static FILE       *fp;
  static dictionary *ini;
  static uint32_t updated_motion_s, updated_rcs_s;
  static time_t  rcs_time_begin;
public:
  static AccumuloverTime* GetInstance(const std::string& file_path);
  static void UpdateAccumuloverTime(uint32_t motion_s, uint32_t rcs_s = 0, bool is_absolutely = true);
  ~AccumuloverTime();
};
