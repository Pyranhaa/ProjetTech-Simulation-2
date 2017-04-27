using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class export_screen : MonoBehaviour {

	protected static int width = 640;
	protected static int height = 480;
	protected static int depth = 1; //Je sais pas ce que c'est

	protected Camera cg, cd;
	
	[DllImport ("unity_controler")]
    private extern static void print_mats(int left, int right, int w, int h);

	// Use this for initialization
	void Start () {
		cg = GameObject.Find("/Robot/LeftCamera_Container/Camera_gauche").GetComponent(typeof(Camera)) as Camera;
 		cd = GameObject.Find("/Robot/RightCamera_Container/Camera_droite").GetComponent(typeof(Camera)) as Camera;

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
		if (Input.GetKeyDown (KeyCode.F12)) {
			print("Ping");
			print_mats((int) cg.targetTexture.GetNativeTexturePtr(), (int) cd.targetTexture.GetNativeTexturePtr(), width, height);
			//print((int) cg.targetTexture.GetNativeTexturePtr() + " " + (int) cd.targetTexture.GetNativeTexturePtr());
		}
	}
}
