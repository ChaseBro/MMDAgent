/* Flite TTS Engine Class wrapper */
class Flite
{
private:

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

};
