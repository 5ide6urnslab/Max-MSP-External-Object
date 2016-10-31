/*************************************************************************
 * File Name          : dataCoupling
 * Author             : Show Kawabata(5ide6urns lab)
 * Version            : v1.01
 * Date               : 03/11/2016
 * Parts required     : Arduino UNO R3, Thumb Joystick(Sparkfun)
 * Description        : The data of 1byte(MSB) is combined with other data of 1byte(LSB).
 *
 *                      (ex.) The serial communication of Arduino send every 1byte data.
 *                            This data of 2byte can not be send once.
 *                            It is splited every 1byte data, when the serial is send.
 *
 * Copyright          : Copyright (C) 2016 5ide6urns lab All right reserved.
 *
 * License            : This program is free software; you can redistribute it
 *                      and/or modify it under the terms of the GNU General Public
 *                      License as published by the Free Software Foundation;
 *                      either version 2 of the License, or (at your option) any
 *                      later version.
 *
 *                      This program is distributed in the hope that it will be
 *                      useful, but WITHOUT ANY WARRANTY; without even the implied
 *                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *                      PURPOSE. See the GNU General Public License for more
 *                      details.
 *
 *                      You should have received a copy of the GNU General Public
 *                      License along with this program; if not, write to the Free
 *                      Software Foundation, Inc., 59 Temple Place, Suite 330,
 *                      Boston, MA 02111-1307 USA
 *
 * History            : 02/29/2016 v1.00 Show Kawabata Create on.
 *                      03/11/2016 v1.01 Show Kawabata [Bug fix] check the start bit of send data.
 **************************************************************************/

#include "ext.h"							// standard Max include, always required.
#include "ext_obex.h"						// required for new style Max object.

////////////////////////// object struct
typedef struct _dataCoupling
{
	t_object					ob;			// the object itself (must be first).
    
    /* [Note]: int value
     *   - received from the left inlet and stored internally for each object instance.
     */
    long                        p_value0;   // 1st MSB(8bit)
    long                        p_value1;   // 1st LSB(8bit)
    
    long                        receiveSts; // 1:MSB, 2:LSB

    /* [Note]: outlet creation
     *   - inlets are automatic, but objects must "own" their own outlets.
     */
    void                        *p_outlet;  // In the case of 1 data coupling
    
} t_dataCoupling;

///////////////////////// function prototypes
void *dataCoupling_new(long n);
void dataCoupling_free(t_dataCoupling *x);
void dataCoupling_assist(t_dataCoupling *x, void *b, long m, long a, char *s);

void dataCoupling_bang(t_dataCoupling *x);
void dataCoupling_int(t_dataCoupling *x, long n);

//////////////////////// global class pointer variable
void *dataCoupling_class;


/*! *******************************************************************
 *  @fn         ext_main [Default function]
 *  @brief      This function is the main process of external object.
 *
 *  @param[in]  r   :   Unknown.
 *  @return     void
 *  @version    v1.00
 *  @date       02/29/2016  v1.00:  Create on.
 ***********************************************************************/
void ext_main(void *r)
{
	t_class *c;

    // A_DEFLONG : int type argument checked the type is default 0.
	c = class_new("dataCoupling", (method)dataCoupling_new, (method)dataCoupling_free, (long)sizeof(t_dataCoupling),
				  0L /* leave NULL!! */, A_DEFLONG, 0);

	/* you CAN'T call this from the patcher */
    
    // (optional) assistance method needs to be declared like this.
	class_addmethod(c, (method)dataCoupling_assist,	"assist", A_CANT, 0);
    
    // the method it uses when it gets a bang in the left inlet.
    class_addmethod(c, (method)dataCoupling_bang, "bang", 0 );
    
    // the method for an int in the left inlet (inlet 0).
    class_addmethod(c, (method)dataCoupling_int, "int", A_LONG, 0);
    
    // add to the hint function(CLASS_BOX).
	class_register(CLASS_BOX, c);
	dataCoupling_class = c;

	post(" dataCoupling object loaded... ", 0);
}

/*! *******************************************************************
 *  @fn         dataCoupling_assist [Default function]
 *  @brief      This function is the assist message for inlet and outlet.
 *
 *  @param[in]  x   :   the instance of object struct.
 *  @param[in]  b   :   Unknown.
 *  @param[in]  m   :   the mouse pointer status.
 *  @param[in]  a   :   the number of inlet and outlet.
 *  @param[out] s   :   the assist message.
 *  @return     void
 *  @version    v1.00
 *  @date       02/29/2016  v1.00:  Create on.
 ***********************************************************************/
void dataCoupling_assist(t_dataCoupling *x, void *b, long m, long a, char *s)
{
    /******************************************
     * Inlet assist
     ******************************************/
	if (m == ASSIST_INLET) {
		switch (a) {
            case 0:
                sprintf(s, " Inlet %ld: Left Int Type. You must send the 1st data of 0x7E before data content. ", a);
                break;
            default:
                break;
		}
	}
    /******************************************
     * Outlet assist
     ******************************************/
	else {
		switch (a) {
            case 0:
                sprintf(s, " 1st data of 2byte coupling ");
                break;
            default:
                break;
		}
	}
}

/*! *******************************************************************
 *  @fn         dataCoupling_free [Default function]
 *  @brief      This function is released the object.
 *
 *  @param[in]  x   :   the instance of object struct.
 *  @return     void
 *  @version    v1.00
 *  @date       02/29/2016  v1.00:  Create on.
 ***********************************************************************/
void dataCoupling_free(t_dataCoupling *x)
{
	;
}

/*! *******************************************************************
 *  @fn         dataCoupling_new [Default function]
 *  @brief      This function is created the Max external object
 *              and initilize process.
 *
 *  @param[in]  n   :   the received data in the left inlet.
 *                      the int argument typed into object box (A_DEFLONG).
 *                      -- defaults to 0 if no args are typed.
 *  @return     void
 *  @version    v1.01
 *  @date       02/29/2016  v1.00:  Create on.
 *              03/11/2016  v1.01:  [Bug fix] check the start bit of send data.
 ***********************************************************************/
void *dataCoupling_new(long n)
{
	t_dataCoupling *x;

    // create a new instance of this object.
	x = (t_dataCoupling *)object_alloc(dataCoupling_class);

    /*  [Note]: create a 2nd int inlet
     *    - If the 2nd int inlet is added, you must be called the function "intin(x,1)".
     *      (leftmost inlet is automatic - all objects have one inlet by default)
     */
    
    // create an int outlet and assign it
    // to our outlet variable in the instance's data structure.
    x->p_outlet  = outlet_new(x, NULL);
    
    
    // set initial (default) left operand value in the instance's data structure.
    x->p_value0	= 0;
    x->p_value1 = 0;
    
    x->receiveSts = 0;  // [v1.01]
    
    post(" new coupling object instance added to patch... ",0);

	return (x);
}

/*! *******************************************************************
 *  @fn         dataCoupling_int
 *  @brief      This function is stored the data for coupling.
 *
 *  @param[in]  x   :   the instance of object struct.
 *  @param[in]  n   :   the received data in the left inlet.
 *  @return     void
 *  @version    v1.01
 *  @date       02/29/2016  v1.00:  Create on.
 *              03/11/2016  v1.01:  [Bug fix] check the start bit of send data.
 ***********************************************************************/
void dataCoupling_int(t_dataCoupling *x, long n){
    
    /******************************************
     * [v1.01] Start Bit
     ******************************************/
    if(x->receiveSts == 0){
        
        // check the start bit.
        if(n == 0x7E){
            
            // If the Start bit is "0x7E", the data coupling process is implemented.
            x->receiveSts = 1;
        }
    }
    /******************************************
     * MSB
     ******************************************/
     else if(x->receiveSts == 1){
         
         x->p_value0 = n;
         x->receiveSts = 2;

         post(" The input of MSB is %ld ",x->p_value0);
     }
     /******************************************
      * LSB
      ******************************************/
     else if(x->receiveSts == 2){

        x->p_value1 = n;
         
        // the 2nd data is end. If It is received, the process is returned Start bit checking process.
        x->receiveSts = 0;
            
        dataCoupling_bang(x);

        post(" The input of LSB is %ld ",x->p_value1);
    }
}

/*! *******************************************************************
 *  @fn         dataCoupling_bang
 *  @brief      This function is the data coupling.
 *
 *  @param[in]  x   :   the instance of object struct.
 *  @return     void
 *  @version    v1.00
 *  @date       02/29/2016  v1.00:  Create on.
 ***********************************************************************/
void dataCoupling_bang(t_dataCoupling *x)
{
    // MSB(8bit) and LSB(8bit) is combined.
    long coupling = (x->p_value0 << 8) | x->p_value1;
    outlet_int(x->p_outlet, coupling);

    post(" The output of MSB is %ld ", x->p_value0);
    post(" The output of LSB is %ld ", x->p_value1);
    
    post(" Data Coupling is %ld ", coupling);
}