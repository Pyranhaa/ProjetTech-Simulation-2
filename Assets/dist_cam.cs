using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class dist_cam : MonoBehaviour {

	public float dist;
	public float step;

	private GameObject cg;
	private GameObject cd;

	// Use this for initialization
	void Start () {
		cg = GameObject.Find("/Robot/Camera_gauche");
		cd = GameObject.Find("/Robot/Camera_droite");

		setValue();
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown (KeyCode.K)) {
			dist += step;
			setValue();
		}
		if (Input.GetKeyDown (KeyCode.L)) {
			dist -= step;
			setValue();
		}
		if (Input.GetKeyDown (KeyCode.P))
			print("Distance: " + dist);
	}

	private void setValue() {
		float diff = (cg.transform.position - cd.transform.position).magnitude;
		float f = (dist - diff)/2;
		Vector3 v = new Vector3 (f, 0, 0);
		
		cg.transform.Translate (-v);
		cd.transform.Translate (v);
	}
}
