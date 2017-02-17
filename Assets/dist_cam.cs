using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class dist_cam : MonoBehaviour {

	public float dist;

	// Use this for initialization
	void Start () {
		//transform.Translate (transform.TransformDirection(transform.forward) * 2);
		GameObject cg = GameObject.Find("/Robot/Camera_gauche");
		GameObject cd = GameObject.Find("/Robot/Camera_droite");

		float diff = (cg.transform.position - cd.transform.position).magnitude;
		float f = Mathf.Abs(diff - dist)/2;
		Vector3 v = new Vector3 (f, 0, 0);
		//*
		cg.transform.Translate (-v);
		cd.transform.Translate (v);
		//*/
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
