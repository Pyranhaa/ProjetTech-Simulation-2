using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class export_screen : MonoBehaviour {

	protected static int width = 500;
	protected static int height = 500;
	protected static int depth = 24; //Je sais pas ce que c'est

	protected Camera cg, cd;

	[DllImport ("opencv-plugin/libprocessor.so")]
	protected static extern void display_texture (int tex_id, int width, int height);

	// Use this for initialization
	void Start () {
		cg = GameObject.Find("/Robot/Camera_gauche").GetComponent(typeof(Camera)) as Camera;
 		cd = GameObject.Find("/Robot/Camera_droite").GetComponent(typeof(Camera)) as Camera;

		print ("Attaching render textures to cameras");
		RenderTexture gauche = new RenderTexture (width, height, depth);
		RenderTexture droite = new RenderTexture (width, height, depth);

		cg.targetTexture = gauche;
		cd.targetTexture = droite;
		gauche.Create ();
		droite.Create ();
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown (KeyCode.F)) {
			print("1");			
			display_texture((int) cg.targetTexture.GetNativeTexturePtr (), width, height);
			print("2");
			//display_texture((int) cd.targetTexture.GetNativeTexturePtr (), width, height);
			print("Fin");
		}
	}
}
