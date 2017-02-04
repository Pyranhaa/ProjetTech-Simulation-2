using UnityEngine;
using System.Collections.Generic;
using System.Collections; 

public class ScreenCapture : MonoBehaviour
{    
	private int count = 0;

	void start(){
		print ("ScreenShot engine");
	}

	void Update()
	{
		if (Input.GetKeyDown(KeyCode.F12))
		{        
			string screenshotFilename;
			do
			{
				count++;
				screenshotFilename = "screenshot" + count + ".jpeg";

			} while (System.IO.File.Exists(screenshotFilename));

			print ("Souriez !");
			Application.CaptureScreenshot(screenshotFilename);
		}
	}
}