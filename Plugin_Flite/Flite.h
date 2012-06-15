/* Flite TTS Engine Class wrapper */
class Flite
{
private:

   cst_utterance *m_utt; /* Flite utterance */

   /* initialize system */
   void initialize();

   /* free system */
   void clear();

public:

   /** Thread constructor */
   Flite();

   /** Thread deconstructor */
   ~Flite();

   /**
    * Load models from file 
    * @param modelNames Array of strings which are locations of .festvox files to be imported relative to the .mdf
    * @param numModels The number of models to be loaded
    * @returns True if the correct number of models were successfully loaded, false otherwise
    **/
   bool load(char **modelNames, int numModels);

   /**
    * Basic synthesize function, synthsizes and plays directly 
    * @param voiceName The voice to be used, must have been loaded previously with load()
    * @param text The text to be synthesized
    * @return The length in seconds of the synthesized speech
    **/
   float synthesize(char *voiceName, char *text);

   /**
    * Extract phonemes from and utterance and return a string with phonemes ans durations 
    * @param lip The string which will be filled with the phoneme sequence of the form: "phoneme,length(ms),phoneme,length..."
    **/
   void getPhonemeSequence(char *lip);

   /**
    * Synthesize text, save result internally
    * @param voiceName The voice to be used, must have been loaded previously with load()
    * @param text The text to be synthesized
    * @return True if the text was successfully synthesized with the given voice, false otherwise
    **/
   bool synth_text(char *voiceName, char *text);

   /** Play the wave file that is saved internally, synth_text must have been called first */
   void play_saved();
};
