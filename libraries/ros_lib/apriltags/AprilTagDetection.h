#ifndef _ROS_apriltags_AprilTagDetection_h
#define _ROS_apriltags_AprilTagDetection_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Point32.h"
#include "geometry_msgs/Pose.h"

namespace apriltags
{

  class AprilTagDetection : public ros::Msg
  {
    public:
      std_msgs::Header header;
      uint32_t id;
      geometry_msgs::Point32 corners2d[4];
      float tag_size;
      geometry_msgs::Pose pose;

    AprilTagDetection():
      header(),
      id(0),
      corners2d(),
      tag_size(0),
      pose()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->id >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->id >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->id >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->id >> (8 * 3)) & 0xFF;
      offset += sizeof(this->id);
      for( uint8_t i = 0; i < 4; i++){
      offset += this->corners2d[i].serialize(outbuffer + offset);
      }
      union {
        float real;
        uint32_t base;
      } u_tag_size;
      u_tag_size.real = this->tag_size;
      *(outbuffer + offset + 0) = (u_tag_size.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_tag_size.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_tag_size.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_tag_size.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->tag_size);
      offset += this->pose.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->id =  ((uint32_t) (*(inbuffer + offset)));
      this->id |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->id |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->id |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->id);
      for( uint8_t i = 0; i < 4; i++){
      offset += this->corners2d[i].deserialize(inbuffer + offset);
      }
      union {
        float real;
        uint32_t base;
      } u_tag_size;
      u_tag_size.base = 0;
      u_tag_size.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_tag_size.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_tag_size.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_tag_size.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->tag_size = u_tag_size.real;
      offset += sizeof(this->tag_size);
      offset += this->pose.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "apriltags/AprilTagDetection"; };
    const char * getMD5(){ return "cbbc500741705b6142a546b4696bb7f5"; };

  };

}
#endif