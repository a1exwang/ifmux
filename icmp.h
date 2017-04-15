//
// Created by alexwang on 4/15/17.
//

#ifndef IFMUX_ICMP_H
#define IFMUX_ICMP_H

#pragma pack(1)
typedef struct {
  unsigned int version :4;
  unsigned int ihl :4;
  unsigned char tos;
  unsigned short length;
  unsigned short identification;
  unsigned int reserved1 :1;
  unsigned int dont_fragment :1;
  unsigned int last_fragment :1;
  unsigned int fragment_offset :13;
  unsigned char ttl;
  unsigned char protocol;
  unsigned short checksum;
  unsigned int src;
  unsigned int dst;
  char data[0];
} ip;
#pragma pack(pop)

void recalculate_checksum(ip *p);

#endif //IFMUX_ICMP_H
