using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class dumpRender2Texture : MonoBehaviour {
	private Camera cam_droite;
	private Camera cam_gauche;
	private int width;
	private int height;
	private int depth;

	public Texture2D capture_droite;
	public Texture2D capture_gauche;


	public dumpRender2Texture() {
		cam_droite = GameObject.Find("Camera_droite").GetComponent<Camera>();
		cam_gauche = GameObject.Find("Camera_gauche").GetComponent<Camera>();

		width = Screen.width;
		height = Screen.height;

		RenderTexture render_tex_droite = new RenderTexture(width,
													  		height,
													  		depth,
		                                              		RenderTextureFormat.BGRA32); //BGRA car opencv prend des BGR.

		RenderTexture render_tex_gauche = new RenderTexture(width,
													  		height,
													  		depth,
															RenderTextureFormat.BGRA32);
		cam_droite.targetTexture = render_tex_droite;
		cam_gauche.targetTexture = render_tex_gauche;

		capture_droite = new Texture2D(width, height, TextureFormat.RGB24, false);
		capture_gauche = new Texture2D(width, height, TextureFormat.RGB24, false);

		capture_droite.ReadPixels(new Rect(width/2, 0, width, height), 0, 0);
		capture_gauche.ReadPixels(new Rect(0, 0, width/2, height), 0, 0);

		camera_droite.targetTexture = null;
		camera_gauche.targetTexture = null;

		Destroy (render_tex_droite);
		Destroy (render_tex_gauche);

	}
}
