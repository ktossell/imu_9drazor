/*
 * Ros.cpp
 *
 *  Created on: Oct 9, 2010
 *      Author: asher
 */

#include "Ros.h"
#include "string.h"
#include "WProgram.h"



#include <stdio.h>

void initRos(){
	//Set up the serial communicaton and wait a bit to make sure
	//that the program and grab its id before we go into any other
	//set up routines
	Serial.begin(57600);
	for (int i; i<100; i++) {ros.spin();delay(10);}
}


int uart_putchar(char c, FILE *stream)
{
	Serial.write(c);
  return 0;
}

int uart_getchar(FILE *stream)
{
	return Serial.read();
}

FILE* ros_io = fdevopen(uart_putchar, uart_getchar);


Ros::Ros(char * node_name) : name(node_name){
	// TODO Auto-generated constructor stub
	//this->node_name = name;

	this->packet_data_left = 0;
	this->buffer_index =0;
	this->header = (packet_header *) this->buffer;
	//this->cb_list[0] = getID;
	this->com_state = header_state;

}

void Ros::subscribe(char * topic, ros_cb funct, Msg* msg){
	int tag = getTopicTag(topic);
	this->cb_list[tag] = funct;
	this->msgList[tag] = msg;
}

void Ros::publish(Publisher pub, Msg* msg){
	uint16_t bytes = msg->serialize(this->outBuffer);
		this->send(outBuffer,bytes,0,pub);
	}

void Ros::resetStateMachine(){
	packet_data_left = 0;
	buffer_index = 0;
	com_state = header_state;
}


void Ros::spin(){

	int com_byte =  uart_getchar(ros_io);


	while (com_byte != -1) {
		//If the buffer index is about to over flow, or it hasnt been reset in a long time..
		if (buffer_index > ROS_BUFFER_SIZE) buffer_index=0;
		if ( (millis() - packet_start) > 30) {buffer_index=0; packet_start=millis();}

		buffer[buffer_index] = com_byte;
		buffer_index++;

		if(com_state == header_state){
			if ( buffer_index == sizeof(packet_header)){
				com_state = msg_data_state;
				this->packet_data_left = header->msg_length;
				packet_start = millis();
			}
		}
		if (com_state ==  msg_data_state){
			packet_data_left--;
			if (packet_data_left <0){
				resetStateMachine();
				if (header->packet_type ==255) this->getID();
				if (header->packet_type==0){ //topic,
						//ie its a valid topic tag
						//then deserialize the msg
						this->msgList[header->topic_tag]->deserialize(buffer+4);
						//call the registered callback function
						this->cb_list[header->topic_tag](this->msgList[header->topic_tag]);
						//this->cb_list[0](this->msgList[0]);

				}
				if(header->packet_type == 1){ //service
				}
			}
		}

		com_byte =  uart_getchar(ros_io);
	}
}


Publisher Ros::advertise(char* topic){

	//This section is to be dynamically generated by python script
	//this set of comparisons finds the topic id for the topic
	//generates the publisher and returns it

	return getTopicTag(topic);
}


void Ros::send(uint8_t * data, uint16_t length, char packet_type, char topicID){

	fputc(packet_type,ros_io);

	//send msg ID
	fputc(topicID,ros_io);
	//send length of message to aid in io code on other side
	fwrite(&length,2,1,ros_io);

	fwrite(data, length,1,ros_io);
}

void Ros::getID(){
	uint16_t size = this->name.serialize(ros.outBuffer);
	this->send(outBuffer, size, 255, 0);
}


Ros::~Ros() {
	// TODO Auto-generated destructor stub
}
