using UnityEngine;
using System.Runtime.InteropServices;

public class Controler : MonoBehaviour {
    [DllImport ("opencv-plugin/unity_controler.so")]
    protected static extern float getVx();
    [DllImport ("opencv-plugin/unity_controler.so")]
    protected static extern float getVy();
    [DllImport ("opencv-plugin/unity_controler.so")]
    protected static extern float getOmega();
    [DllImport ("opencv-plugin/unity_controler.so")]
    protected static extern void process(int tex_left, int tex_right, int width, int height, float vx, float vy, float omega);

    protected const int updateRate = 1; //Fréquence de rafraichissement en Hz
    protected float timeSinceLastUpdate = 0; //En s
    protected Camera cg, cd;
    protected static int width = 640; //Dimesions des images du vrai robot
	protected static int height = 480;
	protected static int depth = 24; //Je sais pas ce que c'est

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
        //deltaTime: temps écoulé depuis la dernière trame
        timeSinceLastUpdate += Time.deltaTime;
        if (timeSinceLastUpdate < 1/updateRate) // Lambda = 1/Frequence
            return;
        Rigidbody rb = GetComponent<Rigidbody>();
        Vector3 v = rb.velocity;
        print("Updating controler: current speed: " + v + "; current angle:" + rb.angularVelocity);
        process((int) cg.targetTexture.GetNativeTexturePtr(), (int) cd.targetTexture.GetNativeTexturePtr(),
                width, height, v.x, v.y, rb.angularVelocity.y);
        
        //Calcul de la force à appliquer deltaV / t = a; a * m = F
        Vector3 f = ((new Vector3(getVx(), getVy(), 0) - v) / Time.deltaTime) * rb.mass;
        //Couple à appliquer accélération angulaire * m = F; C = F*rayon
        Vector3 c = ((new Vector3(0, getOmega(), 0) - rb.angularVelocity) / Time.deltaTime) * rb.mass * (transform.localScale.z/2); //A vérifier ?

        rb.AddForce(f);
        rb.AddTorque(c);
        print("Update complete, new speed:" + rb.velocity + "; angle: "+ rb.angularVelocity);
    }

}