
/* This file was autogenerated as a part of the avr_bridge pkg

avr_bridge was written by 
Adam Stambler and Phillip Quiza of Rutgers University.

*/

#include "Ros.h"

char Ros::getTopicTag(char * topic){
if (strcmp(topic, "imu") ==0)
	 return 0;


	return 0;
}
Ros ros("imu"); //autogenerated Ros object