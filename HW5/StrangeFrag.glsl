#version 330 core
#define PI 3.14159265358979323846264338328
/**
\file StrangeFrag.glsl

\brief Strange fragment shader that colors fragments by the gl_FragCoord.

The shader takes a uniform int for the "option", which is which number the user presses. Numbers 0-9 work, the
fractals were implemented in 7-9. You can switch back and forth as many times, the 0 produces a black screen.


*/

in vec4 pos;
out vec4 col;


uniform int option;
uniform float time;


float t = 0;
/**
setColor function takes care of all of the different uniform variables supplied from the UI/Graphics engine. There are 9
cases. It takes a vec4 p which represents a point on the screen (pixel), since it is called for every fragment in the main.

\param vec4 p --- Current pixel

**/
vec4 setColor(vec4 p)
{


    /** This float r helps us determine the pixel's length from the center of the screen. **/
    float r = length(vec2(p));

    /** If option is 0, just color the entire box that covers the screen black. **/
    if (option == 0)
        return vec4(0, 0, 0, 1);

    /** If option is 1, return a red color based on the pixels value. Sin alternates from -1 to 1 so the colors on screen will
    shift from black to red in a circle since we are using r(length) **/
    if (option == 1)
        return vec4(sin(30*r), 0, 0, 1);

    /** If option is two, do the same thing except set the green value to be a slightly higher than the red waves. **/
    if (option == 2)
        return vec4(sin(30*r+2), sin(30*r), 0, 1);

    /** Here is where we draw red lines in one direction, and purple lines in another. The logic is as follows:
    Look at the floor and ceiling of the pixel's x and y plus and minus .1, and see if the pixel is between there. If it is, color it black.
    Otherwise, color the other possibilies (it lies in the ceiling -.1 margin or floor +.1 margin for both x and y)
    purple or red, depending on x or y.  **/
    if (option == 3)
    {
        //draws the box lines... try to add another line cutting through the middle of each box. Ceil/2?
        if (((20*p.x > floor(20*p.x)+.1 && 20*p.x < ceil(20*p.x)-.1) && (20*p.y > floor(20*p.y)+.1 && 20*p.y < ceil(20*p.y)-.1)) )
            return vec4(0, 0, 0, 1);//after pixels are seperated into boxes, color them black
        else if (20*p.x < floor(20*p.x)+.1 || 20*p.x > ceil(20*p.x)-.1)
            return vec4(.75, 0, .2, 1);
        else if (20*p.y < floor(20*p.y)+.1 || 20*p.y > ceil(20*p.y)-.1)
            return vec4(.5, 0, 1, 1);
    }


    /** Logic for option 4 to make checkerboard: Check if floor of x is even and floor of y is odd. If so, color it red.(y axis)
    This is because we want to color every other box red or black. Also, if the floor of the x is odd, and the floor of the y
    is even, color it red.(x axis) Otherwise make them black. **/
    if (option == 4)
    {

        if (mod(floor(5*p.x), 2) == 0 && mod(floor(5*p.y), 2) != 0)//red y axis
            return vec4(1, 0, 0, 1);
        else if (mod(floor(5*p.x), 2) != 0 && mod(floor(5*p.y), 2) == 0)//red x axis
            return vec4(1, 0, 0, 1);
        else
            return vec4(0, 0, 0, 1);//black




    }
    /** This one used a sine function in the blue slot, but links to the time elapsed. **/
    if (option == 5)
    {
        return vec4(0, 0, sin(15*r-time), 1);

    }


    if (option == 6)
    {
        /** Takes care of y axis alternation, uses the abs value of cosine and sine for each so that when
        one is 1, the other is zero. So when the red value is 1 the green value is zero and vice versa.   **/
        if (mod(floor(5*p.x), 2) == 0 && mod(floor(5*p.y), 2) != 0)//both red cases
            return vec4(abs(cos(time/3)), 0, sin(15*r-time), 1);

        /** Same as above but for the x axis **/
        else if (mod(floor(5*p.x), 2) != 0 && mod(floor(5*p.y), 2) == 0)//red
            return vec4(abs(cos(time/3)), 0, sin(15*r-time), 1);

        /** Used to color the blacks that haven't been touched already. Adds the blue circle for these. **/
        else
            return vec4(0, abs(sin(time/3)), sin(15*r-time), 1);//blacks


    }
    /** This is where the fractals start. **/
    if (option == 7)
    {
        //float i = sqrt(-1); did not work

        float cX = p.x;
        float cY = p.y;//multiplied by i to treat as imaginary??

        float qX = p.x;//initial values
        float qY = p.y;


        float dist = 0;

        for (int i = 0; i < 100; i++)
        {
            float tempQx = qX;
            float tempQy = qY;

            /**find out the q = q^2 + c;
            do (a+bi)^2 = (aa-bb) + (ab + ba)i = q^2 and then add cX and cY
            in this case, a = qX and b = qY
            and (ab+ba) is the imaginary part so that is qY of q;
            so qX + qY = (aa-bb) + (ab + ba)**/

            qX = tempQx*tempQx - tempQy*tempQy;
            qY = tempQx*tempQy + tempQy*tempQx;
            /**now we have q^2**/

            qX = qX + cX;
            qY = qY + cY;
            /**now we have q^2 + c

            now we have to find the length**/
            dist = length(qX) + length(qY);//same as length(q) if it was a point

            /** note the first time it is above 4**/
            if (dist > 4)
            {
                break;
            }
        }
        /** Colors specified on assignment for dist values **/
        if (dist <= 25)
        {
            return vec4(dist/25, 0, 0, 1);

        }
        else if (dist > 25)
            return vec4(1, 1, 1, 1);

    }
    if (option == 8)
    {
        //float i = sqrt(-1); did not work

        float cX = p.x;
        float cY = p.y;//multiplied by i to treat as imaginary??

        float qX = p.x;//initial values
        float qY = p.y;


        float dist = 0;

        for (int i = 0; i < 100; i++)
        {
            float tempQx = qX;
            float tempQy = qY;
            float temp2y, temp2x;

            /**find out the q = q^2 + c;
            do (a+bi)^2 = (aa-bb) + (ab + ba)i = q^2 and then add cX and cY
            in this case, a = qX and b = qY
            and (ab+ba) is the imaginary part so that is qY of q;
            so qX + qY = (aa-bb) + (ab + ba)**/

            qX = tempQx*tempQx - tempQy*tempQy;
            qY = tempQx*tempQy + tempQy*tempQx;

            /**now we have q^2**/

            temp2x = qX;
            temp2y = qY;


            /**now get q^3, using q^2 = temp2x and temp2y**/
            qX = temp2x*tempQx - temp2y*tempQy;
            qY = temp2x*tempQy + temp2y*tempQx;


            qX = qX + cX;
            qY = qY + cY;
            /**now we have q^3 + c**/

            /**now we have to find the length**/
            dist = length(qX) + length(qY);//same as length(q) if it was a point

            if (dist > 4)
            {
                break;
            }
        }

        if (dist <= 25)
        {
            return vec4(dist/25, 0, 0, 1);

        }
        else if (dist > 25)
            return vec4(1, 1, 1, 1);

    }

    if (option == 9)
    {
        //float i = sqrt(-1); did not work

        float cX = p.x;
        float cY = p.y;//multiplied by i to treat as imaginary??

        float qX = p.x;//initial values
        float qY = p.y;


        float dist = 0;

        for (int i = 0; i < 100; i++)
        {
            float tempQx = qX;
            float tempQy = qY;
            float temp2y, temp2x;

            /**find out the q = q^2 + c;
            do (a+bi)^2 = (aa-bb) + (ab + ba)i = q^2 and then add cX and cY
            in this case, a = qX and b = qY
            and (ab+ba) is the imaginary part so that is qY of q;
            so qX + qY = (aa-bb) + (ab + ba)**/

            qX = tempQx*tempQx - tempQy*tempQy;
            qY = tempQx*tempQy + tempQy*tempQx;

            /**now we have q^2**/

            temp2x = qX;
            temp2y = qY;


            /**now get q^3**/
            qX = temp2x*tempQx - temp2y*tempQy;
            qY = temp2x*tempQy + temp2y*tempQx;

            /**now get q^4**/
            float temp3x = qX;
            float temp3y = qY;

            qX = temp3x*tempQx - temp3y*tempQy;
            qY = temp3x*tempQy + temp3y*tempQx;



            qX = qX + cX;
            qY = qY + cY;
            /**now we have q^4 + c

            /**now we have to find the length**/
            dist = length(qX) + length(qY);//same as length(q) if it was a point

            if (dist > 4)
            {
                break;
            }
        }

        if (dist <= 25)
        {
            return vec4(dist/25, 0, 0, 1);

        }
        else if (dist > 25)
            return vec4(1, 1, 1, 1);

    }

}

void main()
{
    col = setColor(pos);
}
