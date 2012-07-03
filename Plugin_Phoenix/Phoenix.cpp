#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <MMDAgent.h>
#include "parse.h"
#include "pconf.h"
#include "globals_parse.h"
#include "print_structs.h"
#include "phoenix.h"


void Phoenix::initialize()
{
	// initialize all vars to null
	m_mmdagent = NULL;// MMDAgent instance
	out_ptr = outbuf;
}

void Phoenix::clear()
{
	// free all memory
	
}

Phoenix::Phoenix()
{
	initialize();
}

Phoenix::~Phoenix()
{
	clear();
}

bool Phoenix::load(MMDAgent *mmdagent, /netfile/)
{
	m_mmdagent = mmdagent;
	//TODO: build argc and argv correctly!
	argc = 5;
	argv[0] = "parse_text.c";
	argv[1] = "-dir";
	argv[2] = "Grammar";
	argv[3] = "-grammar";
	argv[4] = "MGram.net";
	config(argc, argv);


}

char* Phoenix::parseUtterance(char* utterance)
{
	// Read grammar, initialize parser, malloc space, etc
	init_parse(get_dir(), get_dict_file(), get_grammar_file(), get_forms_file(), get_priority_file());
	
	newUtterance = malloc(2 * strlen(utterance));
	// Strip out punctuation, comments, and tokenize
	this.strip_line(utterance,newUtterance);

	parse(newUtterance, get_gram());

	for(i=0; i < get_num_parses(); i++) {
		print_parse(i, out_ptr, 0, get_gram());
		out_ptr += strlen(out_ptr);
	}

	if( get_num_parses() && get_extract() ) {
        	for(i= 0; i < get_num_parses(); i++ ) print_parse(i, out_ptr, 1, get_gram());
                out_ptr += strlen(out_ptr);
        }
	
	// clear parser temps
	reset(get_num_nets());
	// free the line buffer
	free(newUtterance);
	return outbuf;
}

void Phoenix::strip_line(char *line, char *newLine)
{
	char    *from, *to;
        to = newLine;
        for(from = line; ;from++ ) {
                if( !(*from) ) break;

#ifdef SNOR
                /* for SNOR input */
                if( *from == '(' ) {
                        strcpy(utt_name, from);
                        break;
                }
#endif

                switch(*from) {

                        /* filter these out */
#ifndef SNOR
        case '(' :
        case ')' :
        case '[' :
        case ']' :
#endif
        case ':' :
	case '?' :
        case '!' :
        case '\n' :
                break;

                /* replace with space */
        case ',' :
        case '\\' :
                *to++ = ' ';
                break;


        case '#' :
                for( ++from; *from != '#' && *from; from++);
                if( *from == '#' ) from++;
                break;

        case '-' :
                /* if partial word, delete word */
                if( isspace( *(from+1) ) ) {
                        while( (to != line) && !isspace( *(--to) ) ) ;
                        /* replace with space */
                        *to++ = ' ';
                }
                else {
                        /* copy char */
                        *to++ = *from;
                }
                break;

#ifdef SNOR
        case '[' :
                /* skip past comment */
                while( *from != ']' ) {
                        if( !*from ) break;
                        from++;
                }
                break;
#endif
                /* add space before character */
        case '.' :
        case '\'' :
                *to++ = ' ';
                /* Break intentionally omitted here */

        default:
                /* copy char */
                *to++ = (islower(*from)) ? toupper(*from) : *from;
                }
                if( !from ) break;

        }
        *to= 0;

}

