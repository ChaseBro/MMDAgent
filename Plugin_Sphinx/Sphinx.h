/* Class for Sphinx instance */
class Sphinx
{
private :

   ps_decoder_t *m_ps; /* Recognizer instance */
   cmd_ln_t *m_config; /* Configuration instance */

   MMDAgent *m_mmdagent;

   char *m_languageModel;
   char *m_dictionary;
   char *m_acousticModel;
   char *m_configFile;
   char *m_userDictionary;

   ad_rec_t *m_ad;
   cont_ad_t *m_cont;

   void (*m_callbackRecogBegin)(void *data); /* Callback function for begininning of recognition */
   void *m_callbackRecogBeginData; /* data for callback function for beginning of recognition */
   void (*m_callbackRecogResult)(const char *result, void *data); /* Callback funtion for end of recognition */
   void *m_callbackRecogResultData; /* data for callback function for end of recognition */

   /* Main recognition loop */
   void run();

public :

   /* basic constructor */
   Sphinx();

   /* basic deconstructor */
   ~Sphinx();

   /* Initialize Sphinx */
   void initialize();

   /* Free Sphinx */
   void clear();

   /* load config and  */
   bool load(MMDAgent *m_mmdagent, const char *languageModel, const char *dictionary, const char *acousticModel, const char *configFile, const char *userDictionary);

   /* start recognizing from microphone */
   void start();

   /* Tell the recognizer to pause */
   void pause();

   /* Tell the recognizer to resume */
   void resume();

   /* Register callback function when recognition starts */
   void set_callback_begin(void (*callbackRecogBegin)(void *data), void *data);

   /* Register callback function when recognition ends */
   void set_callback_return(void (*callbackRecogResult)(const char *result, void *data), void *data);

};
