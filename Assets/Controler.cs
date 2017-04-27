using UnityEngine;
using System.Runtime.InteropServices;

public class Controler : MonoBehaviour {
    [DllImport ("unity_controler")]
    protected static extern float getVx();
    [DllImport ("unity_controler")]
    protected static extern float getVy();
    [DllImport ("unity_controler")]
    protected static extern float getOmega();
    [DllImport ("unity_controler")]
    protected static extern void process(int tex_left, int tex_right, int width, int height, float vx, float vy, float omega);

    protected static int width = 640; //Dimesions des images du vrai robot
	protected static int height = 480;
	protected static int depth = 1; //Je sais pas ce que c'est
    protected const float updateRate = 1f; //Fréquence de rafraichissement en Hz
    protected const float maxSpeed = 2f; //Vitesse de déplacement max du bouzin
    protected float timeSinceLastUpdate = 0; //En s
    protected Camera cg, cd;

    void Start () {
        print("Start Controler");
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

    void Update() {
        Rigidbody rb = GetComponent<Rigidbody>();
        Vector3 v = rb.velocity;
        //deltaTime: temps écoulé depuis la dernière trame
        timeSinceLastUpdate += Time.deltaTime;
        if (timeSinceLastUpdate < 1/updateRate) { // Lambda = 1/Frequence
            timeSinceLastUpdate = 0;
            process((int) cg.targetTexture.GetNativeTexturePtr(), (int) cd.targetTexture.GetNativeTexturePtr(),
                    width, height, v.x, v.y, rb.angularVelocity.y);

            Vector3 mv = gameObject.transform.forward * getVx() - gameObject.transform.right * getVy();
            mv = mv.normalized*maxSpeed - v; //J'enlève la vitesse actuelle pour ne pas accèlerer à l'infinie
            rb.AddForce(mv, ForceMode.Impulse);
            //print("Mvmnt: "+mv);

            Vector3 ro = - gameObject.transform.up * (getOmega() - rb.angularVelocity.y);
            ro = ro*maxSpeed/2; //Je normalise pas le vecteur puisqu'il a qu'une composante, la valeur de celle ci serait 1, donc un produit en croix c'est pareil
            rb.AddTorque(ro, ForceMode.Impulse);
            //print("Rot: "+ro);        
        }
    }

}