#include <iostream>
#include <cmath>
#include "cpgplot.h"
#include "aux.h"

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
	
	float x[n+1], y[n+1], vx[2*n+1], vy[2*n+1], ax[n+1], ay[n+1],
		  X1[n+1], Y1[n+1], X2[n+1], Y2[n+1], r1[n+1], r2[n+1];
	
	// Notation: Mass => in Solar Masses, Distance => in AUs, Time => in Years
	
	const float G = 39.62545507; // units defined by the notation
	float period = (2*M_PI)*(sqrt(r*r*r))/(sqrt(2*G)); // Centre of Mass = 2 solar masses 
	float dt;

	int i, j;

	dt = (n_orbits)*(period)/(n);

	//-----------------------------------------------------------------------------------
	// leapfrog method
	//-----------------------------------------------------------------------------------

	// initial values
	
	x[0] = r;
	y[0] = 0.0;
	vx[0] = 0.0;
	vy[0] = (sqrt(2*G/r));


	for (i=0; i<(2*n); i+=2) {

		j = int(i/2);

		// kick

		X1[j] = pos_Mx (dt*j);	// Position of Stars at time = t
		X2[j] = (-1)*(pos_Mx (dt*j));
		Y1[j] = pos_My (dt*j);
		Y2[j] = (-1)*(pos_My (dt*j));

		r1[j] = dist_P (x[j], X1[j], y[j], Y1[j]);
		r2[j] = dist_P (x[j], X2[j], y[j], Y2[j]);

		ax[j] = acceleration (x[j], X1[j], X2[j], r1[j], r2[j]); // Acceleration at time = t
		ay[j] = acceleration (y[j], Y1[j], Y2[j], r1[j], r2[j]);
				
		vx[i+1] = vx[i] + ((dt)/2)*(ax[j]); // Velocity at time = t + dt/2
		vy[i+1] = vy[i] + ((dt)/2)*(ay[j]);

		// drift

		x[j+1] = x[j] + (dt)*(vx[i+1]); // Position of planet drifted forward at t + dt
		y[j+1] = y[j] + (dt)*(vy[i+1]);
		
		// kick
		
		X1[j+1] = pos_Mx (dt*(j+1));	// Position of Stars at time = t + dt
		X2[j+1] = (-1)*(pos_Mx (dt*(j+1)));
		Y1[j+1] = pos_My (dt*(j+1));
		Y2[j+1] = (-1)*(pos_My (dt*(j+1)));

		r1[j+1] = dist_P (x[j+1], X1[j+1], y[j+1], Y1[j+1]);
		r2[j+1] = dist_P (x[j+1], X2[j+1], y[j+1], Y2[j+1]);

		ax[j+1] = acceleration (x[j+1], X1[j+1], X2[j+1], r1[j+1], r2[j+1]); // Acceleration at time = t + dt
		ay[j+1] = acceleration (y[j+1], Y1[j+1], Y2[j+1], r1[j+1], r2[j+1]);

		vx[i+2] = vx[i+1] + ((dt)/2)*(ax[j+1]); // Velocity at time = t + dt
		vy[i+2] = vy[i+1] + ((dt)/2)*(ay[j+1]);

	} 

	/*for (i=0; i<n; i++) {
		cout << "X1[" << i << "] = " << X1[i] << "\n";
	}*/

	
	if (!cpgopen("/XWINDOW")) return 1;

	cpgenv(-(r + 10.0),(r + 10.0),-(r + 10.0),(r + 10.0),1,1);
	cpglab("x", "y", "path of orbit");
	cpgsci(5);
	
	// Plot the curve
	cpgline(n+1,x,y);

	cpgsci(3);
	cpgline(n+1,X1,Y1);	

	// Energy plot, different axes (after a pause)
	cpgenv(0.,2*(period),-25.,25.,0,1);
	cpglab("t", "Energy", "Energy vs. Time");

	float KE[n], PE[n], E[n], T[n];

	for (i=0; i<n; i++) {
		T[i] = dt*i;
	}

	for (i=0; i<(2*n); i+=2) {

		j = int(i/2);
	
		KE[j] = (0.5)*((vx[i+1]*vx[i+1])+(vy[i+1]*vy[i+1]));
		PE[j] = (-1*G)*((1/r1[j])+(1/r2[j]));

		E[j] = KE[j] + PE[j];
	}

	cpgsci(4);
	cpgline(n,T,KE);
	cpgsci(6);
	cpgline(n,T,PE);
	cpgsci(2);
	cpgline(n,T,E);

	// Pause and then close the plot window
	cpgclos();
}
