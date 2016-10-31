
/*************************************************************************
 * File Name          : DataCoupling
 * Author             : Show Kawabata(5ide6urns lab)
 * Version            : v1.00
 * Date               : 10/31/2016
 * Parts required     : Arduino UNO R3
 * Description        :
 * License            : Released under the MIT license.
 *                      http://opensource.org/licenses/mit-license.php
 *
 * Copyright          : Copyright (C) 2016 5ide6urns lab All right reserved.
 * History            : 10/31/2016 v1.00 Show Kawabata Create on.
 **************************************************************************/

#include <Arduino.h>                    // PlatformIO Input Completion.

/*************************************
 * Serial Definition.
 *************************************/
#define SERIAL_BAURATE        115200

/*! ********************************************************************
 *  @fn         setup [Default function]
 *  @brief      This function is the initilize process.
 *
 *  @param[in]  void
 *  @return     void
 *  @version    v1.00
 *  @date       10/31/2016 v1.00 Create on.
 ***********************************************************************/
void setup(){

  /*************************************
   * Serial Initialize Process.
   *************************************/
  Serial.begin(SERIAL_BAURATE);

  return;
}

/*! ********************************************************************
 *  @fn         loop [Default function]
 *  @brief      This function is the loop process.
 *
 *  @param[in]  void
 *  @return     void
 *  @version    v1.00
 *  @date       10/31/2016 v1.00 Create on.
 ***********************************************************************/
void loop(){
  
  short a = 2;

  Serial.write(0x7E);
  Serial.write(a >> 8);
  Serial.write(a & B11111111);

  return;
}
