#include "accumulover_time.hpp"

#include <sstream>
#include <iostream>


AccumuloverTime* AccumuloverTime::accumulover_time_ = nullptr;
FILE       *AccumuloverTime::fp = nullptr;
dictionary *AccumuloverTime::ini = nullptr;
uint32_t AccumuloverTime::updated_motion_s = 0;
uint32_t AccumuloverTime::updated_rcs_s = 0;
time_t  AccumuloverTime::rcs_time_begin;



AccumuloverTime::AccumuloverTime(const std::string& file_path) {
  ini = iniparser_load(file_path.c_str());
  if (ini == nullptr){
    fp = fopen(file_path.c_str(), "w");
    fprintf(fp,"[record]\n;initial file.");
    fclose(fp);
    ini = iniparser_load(file_path.c_str());
  }
  if ( ini == nullptr ) {
      printf("stone:iniparser_load error!\n");
      exit(-1);
  }
  fp = fopen(file_path.c_str(), "w");

  //printf("%s\n", iniparser_getstring(ini, "record:motion_time_string", "null"));
}

AccumuloverTime::~AccumuloverTime() {
  fclose(fp);
  //释放
  iniparser_freedict(ini);
}

AccumuloverTime* AccumuloverTime::GetInstance(const std::string& file_path){
    if (accumulover_time_ == 0){
      rcs_time_begin = time(nullptr);
      accumulover_time_ = new AccumuloverTime(file_path);
    }
    return accumulover_time_;
  };

void AccumuloverTime::UpdateAccumuloverTime(uint32_t motion_s, uint32_t rcs_s, bool is_absolutely){
  updated_motion_s = motion_s + (is_absolutely ? 0 : updated_motion_s);
  if (rcs_s == 0) {
    rcs_s = time(nullptr) - rcs_time_begin;
    is_absolutely = true;
  }
  
  updated_rcs_s = rcs_s + (is_absolutely ? 0 : updated_rcs_s);


  int mt = iniparser_getint(ini,"record:motion_time",0) + updated_motion_s;
  int rt = iniparser_getint(ini,"record:rcs_time",0) + updated_rcs_s;

  std::stringstream ssss;
  ssss << mt/3600 << ":" << mt%3600/60 << ":" << mt%3600%60;
  // std::cout << ssss.str().c_str() << "  " << mt << std::endl;
  iniparser_set(ini,"record:motion_time_string",ssss.str().c_str());
  ssss.str("");
  ssss.clear();
  ssss << rt/3600 << ":" << rt%3600/60 << ":" << rt%3600%60;
  iniparser_set(ini,"record:rcs_time_string",ssss.str().c_str());
  ssss.str("");
  ssss.clear();
  ssss << mt;
  iniparser_set(ini,"record:motion_time",ssss.str().c_str());
  ssss.str("");
  ssss.clear();
  ssss << rt;
  iniparser_set(ini,"record:rcs_time",ssss.str().c_str());
  ssss.str("");
  ssss.clear();

  iniparser_dump_comment("string结尾的格式是——时：分：秒.", fp);
  iniparser_dump_comment("time结尾的格式是——秒", fp);
  iniparser_dump_ini(ini,fp);iniparser_dumpsection_ini(ini, "ipaddrpool", fp);

};