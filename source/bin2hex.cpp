#include <stdlib.h>
#include <cstdio>
#include <cstdint>
#include <string>
#include <cstring>
#include <vector>
#include <cctype>
#include "includes/rrFile.cpp"
#include "includes/rrFile.h"
uint8_t twos_complement(uint8_t val)
{
    return -(unsigned int)val;
}
char* hextostr(uint16_t data) {
  char *newdata = new char[5]();
  if (data <= 0xFF) {
    sprintf(newdata, "%02x", data);
  }
  else {
    sprintf(newdata, "%04x", data);
  }
  return newdata;
}
char* strupr(char* chararray) {
  char *newdata = new char[strlen(chararray)];
  for (uint16_t times = 0x00; times < strlen(chararray); times++) {
    if (isdigit(chararray[times])) {
      strncat(newdata, &chararray[times], 0x01);
    }
    else if (ispunct(chararray[times])) {
      strncat(newdata, &chararray[times], 0x01);
    }
    else if (islower(chararray[times])) {
      const char upperchar = static_cast<char>(toupper(chararray[times]));
      strncat(newdata, &upperchar, 0x01);
    }
  }
  return newdata;
}
int main(int argc, char* argv[]) {
  rrFile source;
  rrFile destination;
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
        if (source.openFile(infile) == EXIT_FAILURE) {
          printf("Invalid source file name. File does not exist. Exiting. \n");
          exit(0x01);
        }
      }
      else if (count == 0x01) {
        outfile = argv[times];
        if (destination.openFile(outfile) == EXIT_FAILURE) {
          printf("Invalid destination file name. File does not exist. \n");
          printf("Would you like to create a file with that name? y/n: ");
            char character = static_cast<char>(getchar());
            if (character == 'y') {
              destination.createFile(outfile);
              destination.openFile(outfile);
              break;
              //printf("y \n");
            }
            else if (character == 'n') {
              //printf("n \n");
              exit(0x02);
            }
        }
        destination.removeFile(outfile);
        destination.createFile(outfile);
        destination.openFile(outfile);
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
  std::vector<uint8_t> binarys;
  source.readByteStream(binarys, source.fileSize(), 0x00, true);
  uint16_t segments = (source.fileSize() / 0xFF);
  uint8_t remainingbytes = ((source.fileSize() % 0xFF) - 0x01);
  const uint8_t bytecount = 0xFF;
  const uint8_t recordtype = 0x00;
  const uint8_t linereturn = 0x0A;
  for (uint16_t times = 0x00; times < segments; times++) {
    std::vector<char*> datapackets;
    uint16_t address = (times * 0xFF);
    uint8_t checksum = 0x00;
    char* checksumchar;
    char data[((0xFF * 0x02))];
    sprintf(data, ":%02x%04x%02x", bytecount, address, recordtype);
    for (uint8_t times = 0x00; times != 0xFF; times++) {
      char* thisdata = hextostr(binarys.at(address + times));
      strcat(data, thisdata);
      checksum = checksum + binarys.at(address + times);
      delete thisdata;
      thisdata = NULL;
    }
    checksum = checksum + bytecount + recordtype + address;
    checksum = checksum & 0xFF;
    checksum = twos_complement(checksum);
    checksumchar = hextostr(checksum);
    char outdatalwr[strlen(data) + 0x03];
    strcpy(outdatalwr, data);
    strcat(outdatalwr, checksumchar);
    char* output = strupr(outdatalwr);
    strcat(output, "\n");
    destination.writeString(output, address, false);
  }
  if (remainingbytes > 0x00) {
  char remainingdatabytes[(remainingbytes * 0xFF) + 0x01];
  uint16_t address = ((segments * 0xFF) + 0x01);
  uint8_t checksum = 0x00;
  char* checksumchar;
  char data[(remainingbytes * 0x02)];
  sprintf(data, ":%02x%04x%02x", remainingbytes, ((segments * 0xFF) + 0x01), recordtype);
    for (uint8_t times = 0x00; times != remainingbytes; times++) {
      char* thisdata = hextostr(binarys.at(address + times));
      strcat(data, thisdata);
      checksum = checksum + binarys.at(address + times);
      delete thisdata;
      thisdata = NULL;
    }
    checksum = checksum + bytecount + recordtype + address;
    checksum = checksum & 0xFF;
    checksum = twos_complement(checksum);
    checksumchar = hextostr(checksum);
    char outdatalwr[strlen(data)];
    sprintf(outdatalwr, "%s%s", data, checksumchar);
    //strcpy(outdatalwr, data);
    //strcat(outdatalwr, checksumchar);
    char* output = strupr(outdatalwr);
    strcat(output, "\n");
    destination.writeString(output, destination.fileSize(), false);
  }
  destination.writeString(":00000001FF", destination.fileSize(), true);
}
