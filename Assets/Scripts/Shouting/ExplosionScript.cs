using UnityEngine;
using System.Collections;
using CompleteProject;

public class ExplosionScript : MonoBehaviour {

    public float radius = 2.25f;
    public int damage = 100;

    // Use this for initialization
    void Start() {
        Debug.Log("Boom!");
        Collider[] victims = Physics.OverlapSphere(gameObject.transform.position, radius);
        Debug.Log(victims.Length + " victims!");
        EnemyHealth enemyHealth;
        PlayerHealth playerHealth;

        foreach(Collider col in victims) {
            Debug.Log(col.name);
            enemyHealth = col.gameObject.GetComponent<EnemyHealth>();
            if(enemyHealth != null) {
                Debug.Log("Damaging enemy!");
                enemyHealth.TakeDamage(damage);
            }
            else {
                playerHealth = col.gameObject.GetComponent<PlayerHealth>();
                if(playerHealth != null) {
                    Debug.Log("Damaging player!");
                    playerHealth.TakeDamage(damage);
                }
            }
        }
    }
}
