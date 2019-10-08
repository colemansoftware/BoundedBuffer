/* Nicholas Coleman
 * CSCI 4100
 * Assignment 3
 * Header file for bounded buffer implementation
 */

#ifndef __BBUFF_H__
#define __BBUFF_H__

#include <pthread.h>

#define BUFFER_SIZE 10

/* Structure for a message */
struct bb_msg {

  /* Thread ID */
  int t_id;

  /* Message ID */
  int m_id;
};

/* Initialize a message structure */
void bb_init_msg(struct bb_msg *msg, int t_id, int m_id);

/* Copy the source message to the destination message */
void bb_copy_msg(struct bb_msg * source, struct bb_msg * destination);

/* Display the contents of a message along with the id of the receiver */
void bb_display_msg(struct bb_msg *msg, int receiver);

/* Structure for a bounded buffer */
struct bbuff {
  
  /* Array of messages */
  struct bb_msg messages[BUFFER_SIZE];

  /* Number of messages sent */
  unsigned int in;

  /* Number of messages received */
  unsigned int out;

  /* Lock to protect shared data */
  pthread_mutex_t lock;
};

/* Initialize the bounded buffer */
void bb_init(struct bbuff * buffer);

/* Send a message to a bounded buffer */
void bb_send(struct bbuff * buffer, struct bb_msg * message);

/* Receive a message from a bounded buffer */
void bb_receive(struct bbuff * buffer, struct bb_msg * message);

#endif
