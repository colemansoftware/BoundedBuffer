/* Coleman DeMars
 * CSCI 4100
 * Assignment 3
 * This program tests a bounded buffer by creating multiple 
 * sending and receiving threads
 */

#include <stdio.h>
#include <stdlib.h>
#include "bbuff.h"
#include <pthread.h>

#include <unistd.h>

/* Structure to hold thread arguments */
struct t_args {

	/* Thread ID */
	int t_id;

	/* Number of messages to send/receive */
	int num_msgs;

	/* Bounded buffer to use */
	struct bbuff * buffer;
};

/* Initialize t_args structure */
void t_args_init(struct t_args *args, int t_id, int num_msgs,
		struct bbuff * buffer);

/* Function for sending thread to execute */
void * send_msgs(void * args);

/* Function for receiving thread to execute */
void * receive_msgs(void * args);

/* Main function */
int main(int argc, char *argv[]) {
	/* YOUR CODE HERE */

	struct bbuff buffer;

	//1.	Declare a pthread_t variable for each of the senders and the receivers.
	//2.	Declare a bbuff structure and initialize it using bb_init. only one buffer
	bb_init(&buffer);

	//Declare a t_args structure for each of the sending and receiving threads and initialize them using t_args_init.

	pthread_t send[3];
	struct t_args sendArg[3];
	for (int i = 0; i < 3; i++){
		t_args_init(&sendArg[i], i, 8, &buffer);
		//Start each of the sending threads using pthread_create, the send_msgs functions, and the appropriate t_args structure.
		pthread_create(&send[i], NULL, send_msgs, &sendArg[i]);
	}
	//Wait for each of the sending and receiving threads to complete using pthread_join.




	pthread_t receive[4];
	//Declare a t_args structure for each of the sending and receiving threads and initialize them using t_args_init.
	struct t_args recArg[4];
	for (int i = 0; i < 4; i++){
		t_args_init(&recArg[i], i, 6, &buffer);
		//Start each of the sending threads using pthread_create, the receive functions, and the appropriate t_args structure.
		pthread_create(&receive[i], NULL, receive_msgs, &recArg[i]);
	}
	//Wait for each of the sending and receiving threads to complete using pthread_join.
	pthread_join(send[0], NULL);
	pthread_join(send[1], NULL);
	pthread_join(send[2], NULL);
	pthread_join(receive[0], NULL);
	pthread_join(receive[1], NULL);
	pthread_join(receive[2], NULL);
	pthread_join(receive[3], NULL);


}

/* Initialize t_args structure */
void t_args_init(struct t_args *args, int t_id, int num_msgs,
		struct bbuff * buffer) {
	/* YOUR CODE HERE */

	/*•	args should be a pointer to a t_args structure that already exists.
	 * I recommend either creating one of these per thread or creating an
	 * array of them for the sender threads and an array for the receiver threads.
	 •	t_id should be an integer identifying which sender or which receiver
	 the t_args structure corresponds to. If you are using arrays to keep
	 track of your pthread_t and t_args structures, the index of the thread would work here.
	 •	num_msgs should be the number of messages that the sender or receiver
	 thread is sending. The value here is up to you, but you should make sure
	 that the total number of messages sent by the sending threads is the
	 same as the total number of messages received by the receiving threads.
	 •	buffer should be a pointer to a bbuff structure that already exists.
	 This should be the same buffer for all of the threads.
	 * */
	args->num_msgs = num_msgs;
	args->t_id = t_id;
	args->buffer = buffer;

}

/* Function for sending thread to execute */
void * send_msgs(void * args) {
	/* YOUR CODE HERE */

	/* 1.	Cast the void pointer args to a pointer to a t_args structure using the following statement:*/
	struct t_args * real_args = (struct t_args *) args;
	/* 2.	Declare a bb_msg structure. (Note that in C you have to include the keyword struct when declaring a structure.)*/
	struct bb_msg send_bb_msg;

	/* 3.	Send the number of messages specified in the t_args structure by repeatedly doing the following:
	 1.	Initialize the bb_msg structure with the appropriate thread id and message id using bb_init_msg.*/

	int t = real_args->buffer->in - real_args->buffer->out;

	if (t == 10){
		printf("\nBuffer full, Sleep");
		sleep(1);
	}
	for (int i = 0; i < real_args->num_msgs; i++) {
		//i is the counter for the unique m_id
		bb_init_msg(&send_bb_msg, real_args->t_id, i);
		/*2.	Send the message to the buffer in the t_args structure using bb_send.*/
		bb_send(real_args->buffer, &send_bb_msg);
	}
	return (NULL);
}

/* Function for receiving thread to execute */
void * receive_msgs(void * args) {
	/* YOUR CODE HERE */
	/*1.  Cast the void pointer args to a pointer to a t_args structure.*/
	struct t_args * real_args = (struct t_args *) args;
	/*2.	Declare a bb_msg structure.*/
	struct bb_msg received_bb_msg;
	int t = real_args->buffer->in - real_args->buffer->out;
	printf("\nrec Buffer, thread %d, %d", t, real_args->t_id);
	if (t == 0){
		sleep(.5);
	}
	/*3.	Receive the number of messages specified in the t_args structure by repeatedly doing the following:*/
	/*1.	Receive the message from the buffer in the t_args structure using bb_receive.*/
	for (int i = 0; i < real_args->num_msgs; i++) {
		bb_receive(real_args->buffer, &received_bb_msg);
		//Display the message along with the thread id from the t_args structure using bb_display_msg. */
		bb_display_msg(&received_bb_msg, real_args->t_id);
	}
	/*2.	Display the message along with the thread id from the t_args structure using bb_display_msg. */

	return (NULL);
}
