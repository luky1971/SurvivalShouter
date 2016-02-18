using UnityEngine;
using System.Collections;
using CompleteProject;

public class ExplosionScript : MonoBehaviour {

    public float radius = 2.25f;
    public int damage = 100;

    // Use this for initialization
    void Start() {
        Collider[] victims = Physics.OverlapSphere(gameObject.transform.position, radius);
        EnemyHealth enemyHealth;
        PlayerHealth playerHealth;

        foreach(Collider col in victims) {
            enemyHealth = col.gameObject.GetComponent<EnemyHealth>();
            if(enemyHealth != null) {
                enemyHealth.TakeDamage(damage);
            }
            else {
                playerHealth = col.gameObject.GetComponent<PlayerHealth>();
                if(playerHealth != null) {
                    playerHealth.TakeDamage(damage);
                }
            }
        }
    }
}
