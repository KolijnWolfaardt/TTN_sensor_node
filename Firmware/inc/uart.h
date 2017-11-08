/* 
 * File:   uart.h
 * Author: kolijn
 *
 * Created on 23 April 2017, 9:47 PM
 */

#ifndef UART_H
#define	UART_H

#define UART1 1
#define UART2 2

void uart1_add_bytes(char buffer[], int length);
void uart_send_message(char buffer[], int length, int UART);
void uart_1_rx_interrupt(void);
void uart_1_tx_interrupt(void);
void setup_uart_1(void);
void setup_uart_2(void);
int uart1_busy();
int uart1_wait_for_message(int max_delay);
void clear_message();
int send_and_wait(char message[], int length, int delay);

void uart_2_rx_interrupt(void);
void uart_2_tx_interrupt(void);


extern char uart_1_rx_buffer[];
extern char uart_1_tx_buffer[];

extern int message_available;
extern int uart1_rx_buf_write_pos;

extern int _u1_tx_buf_read_pos;
extern int _u1_tx_buf_write_pos;

#endif	/* UART_H */

