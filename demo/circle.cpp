#include <iostream>
#include <cmath>
#include "cpgplot.h"
using namespace std;

int main()
{
	int n;
	cout << "Enter the no. of intervals to perform.\n";
	cin >> n;

	float r;
	cout << "Enter the distance of the planet from the centre.(in AUs)\n";
	cin >> r;

	int n_orbits;
	cout << "Enter the no. of orbits.\n";
	cin >> n_orbits;
	
	float x[n+1], y[n+1], vx[2*n+1], vy[2*n+1], ax[n+1], ay[n+1];
	
	// Notation: Mass => in Solar Masses, Distance => in AUs, Time => in Years
	
	const float G = 39.62545507; // units defined by the notation
	float period = (2*M_PI)*(sqrt(r*r*r))/(sqrt(G)); // Star's Mass = 1 solar mass 
	float dt;
	float x0 = r + 1.0;

	int i;

	dt = (n_orbits)*(period)/(n);

	if (!cpgopen("/XWINDOW")) return 1;

	cpgenv(-(x0),(x0),-(x0),(x0),1,1);
	cpglab("x", "y", "path of orbit");
	cpgsci(5);

	//-----------------------------------------------------------------------------------
	// leapfrog method
	//-----------------------------------------------------------------------------------

	// initial values
	x[0] = r;
	y[0] = 0.0;
	vx[0] = 0.0;
	vy[0] = sqrt(G/r); // constant orbital velocity in +y direction for uniform circular motion

/*	int j;
	for (j=0; j<n; j++) {

		// kick

		ax[j] = ((-1)*(G)/(pow(r,3)))*(x[j]);
		ay[j] = ((-1)*(G)/(pow(r,3)))*(y[j]);

		vx[j+1] = vx[j] + ((dt)/2)*(ax[j]); // Velocity at time = t + dt/2
		vy[j+1] = vy[j] + ((dt)/2)*(ay[j]);

		// drift

		x[j+1] = x[j] + (dt)*(vx[j+1]); // Position drifted forward at t + dt
		y[j+1] = y[j] + (dt)*(vy[j+1]);

	} */
	
	for (i=0; i<(2*n); i+=2) {

		float dist = sqrt((pow((x[int(i/2)]),2))+(pow((y[int(i/2)]),2)));
	
		// kick

		ax[int(i/2)] = ((-1)*(G)/(pow(r,3)))*(x[int(i/2)]); // Acceleration at time = t
		ay[int(i/2)] = ((-1)*(G)/(pow(r,3)))*(y[int(i/2)]); 
		
		vx[i+1] = vx[i] + ((dt)/2)*(ax[int(i/2)]); // Velocity at time = t + dt/2
		vy[i+1] = vy[i] + ((dt)/2)*(ay[int(i/2)]);

		// drift

		x[int(i/2)+1] = x[int(i/2)] + (dt)*(vx[i+1]); // Position drifted forward at t + dt
		y[int(i/2)+1] = y[int(i/2)] + (dt)*(vy[i+1]);

		// cout << "t = " << dt*(int(i/2)+1) << " and" << " Distance = " << dist << "\n";  
		
		// kick

		ax[int(i/2)+1] = ((-1)*(G)/(pow(r,3)))*(x[int(i/2)+1]); // Acceleration at time = t + dt
		ay[int(i/2)+1] = ((-1)*(G)/(pow(r,3)))*(y[int(i/2)+1]);

		vx[i+2] = vx[i+1] + ((dt)/2)*(ax[int(i/2)+1]); // Velocity at time = t + dt
		vy[i+2] = vy[i+1] + ((dt)/2)*(ay[int(i/2)+1]);

		// cout << "Acceleration = " << sqrt((pow((ax[int(i/2)]),2))+(pow((ay[int(i/2)]),2))) << "\n";

	} 
	
	// Plot the curve
	cpgline(n+1,x,y);

	// Pause and then close the plot window
	cpgclos();
}
