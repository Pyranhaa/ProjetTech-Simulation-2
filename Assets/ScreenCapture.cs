using UnityEngine;
using System.Collections.Generic;
using System.Collections;
using System.IO;

public class ScreenCapture : MonoBehaviour
{    
	private int count = 0;
	//private dumpRender2Texture render;

	protected Camera cg, cd;

	void Start(){
		print ("ScreenShot engine");

		cg = GameObject.Find("/Robot/Camera_gauche").GetComponent(typeof(Camera)) as Camera;
 		cd = GameObject.Find("/Robot/Camera_droite").GetComponent(typeof(Camera)) as Camera;
	}

	void Update()
	{
		if (Input.GetKeyDown(KeyCode.M))
		{        
			string screenshotFilename;
			do
			{
				count++;
				screenshotFilename = "screenshot" + count + ".png";

			} while (System.IO.File.Exists(screenshotFilename));

			print ("Souriez !");
			//Application.CaptureScreenshot(screenshotFilename);
			/*
			render = new dumpRender2Texture();
			byte[] camera_droite = render.capture_droite.EncodeToPNG ();
			byte[] camera_gauche = render.capture_gauche.EncodeToPNG ();

			File.WriteAllBytes ("Camera_droite_" + screenshotFilename, camera_droite);
			File.WriteAllBytes ("Camera_Gauche_" + screenshotFilename, camera_gauche);
			*/
			File.WriteAllBytes ("Camera_gauche_" + screenshotFilename, getTexture(cg).EncodeToPNG());
			File.WriteAllBytes ("Camera_droite_" + screenshotFilename, getTexture(cd).EncodeToPNG());

		}
	}

	private Texture2D getTexture(Camera cam) {
		Texture2D tex = new Texture2D(cam.targetTexture.width, cam.targetTexture.height, TextureFormat.RGB24, false);
		RenderTexture.active = cam.targetTexture;
		tex.ReadPixels(new Rect(0, 0, cam.targetTexture.width, cam.targetTexture.height), 0, 0);
		tex.Apply();

		return tex;
	}
}