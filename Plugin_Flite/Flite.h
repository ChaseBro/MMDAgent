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

   /* Thred constructor */
   Flite();

   /* Thread deconstructor */
   ~Flite();

   /* Load models from file */
   bool load(char **modelNames, int numModels);

   /* Basic synthesize function, synthsizes and plays directly */
   float synthesize(char *voice_name, char *text);

   /* Extract phonemes from and utterance and return a string with phonemes ans durations */
   void getPhonemeSequence(char *lip);

   /* Synthesize text, save result internally */
   bool synth_text(char *voice_name, char *text);

   /* Play the wave file that is saved internally */
   void play_saved();
};
