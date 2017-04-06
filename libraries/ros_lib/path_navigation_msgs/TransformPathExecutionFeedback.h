#ifndef _ROS_path_navigation_msgs_TransformPathExecutionFeedback_h
#define _ROS_path_navigation_msgs_TransformPathExecutionFeedback_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace path_navigation_msgs
{

  class TransformPathExecutionFeedback : public ros::Msg
  {
    public:

    TransformPathExecutionFeedback()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
     return offset;
    }

    const char * getType(){ return "path_navigation_msgs/TransformPathExecutionFeedback"; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

}
#endif