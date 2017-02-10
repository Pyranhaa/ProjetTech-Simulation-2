using UnityEngine;
using System.Collections.Generic;
using System.Collections;
using System.IO;

public class ScreenCapture : MonoBehaviour
{    
	private int count = 0;
	private dumpRender2Texture render;

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
				screenshotFilename = "screenshot" + count + ".png";

			} while (System.IO.File.Exists(screenshotFilename));

			print ("Souriez !");
			//Application.CaptureScreenshot(screenshotFilename);

			render = new dumpRender2Texture();
			byte[] camera_droite = render.capture_droite.EncodeToPNG ();
			byte[] camera_gauche = render.capture_gauche.EncodeToPNG ();

			File.WriteAllBytes ("Camera_droite_" + screenshotFilename, camera_droite);
			File.WriteAllBytes ("Camera_Gauche_" + screenshotFilename, camera_gauche);
		}
	}
}