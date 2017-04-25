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

    protected const float updateRate = 1f; //Fréquence de rafraichissement en Hz
    protected float timeSinceLastUpdate = 0; //En s
    protected Camera cg, cd;
    protected static int width = 640; //Dimesions des images du vrai robot
	protected static int height = 480;
	protected static int depth = 1; //Je sais pas ce que c'est

    //Déplacements
    protected float vx = 0;
    protected float vy = 0;
    protected float omega = 0;

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
        Rigidbody rb = GetComponent<Rigidbody>();
        Vector3 v = rb.velocity;
        timeSinceLastUpdate += Time.deltaTime;
        if (timeSinceLastUpdate >= 1/updateRate) { // Lambda = 1/Frequence
            timeSinceLastUpdate = 0;
            process((int) cg.targetTexture.GetNativeTexturePtr(), (int) cd.targetTexture.GetNativeTexturePtr(),
                    width, height, v.x, v.y, rb.angularVelocity.y);
            print("speed:" + rb.velocity + "; angle: "+ rb.angularVelocity);
        }
        
        //Dans les calculs suivant les valeurs sont adaptées pour notre scène Unity, Vx = -x; Vy = -z; omega = -y

        //Calcul de la force à appliquer deltaV / t = a; a * m = F
        Vector3 f = ((new Vector3(-getVx(), 0, -getVy()) - v) / Time.deltaTime) * rb.mass;
        rb.AddForce(f);
        //Couple à appliquer accélération angulaire * m = F; C = F*rayon
        /*
        Vector3 c = ((new Vector3(0, -getOmega(), 0) - rb.angularVelocity) / Time.deltaTime) * rb.mass * (transform.localScale.z/2); //A vérifier ?
        rb.AddTorque(c);
        //*/
    }

}