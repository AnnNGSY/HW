//  Train Project -
// Train Window class implementation
// - note: this is code that a student might want to modify for their project
//   see the header file for more details
// - look for the TODO: in this file
// - also, look at the "TrainView" - its the actual OpenGL window that
//   we draw into
//
// Written by Mike Gleicher, October 2008
//




#include "TrainWindow.H"
#include "TrainView.H"
#include "CallBacks.H"

#include <FL/fl.h>
#include <FL/Fl_Box.h>

// for using the real time clock
#include <time.h>



/////////////////////////////////////////////////////
TrainWindow::TrainWindow(const int x, const int y) : Fl_Double_Window(x,y,800,600,"Train and Roller Coaster")
{
	// make all of the widgets
	begin();	// add to this widget
	{
		int pty=5;			// where the last widgets were drawn

		trainView = new TrainView(5,5,590,590);
		trainView->tw = this;
		trainView->world = &world;
		this->resizable(trainView);

		// to make resizing work better, put all the widgets in a group
		widgets = new Fl_Group(600,5,190,590);
		widgets->begin();

		runButton = new Fl_Button(605,pty,60,20,"Run");
		togglify(runButton);

		Fl_Button* fb = new Fl_Button(700,pty,25,20,"@>>");
		fb->callback((Fl_Callback*)forwCB,this);
		Fl_Button* rb = new Fl_Button(670,pty,25,20,"@<<");
		rb->callback((Fl_Callback*)backCB,this);
		
		arcLength = new Fl_Button(730,pty,65,20,"ArcLength");
		togglify(arcLength,1);
  
		pty+=25;
		speed = new Fl_Value_Slider(655,pty,140,20,"speed");
		speed->range(0,10);
		speed->value(2);
		speed->align(FL_ALIGN_LEFT);
		speed->type(FL_HORIZONTAL);

		pty += 30;

		// camera buttons - in a radio button group
		Fl_Group* camGroup = new Fl_Group(600,pty,195,20);
		camGroup->begin();
		worldCam = new Fl_Button(605, pty, 60, 20, "World");
        worldCam->type(FL_RADIO_BUTTON);		// radio button
        worldCam->value(1);			// turned on
        worldCam->selection_color((Fl_Color)3); // yellow when pressed
		worldCam->callback((Fl_Callback*)damageCB,this);
		trainCam = new Fl_Button(670, pty, 60, 20, "Train");
        trainCam->type(FL_RADIO_BUTTON);
        trainCam->value(0);
        trainCam->selection_color((Fl_Color)3);
		trainCam->callback((Fl_Callback*)damageCB,this);
		topCam = new Fl_Button(735, pty, 60, 20, "Top");
        topCam->type(FL_RADIO_BUTTON);
        topCam->value(0);
        topCam->selection_color((Fl_Color)3);
		topCam->callback((Fl_Callback*)damageCB,this);
		camGroup->end();

		pty += 30;

		// browser to select spline types
		// TODO: make sure these choices are the same as what the code supports
		splineBrowser = new Fl_Browser(605,pty,120,68,"Spline Type");
		splineBrowser->type(2);		// select
		splineBrowser->callback((Fl_Callback*)damageCB,this);
		splineBrowser->add("Linear");
		splineBrowser->add("Cardinal Cubic");
		splineBrowser->add("Catmull-Rom");
		splineBrowser->add("B-Splines");
		splineBrowser->select(2);
	
		pty += 100;
		tension = new Fl_Value_Slider(655,pty,140,20,"Tension");
		tension->range(0.05,1);
		tension->value(0.5);
		tension->callback((Fl_Callback*)damageCB,this);
		tension->align(FL_ALIGN_LEFT);
		tension->type(FL_HORIZONTAL);

		pty += 30;
		// add and delete points
		Fl_Button* ap = new Fl_Button(605,pty,80,20,"Add Point");
		ap->callback((Fl_Callback*)addPointCB,this);
		Fl_Button* dp = new Fl_Button(690,pty,80,20,"Delete Point");
		dp->callback((Fl_Callback*)deletePointCB,this);

		pty += 25;
		// reset the points
		resetButton = new Fl_Button(735,pty,60,20,"Reset");
		resetButton->callback((Fl_Callback*)resetCB,this);
		Fl_Button* loadb = new Fl_Button(605,pty,60,20,"Load");
		loadb->callback((Fl_Callback*) loadCB, this);
		Fl_Button* saveb = new Fl_Button(670,pty,60,20,"Save");
		saveb->callback((Fl_Callback*) saveCB, this);

		pty += 25;
		// roll the points
		Fl_Button* rx = new Fl_Button(605,pty,30,20,"R+X");
		rx->callback((Fl_Callback*)rpxCB,this);
		Fl_Button* rxp = new Fl_Button(635,pty,30,20,"R-X");
		rxp->callback((Fl_Callback*)rmxCB,this);
		Fl_Button* rz = new Fl_Button(670,pty,30,20,"R+Z");
		rz->callback((Fl_Callback*)rpzCB,this);
		Fl_Button* rzp = new Fl_Button(700,pty,30,20,"R-Z");
		rzp->callback((Fl_Callback*)rmzCB,this);	

		// TODO: add widgets for all of your fancier features here
		pty+=30;
		// browser to select track types
		// TODO: make sure these choices are the same as what the code supports
		trackBrowser = new Fl_Browser(605,pty,120,68,"Track Type");
		trackBrowser->type(2);		// select
		trackBrowser->callback((Fl_Callback*)damageCB,this);
		trackBrowser->add("Normal");
		trackBrowser->add("No Ties");
		trackBrowser->add("Road");
		trackBrowser->add("Nothing");
		trackBrowser->select(1);

		pty += 100;
		floorFlatness = new Fl_Value_Slider(675,pty,120,20,"Unflatness");
		floorFlatness->range(0,3);
		floorFlatness->value(1);
		floorFlatness->step(0.5);
		floorFlatness->align(FL_ALIGN_LEFT);
		floorFlatness->type(FL_HORIZONTAL);
		floorFlatness->callback((Fl_Callback*)damageCB,this);

		pty += 30;
		carriagesNum = new Fl_Value_Slider(675,pty,120,20,"Carriages");
		carriagesNum->range(0,4);
		carriagesNum->value(0);
		carriagesNum->step(1);
		carriagesNum->align(FL_ALIGN_LEFT);
		carriagesNum->type(FL_HORIZONTAL);
		carriagesNum->callback((Fl_Callback*)damageCB,this);

		pty += 30;
		// weather buttons - in a radio button group
		Fl_Group* weaGroup = new Fl_Group(600,pty,195,20);
		weaGroup->begin();
		fog = new Fl_Button(600, pty, 35, 20, "Fog");
        fog->type(FL_RADIO_BUTTON);		// radio button
        fog->value(0);			
        fog->selection_color((Fl_Color)3); // yellow when pressed
		fog->callback((Fl_Callback*)damageCB,this);
		rain = new Fl_Button(640, pty, 40, 20, "Rain");
        rain->type(FL_RADIO_BUTTON);
		rain->value(0);
        rain->selection_color((Fl_Color)3);
		rain->callback((Fl_Callback*)damageCB,this);
		snow = new Fl_Button(685, pty, 45, 20, "Snow");
        snow->type(FL_RADIO_BUTTON);
        snow->value(0);
        snow->selection_color((Fl_Color)3);
		snow->callback((Fl_Callback*)damageCB,this);
		nowea = new Fl_Button(735, pty, 55, 20, "Normal");
        nowea->type(FL_RADIO_BUTTON);
        nowea->value(1);// turned on
        nowea->selection_color((Fl_Color)3);
		nowea->callback((Fl_Callback*)damageCB,this);
		weaGroup->end();

  /*	weathers, not mu.ex.
		fog = new Fl_Button(605,pty,60,20,"Fog");
		togglify(fog,0);
		snow = new Fl_Button(730,pty,60,20,"Snow");
		togglify(snow,0);
  		rain = new Fl_Button(670,pty,60,20,"Rain");
		togglify(rain,0);
		*/
		pty+=25;
  		physics = new Fl_Button(605,pty,65,20,"Physics");
		togglify(physics,0);
		// we need to make a little phantom widget to have things resize correctly
		Fl_Box* resizebox = new Fl_Box(600,595,200,5);
		widgets->resizable(resizebox);

		widgets->end();
	}
	end();	// done adding to this widget

	// set up callback on idle
	Fl::add_idle((void (*)(void*))runButtonCB,this);
}

// handy utility to make a button into a toggle
void TrainWindow::togglify(Fl_Button* b, int val)
{
    b->type(FL_TOGGLE_BUTTON);		// toggle
    b->value(val);		// turned off
    b->selection_color((Fl_Color)3); // yellow when pressed	
	b->callback((Fl_Callback*)damageCB,this);
}

void TrainWindow::damageMe()
{
	if (trainView->selectedCube >= ((int)world.points.size()))
		trainView->selectedCube = 0;
	trainView->damage(1);
}
float TrainWindow::arcLen(int mode,float dis)
{
	int n;
	if(!mode)
		n= (int)ceil(world.trainU);
	else
		n= (int)ceil(world.carU[mode-1]);
	Pnt3f po1,po2;
	Pnt3f p0(world.points[(n)%world.points.size()].pos);
	Pnt3f p1(world.points[(n+1)%world.points.size()].pos);
	Pnt3f m0=(world.points[(n+world.points.size()-1)%world.points.size()].pos);
	Pnt3f m1=(world.points[(n + 2) %world.points.size()].pos);
	float t;

	if(!mode)
		t=world.trainU-n+1;
	else
		t=world.carU[mode-1]-n+1;
	float tt = t * t;
	float ttt = t * tt;
	if(splineBrowser->value() -1){
		switch (splineBrowser->value()) {
		case 2:{
			const float s = (float)tension->value();
			po1 =(((-1.f * s) * ttt + (2.f * s) * tt + (-1.f * s) * t) * m0 + ((2.f - s) * ttt + (s - 3.f) * tt + 1.f) * p0 + ((s - 2.f) * ttt + (3.f - 2.f * s) * tt + (1.f * s) * t) * p1 + ((1.f * s) * ttt + (-1.f * s) * tt) * m1);
			break;
			   }
		case 3:{
			po1 =(0.5f * ((-1.f * ttt + 2.f * tt - t) * m0 + (3.f * ttt - 5.f * tt + 2.f) * p0 + (-3.f * ttt + 4.f * tt + t) * p1 + (ttt - tt) * m1));
			break;
			   }
		case 4:{
			po1 = ((1.f / 6.f) * ((-1.f * ttt + 3.f * tt - 3.f * t + 1.f) * m0 + (3.f * ttt - 6.f * tt + 4.f) * p0 + (-3.f * ttt + 3.f * tt + 3.f * t + 1.f) * p1 + (1.f * ttt) * m1));
			break;
			   }
		}

		if(!mode)
			t= (world.trainU-n+1.1f);
		else
			t=world.carU[mode-1]-n+1.1f;
		if( t >= world.points.size() )
			t -= world.points.size() ;
		tt = t * t;
		ttt = t * tt;
		switch (splineBrowser->value()) {
			case 2:{
				const float s = (float)tension->value();
				po2 =(((-1.f * s) * ttt + (2.f * s) * tt + (-1.f * s) * t) * m0 + ((2.f - s) * ttt + (s - 3.f) * tt + 1.f) * p0 + ((s - 2.f) * ttt + (3.f - 2.f * s) * tt + (1.f * s) * t) * p1 + ((1.f * s) * ttt + (-1.f * s) * tt) * m1);
			break;
				   }
			case 3:{
				po2 =(0.5f * ((-1.f * ttt + 2.f * tt - t) * m0 + (3.f * ttt - 5.f * tt + 2.f) * p0 + (-3.f * ttt + 4.f * tt + t) * p1 + (ttt - tt) * m1));
			break;
				   }
			case 4:{
				po2 = ((1.f / 6.f) * ((-1.f * ttt + 3.f * tt - 3.f * t + 1.f) * m0 + (3.f * ttt - 6.f * tt + 4.f) * p0 + (-3.f * ttt + 3.f * tt + 3.f * t + 1.f) * p1 + (1.f * ttt) * m1));
			break;
				   }
			}
		Pnt3f l = po1 - po2;
		return (dis /( sqrt(l.x * l.x + l.y * l.y + l.z * l.z)/2));
	}else{
		Pnt3f l =world.points[(n)%world.points.size()].pos-world.points[(n+1)%world.points.size()].pos;	
		return dis/(sqrt(l.x * l.x + l.y * l.y + l.z * l.z)/10);	
	}
}
/////////////////////////////////////////////////////
// this will get called (approximately) 30 times per second
// if the run button is pressed
void TrainWindow::advanceTrain(float dir)
{
	// TODO: make this work for your train
int n = (int)ceil(world.trainU);
	Pnt3f po;
	Pnt3f p0(world.points[(n)%world.points.size()].pos);
	Pnt3f p1(world.points[(n+1)%world.points.size()].pos);
	Pnt3f m0=(world.points[(n+world.points.size()-1)%world.points.size()].pos);
	Pnt3f m1=(world.points[(n + 2) %world.points.size()].pos);
	float t = world.trainU-n+1;
	float tt = t * t;
	float ttt = t * tt;
	if(splineBrowser->value() -1){
		switch (splineBrowser->value()) {
		case 2:{
			const float s = (float)tension->value();
			po =(((-1.f * s) * ttt + (2.f * s) * tt + (-1.f * s) * t) * m0 + ((2.f - s) * ttt + (s - 3.f) * tt + 1.f) * p0 + ((s - 2.f) * ttt + (3.f - 2.f * s) * tt + (1.f * s) * t) * p1 + ((1.f * s) * ttt + (-1.f * s) * tt) * m1);
			break;
			   }
		case 3:{
			po =(0.5f * ((-1.f * ttt + 2.f * tt - t) * m0 + (3.f * ttt - 5.f * tt + 2.f) * p0 + (-3.f * ttt + 4.f * tt + t) * p1 + (ttt - tt) * m1));
			break;
			   }
		case 4:{
			po = ((1.f / 6.f) * ((-1.f * ttt + 3.f * tt - 3.f * t + 1.f) * m0 + (3.f * ttt - 6.f * tt + 4.f) * p0 + (-3.f * ttt + 3.f * tt + 3.f * t + 1.f) * p1 + (1.f * ttt) * m1));
			break;
			   }
		}

	}
	// note - we give a little bit more example code here than normal,
	// so you can see how this works
	if(physics->value()){
		float tempe=1000-po.y*10;
		if(tempe<0) tempe = 0.1f;
		speed->value(sqrt(tempe)/10);
	}
	if (arcLength->value()) {
		world.trainU +=  dir * ((float)speed->value() * arcLen(0)*.1f);//not cal for linear yet
		if(dir>0)
			if((world.carU[0] <world.trainU - ( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2)||(ceil(world.carU[0] )!=ceil(world.trainU - ( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2)))
				world.carU[0] = world.trainU -( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2;
		if(dir<0)
			if((world.carU[0]>world.trainU - ( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2)||(ceil(world.carU[0] )!=ceil(world.trainU - ( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2)))
				world.carU[0] = world.trainU -( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2;
		for (int i= 1;i<4;i++) {
			if(dir>0)
				if((world.carU[i]  <world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2||(ceil(world.carU[i] )!=ceil(world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2))))
					world.carU[i] =world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2;
			if(dir<0)
				if((world.carU[i]  >world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2||(ceil(world.carU[i] )!=ceil(world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2))))
					world.carU[i] =world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2;
		}
		//for (int i= 0;i<4;i++) world.carU[i] +=  dir * ((float)speed->value() * arcLen(i+1)*.1f);
		//considering the arc length requirement
	} else {
		//the basic functionality
		world.trainU +=  dir * ((float)speed->value() * .1f);
		if(dir>0)
			if((world.carU[0] <world.trainU - ( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2)||(ceil(world.carU[0] )!=ceil(world.trainU - ( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2)))
				world.carU[0] = world.trainU -( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2;
		if(dir<0)
			if((world.carU[0]>world.trainU - ( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2)||(ceil(world.carU[0] )!=ceil(world.trainU - ( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2)))
				world.carU[0] = world.trainU -( (arcLen(0)*.6f)+ (arcLen(1)*.6f))/2;
		for (int i= 1;i<4;i++) {
			if(dir>0)
				if((world.carU[i]  <world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2||(ceil(world.carU[i] )!=ceil(world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2))))
					world.carU[i] =world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2;
			if(dir<0)
				if((world.carU[i]  >world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2||(ceil(world.carU[i] )!=ceil(world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2))))
					world.carU[i] =world.carU[i-1] -  ( (arcLen(i)*.5f)+ (arcLen(i+1)*.5f))/2;
		}
	}

	float nct = static_cast<float>(world.points.size());
	if (world.trainU > nct) world.trainU -= nct;
	if (world.trainU < 0) world.trainU += nct;
	for (int i= 0;i<4;i++) {
		if (world.carU[i]  > nct) world.carU[i]  -= nct;
		if (world.carU[i] < 0) world.carU[i]  += nct;
	}


}