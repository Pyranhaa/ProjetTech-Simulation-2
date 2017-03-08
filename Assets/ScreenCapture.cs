using UnityEngine;
using System.Collections.Generic;
using System.Collections;
using System.IO;

public class ScreenCapture : MonoBehaviour
{    
	private int count = 0;
	//private dumpRender2Texture render;

	protected Camera cg, cd;
	protected string dir = "screens/";
	void Start(){
		print ("ScreenShot engine");

		cg = GameObject.Find("/Robot/Camera_gauche").GetComponent(typeof(Camera)) as Camera;
 		cd = GameObject.Find("/Robot/Camera_droite").GetComponent(typeof(Camera)) as Camera;

		if (!Directory.Exists(dir)) Directory.CreateDirectory(dir);		 
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
			Texture2D gauche = getTexture(cg);
			Texture2D droite = getTexture(cd);
			Texture2D merge = new Texture2D(gauche.width + droite.width, gauche.height);

			for (int y = 0; y < merge.height; y++) {
				for (int x = 0; x < gauche.width; x++)
					merge.SetPixel(x, y, gauche.GetPixel(x, y));
				for (int x = 0; x < droite.width; x++)
					merge.SetPixel(x + gauche.width, y, droite.GetPixel(x, y));				
			}
			File.WriteAllBytes (dir + screenshotFilename, merge.EncodeToPNG());

			/*for (int y = 0; y < tmp.rows; y++) {
				for (int x = 0; x < left.cols; x++) {
					tmp.at<cv::Vec3b>(y, x) = left.at<cv::Vec3b>(y, x);
				}
				for (int x = 0; x < right.cols; x++) {
					tmp.at<cv::Vec3b>(y, x + left.cols) = right.at<cv::Vec3b>(y, x);
					//cout << x + left.cols+1 << endl;
				}
			}*/
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