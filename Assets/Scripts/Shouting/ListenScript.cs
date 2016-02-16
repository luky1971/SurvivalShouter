using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class ListenScript : MonoBehaviour {
    
    // import splistener library functions
    [DllImport("splistener")]
    private static extern bool spInitListener(string model_path, string mic_name, int sample_rate, int delay);
    [DllImport("splistener")]
    private static extern string spGetWords();
    [DllImport("splistener")]
    private static extern void spCleanUp();
    [DllImport("splistener")]
    private static extern string spGetError();

    IEnumerator Listen()
    {
        string s;
        while (true)
        {
            s = spGetWords();
            if (s.Length > 0) {
                Debug.Log(s);
            }
            yield return null;
        }
    }
    
    void Awake() {
        if(!spInitListener(Application.dataPath + "/libs/pocketsphinx/model/en-us/", null, 16000, 100)) {
            Debug.Log("splistener failed to initialize!");
            Debug.Log("splistener error: " + spGetError());
        }
        else {
            // TODO: start voice command coroutine
            Debug.Log("splistener initialized successfully!");
            StartCoroutine(Listen());
        }
    }

    void OnApplicationQuit() {
        spCleanUp();
    }
}
