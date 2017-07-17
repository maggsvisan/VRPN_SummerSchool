#include "vrpn_Analog.h"
#include "vrpn_Button.h"

#include <iostream>
using namespace std;

int zone;
double x_coord = 0.0;
double y_coord = 0.0;
int Activated = 1;
int state=0;
int previousState= 0;

void sound(int state){
	cout << state << endl;
	
	if (previousState != state){
		cout <<"\a";
		previousState= state;
	}
	
	else 
		cout<<"Same region"<< endl;
}

void VRPN_CALLBACK handle_analog( void* userData, const vrpn_ANALOGCB a )
{
	int nbChannels = a.num_channel;

	x_coord= a.channel[0];
	y_coord= a.channel[1];

	if(Activated ==1){
		if (x_coord < 0.5 && y_coord <0.5){		
			cout<< "Region A"<< endl;
			state=1;
		}

		if (x_coord > 0.5 && y_coord <0.5){
			cout<< "Region B"<< endl;
			state=2;
		}

		if (x_coord < 0.5 && y_coord > 0.5){
			
			cout<< "Region C"<< endl;
			state=3;			
		}

		if (x_coord > 0.5 && y_coord >0.5){
			
			cout<< "Region D"<< endl;
			state=4;
			
		}
	
		sound(state);
	}


}

void VRPN_CALLBACK handle_button( void* userData, const vrpn_BUTTONCB b )
{ 
	if(b.button==2 && b.state==1 && Activated ==1){ // BIP when rightButton is pressed
		cout <<"\a";

	}

	if(b.button==0&& b.state==1){ // deactivate Client 
		Activated = 0;
		cout <<"Deactivated" << endl;
	}

	if(b.button==2 && b.state==1){ 
		Activated = 1;
		cout <<"Activated" << endl;
	}

	

}

int main(int argc, char* argv[])
{
	vrpn_Analog_Remote* vrpnAnalog = new vrpn_Analog_Remote("Mouse0@localhost:3883");
	vrpn_Button_Remote* vrpnButton = new vrpn_Button_Remote("Mouse0@localhost:3883");
	vrpnAnalog->register_change_handler( 0, handle_analog );
	vrpnButton->register_change_handler( 0, handle_button );

	while(1)
	{
		vrpnAnalog->mainloop();
		vrpnButton->mainloop();
	}

	return 0;
}
