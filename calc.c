/* GROUP MEMBERS 
THESELE KABELO
MPHO LEMENA
KATLEHO MOTSEKI

*/




#include <minix/drivers.h>
#include <minix/endpoint.h>
#include <minix/rs.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <minix/type.h>
#include <minix/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* Function prototypes */
static void calc_init(void);
static void calc_main_loop(void);
static int calc_receive_message(message *m);
static void calc_handle_add(message *m);
static void calc_handle_multiply(message *m);


int main(int argc, char *argv[])
{
    calc_init();
    calc_main_loop();
    return 0;
}

static void calc_init(void)
{
    printf("Calc server starting...\n");
    
    /* Initialize server */
    if (rs_init() != OK) {
        panic("calc: rs_init failed");
    }
    
    /* Announce we're up */
    chardriver_announce();
}

static void calc_main_loop(void)
{
    message m;
    int result;
    
    while (TRUE) {
        /* Receive message */
        result = calc_receive_message(&m);
        if (result != OK) {
            continue;
        }
        
        /* Handle different message types */
        switch (m.m_type) {
            case CALC_ADD:
                calc_handle_add(&m);
                break;
            case CALC_MULTIPLY:
                calc_handle_multiply(&m);
                break;
            default:
                m.m_type = EINVAL;
                break;
        }
        
        /* Send reply */
        reply(m.m_source, &m);
    }
}

static int calc_receive_message(message *m)
{
    int r;
    
    r = sef_receive(ANY, m);
    if (r != OK) {
        printf("calc: receive failed (%d)\n", r);
        return r;
    }
    
    return OK;
}

static void calc_handle_add(message *m)
{
    int a, b, result;
    
    /* Extract operands */
    a = m->m1_i1;
    b = m->m1_i2;
    
    /* Perform addition in user space */
    result = a + b;
    
    /* Return result */
    m->m1_i1 = result;
    m->m_type = OK;
    //User space call
    printf("calc: add(%d, %d) = %d\n", a, b, result);
}

static void calc_handle_multiply(message *m)
{
    int a, b, result;
    int r;
    
    /* Extract operands */
    a = m->m1_i1;
    b = m->m1_i2;
    
    /* Call kernel for multiplication */
    r = sys_multiply(a, b, &result);
    if (r != OK) {
        m->m_type = r;
        return;
    }
    
    /* Return result */
    m->m1_i1 = result;
    m->m_type = OK;
    
    printf("calc: multiply(%d, %d) = %d (via kernel)\n", a, b, result);
}