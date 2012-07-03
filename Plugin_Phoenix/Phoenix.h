/* Class for Phoenix instance */
class Phoenix
{
private :

	MMDAgent *m_mmdagent;
	char *outbuf[10000], *out_ptr;
	int argc;
	char *argv[];
public :

	// basic constructor
	Phoenix();
	// basic deconstructor
	~Phoenix();

	// initialize phoenix
	void initialize();

	// free phoenix
	void clear();

	// load and configure phoenix parser
	bool load(MMDAgent *m_mmdagent, char* netFile);

	// tokenize input and remove punctuation
	void strip_line(char *line, char *newLine);

	// parse a single utterance
	char* parseUtterance(char* utterance);
};
