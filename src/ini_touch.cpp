#include <inttypes.h>
#include <stdlib.h>
#include <iostream>


#include "iniparser.h"
#include "accumulover_time.hpp"

#define INI_PATH "./test.ini"
#define TIME_PATH "./time.ini"

int main(int argc,char* argv[] ){
  AccumuloverTime* at = AccumuloverTime::GetInstance(TIME_PATH);
  sleep(3);
  AccumuloverTime::UpdateAccumuloverTime(1,0);
  return 0;

  FILE       *fp          = nullptr;
  dictionary *ini         = nullptr;
   
  //加载文件
  ini = iniparser_load(INI_PATH);
  if ( ini == nullptr ) {
      printf("stone:iniparser_load error!\n");
      exit(-1);
  }

  //设置ipaddrpool
  iniparser_set(ini, "ipaddrpool:start", "192.168.1.1");
  iniparser_set(ini, "ipaddrpool:end", "192.168.1.100");

  //读取数据
  printf("%s\n", iniparser_getstring(ini, "ipaddrpool:start", "null"));
  printf("%s\n", iniparser_getstring(ini, "ipaddrpool:end", "null"));
  printf("%s\n", iniparser_getstring(ini, "filepath:leasefile", "null"));
  printf("%s\n", iniparser_getstring(ini, "network:interface", "null"));
  printf("%s\n", iniparser_getstring(ini, "opt:dns1", "null"));
  printf("%s\n", iniparser_getstring(ini, "opt:dns2", "null"));
  printf("%s\n", iniparser_getstring(ini, "opt:subnet", "null"));
  printf("%s\n", iniparser_getstring(ini, "opt:router", "null"));
  printf("%s\n", iniparser_getstring(ini, "opt:domain", "null"));
  printf("%s\n", iniparser_getstring(ini, "opt:lease", "null"));
  printf("%s\n", iniparser_getstring(ini, "opt:t1", "null"));
  printf("%s\n", iniparser_getstring(ini, "opt:t2", "null"));

  //写入文件
  fp = fopen(INI_PATH, "w");
  if( fp == nullptr ) {
      printf("stone:fopen error!\n");
      exit(-1);
  }

  iniparser_dumpsection_ini(ini, "ipaddrpool", fp);
  iniparser_dumpsection_ini(ini, "filepath", fp);
  iniparser_dump_comment("test comment.", fp);
  iniparser_dump_comment("test 中文.", fp);
  iniparser_dumpsection_ini(ini, "network", fp);
  iniparser_dumpsection_ini(ini, "opt", fp);
  fclose(fp);

  //释放
  iniparser_freedict(ini);

  return  (0);
}