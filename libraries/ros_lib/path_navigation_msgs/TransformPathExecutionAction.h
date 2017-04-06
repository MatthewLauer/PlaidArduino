#ifndef _ROS_path_navigation_msgs_TransformPathExecutionAction_h
#define _ROS_path_navigation_msgs_TransformPathExecutionAction_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "path_navigation_msgs/TransformPathExecutionActionGoal.h"
#include "path_navigation_msgs/TransformPathExecutionActionResult.h"
#include "path_navigation_msgs/TransformPathExecutionActionFeedback.h"

namespace path_navigation_msgs
{

  class TransformPathExecutionAction : public ros::Msg
  {
    public:
      path_navigation_msgs::TransformPathExecutionActionGoal action_goal;
      path_navigation_msgs::TransformPathExecutionActionResult action_result;
      path_navigation_msgs::TransformPathExecutionActionFeedback action_feedback;

    TransformPathExecutionAction():
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

    const char * getType(){ return "path_navigation_msgs/TransformPathExecutionAction"; };
    const char * getMD5(){ return "dea92258dd5f657e3f6f77aa6f0e08ea"; };

  };

}
#endif