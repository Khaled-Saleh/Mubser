#include <XnOpenNI.h>
#include <XnLog.h>
#include <XnCppWrapper.h>
#include <iostream>
#include <math.h>
#define SAMPLES_XML "SamplesConfig.xml"
using namespace std;


using namespace xn;



int mapDepth(long depth, int pixelCount)
{
  double maxVal = 100;
	double max = 2000.0;
	double min = 300.0;
	double avg = depth/(double)pixelCount;
	
	if(avg < min)
		return 100;
	
	else
		return (int)(maxVal*exp(-(log(maxVal)*(avg-min))/(max-min))-1);
}


int main ()
{
	
	XnStatus rc;
	Context context;	
	ScriptNode scriptNodeArg;	
	EnumerationErrors errors;
	DepthGenerator depth;
	DepthMetaData depthMD;
	
	rc = context.InitFromXmlFile(SAMPLES_XML, &errors);
 	
	rc= context.FindExistingNode(XN_NODE_TYPE_DEPTH, depth);
			
			
	int xRes=0, yRes=0, z1=0, z2=0, z3=0;
	int duty[] = {0,0,0};
	
	while (true)
	{
				
		long depths[]={0,0,0};	
		int pixelCount = 0;
		
		context.WaitAnyUpdateAll();
				
		depth.GetMetaData(depthMD);
		
		if(xRes == 0)
		{
			xRes = depthMD.XRes();
			yRes = depthMD.YRes();
			z1 = xRes/3;
			z2 = 2*xRes/3;
			z3 = xRes;
		}
		
		for(int x=10; x<xRes-5; x+=10)
		{
			for(int y=yRes/2; y<yRes; y+=10)
			{
				pixelCount++;
				long d = depthMD.operator()(x,y);
				
				if(x<z1)
				{
					depths[0]+=d;
				}
				else if(x>=z1 && x<z2)
				{
					depths[1]+=d;
				}
				else
				{
					depths[2]+=d;
				}
			}
		}
		
		for(int i =0; i< 3; i++)
		{
			duty[i] = mapDepth(depths[i], pixelCount);
		}
		
		printf("DutyR: %d, DutyM: %d, DutyL: %d\n", duty[0],duty[1],duty[2]);
				
		
	}
		
		
	return 0;
}
