using UnityEngine;
using System.Collections;

public class BombScript : MonoBehaviour {

    public GameObject explosion;
    public float timer = 1.5f;
    private float elapsed;

    // Use this for initialization
    void Start() {
        elapsed = 0.0f;
    }
    
    // Update is called once per frame
    void Update() {
        elapsed += Time.deltaTime;
        if(elapsed > timer) {
            Instantiate(explosion, gameObject.transform.position, Quaternion.identity);
            Destroy(gameObject);
        }
    }
}
