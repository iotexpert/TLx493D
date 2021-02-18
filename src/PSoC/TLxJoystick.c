#include <math.h>

#include "TLx493D.h"
#include "TLXJoyStick.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define JOYSTICK_L   (50)

void TLxJoystickCovertXY(TLx493D_data_frame_t *frame, TLxJoyStickXY_t *result)
{

    float radius;
    float theta;
    float phi;

  	/* To calculate the Joystick postion, we need spherical coordinates of the magnet's position */
    /* These formulas are taken from application note "Infineon 3D Magnetic Sensor: How to Make a Magnetic Design for Joysticks", Appendix 6.1. */
    /* The app note can be found at: https://www.infineon.com/dgdl/Infineon-3D_Magnetic_Sensor_Joystick-AN-v01_00-EN-AN-v01_00-EN.pdf?fileId=5546d46255dd933d0155e3cf23a91753 */
    radius = sqrt( pow((float)frame->x, 2) + pow((float)frame->y, 2) + pow((float)frame->z, 2) );
    theta = acos ((float)frame->z/radius);
    /* phi */
    if(frame->x == 0)
    {
        if(frame->y < 0 )
        {
            phi = -(M_PI/2.0);
        }
        else // y >= 0
        {
            phi = (M_PI/2.0);
        }
    }
    else
    {
        phi = atan ( (float)frame->y / (float)frame->x );  // Valid if x > 0
        if (frame->x < 0)
        {
            if( frame->y < 0)
            {
                phi = phi - M_PI;
            }
            else // y >= 0
            {
                phi = phi + M_PI;
            }
        }
    }

    /* Convert spherical coordinates into X/Y coordinates of the Joystick tip position from 0 to 100 where the joystick length is 50 */
    /* Note that phi and theta are swapped from the traditional directions. This was done to match what is used in teh application note referenced above */
    /* X =  -(p cos(phi) sin(theta)) */
    /* Y =  -(p sin(phi) sin(theta)) */
    result->x = (int8_t)(JOYSTICK_L - (JOYSTICK_L * cos(phi) * sin(theta)));
    result->y = (int8_t)(JOYSTICK_L - (JOYSTICK_L * sin(phi) * sin(theta)));
}