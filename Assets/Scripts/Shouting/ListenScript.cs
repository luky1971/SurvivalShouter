using UnityEngine;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

// Note: the multithreading in this script should not be considered safe.
// This script is mainly for demonstration purposes, and a production version
// should probably have more thread syncing mechanisms in place
public class ListenScript : MonoBehaviour {
    
    // import splistener library functions
    [DllImport("splistener")]
    private static extern bool spInitListener(string model_path, string mic, int sample_rate);
    [DllImport("splistener")]
    private static extern bool spListen(StringBuilder words, int len);
    [DllImport("splistener")]
    private static extern void spCleanUp();
    [DllImport("splistener")]
    private static extern string spGetError();

    private static Thread listen_thread;
    private static string words = "";
    private static float transTime = 0f;

    void Listen() {
        while(true) {
            Debug.Log(Time.time);
            Thread.Sleep(100);
        }
    }

    void Awake() {
        if (!spInitListener(Application.dataPath + "/libs/pocketsphinx/model/en-us/", null, 16000)) {
            Debug.Log("splistener failed to initialize!");
            Debug.Log("splistener error: " + spGetError());
        }
        else {
            // start voice command thread
            Debug.Log("splistener initialized successfully!");
            listen_thread = new Thread(new ThreadStart(Listen));
            listen_thread.Start();
        }
    }

	// Use this for initialization
	void Start() {

	}
	
	// Update is called once per frame
	void Update() {
        // Debug.Log(words);
	}

    void OnApplicationQuit() {
        listen_thread.Abort();
        spCleanUp();
    }
}
