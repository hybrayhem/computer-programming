#include <math.h>
#include <stdio.h>

int main() {
    float sum = 0.0, old_p = 0.0, p = 0.0; // position
    float f, theta;

    /*for (int i = 0; i < 120; i++) {
        theta = i;
        float gama = theta * 3.14 / 180;
        f = sqrt(pow(259.26, 2) + pow(102.83, 2) - (2 * (259.26) * (102.83) * cos(gama)));
        float d = f - 240;
        printf("Angle: %f -> Distance: %f ", theta, d);

        f = d + 240;
        gama = acos((pow(259.26, 2) + pow(102.83, 2) - pow(f, 2)) / (2 * (259.26) * (102.83)));
        theta = gama * 180 / 3.14;
        printf(" -> Angle: %f\n", theta);
    }*/

    for (f = 240; f <= 340; f += 10) {

        float gama = acos((pow(259.26, 2) + pow(102.83, 2) - pow(f, 2)) / (2 * (259.26) * (102.83)));
        p = gama;
        printf("[%.f] Velocity (rad/s): %f - %f = %f\n", f - 240, p, old_p, p - old_p);
	
	if(p - old_p == p - old_p)
		sum += p - old_p;

        old_p = p;
    }
    sum -= 1.182452;
    printf("\nort %f\n\n\n\n", sum/10.0); // 0.117660



    /*for (f = 240; f <= 340; f += 5) {

        float gama = acos((pow(258.5, 2) + pow(75.24, 2) - pow(f, 2)) / (2 * (258.5) * (75.24)));
        p = gama;
        printf("[%.f] Velocity (rad/s): %f - %f = %f\n", f - 240, p, old_p, p - old_p);
        
	if(p - old_p == p - old_p)
		sum += p - old_p;
	old_p = p;
    }
    sum -= 1.178173;
    printf("\nort %f\n", sum/20.0); // 0.080212 */

    return 0;
}
