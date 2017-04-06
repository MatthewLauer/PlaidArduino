#ifndef _ROS_apriltags_AprilTagDetections_h
#define _ROS_apriltags_AprilTagDetections_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "apriltags/AprilTagDetection.h"

namespace apriltags
{

  class AprilTagDetections : public ros::Msg
  {
    public:
      std_msgs::Header header;
      uint8_t detections_length;
      apriltags::AprilTagDetection st_detections;
      apriltags::AprilTagDetection * detections;

    AprilTagDetections():
      header(),
      detections_length(0), detections(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset++) = detections_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < detections_length; i++){
      offset += this->detections[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint8_t detections_lengthT = *(inbuffer + offset++);
      if(detections_lengthT > detections_length)
        this->detections = (apriltags::AprilTagDetection*)realloc(this->detections, detections_lengthT * sizeof(apriltags::AprilTagDetection));
      offset += 3;
      detections_length = detections_lengthT;
      for( uint8_t i = 0; i < detections_length; i++){
      offset += this->st_detections.deserialize(inbuffer + offset);
        memcpy( &(this->detections[i]), &(this->st_detections), sizeof(apriltags::AprilTagDetection));
      }
     return offset;
    }

    const char * getType(){ return "apriltags/AprilTagDetections"; };
    const char * getMD5(){ return "7f7dd9e733b444cdc111ec1690c66971"; };

  };

}
#endif