#ifndef _ROS_path_navigation_msgs_PathExecutionAction_h
#define _ROS_path_navigation_msgs_PathExecutionAction_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "path_navigation_msgs/PathExecutionActionGoal.h"
#include "path_navigation_msgs/PathExecutionActionResult.h"
#include "path_navigation_msgs/PathExecutionActionFeedback.h"

namespace path_navigation_msgs
{

  class PathExecutionAction : public ros::Msg
  {
    public:
      path_navigation_msgs::PathExecutionActionGoal action_goal;
      path_navigation_msgs::PathExecutionActionResult action_result;
      path_navigation_msgs::PathExecutionActionFeedback action_feedback;

    PathExecutionAction():
      action_goal(),
      action_result(),
      action_feedback()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->action_goal.serialize(outbuffer + offset);
      offset += this->action_result.serialize(outbuffer + offset);
      offset += this->action_feedback.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->action_goal.deserialize(inbuffer + offset);
      offset += this->action_result.deserialize(inbuffer + offset);
      offset += this->action_feedback.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "path_navigation_msgs/PathExecutionAction"; };
    const char * getMD5(){ return "211eeb70358fe55a3305c41b8fdda45a"; };

  };

}
#endif