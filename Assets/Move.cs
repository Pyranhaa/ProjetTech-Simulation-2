using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class Move : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		// Forward
		if (Input.GetKey(KeyCode.UpArrow)) {
			transform.Translate(Vector3.forward * Time.deltaTime);
		}
		// Backward
		if (Input.GetKey(KeyCode.DownArrow)) {
			transform.Translate(Vector3.back * Time.deltaTime);
		}
		// Left
		if (Input.GetKey(KeyCode.LeftArrow)) {
			transform.Translate(Vector3.left * Time.deltaTime);
		}
		// Right
		if (Input.GetKey(KeyCode.RightArrow)) {
			transform.Translate(Vector3.right * Time.deltaTime);
		}
	}
}
