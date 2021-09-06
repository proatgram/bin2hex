#include <cstdio>
#include <cstdint>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include "includes/rrFile/rrFile.cpp"
#include "includes/rrFile/rrFile.h"
uint8_t twos_complement(uint8_t val)
{
    return -(unsigned int)val;
}
char hextostr(uint16_t data) {
  uint8_t datauppr = data & 0xFF00;
  char dataupprchr[3];
  sprintf(dataupprchr, "%x", datauppr);
  uint8_t datalwr = data & 0x00FF;
  char datalwrchr[3];
  sprintf(datalwrchr, "%x", datalwr);
  char newdata[5];
  char zero[2] = {'0', '0'};
  if (datauppr < 0x10 || datalwr < 0x10) {
    if (datauppr == 0x00) {
      strncat(newdata, zero, 0x02);
    }
    else if (datauppr < 0x10) {
      strncat(newdata, zero, 0x01);
      strncat(newdata, dataupprchr, 0x02);
    }
    else {
      strncat(newdata, dataupprchr, 0x02);
    }
    if (datalwr == 0x00) {
      strncat(newdata, zero, 0x02);
    }
    else if (datalwr < 0x10) {
      strncat(newdata, zero, 0x01);
      strncat(newdata, datalwrchr, 0x02);
    }
    else {
      strncat(newdata, datalwrchr, 0x02);
    }
  }
  else {
    strncat(newdata, dataupprchr, 0x02);
    strncat(newdata, datalwrchr, 0x02);
  }
  return *newdata;
}

int main(int argc, char* argv[]) {
  if (argc == 0x01) {
  printf("Not enough agruments. %d  Exiting. \n", (argc - 0x01));
  return 0x01;
  }
 char* infile;
 char* outfile;
  uint8_t count = 0x00;
  for (uint8_t times = 0x01; times < argc; times++) {
    std::string arg = argv[times];
    if (arg.at(0x00) != '-') {
      if (count == 0x00) {
        infile = argv[times];
        printf("Infile: %s\n", infile);
      }
      else if (count == 0x01) {
        outfile = argv[times];
        printf("Outfile: %s", outfile);
      }
      else if (count == 0x02) {
        printf("Count error. Exiting. \n");
        return 0x01;
      }
      count++;
    }
    if (arg == "-h" || arg == "--help") {
      printf("Usage: \n\n   bin2hex infile outfile [agrs] \nPurpose: \n\n   To convert");
      printf(" binary files to Intel HEX file format.\nNotes: \n\n   The input");
      printf(" and output file are both required. \n\n   If the output (outfile)");
      printf(" already exists, then a choice will be presented to\n   either overwrite");
      printf(" the outfile, or to enter a new filename as the outfile.\n   Type y or n to choose. \n");
      printf("Arguments: \n\n   -h [--help] Display this help message. \n\n   ");
    }
  }
  rrFile file;
  file.openFile(infile);
  std::vector<uint8_t> binarys;
  file.readByteStream(binarys, file.fileSize(), 0x00, true);
  uint16_t segments = (file.fileSize() / 0xFF);
  int remainingbytes = (file.fileSize() % 0xFF);
  for (uint8_t times = 0x00; times < segments; times++) {
    const uint8_t bytecount = 0xFF;
    uint16_t address = 0x0102;//(times * 0xFF);
    const uint8_t recordtype = 0x00;
    const uint8_t linereturn = 0x0A;
    int checksum;
    char data[0xFF];
    for (uint8_t times = 0x00; times < 0xFF; times++) {
       char filedata[2];
       sprintf(filedata, "%x", binarys.at(times));
       data[times] = *filedata;
       checksum + binarys.at(times);
       printf("0x%s \n", filedata);
    }
    checksum = twos_complement((checksum + bytecount + address + recordtype) & 0xFF);
    char segdata[(0xFF * 0x02) + 0x0A];
    printf("%s", hextostr(address));
  }
}
