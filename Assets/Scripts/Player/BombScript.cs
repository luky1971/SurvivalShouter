using UnityEngine;
using System.Collections;

public class BombScript : MonoBehaviour {

    public GameObject explosion;
    public float timer = 3.0f;
    private float elapsed;

	// Use this for initialization
	void Start () {
        elapsed = 0.0f;
	}
	
	// Update is called once per frame
	void Update () {
        elapsed += Time.deltaTime;
        if(elapsed > timer) {
            Instantiate(explosion, GetComponent<Transform>().position, Quaternion.identity);
            Destroy(gameObject);
        }
	}
}
