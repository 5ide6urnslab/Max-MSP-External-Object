/*************************************************************************
 * File Name          : convertBpm2Ms
 * Author             : Show Kawabata(5ide6urns lab)
 * Version            : v1.01
 * Date               : 06/25/2016
 * Parts required     : Adafruit Trinket Pro(5v), XBee ZB, Pulse Sensor, 
 *                      dataCoupling object(Max External Object)
 *
 * Description        : A BPM of Pulse Sensor is converted a Microsecound(MS).
 *
 * License            :
 * Copyright          : Copyright (C) 2016 5ide6urns lab All right reserved.
 * History            : 06/21/2016 v1.00 Show Kawabata Create on.
 *                      06/25/2016 v1.01 Show Kawabata [Bug fix] cast as float type.
 **************************************************************************/

#include "ext.h"							// standard Max include, always required.
#include "ext_obex.h"						// required for new style Max object.

//#define DEBUG                               // For Console Debug.

////////////////////////// object struct
typedef struct _convertBpm2Ms
{
	t_object					ob;			// the object itself (must be first).
    
    /* [Note]: float value
     *   - received from the left inlet.
     */
    float                        p_value;   // [Left Inlet] BPM (Bit Per Minutes).
    
    
    /* [Note]: outlet creation
     *   - inlets are automatic, but objects must "own" their own outlets.
     */
    void                        *p_outlet;  // [Left Outlet] Outlet instance.
    
} t_convertBpm2Ms;

///////////////////////// function prototypes
void *convertBpm2Ms_new(long n);
void convertBpm2Ms_free(t_convertBpm2Ms *x);
void convertBpm2Ms_assist(t_convertBpm2Ms *x, void *b, long m, long a, char *s);

void convertBpm2Ms_int(t_convertBpm2Ms *x, long n);
void convertBpm2Ms_bang(t_convertBpm2Ms *x);

//////////////////////// global class pointer variable
void *convertBpm2Ms_class;


/*! *******************************************************************
 *  @fn         ext_main [Default function]
 *  @brief      This function is the main process of external object.
 *
 *  @param[in]  r   :   Unknown.
 *  @return     void
 *  @version    v1.00
 *  @date       06/21/2016  v1.00:  Create on.
 ***********************************************************************/
void ext_main(void *r)
{
	t_class *c;

    // A_DEFLONG : int type argument checked the type is default 0.
	c = class_new("convertBpm2Ms", (method)convertBpm2Ms_new,
                  (method)convertBpm2Ms_free, (long)sizeof(t_convertBpm2Ms),
				  0L /* leave NULL!! */, A_DEFLONG, 0);

	/* you CAN'T call this from the patcher */
    
    // (optional) assistance method needs to be declared like this.
	class_addmethod(c, (method)convertBpm2Ms_assist, "assist", A_CANT, 0);
    
    // the method it uses when it gets a bang in the left inlet.
    class_addmethod(c, (method)convertBpm2Ms_bang, "bang", 0 );
    
    // the method for an float in the left inlet (inlet 0).
    class_addmethod(c, (method)convertBpm2Ms_int, "int", A_LONG, 0);
    
    // add to the hint function(CLASS_BOX).
	class_register(CLASS_BOX, c);
	convertBpm2Ms_class = c;

//-----------------------------------------------------------------------
#ifdef DEBUG
	post(" convertBpm2Ms object loaded... ", 0);
#endif
//-----------------------------------------------------------------------
    
    return;
}

/*! *******************************************************************
 *  @fn         convertBpm2Ms_assist [Default function]
 *  @brief      This function is the assist message for inlet and outlet.
 *
 *  @param[in]  x   :   the instance of object struct.
 *  @param[in]  b   :   Unknown.
 *  @param[in]  m   :   the mouse pointer status.
 *  @param[in]  a   :   the number of inlet and outlet.
 *  @param[out] s   :   the assist message.
 *  @return     void
 *  @version    v1.00
 *  @date       06/21/2016  v1.00:  Create on.
 ***********************************************************************/
void convertBpm2Ms_assist(t_convertBpm2Ms *x, void *b, long m, long a, char *s)
{
    /******************************************
     * Inlet assist.
     ******************************************/
	if (m == ASSIST_INLET) {
		switch (a) {
            case 0:
                sprintf(s, " Inlet %ld: Left Float Type. You must send the BPM data. ", a);
                break;
            default:
                break;
		}
	}
    /******************************************
     * Outlet assist.
     ******************************************/
	else {
		switch (a) {
            case 0:
                sprintf(s, " Outlet %ld: Left Float Type. A BPM is converted a time (ms). ", a);
                break;
            default:
                break;
		}
	}
    
    return;
}

/*! *******************************************************************
 *  @fn         convertBpm2Ms_free [Default function]
 *  @brief      This function is released the object.
 *
 *  @param[in]  x   :   the instance of object struct.
 *  @return     void
 *  @version    v1.00
 *  @date       06/21/2016  v1.00:  Create on.
 ***********************************************************************/
void convertBpm2Ms_free(t_convertBpm2Ms *x)
{
	;
}

/*! *******************************************************************
 *  @fn         convertBpm2Ms_new [Default function]
 *  @brief      This function is created the Max external object
 *              and initilize process.
 *
 *  @param[in]  n   :   the received data in the left inlet.
 *                      the int argument typed into object box (A_DEFLONG).
 *                      -- defaults to 0 if no args are typed.
 *  @return     void
 *  @version    v1.00
 *  @date       06/21/2016  v1.00:  Create on.
 ***********************************************************************/
void *convertBpm2Ms_new(long n)
{
	t_convertBpm2Ms *x;

    // create a new instance of this object.
	x = (t_convertBpm2Ms *)object_alloc(convertBpm2Ms_class);

    /*  [Note]: create a 2nd inlet
     *    - If the 2nd inlet is added, you must be called the function "intin(x,1)".
     *      (leftmost inlet is automatic - all objects have one inlet by default)
     */
    
    // create an int outlet and assign it
    // to our outlet variable in the instance's data structure.
    x->p_outlet = outlet_new(x, NULL);
    
    
    // set initial (default) left operand value in the instance's data structure.
    x->p_value = 0.0;

//-----------------------------------------------------------------------
#ifdef DEBUG
    post(" Max/MSP External Object convertBpm2Ms Instance added to patch... ",0);
#endif
//-----------------------------------------------------------------------
    
	return (x);
}

/*! *******************************************************************
 *  @fn         convertBpm2Ms_int
 *  @brief      This function is converted from a BPM to a Microsecound.
 *
 *  @param[in]  x   :   the instance of object struct.
 *  @param[in]  n   :   the received data in the left inlet.
 *  @return     void
 *  @version    v1.01
 *  @date       06/21/2016  v1.00:  Create on.
 *              06/25/2016  v1.01:  [Bug fix] cast as float type.
 ***********************************************************************/
void convertBpm2Ms_int(t_convertBpm2Ms *x, long n){

//-----------------------------------------------------------------------
#ifdef DEBUG
    post(" A BPM data is %ld ",n);
#endif
//-----------------------------------------------------------------------
    
    /*  [Note]: A BPM is converted a time (ms = note length).
     *
     *      ms = (60 / BPM) * (A) * 1000
     *
     *      "A" is a note.
     *      (ex) "A" quarter note is 4/4.
     */
    float ms   = (4/4) * 1000 * (60 / (float)n);        // [v1.01]
    x->p_value = ms;

//-----------------------------------------------------------------------
#ifdef DEBUG
    post(" A time (ms) data is %ld ",ms);
#endif
//-----------------------------------------------------------------------
    
    convertBpm2Ms_bang(x);

    return;
}

/*! *******************************************************************
 *  @fn         convertBpm2Ms_bang
 *  @brief      This function is outputed from outlet.
 *
 *  @param[in]  x   :   the instance of object struct.
 *  @return     void
 *  @version    v1.00
 *  @date       06/21/2016  v1.00:  Create on.
 ***********************************************************************/
void convertBpm2Ms_bang(t_convertBpm2Ms *x)
{
    outlet_float(x->p_outlet, x->p_value);

    
//-----------------------------------------------------------------------
#ifdef DEBUG
    post(" The output of a time (ms) data is %ld ", x->p_value);
#endif
//-----------------------------------------------------------------------
    
    return;
}