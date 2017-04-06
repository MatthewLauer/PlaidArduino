#ifndef _ROS_path_navigation_msgs_TransformPathExecutionActionGoal_h
#define _ROS_path_navigation_msgs_TransformPathExecutionActionGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalID.h"
#include "path_navigation_msgs/TransformPathExecutionGoal.h"

namespace path_navigation_msgs
{

  class TransformPathExecutionActionGoal : public ros::Msg
  {
    public:
      std_msgs::Header header;
      actionlib_msgs::GoalID goal_id;
      path_navigation_msgs::TransformPathExecutionGoal goal;

    TransformPathExecutionActionGoal():
      header(),
      goal_id(),
      goal()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->goal_id.serialize(outbuffer + offset);
      offset += this->goal.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->goal_id.deserialize(inbuffer + offset);
      offset += this->goal.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "path_navigation_msgs/TransformPathExecutionActionGoal"; };
    const char * getMD5(){ return "f079496ab3986808d6711b0673b28cb2"; };

  };

}
#endif