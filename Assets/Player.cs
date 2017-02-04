using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour {
	public float TranslateSpeed;
	public float RotateSpeed;

	// Use this for initialization
	void Start () {
		print ("Script player start");

		Physics.gravity = new Vector3 (0.0f, -1.0f, 0.0f);

		this.TranslateSpeed = 0.1f;
		this.RotateSpeed = 2.0f;
	}
	
	// Update is called once per frame
	void Update () {
		float horizontal = Input.GetAxis ("Horizontal");
		float vertical = Input.GetAxis ("Vertical");

		transform.Rotate (new Vector3 (0.0f, horizontal * RotateSpeed, 0.0f) );
		transform.Translate (new Vector3 (0.0f, 0.0f, vertical * TranslateSpeed) );
	}
}
