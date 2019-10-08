/* Coleman DeMars
 * CSCI 4100
 * Assignment 3
 * Source file for bounded buffer implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include "bbuff.h"
#include <pthread.h>

#include <unistd.h>

/* Initialize a message structure */
void bb_init_msg(struct bb_msg *msg, int t_id, int m_id) {
	/* YOUR CODE HERE */
	/*used to initialize a message with given thread id and message id. from send_msg function
	 * t_id and m_id is from t_args struct*/
	msg->m_id = m_id;
	msg->t_id = t_id;
}

/* Copy the source message to the destination message */
void bb_copy_msg(struct bb_msg * source, struct bb_msg * destination) {
	/* YOUR CODE HERE */
	destination->m_id = source->m_id;
	destination->t_id = source->t_id;
}

/* Display the contents of a message along with the id of the receiver */
void bb_display_msg(struct bb_msg *msg, int receiver) {
	/* YOUR CODE HERE */
	printf("\n[Sending thread: %d, message %d, receiving thread: %d]", msg->t_id, msg->m_id, receiver);
}

/* Initialize the bounded buffer */
void bb_init(struct bbuff * buffer) {
	/* YOUR CODE HERE */
	/*initialize the lock*/
	pthread_mutex_init(&buffer->lock, NULL);
	buffer->in = 0; //set in and out to 0
	buffer->out = 0;
	for (int i = 0; i < BUFFER_SIZE; i++) {
		buffer->messages[i].m_id = 0;
		buffer->messages[i].t_id = 0;
	}

}

/* Send a message to a bounded buffer */
void bb_send(struct bbuff * buffer, struct bb_msg * message) {
	/* YOUR CODE HERE */
	/*Acquire buffer lock*/
	pthread_mutex_lock(&buffer->lock);
	//printf("buffer %d, %d", buffer->in, buffer->out);

	/* while buffer is full, release lock, then acquire it again so that receiving thread can access the buffer*/
	int t = buffer->in - buffer->out;

	if (t == (BUFFER_SIZE)) {
		//buffer->in--;
		//release lock
		pthread_mutex_unlock(&buffer->lock);
		sleep(1);
		//call to receiving thread so we can empty the buffer
		//bb_receive(buffer, message);

		//lock buffer
		pthread_mutex_lock(&buffer->lock);

	} else {

		/* Load the BUFFER*/
		//bb_copy_msg(message, buffer->messages[buffer->in]);
		buffer->messages[buffer->in].m_id = message->m_id;
		buffer->messages[buffer->in].t_id = message->t_id;
		//printf(" Messages %d, thread %d\n", buffer->messages[buffer->in].m_id,buffer->messages[buffer->in].t_id);
		/* increment the buffer's in variable */
		buffer->in++;

		/* release the buffer lock*/
		pthread_mutex_unlock(&buffer->lock);
	}

}

/* Receive a message from a bounded buffer */
void bb_receive(struct bbuff *buffer, struct bb_msg * message) {
	/* YOUR CODE HERE */
	/*Acquire buffer lock*/
	pthread_mutex_lock(&buffer->lock);

	/* while buffer is empty, release lock, then acquire it again so that sending thread can access the buffer*/
	int t = buffer->in - buffer->out;
	if (t == 0) {
		//release lock
		//printf("\nBuffer is CLEAR");
		pthread_mutex_unlock(&buffer->lock);
		sleep(.5);
		//call the send so we can refill the buffer
		//bb_send(buffer, message);

		return;
		//lock so the buffer can be emptied
		pthread_mutex_lock(&buffer->lock);
	} else {

		/* copy message from bb_msh structure provided to the correct location in the buffer using the bb_copy_msg function*/
		//bb_copy_msg(message, buffer->messages);
		/* increment the buffer's out variable */
		message->m_id = buffer->messages[buffer->out].m_id;
		message->t_id = buffer->messages[buffer->out].t_id;
		buffer->out++;

		/* release the buffer lock*/
		pthread_mutex_unlock(&buffer->lock);
	}

}
