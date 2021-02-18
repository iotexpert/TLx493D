#include <math.h>

#include "TLx493D.h"
#include "TLXJoyStick.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif


void TLxJoystickCovertXY(TLx493D_data_frame_t *frame, TLxJoyStickXY_t *result)
{

    float radius;
    float theta;

  	/* To calculate the Joystick angle,  we need to convert to spherical coordinates */
    radius = sqrt( pow((float)frame->x, 2) + pow((float)frame->y, 2) + pow((float)frame->z, 2) );
    theta = acos ((float)frame->z/radius);

    /* Convert theta to a range of 0 to 50 instead of 0 to PI/2 for one quadrant */
    theta = theta * 100 / M_PI;
    /* Cap the max at 50 */
    if(theta > 50)
    {
        theta = 50;
    }

    if (frame->x < 0) /* Joystick is right - we want values from 50 to 100 where 50 is vertical and 100 is right */
    {
        result->x = (int8_t)(theta + 50);
    }
    else /* Joystick is left - we want values from 0 to 50 where 0 is left and 50 is vertical */
    {
        result->x = (int8_t)(50 - theta) ;
    }
}