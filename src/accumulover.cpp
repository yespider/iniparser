#include "accumulover.hpp"

#include <sstream>
#include <iostream>

FILE       *Accumulover::fp_ = nullptr;
dictionary *Accumulover::ini_ = nullptr;
uint32_t Accumulover::updated_motion_s_ = 0;
uint32_t Accumulover::updated_rcs_s_ = 0;
uint32_t Accumulover::updated_odometer_m_ = 0;
time_t  Accumulover::rcs_time_begin_;



Accumulover::Accumulover(const std::string& file_path) {
  if(ini_ != nullptr) return;
  rcs_time_begin_ = time(nullptr);
  ini_ = iniparser_load(file_path.c_str());
  if (ini_ == nullptr){
    fp_ = fopen(file_path.c_str(), "w");
    fprintf(fp_,"[record]\n;initial file.");
    fclose(fp_);
    ini_ = iniparser_load(file_path.c_str());
  }
  if ( ini_ == nullptr ) {
      printf("stone:iniparser_load error!\n");
      exit(-1);
  }
  fp_ = fopen(file_path.c_str(), "w");

  //printf("%s\n", iniparser_getstring(ini_, "record:motion_time_string", "null"));
}

Accumulover::~Accumulover() {
  fclose(fp_);
  //释放
  iniparser_freedict(ini_);
}

void Accumulover::UpdateTime(uint32_t motion_s, uint32_t rcs_s, bool is_absolutely){
  updated_motion_s_ = motion_s + (is_absolutely ? 0 : updated_motion_s_);
  if (rcs_s == 0) {
    rcs_s = time(nullptr) - rcs_time_begin_;
    is_absolutely = true;
  }
  
  updated_rcs_s_ = rcs_s + (is_absolutely ? 0 : updated_rcs_s_);
}

void Accumulover::UpdateOdometer(uint32_t odometer_m, bool is_absolutely){
  updated_odometer_m_ = odometer_m + (is_absolutely ? 0 : updated_odometer_m_);
}
void Accumulover::Dump(bool do_display){
  int mt = iniparser_getint(ini_,"record:motion_time",0) + updated_motion_s_;
  int rt = iniparser_getint(ini_,"record:rcs_time",0) + updated_rcs_s_;
  int o = iniparser_getint(ini_,"record:odometer",0) + updated_odometer_m_;

  std::stringstream ssss;
  ssss << mt/3600 << ":" << mt%3600/60 << ":" << mt%3600%60;
  // std::cout << ssss.str().c_str() << "  " << mt << std::endl;
  iniparser_set(ini_,"record:motion_time_string",ssss.str().c_str());
  ssss.str("");
  ssss.clear();
  ssss << rt/3600 << ":" << rt%3600/60 << ":" << rt%3600%60;
  iniparser_set(ini_,"record:rcs_time_string",ssss.str().c_str());
  ssss.str("");
  ssss.clear();
  ssss << mt;
  iniparser_set(ini_,"record:motion_time",ssss.str().c_str());
  ssss.str("");
  ssss.clear();
  ssss << rt;
  iniparser_set(ini_,"record:rcs_time",ssss.str().c_str());
  ssss.str("");
  ssss.clear();
  ssss << o;
  iniparser_set(ini_,"record:odometer",ssss.str().c_str());
  ssss.str("");
  ssss.clear();

  iniparser_dump_comment("string结尾的格式是——时：分：秒.", fp_);
  iniparser_dump_comment("time结尾的格式是——秒", fp_);
  iniparser_dump_comment("odometer的格式是——米", fp_);
  iniparser_dump_ini(ini_,fp_);iniparser_dumpsection_ini(ini_, "ipaddrpool", fp_);

  if (do_display) {
    std::cout << "本次运动信息" << std::endl;
    ssss << "系统时间：当次"<<updated_rcs_s_<< "秒, 历史累计" << rt << "秒";
    std::cout << ssss.str() << std::endl;
    ssss.str("");
    ssss.clear();
    ssss << "运动时间：当次"<<updated_motion_s_<< "秒, 历史累计" << mt << "秒";
    std::cout << ssss.str() << std::endl;
    ssss.str("");
    ssss.clear();
    ssss << "运动里程：当次"<<updated_odometer_m_<< "米, 历史累计" << o << "米";
    std::cout << ssss.str() << std::endl;
    ssss.str("");
    ssss.clear();
  }
};