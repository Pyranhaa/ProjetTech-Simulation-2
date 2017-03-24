using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class Move : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}

	/** MOVEMENT FUNCTIONS **/
	// MOVE FORWARD
	void moveForward(){
		transform.Translate (Vector3.forward * Time.deltaTime);
	}
	// MOVE BACKWARD
	void moveBackward(){
		transform.Translate (Vector3.back * Time.deltaTime);
	}
	// MOVE TO THE LEFT
	void moveLeft(){
		transform.Translate(Vector3.left * Time.deltaTime);
	}
	// MOVE TO THE RIGHT
	void moveRight(){
		transform.Translate (Vector3.right * Time.deltaTime);
	}

	// Update is called once per frame
	void Update () {
		
		/***********************/
		/** INPUTS MANAGEMENT **/
		/***********************/
		// MOVE INPUTS
		// UPARROW
		if (Input.GetKey(KeyCode.UpArrow)) {
			moveForward ();
		}
		// DOWNARROW
		if (Input.GetKey(KeyCode.DownArrow)) {
			moveBackward ();
		}
		// LEFTARROW
		if (Input.GetKey(KeyCode.LeftArrow)) {
			moveLeft ();
		}
		// RIGHTARROW
		if (Input.GetKey(KeyCode.RightArrow)) {
			moveRight ();
		}
	}
}
