/* definitions */

#define SYNTH_EVENTSTART      "SYNTH_EVENT_START"
#define SYNTH_EVENTSTOP       "SYNTH_EVENT_STOP"
#define COMMANDSTARTLIP "LIPSYNC_START"
#define COMMANDSTOPLIP  "LIPSYNC_STOP"

/* Flite_Thread: thread for Flite */
class Flite_Thread
{
private:

   MMDAgent *m_mmdagent;

   GLFWmutex m_mutex;
   GLFWcond m_cond;
   GLFWthread m_thread;

   int m_count;

   bool m_speaking;
   bool m_kill;

   char *m_charaBuff;
   char *m_styleBuff;
   char *m_textBuff;

   Flite m_flite;          /* Flite TTS Engine */
   int m_numModels;        /* number of models */
   char **m_modelNames;    /* model names */

   /* initialize: initialize thread */
   void initialize();

   /* clear: free thread */
   void clear();

public:

   /* Flite_Thread: thread constructor */
   Flite_Thread();

   /* ~Flite_Thread: thread destructor */
   ~Flite_Thread();

   /* loadAndStart: load models and start thread */
   bool loadAndStart(MMDAgent *mmdagent, const char *config);

   /* stopAndRelease: stop thread and free Flite */
   void stopAndRelease();

   /* run: main thread loop for TTS */
   void run();

   /* isRunning: check running */
   bool isRunning();

   /* isSpeaking: check speaking */
   bool isSpeaking();

   /* checkCharacter: check speaking character */
   bool checkCharacter(const char *chara);

   /* synthesis: start synthesis */
   void synthesis(const char *chara, const char *style, const char *text);

   /* stop: stop synthesis */
   void stop();
};
