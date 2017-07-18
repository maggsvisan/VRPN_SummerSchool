#include "vrpn_Tracker.h"

#include <iostream>
#include <cmath> 
#include "Winuser.h"

using namespace std;

double spine; // 3
double y_leftHand;//7
double y_rightHand;//11
double x_leftHand;
double x_rightHand;

double x_head;
double y_head;
double x_POS;
double y_POS;

double result;
int state= 0;// off
int previousState=0;


void openPaint(){
	system("start mspaint.exe");
	state = 1;
}

void closePaint (){
	system("TASKKILL /F /IM mspaint.exe");
	state=0;
}


void VRPN_CALLBACK handle_tracker( void* userData, const vrpn_TRACKERCB b )
{
	if (b.sensor == 3) {
		spine= b.pos[1];
		cout << "spine:" <<spine<<endl;
	}

	if (b.sensor == 0) {
		y_head= b.pos[1];
		cout << "y_head:" <<y_head<<endl;
	}

	if (b.sensor == 7) {
		y_leftHand= b.pos[1];
		x_leftHand=b.pos[0];
		cout << "y_leftHand:" << y_leftHand<<endl;
	}

	if (b.sensor == 11) {

		y_rightHand= b.pos[1];
		x_rightHand=b.pos[0];
		
		//move the hand with calibration
		x_POS= x_rightHand*1000;
		y_POS= y_rightHand*1000;
		
		SetCursorPos(x_POS, y_POS);
		
	}

	result= abs(x_leftHand-x_rightHand);

	//Motion State
	if (result < 0.04 ) { 
		cout<< "clap";

		if (state==0) {
			openPaint();
		}
	}

	if (y_head < y_leftHand  ){
		cout << "y_leftHandUp" <<endl;
		if (state==1){
			closePaint ();
		}
		
	}

	else if (spine <y_rightHand){
		cout<< "y_rightHandUp"<< endl;
	}

	else{
		cout <<"reading"<< endl;
	}
}

int main(int argc, char* argv[])
{
	vrpn_Tracker_Remote* vrpnTracker0 = new vrpn_Tracker_Remote("Tracker0@localhost:3883");//Changer en local
	vrpnTracker0->register_change_handler(0, handle_tracker);
	while(1)
	{	
		vrpnTracker0->mainloop();
		Sleep(100);
	}

	return 0;
}
