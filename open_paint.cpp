#include "vrpn_Tracker.h"

#include <iostream>
#include <cmath> 
#include "Winuser.h"

using namespace std;

double x_spine;
double y_spine;

int newX;
int newY;
//double spine; // 3
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
		x_spine = b.pos[0];
		y_spine = b.pos[1];
		//cout << "spine:" <<spine<<endl;
	}

	if (b.sensor == 0) {
		y_head= b.pos[1];
		x_head= b.pos[0];
		//cout << "y_head:" <<y_head<<endl;
	}

	if (b.sensor == 7) {
		y_leftHand= b.pos[1];
		x_leftHand=b.pos[0];
	//	cout << "y_leftHand:" << y_leftHand<<endl;
	}

	if (b.sensor == 11 && state==1 ) {

		y_rightHand= b.pos[1];
		x_rightHand=b.pos[0];
		int newX = (x_rightHand - x_head + 0.3)*1960 ;
		int newY = -(y_rightHand- y_head)*1020;
		//move the hand with calibration
		//x_POS= x_rightHand*30000;
		//y_POS= -(y_rightHand*2000);
		
		//SetCursorPos(x_POS, y_POS);
		SetCursorPos(newX, newY);
		
	}

	result= abs(x_leftHand-x_rightHand);
	

	cout << "result : "<< result <<"\n";

	//Motion State
	if (result < 0.4 ) { 
	//	cout<< "clap";

		if (state==0) {
			openPaint();
		}
	}

	if (y_head < y_leftHand && y_head < y_rightHand ){
	//	cout << "y_leftHandUp" <<endl;
		if (state==1){
			closePaint();
		}	
	}

	if ((y_head < y_leftHand) && state==1 ){
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	}
	
	if ((y_head > y_leftHand) && state==1 ){
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	}
	
	
	if (y_spine < y_rightHand ){
	//	cout<< "y_rightHandUp"<< endl;
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
