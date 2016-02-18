using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Runtime.InteropServices;

public class BombShout : MonoBehaviour {
    
    // import splistener library functions
    [DllImport("splistener")]
    private static extern bool spInitListener(string hmm_path, string kws_path, string lm_path, string dict_path, int sample_rate, int delay);
    [DllImport("splistener")]
    private static extern string spGetWords();
    [DllImport("splistener")]
    private static extern void spCleanUp();
    [DllImport("splistener")]
    private static extern string spGetError();

    public GameObject bomb;
    public Text bombText;

    public float cooldown = 10.0f;
    public float initDelay = 2.0f; // delay at beginning of game before bombs can be used
    private float lastTime;
    bool spinit = false;

    void ProcessShout(string shout) {
        if(Time.time - lastTime > cooldown 
            && (true || shout.IndexOf("bomb") >= 0 
             || shout.IndexOf("blow") >= 0 
             || shout.IndexOf("boom") >= 0)) {
            Instantiate(bomb, 
                        new Vector3(transform.position.x, 
                                    transform.position.y, 
                                    transform.position.z), 
                        Quaternion.identity);
            lastTime = Time.time;
            bombText.text = "Bombs reloading...";
            bombText.color = Color.red;
        }
    }

    IEnumerator Listen() {
        string s;
        while (true) {
            s = spGetWords();
            if (s.Length > 0) {
                Debug.Log(s);
                ProcessShout(s);
            }
            yield return null;
        }
    }
    
    void Awake() {
        lastTime = initDelay - cooldown;
        if (!spInitListener(Application.dataPath + "/pocketsphinx/model/en-us/en-us",
            null, //Application.dataPath + "/keywords", // not using keyword search mode for now because of splistener bug
            Application.dataPath + "/pocketsphinx/model/en-us/en-us.lm.bin",
            Application.dataPath + "/pocketsphinx/model/en-us/cmudict-en-us.dict", // dict
            16000, 100)) {

            Debug.Log("splistener failed to initialize!");
            Debug.Log("splistener error: " + spGetError());
            bombText.text = "Speech system error";
            bombText.color = Color.white;
        }
        else {
            Debug.Log("splistener initialized successfully!");
            spinit = true;
            StartCoroutine(Listen());
        }
    }

    void Update() {
        if (spinit && Time.time - lastTime > cooldown) {
            bombText.text = "Bomb ready";
            bombText.color = Color.white;
        }
    }

    void OnApplicationQuit() {
        if (spinit) {
            spCleanUp();
        }
    }
}
