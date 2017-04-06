#ifndef _ROS_path_navigation_msgs_PathExecutionActionResult_h
#define _ROS_path_navigation_msgs_PathExecutionActionResult_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "path_navigation_msgs/PathExecutionResult.h"

namespace path_navigation_msgs
{

  class PathExecutionActionResult : public ros::Msg
  {
    public:
      std_msgs::Header header;
      actionlib_msgs::GoalStatus status;
      path_navigation_msgs::PathExecutionResult result;

    PathExecutionActionResult():
      header(),
      status(),
      result()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->status.serialize(outbuffer + offset);
      offset += this->result.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->status.deserialize(inbuffer + offset);
      offset += this->result.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "path_navigation_msgs/PathExecutionActionResult"; };
    const char * getMD5(){ return "c4ec0ac73d7cd1bba8a2ac7f00dcff15"; };

  };

}
#endif