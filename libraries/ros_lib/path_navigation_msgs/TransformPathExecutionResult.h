#ifndef _ROS_path_navigation_msgs_TransformPathExecutionResult_h
#define _ROS_path_navigation_msgs_TransformPathExecutionResult_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"

namespace path_navigation_msgs
{

  class TransformPathExecutionResult : public ros::Msg
  {
    public:
      geometry_msgs::PoseWithCovarianceStamped finalpose;

    TransformPathExecutionResult():
      finalpose()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->finalpose.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->finalpose.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "path_navigation_msgs/TransformPathExecutionResult"; };
    const char * getMD5(){ return "6de191de1f3cf450e599cc31709760e6"; };

  };

}
#endif