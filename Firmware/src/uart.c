/*
 The uart callback system assumes that commands will be terminated with 
 * a <CR><LF>, as it is mainly intended for working with the RN2483 
 * module.
 */
#include <xc.h>
#include <string.h>
#include <pic18f26j50.h>

#include "hardware_init.h"
#include "uart.h"
#include "timer.h"

#define UART_BUFF_SIZE 128

char uart_1_rx_buffer[UART_BUFF_SIZE];
char uart_1_tx_buffer[UART_BUFF_SIZE];

int uart1_rx_buf_write_pos = 0;
int _tx_buf_read_pos = 0;
int _tx_buf_write_pos = 0;

int message_available = 0;

void uart1_add_bytes(char buffer[], int length)
{
//    int i = 0;
//    
//    for (i = 0;i < length; i++)
//    {
//        uart_1_tx_buffer[_tx_buf_write_pos] = buffer[i];
//        
//        _tx_buf_write_pos++;
//        
//        if (buffer[i] == '\n')
//        {
//            // Send the first byte
//            TXREG1 = uart_1_tx_buffer[0];
//            _tx_buf_read_pos = 1;
//            PIE1bits.TX1IE = 1;  
//        }
//    }
}

void uart1_send_message(char buffer[], int length)
{
    int i =0;
    
    //Disable the interrupt, so that we don't compete with writing
    INTCONbits.GIE = 0;
    PIE1bits.RC1IE = 0;
    RCONbits.IPEN = 0;
    //Protection against buffer overflow
    if (length< UART_BUFF_SIZE)
    {
        for (i = 0; i<length; i++)
        {
            uart_1_tx_buffer[i] = buffer[i];
        }
    }
    else
    {
        for (i = 0; i<UART_BUFF_SIZE; i++)
        {
            uart_1_tx_buffer[i] = buffer[i];
        }
    }

    add_char_to_usb('T');
    add_char_to_usb('X');
    add_char_to_usb(':');
    add_buffer_to_usb(buffer, length);
    
    PIE1bits.RC1IE = 1;
    INTCONbits.GIE = 1;
    RCONbits.IPEN = 1;
        
    // Send the first byte
    _tx_buf_read_pos = 1;
    _tx_buf_write_pos = length;
    
    TXREG1 = uart_1_tx_buffer[0];
    PIE1bits.TX1IE = 1; 
}

void uart_1_rx_interrupt(void)
{
    // Copy the byte to the buffer
    uart_1_rx_buffer[uart1_rx_buf_write_pos] = RCREG1;
    
    if (uart_1_rx_buffer[uart1_rx_buf_write_pos] == '\n')
    {
        add_char_to_usb(' ');
        add_char_to_usb(' ');
        add_char_to_usb(':');
        add_buffer_to_usb(uart_1_rx_buffer,uart1_rx_buf_write_pos);
        uart1_rx_buf_write_pos++;
        message_available = 1;
    }
    else
    {
        uart1_rx_buf_write_pos++;
    }
    
}

void uart_1_tx_interrupt(void)
{
    //Copy the character to the buffer
    if (_tx_buf_read_pos < _tx_buf_write_pos)
    {
        TXREG1 = uart_1_tx_buffer[_tx_buf_read_pos];
        _tx_buf_read_pos++;

    }
    else
    {
//        // If the positions are now the same, reset both
//        if (_tx_buf_read_pos == _tx_buf_write_pos)
//        {
            _tx_buf_read_pos = 0;
            _tx_buf_write_pos = 0;
            // And disable the interrupt
            PIE1bits.TX1IE = 0;  
//        }
    }
    
    
}

void setup_uart_1(void)
{
    TXSTA1bits.CSRC = 1;        // Async mode, don't care
    TXSTA1bits.TX9 = 0;         // Select 8 bit transmission
    TXSTA1bits.TXEN = 1;        // Enable the transmitter
    TXSTA1bits.SYNC = 0;        // Asynchronous mode
    TXSTA1bits.BRGH = 0;        // High speed mode
    
    RCSTA1bits.SPEN = 1;        // Enable the serial port
    RCSTA1bits.RX9 = 0;         // Disable 9 bits mode
    RCSTA1bits.SREN = 0;        // Don't care
    RCSTA1bits.CREN = 1;        // Enable receiver
    RCSTA1bits.ADDEN = 0;       // Don't care
   
    BAUDCON1bits.BRG16 = 1;     // Enable 16 bit baud generator
    
    SPBRG1 = 51;
//    SPBRG1 = 311;
            
    //  UART 1 is use to transmit to the RN2483 IC, and has a default of:
    //    Baud Rate 57600 bps
    //    Packet Length 8 bit
    //    Parity Bit No
    //    Stop Bits 1 bit
    
    // Make the RX pin an input
    TRISCbits.TRISC7 = 1;
    
    // Disable interrupts
    PIR1bits.TX1IF = 0;  // Clear the flag
    IPR1bits.TX1IP = 1;  // High priority 
    
    PIR1bits.RC1IF = 0; // Clear the flag
    PIE1bits.RC1IE = 1; // Enable the interrupt
    IPR1bits.RC1IP = 1; // High priority
    
    TXSTA1bits.TXEN = 1;        //Enable the transmitter  
}

int uart1_busy()
{
    return (_tx_buf_read_pos == _tx_buf_write_pos);
}

int uart1_wait_for_message(int max_delay)
{    
    delay_ms_no_wait(max_delay);
    
    clear_message();
    while ((message_available == 0) | (timer_0_cycle_counter != 0));
    
    if (message_available)
    {
        stop_timer();
        return uart1_rx_buf_write_pos;
    }
    else
    {
        clear_message();
        stop_timer();
        return -1;
    }
    
    
}


/**
 * Reset the received message
 */
void clear_message()
{
    message_available = 0;
    uart1_rx_buf_write_pos = 0;
}


/** Send the message over the UART. Wait for *delay* microseconds 
 * for a response. If a response is received, return the length
 * of the message. Otherwise return -1, to show that no message 
 * was received.
 */
int send_and_wait(char message[], int length, int delay)
{
//    delay_ms_no_wait(2);
//    while (uart1_busy() & (timer_0_cycle_counter != 0));
    
    clear_message();
    uart1_send_message(message, length);
    while (uart1_busy());
    
    return uart1_wait_for_message(delay);
}
