
/*
	Vec3.h
	This msg file was autogenerated by the Rutgers avr_bridge software.
	*/ 
#ifndef VEC3_H
#define VEC3_H
#include "Msg.h"
#include "vector.h"
#include "ros_string.h"


namespace imu_9drazor {
class Vec3 : public Msg{
   public:
	Vec3();
	Vec3(uint8_t * data);
	~Vec3();

	uint16_t bytes();
	uint16_t serialize(uint8_t * out_buffer);
	uint16_t deserialize(uint8_t * data);

	float x;
	float y;
	float z;
   private:

}; /***** ending class Vec3 *******/


} /***** ending namespace imu_9drazor *******/

#endif  /****   VEC3_H    *****/