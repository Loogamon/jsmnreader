/*
* MIT License
*
* Copyright (c) 2023 Zachary Tabikh
* Original Copyright (c) 2010 Serge Zaitsev
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

//JSMN Reader v1.0

#ifndef JSMN_H
#define JSMN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef JSMN_STATIC
#define JSMN_API static
#else
#define JSMN_API extern
#endif

	/**
	* JSON type identifier. Basic types are:
	* 	o Object
	* 	o Array
	* 	o String
	* 	o Other primitive: number, boolean (true/false) or null
	*/
	typedef enum {
		JSMN_UNDEFINED = 0,
		JSMN_OBJECT = 1 << 0,
		JSMN_ARRAY = 1 << 1,
		JSMN_STRING = 1 << 2,
		JSMN_PRIMITIVE = 1 << 3
	} jsmntype_t;

	enum jsmnerr {
		/* Success! */
		JSMN_SUCCESS = 0,
		/* Not enough tokens were provided */
		JSMN_ERROR_NOMEM = -1,
		/* Invalid character inside JSON string */
		JSMN_ERROR_INVAL = -2,
		/* The string is not a full JSON packet, more bytes expected */
		JSMN_ERROR_PART = -3,
		/* File not found */
		JSMN_ERROR_NOFILE = -4
	};

	/**
	* JSON token description.
	* type		type (object, array, string etc.)
	* start	start position in JSON data string
	* end		end position in JSON data string
	*/
	typedef struct jsmntok {
		jsmntype_t type;
		int start;
		int end;
		int size;
#ifdef JSMN_PARENT_LINKS
		int parent;
#endif
	} jsmntok_t;

	/**
	* JSON parser. Contains an array of token blocks available. Also stores
	* the string being parsed now and current position in that string.
	*/
	typedef struct jsmn_parser {
		unsigned int pos;     /* offset in the JSON string */
		unsigned int toknext; /* next token to allocate */
		int toksuper;         /* superior token node, e.g. parent object or array */
	} jsmn_parser;

	/**
	* Create JSON parser over an array of tokens
	*/
	JSMN_API void jsmn_init(jsmn_parser *parser);

	/**
	* Run JSON parser. It parses a JSON data string into and array of tokens, each
	* describing
	* a single JSON object.
	*/
	JSMN_API int jsmn_parse(jsmn_parser *parser, const char *js, const unsigned int len,
		jsmntok_t *tokens, const unsigned int num_tokens, const unsigned int parsemode_sizecheck);

	/* ---- JSMN READER STUFF ---- */

	typedef struct jsmnreader_obj_struct
	{
		char * txt;
		unsigned int txt_size;
		jsmntok_t * tokens;
		unsigned int tokens_count;
	} jsmnreader_obj;

	typedef enum {
		JSMNR_BOTH,
		JSMNR_KEYONLY,
		JSMNR_ITEMONLY,
	} jsmnreaderobjread_t;

	/**
	* (JSMN Reader): Initalizes the reader data, as well as sets up malloc. Should be the first function used.
	*/
	JSMN_API void jsmnreader_init(jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Frees the reader data from memory. Should be the last function used.
	*/
	JSMN_API void jsmnreader_free(jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Loads a C string to populate the tokens within the reader. Can return an int for checking errors loading.
	*/
	JSMN_API int jsmnreader_load(char * str, unsigned int str_size, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Loads a C string from a text file to populate the tokens within the reader. Can return an int for checking errors loading.
	*/
	JSMN_API int jsmnreader_fileload(char * filepath, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Outputs the raw string contents of the reader's JSON string.
	*/
	JSMN_API void jsmnreader_print_string(jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Outputs a list of the reader's tokens.
	*/
	JSMN_API void jsmnreader_print_tokens(jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Returns an int if the token successfully found. Returns 0 in failure.
	*/
	JSMN_API int jsmnreader_token_get_int(unsigned int index, jsmnreader_obj * reader);

    /**
	* (JSMN Reader): Returns an unsigned int if the token successfully found. Returns 0 in failure.
	*/
	JSMN_API unsigned int jsmnreader_token_get_uint(unsigned int index, jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Returns a float if the token successfully found. Returns 0 in failure.
	*/
	JSMN_API float jsmnreader_token_get_float(unsigned int index, jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Returns an allocated C string if the token was successfully found; it also can be used to grab the key string. Returns as a blank string in failure. Compatible with other types of items. Remember to free the C string after usage.
	*/
	JSMN_API char * jsmnreader_token_get_string(unsigned int index, jsmnreader_obj * reader);

    /**
	* (JSMN Reader): Returns an allocated C string of "raw" contents (strings with quotations, true/false/null, etc.) if the token was successfully found; it also can be used to grab the key string. Returns as a blank string in failure. Remember to free the C string after usage.
	*/
	JSMN_API char * jsmnreader_token_get_raw(unsigned int index, jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Returns a 1 if the null token was successfully found. Returns 0 in failure.
	*/
	JSMN_API int jsmnreader_token_is_null(unsigned int index, jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Returns a 1 if a special token (true/false/null) was successfully found. Returns 0 in failure.
	*/
	JSMN_API int jsmnreader_token_is_special(unsigned int index, jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Returns the token's ID if the object was successfully found. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
	*/
	JSMN_API unsigned int jsmnreader_token_get_object(unsigned int index, jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Returns the token's ID if the array was successfully found. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
	*/
	JSMN_API unsigned int jsmnreader_token_get_array(unsigned int index, jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Returns the token's size if the object or array was successfully found. Returns 0 in failure.
	*/
	JSMN_API unsigned int jsmnreader_token_size(unsigned int index, jsmnreader_obj * reader);

	/**
	* (JSMN Reader): Returns the token's ID if the token was successfully found. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
	* 'mypath' usage appears as "repository\\type" like a filepath, use a blank string "" if you want to grab from the root from the 'offset'. Generally, 'offset' comes from object/array-related output.
	*/
	JSMN_API unsigned int jsmnreader_tree_get_x(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Returns the token's ID if the token was successfully found. 
	*/
	JSMN_API void jsmnreader_token_array_tokens(unsigned int ** arrays, unsigned int * arrays_size, unsigned int offset, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Returns a token ID from a specified index within the array's tokens. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
	*/
	JSMN_API unsigned int jsmnreader_token_array(unsigned int index, unsigned int offset, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Outputs the token if the token was successfully found from the path.
	*/
	JSMN_API unsigned int jsmnreader_tree_anyprint(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Returns an int if the token successfully found. Returns 0 in failure.
	* 'mypath' usage appears as "repository\\type" like a filepath, use a blank string "" if you want to grab from the root from the 'offset'. Generally, 'offset' comes from object/array-related output.
	*/
	JSMN_API int jsmnreader_tree_get_int(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader);

    /**
	* (JSMN Reader): Returns an unsigned int if the token successfully found. Returns 0 in failure.
	* 'mypath' usage appears as "repository\\type" like a filepath, use a blank string "" if you want to grab from the root from the 'offset'. Generally, 'offset' comes from object/array-related output.
	*/
	JSMN_API unsigned int jsmnreader_tree_get_uint(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Returns a float if the token was successfully found. Returns 0 in failure.
	* 'mypath' usage appears as "repository\\type" like a filepath, use a blank string "" if you want to grab from the root from the 'offset'. Generally, 'offset' comes from object/array-related output.
	*/
	JSMN_API float jsmnreader_tree_get_float(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Returns an allocated C string if the token was successfully found. Returns as a blank string in failure. Compatible with other types of items. Remember to free the C string after usage.
	* 'mypath' usage appears as "repository\\type" like a filepath, use a blank string "" if you want to grab from the root from the 'offset'. Generally, 'offset' comes from object/array-related output.
	*/
	JSMN_API char * jsmnreader_tree_get_string(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader);

    /**
	* (JSMN Reader): Returns an allocated C string of "raw" contents (strings with quotations, true/false/null, etc.) if the token was successfully found. Returns as a blank string in failure. Remember to free the C string after usage.
	* 'mypath' usage appears as "repository\\type" like a filepath, use a blank string "" if you want to grab from the root from the 'offset'. Generally, 'offset' comes from object/array-related output.
	*/
	JSMN_API char * jsmnreader_tree_get_raw(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Returns the token's ID if the object token was successfully found. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
	* 'mypath' usage appears as "repository\\type" like a filepath, use a blank string "" if you want to grab from the root from the 'offset'. Generally, 'offset' comes from object/array-related output.
	*/
	JSMN_API unsigned int jsmnreader_tree_get_object(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Returns the token's ID if the array token was successfully found. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
	* 'mypath' usage appears as "repository\\type" like a filepath, use a blank string "" if you want to grab from the root from the 'offset'. Generally, 'offset' comes from object/array-related output.
	*/
	JSMN_API unsigned int jsmnreader_tree_get_array(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Returns the token's ID if the token was successfully found. Is arguably redundant to jsmnreader_tree_get_x(), but was implemented for naming consistency. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
	* 'mypath' usage appears as "repository\\type" like a filepath, use a blank string "" if you want to grab from the root from the 'offset'. Generally, 'offset' comes from object/array-related output.
	*/
	JSMN_API unsigned int jsmnreader_tree_get_any(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader);

    /**
	* (JSMN Reader): Populates an unsigned int array with the indexes of the object's tokens.
	* 'read_setting' makes use of the 'jsmnreaderobjread_t' enum (JSMNR_BOTH, JSMNR_KEYONLY, JSMNR_ITEMONLY) for listing the tokens within the object.
	*/
	JSMN_API void jsmnreader_token_object_tokens(unsigned int ** arrays, unsigned int * arrays_size, unsigned int offset, jsmnreaderobjread_t read_setting, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Returns a token ID from a specified index within the object's tokens. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
	* 'read_setting' makes use of the 'jsmnreaderobjread_t' enum (JSMNR_BOTH, JSMNR_KEYONLY, JSMNR_ITEMONLY) for listing the tokens within the object.
	*/
	JSMN_API unsigned int jsmnreader_token_object(unsigned int index, unsigned int offset, jsmnreaderobjread_t read_setting, struct jsmnreader_obj_struct * reader);

	/**
	* (JSMN Reader): Outputs a list of the visible tokens from the path.
	*/
	JSMN_API void jsmnreader_tree_print(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader);

#ifndef JSMN_HEADER
	/**
	* Allocates a fresh unused token from the token pool.
	*/
	static jsmntok_t *jsmn_alloc_token(jsmn_parser *parser, jsmntok_t *tokens,
		const unsigned int num_tokens,
		const unsigned int parsemode_sizecheck) {
		jsmntok_t *tok;
		if (!parsemode_sizecheck)
        {
		if (parser->toknext >= num_tokens) {
			return NULL;
		}
		}
		if (parser->toknext < num_tokens)
        {
		tok = &tokens[parser->toknext++];
		//if (!parsemode_sizecheck)
        //{
		tok->start = tok->end = -1;
		tok->size = 0;
#ifdef JSMN_PARENT_LINKS
		tok->parent = -1;
#endif
        }
        //}
		return tok;
	}

	/**
	* Fills token type and boundaries.
	*/
	static void jsmn_fill_token(jsmntok_t *token, const jsmntype_t type,
		const int start, const int end) {
		token->type = type;
		token->start = start;
		token->end = end;
		token->size = 0;
	}

	/**
	* Fills next available token with JSON primitive.
	*/
	static int jsmn_parse_primitive(jsmn_parser *parser, const char *js,
		const unsigned int len, jsmntok_t *tokens,
		const unsigned int num_tokens,
		const unsigned int parsemode_sizecheck) {
		jsmntok_t *token;
		int start;

		start = parser->pos;

		for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
			switch (js[parser->pos]) {
				/* In strict mode primitive must be followed by "," or "}" or "]" */
			case '\t':
			case '\r':
			case '\n':
			case ' ':
			case ',':
			case ']':
			case '}':
				goto found;
			default:
				/* to quiet a warning from gcc*/
				break;
			}
			if (js[parser->pos] < 32 || js[parser->pos] >= 127) {
				parser->pos = start;
				return JSMN_ERROR_INVAL;
			}
		}
		/* In strict mode primitive must be followed by a comma/object/array */
		parser->pos = start;
		return JSMN_ERROR_PART;

	found:
		if (tokens == NULL) {
			parser->pos--;
			return 0;
		}
		token = jsmn_alloc_token(parser, tokens, num_tokens, parsemode_sizecheck);
		if (token == NULL) {
			parser->pos = start;
			return JSMN_ERROR_NOMEM;
		}
		jsmn_fill_token(token, JSMN_PRIMITIVE, start, parser->pos);
#ifdef JSMN_PARENT_LINKS
		token->parent = parser->toksuper;
#endif
		parser->pos--;
		return 0;
	}

	/**
	* Fills next token with JSON string.
	*/
	static int jsmn_parse_string(jsmn_parser *parser, const char *js,
		const unsigned int len, jsmntok_t *tokens,
		const unsigned int num_tokens,
		const unsigned int parsemode_sizecheck) {
		jsmntok_t *token;

		int start = parser->pos;

		/* Skip starting quote */
		parser->pos++;

		for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
			char c = js[parser->pos];

			/* Quote: end of string */
			if (c == '\"') {
				if (tokens == NULL) {
					return 0;
				}
				token = jsmn_alloc_token(parser, tokens, num_tokens, parsemode_sizecheck);
				if (token == NULL) {
					parser->pos = start;
					return JSMN_ERROR_NOMEM;
				}
				jsmn_fill_token(token, JSMN_STRING, start + 1, parser->pos);
#ifdef JSMN_PARENT_LINKS
				token->parent = parser->toksuper;
#endif
				return 0;
			}

			/* Backslash: Quoted symbol expected */
			if (c == '\\' && parser->pos + 1 < len) {
				int i;
				parser->pos++;
				switch (js[parser->pos]) {
					/* Allowed escaped symbols */
				case '\"':
				case '/':
				case '\\':
				case 'b':
				case 'f':
				case 'r':
				case 'n':
				case 't':
					break;
					/* Allows escaped symbol \uXXXX */
				case 'u':
					parser->pos++;
					for (i = 0; i < 4 && parser->pos < len && js[parser->pos] != '\0';
						i++) {
						/* If it isn't a hex character we have an error */
						if (!((js[parser->pos] >= 48 && js[parser->pos] <= 57) ||   /* 0-9 */
							(js[parser->pos] >= 65 && js[parser->pos] <= 70) ||   /* A-F */
							(js[parser->pos] >= 97 && js[parser->pos] <= 102))) { /* a-f */
							parser->pos = start;
							return JSMN_ERROR_INVAL;
						}
						parser->pos++;
					}
					parser->pos--;
					break;
					/* Unexpected symbol */
				default:
					parser->pos = start;
					return JSMN_ERROR_INVAL;
				}
			}
		}
		parser->pos = start;
		return JSMN_ERROR_PART;
	}

	/**
	* Parse JSON string and fill tokens.
	*/
	JSMN_API int jsmn_parse(jsmn_parser *parser, const char *js, const unsigned int len,
		jsmntok_t *tokens, const unsigned int num_tokens, const unsigned int parsemode_sizecheck) {
		int r;
		int i;
		jsmntok_t *token;
		int count = parser->toknext;

		for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
			char c;
			jsmntype_t type;

			c = js[parser->pos];
			switch (c) {
			case '{':
			case '[':
				count++;
				if (tokens == NULL) {
					break;
				}
				token = jsmn_alloc_token(parser, tokens, num_tokens, parsemode_sizecheck);
				if (token == NULL){
					return JSMN_ERROR_NOMEM;
				}
				if (parser->toksuper != -1) {
				jsmntok_t *t = &tokens[parser->toksuper];

					/* In strict mode an object or array can't become a key */
					if (t->type == JSMN_OBJECT) {
						return JSMN_ERROR_INVAL;
					}

					t->size++;
#ifdef JSMN_PARENT_LINKS
					token->parent = parser->toksuper;
#endif
				}
				token->type = (c == '{' ? JSMN_OBJECT : JSMN_ARRAY);
				token->start = parser->pos;
				parser->toksuper = parser->toknext - 1;
				break;
			case '}':
			case ']':
				if (tokens == NULL) {
					break;
				}
				type = (c == '}' ? JSMN_OBJECT : JSMN_ARRAY);
#ifdef JSMN_PARENT_LINKS
				if (parser->toknext < 1) {
					return JSMN_ERROR_INVAL;
				}
				token = &tokens[parser->toknext - 1];
				for (;;) {
					if (token->start != -1 && token->end == -1) {
						if (token->type != type) {
							return JSMN_ERROR_INVAL;
						}
						token->end = parser->pos + 1;
						parser->toksuper = token->parent;
						break;
					}
					if (token->parent == -1) {
						if (token->type != type || parser->toksuper == -1) {
							return JSMN_ERROR_INVAL;
						}
						break;
					}
					token = &tokens[token->parent];
				}
#else
				for (i = parser->toknext - 1; i >= 0; i--) {
					token = &tokens[i];
					if (token->start != -1 && token->end == -1) {
						if (token->type != type) {
							return JSMN_ERROR_INVAL;
						}
						parser->toksuper = -1;
						token->end = parser->pos + 1;
						break;
					}
				}
				/* Error if unmatched closing bracket */
				if (i == -1) {
					return JSMN_ERROR_INVAL;
				}
				for (; i >= 0; i--) {
					token = &tokens[i];
					if (token->start != -1 && token->end == -1) {
						parser->toksuper = i;
						break;
					}
				}
#endif
				break;
			case '\"':
				r = jsmn_parse_string(parser, js, len, tokens, num_tokens, parsemode_sizecheck);
				if (r < 0) {
					return r;
				}
				count++;
				if (parser->toksuper != -1 && tokens != NULL) {
					tokens[parser->toksuper].size++;
				}
				break;
			case '\t':
			case '\r':
			case '\n':
			case ' ':
				break;
			case ':':
				parser->toksuper = parser->toknext - 1;
				break;
			case ',':
				if (tokens != NULL && parser->toksuper != -1 &&
					tokens[parser->toksuper].type != JSMN_ARRAY &&
					tokens[parser->toksuper].type != JSMN_OBJECT) {
#ifdef JSMN_PARENT_LINKS
					parser->toksuper = tokens[parser->toksuper].parent;
#else
					for (i = parser->toknext - 1; i >= 0; i--) {
						if (tokens[i].type == JSMN_ARRAY || tokens[i].type == JSMN_OBJECT) {
							if (tokens[i].start != -1 && tokens[i].end == -1) {
								parser->toksuper = i;
								break;
							}
						}
					}
#endif
				}
				break;

				/* In strict mode primitives are: numbers and booleans */
			case '-':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 't':
			case 'f':
			case 'n':
				/* And they must not be keys of the object */
				if (tokens != NULL && parser->toksuper != -1) {
					const jsmntok_t *t = &tokens[parser->toksuper];
					if (t->type == JSMN_OBJECT ||
						(t->type == JSMN_STRING && t->size != 0)) {
						return JSMN_ERROR_INVAL;
					}
				}
				r = jsmn_parse_primitive(parser, js, len, tokens, num_tokens, parsemode_sizecheck);
				if (r < 0) {
					return r;
				}
				count++;
				if (parser->toksuper != -1 && tokens != NULL) {
					tokens[parser->toksuper].size++;
				}
				break;


				/* Unexpected char in strict mode */
			default:
				return JSMN_ERROR_INVAL;

			}
		}

		if (tokens != NULL) {
			for (i = parser->toknext - 1; i >= 0; i--) {
				/* Unmatched opened object or array */
				if (tokens[i].start != -1 && tokens[i].end == -1) {
					return JSMN_ERROR_PART;
				}
			}
		}

		return count;
	}

	/**
	* Creates a new parser based over a given buffer with an array of tokens
	* available.
	*/
	JSMN_API void jsmn_init(jsmn_parser *parser) {
		parser->pos = 0;
		parser->toknext = 0;
		parser->toksuper = -1;
	}

	/* ---- JSMN READER STUFF (FUNCTIONS) ---- */

	JSMN_API void jsmnreader_init(jsmnreader_obj * reader)
	{
		reader->txt = (char *) malloc(0);
		reader->tokens = (jsmntok_t *) malloc(0);
		reader->txt_size = 0;
		reader->tokens_count = 0;
	}

	JSMN_API void jsmnreader_free(jsmnreader_obj * reader)
	{
		free(reader->txt);
		free(reader->tokens);
		reader->txt_size = 0;
		reader->tokens_count = 0;
	}

	JSMN_API int jsmnreader_load(char * str, unsigned int str_size, struct jsmnreader_obj_struct * reader)
	{
		jsmn_parser parser;
		int check;
		unsigned int i;
		reader->txt = str;
		reader->txt_size = str_size;
		reader->tokens_count = 2;
		i = 0;

		jsmn_init(&parser);

		reader->tokens = (jsmntok_t *)realloc(reader->tokens, 0);
		check = jsmn_parse(&parser, reader->txt, reader->txt_size, reader->tokens, reader->tokens_count, 1);
		if (check>=0)
			reader->tokens_count=check;

		reader->tokens = (jsmntok_t *)realloc(reader->tokens, (reader->tokens_count)* sizeof(jsmntok_t));
		jsmn_init(&parser);
		check = jsmn_parse(&parser, reader->txt, reader->txt_size, reader->tokens, reader->tokens_count, 0);
		if (check == JSMN_ERROR_NOMEM)
		{
			//if (DEBUGPRINT_FILELOAD)
			//	printf("Not enough! (%d)", reader->tokens_count);
			return JSMN_ERROR_NOMEM;
		}
		if (check == JSMN_ERROR_INVAL)
		{
			//printf("Error! Invalid format!\n");
			reader->tokens_count = 0;
			reader->tokens = (jsmntok_t *)realloc(reader->tokens, (reader->tokens_count)*sizeof(jsmntok_t));
			return JSMN_ERROR_INVAL;
		}
		if (check == JSMN_ERROR_PART)
		{
			//printf("Error! Incomplete JSON!\n");
			reader->tokens_count = 0;
			reader->tokens = (jsmntok_t *)realloc(reader->tokens, (reader->tokens_count)*sizeof(jsmntok_t));
			return JSMN_ERROR_PART;
		}
		//if (DEBUGPRINT_FILELOAD)
		//	printf("Success!");
		return JSMN_SUCCESS;
	}

	JSMN_API int jsmnreader_fileload(char * filepath, struct jsmnreader_obj_struct * reader)
	{
		FILE * str_file;
		reader->txt_size = 0;
		reader->tokens_count = 0;
		//printf("FILE: %s\n", filepath);
		str_file = fopen(filepath, "rb");
		if (!str_file)
		{
			//if (DEBUGPRINT_FILELOAD)
			//printf("Can't read %s.\n", filepath);
			reader->txt = (char *)malloc(0);
			reader->tokens = (jsmntok_t *)malloc(0);
			reader->tokens_count = 0;
			return JSMN_ERROR_NOFILE;
		}
		fseek(str_file, 0, SEEK_END); reader->txt_size = ftell(str_file); fseek(str_file, 0, SEEK_SET);
		reader->txt = (char *)realloc(reader->txt, sizeof(char) * (reader->txt_size + 1)); fread(reader->txt, 1, reader->txt_size, str_file);
		*(reader->txt + (reader->txt_size)) = '\0';
		//if (DEBUGPRINT_FILELOAD)
		//	printf("%s\n", reader->txt);
		fclose(str_file);

		return jsmnreader_load(reader->txt, reader->txt_size, reader);
	}

	JSMN_API void jsmnreader_print_string(jsmnreader_obj * reader)
	{
		if (reader->txt_size > 0)
		{
			printf("%s\n", reader->txt);
		}
	}

	JSMN_API void jsmnreader_print_tokens(jsmnreader_obj * reader)
	{
		unsigned int i;
		i = 0;
		for (i = 0; i < reader->tokens_count; i++)
		{
			printf("%d: (", i);
			switch ((reader->tokens + i)->type)
			{
			case JSMN_OBJECT: printf("OBJECT"); break;
			case JSMN_ARRAY: printf("ARRAY"); break;
			case JSMN_STRING: printf("STRING"); break;
			case JSMN_PRIMITIVE: printf("PRIMITIVE"); break;
			default: printf("%d",(reader->tokens + i)->type); break;
			}
			printf(",%d,%d,%d)\n", (reader->tokens + i)->start, (reader->tokens + i)->end, (reader->tokens + i)->size);
		}
	}

	static char ** jsmnreader_tree_parsepath(char * mypath, char ** input_list, int * path_count)
	{
		char ** path_list;
		char * temp_str;

		int temp_strlen;
		unsigned int i;
		int special_char;
		path_list = input_list;


		temp_str = (char *)malloc(sizeof(char) * 0);
		path_list = (char **)malloc(sizeof(char *) * 0);

		temp_strlen = 0;
		//if (DEBUGPRINT_PARSEPATH) printf("Path: [%s]\n", mypath);
		i = 0;
		*path_count = 1;
		special_char = 0;
		if (strlen(mypath) == 0)
			*path_count = 0;
		while (i <= strlen(mypath))
		{
			if (strlen(mypath) == 0)
				break;
			special_char = 0;
			if (mypath[i] == '\\')
			{
				temp_strlen += 1;
				temp_str = (char *)realloc(temp_str, sizeof(char) * (temp_strlen));
				temp_str[temp_strlen - 1] = '\0';

				special_char = 1;
				/*if (DEBUGPRINT_PARSEPATH)
				{
					printf("[%s]\n", temp_str);
					printf("%d\n", *path_count);
				}*/
				path_list = (char **)realloc(path_list, sizeof(char *) * *path_count + 1);
				path_list[*path_count - 1] = malloc(sizeof(char) * (temp_strlen));
				memcpy(path_list[*path_count - 1], temp_str, sizeof(char) * (temp_strlen));

				temp_strlen = 0;
				temp_str = (char *)realloc(temp_str, 0);
				*path_count = *path_count + 1;
			}
			if (mypath[i] == '\0')
			{
				temp_strlen += 1;
				temp_str = (char *)realloc(temp_str, sizeof(char) * (temp_strlen));
				temp_str[temp_strlen - 1] = '\0';

				special_char = 1;
				//if (DEBUGPRINT_PARSEPATH) printf("[%s]\n", temp_str);

				path_list = (char **)realloc(path_list, sizeof(char *) * *path_count + 1);
				path_list[*path_count - 1] = malloc(sizeof(char) * (temp_strlen));
				memcpy(path_list[*path_count - 1], temp_str, sizeof(char) * (temp_strlen));
				break;
			}
			if (!special_char)
			{
				temp_strlen += 1;
				temp_str = (char *)realloc(temp_str, sizeof(char) * (temp_strlen));
				temp_str[temp_strlen - 1] = mypath[i];
			}
			//if (DEBUGPRINT_PARSEPATH) printf("%c\n", mypath[i]);
			i++;
		}
		//if (DEBUGPRINT_PARSEPATH) printf("COUNT %d\n", *path_count);
		free(temp_str);

		return path_list;
	}

	static char * jsmnreader_extract(unsigned int start, unsigned int end, struct jsmnreader_obj_struct * reader)
	{
		char * txt;
		unsigned int r;
		unsigned int w;
		txt = (char *)malloc(0 * sizeof(char));
		//printf("(%d,", reader->txt_size);
		//printf("%d,%d)", start, end);
		//printf("\n");
		r = start;
		w = 0;
		while (r < end)
		{
			//printf("%c (%d < %d)\n", reader->txt[r], r, end);
			if (reader->txt[r] != '\\')
			{
				w += 1;
				txt = (char *)realloc(txt, (w + 1)* sizeof(char));
				txt[w - 1] = reader->txt[r];
				txt[w] = '\0';
			}
			else
			{
				switch (reader->txt[r + 1])
				{
				case '\\':
					w += 1;
					txt = (char *)realloc(txt, (w + 1)* sizeof(char));
					txt[w - 1] = '\\';
					txt[w] = '\0';
					break;
				case '"':
					w += 1;
					txt = (char *)realloc(txt, (w + 1)* sizeof(char));
					txt[w - 1] = '"';
					txt[w] = '\0';
					break;
				}
				r++;
			}

			r++;
		}
		//printf("[%s]\n", txt);
		return txt;
	}

	JSMN_API int jsmnreader_token_get_int(unsigned int index, jsmnreader_obj * reader)
	{
		int num;
		char * raw_str;
		num = 0;
		if (reader->tokens_count > 0)
		{
			if (index >= 0 && index < reader->tokens_count)
			{
				if (((reader->tokens + index)->type == JSMN_PRIMITIVE))
				{
					raw_str = jsmnreader_extract((reader->tokens + index)->start, (reader->tokens + index)->end, reader);
					num = strtol(raw_str,NULL,10);
                    if (raw_str[0]=='t')
					{
                        if (strcmp(raw_str,"true")==0)
                            num=1;
					}
					free(raw_str);
				}
			}
		}
		return num;
	}

    JSMN_API unsigned int jsmnreader_token_get_uint(unsigned int index, jsmnreader_obj * reader)
	{
		unsigned int num;
		char * raw_str;
		num = 0;
		if (reader->tokens_count > 0)
		{
			if (index >= 0 && index < reader->tokens_count)
			{
				if (((reader->tokens + index)->type == JSMN_PRIMITIVE))
				{
					raw_str = jsmnreader_extract((reader->tokens + index)->start, (reader->tokens + index)->end, reader);
					num = strtoul(raw_str,NULL,10);
                    if (raw_str[0]=='t')
					{
                        if (strcmp(raw_str,"true")==0)
                            num=1;
					}
					free(raw_str);
				}
			}
		}
		return num;
	}

	JSMN_API float jsmnreader_token_get_float(unsigned int index, jsmnreader_obj * reader)
	{
		float num;
		char * raw_str;
		num = 0;
		if (reader->tokens_count > 0)
		{
			if (index >= 0 && index < reader->tokens_count)
			{
				if (((reader->tokens + index)->type == JSMN_PRIMITIVE))
				{
					raw_str = jsmnreader_extract((reader->tokens + index)->start, (reader->tokens + index)->end, reader);
					num = (float) strtod(raw_str,NULL);
					if (raw_str[0]=='t')
					{
                        if (strcmp(raw_str,"true")==0)
                            num=1;
					}
					free(raw_str);
				}
			}
		}
		return num;
	}

	JSMN_API char * jsmnreader_token_get_string(unsigned int index, jsmnreader_obj * reader)
	{
		char * raw_str;
		if (reader->tokens_count > 0)
		{
			if (index >= 0 && index < reader->tokens_count)
			{
				if ( ((reader->tokens + index)->type == JSMN_STRING) || ((reader->tokens + index)->type == JSMN_PRIMITIVE) )
				{
					raw_str = jsmnreader_extract((reader->tokens + index)->start, (reader->tokens + index)->end, reader);
					return raw_str;
				}
			}
		}
		return "";
	}

    JSMN_API int jsmnreader_token_is_null(unsigned int index, jsmnreader_obj * reader)
	{
		int num;
		char * raw_str;
		num = 0;
		if (reader->tokens_count > 0)
		{
			if (index >= 0 && index < reader->tokens_count)
			{
				if (((reader->tokens + index)->type == JSMN_PRIMITIVE))
				{
					raw_str = jsmnreader_extract((reader->tokens + index)->start, (reader->tokens + index)->end, reader);
                    if (raw_str[0]=='n')
					{
                        if (strcmp(raw_str,"null")==0)
                            num=1;
					}
					free(raw_str);
				}
			}
		}
		return num;
	}

    JSMN_API int jsmnreader_token_is_special(unsigned int index, jsmnreader_obj * reader)
	{
		int num;
		char * raw_str;
		num = 0;
		if (reader->tokens_count > 0)
		{
			if (index >= 0 && index < reader->tokens_count)
			{
				if (((reader->tokens + index)->type == JSMN_PRIMITIVE))
				{
					raw_str = jsmnreader_extract((reader->tokens + index)->start, (reader->tokens + index)->end, reader);
                    switch(raw_str[0])
					{
					    case 'n':
                        if (strcmp(raw_str,"null")==0)
                            num=1;
                            break;

                        case 't':
                        if (strcmp(raw_str,"true")==0)
                            num=1;
                            break;

                        case 'f':
                        if (strcmp(raw_str,"false")==0)
                            num=1;
                            break;
					}
					free(raw_str);
				}
			}
		}
		return num;
	}

    JSMN_API char * jsmnreader_token_get_raw(unsigned int index, jsmnreader_obj * reader)
	{
		char * raw_str;
		if (reader->tokens_count > 0)
		{
			if (index >= 0 && index < reader->tokens_count)
			{
			    switch((reader->tokens + index)->type)
                {
                    case JSMN_STRING:
					raw_str = jsmnreader_extract(((reader->tokens + index)->start)-1, ((reader->tokens + index)->end)+1, reader);
					return raw_str;
                    break;

                    case JSMN_PRIMITIVE:
					raw_str = jsmnreader_extract((reader->tokens + index)->start, (reader->tokens + index)->end, reader);
					return raw_str;
                    break;

                    default:
                    return "";
                    break;
                }
			}
		}
		return "";
	}

	JSMN_API unsigned int jsmnreader_token_get_object(unsigned int index, jsmnreader_obj * reader)
	{
		unsigned int num;
		num = -1;
		if (reader->tokens_count > 0)
		{
			if (index >= 0 && index < reader->tokens_count)
			{
				if (((reader->tokens + index)->type == JSMN_OBJECT))
				{
					num = index;
				}
			}
		}
		return num;
	}

	JSMN_API unsigned int jsmnreader_token_get_array(unsigned int index, jsmnreader_obj * reader)
	{
		unsigned int num;
		num = -1;
		if (reader->tokens_count > 0)
		{
			if (index >= 0 && index < reader->tokens_count)
			{
				if (((reader->tokens + index)->type == JSMN_ARRAY))
				{
					num = index;
				}
			}
		}
		return num;
	}

	JSMN_API unsigned int jsmnreader_token_size(unsigned int index, jsmnreader_obj * reader)
	{
		unsigned int num;
		num = 0;
		if (reader->tokens_count > 0)
		{
			if (index >= 0 && index < reader->tokens_count)
			{
				if (((reader->tokens + index)->type == JSMN_OBJECT) || ((reader->tokens + index)->type == JSMN_ARRAY))
				{
					num = (reader->tokens + index)->size;
				}
			}
		}
		return num;
	}

	static void jsmnreader_dataskip(unsigned int * r, unsigned int * objs, int mode, struct jsmnreader_obj_struct * reader)
	{
		unsigned int temp_size;
		unsigned int objs_sub;
		int check_offset;
		check_offset = 0;
		temp_size = (reader->tokens + *r)->size;
		objs_sub = temp_size;
		//if (DEBUGPRINT_SKIP) printf("SIZE %d\n", temp_size);

		if (mode == 0)
		{
			check_offset = 1;
		}
		*r = *r + 1;
		while (*r < reader->tokens_count && objs_sub > 0)
		{
			//if (DEBUGPRINT_SKIP) printf("%d", *r);
			switch ((reader->tokens + *r + check_offset)->type)
			{
			default:
				//if (DEBUGPRINT_SKIP) printf("\n");
				if (mode == 1)
					*r = *r + 1;
				else
					*r = *r + 2;
				objs_sub -= 1;
				//*objs -= 1;
				break;
			case JSMN_OBJECT:
				//if (DEBUGPRINT_SKIP) printf(" ANOTHER OBJECT\n");
				if (mode == 0)
					*r = *r + 1;
				jsmnreader_dataskip(r, &objs_sub, 0, reader);
				objs_sub -= 1;
				//*objs -= 1;
				//*r = *r + 1;
				break;
			case JSMN_ARRAY:
				//if (DEBUGPRINT_SKIP) printf(" ANOTHER ARRAY\n");
				if (mode == 0)
					*r = *r + 1;
				jsmnreader_dataskip(r, &objs_sub, 1, reader);
				objs_sub -= 1;
				//*objs -= 1;
				//*r = *r + 1;
				break;
			}
		}

	}

	JSMN_API unsigned int jsmnreader_tree_get_x(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		unsigned int loc;
		int in_offset;
		int finished_loop;
		int selected_token;
		char * ex_txt;
		char ** path_list;
		unsigned int path_count;
		unsigned int i;
		unsigned int objs;
		unsigned int r;
		r = 0;
		loc = - 1;
		path_list=malloc(0);
		in_offset = 0;
		if (reader->tokens_count > 0)
		{
			if (offset >= 0 && offset < reader->tokens_count)
			{
				switch ((reader->tokens + offset)->type)
				{
				default:
					//if (DEBUGPRINT_TREEGETX) printf("Error! The offset is not on an object or array!\n");
					break;
				case JSMN_OBJECT:
					in_offset = 1;
					break;
				case JSMN_ARRAY:
					in_offset = 1;
					break;
				}
			}
			/*else
			{
				if (DEBUGPRINT_TREEGETX) printf("Error! The offset is in out of range!\n");
			}*/
		}

		if (!in_offset)
		{
			//if (DEBUGPRINT_TREEGETX) printf("Error! Invalid offset!\n");
			return 0;
		}

		path_count = 0;
		path_list = jsmnreader_tree_parsepath(mypath, path_list, &path_count);


		/*if (DEBUGPRINT_TREEGETX)
		{
			printf("---\n");
			for (i = 0; i < path_count; i++)
				printf("%d: %s\n", i, path_list[i]);
		}*/
		objs = (reader->tokens + offset)->size;
		r = offset + 1;
		i = 0;
		//if (DEBUGPRINT_TREEGETX) printf("\nOffset: %d\nSize: %d\n", offset, objs);

		while (objs > 0 && r<reader->tokens_count)
		{
			finished_loop = 0;
			selected_token = 0;
			ex_txt = jsmnreader_extract((reader->tokens + r)->start, (reader->tokens + r)->end, reader);
			if (path_count > 0 && i<path_count)
			{
				if (strcmp(path_list[i], ex_txt) == 0) selected_token = 1;
			}
			//if (selected_token && DEBUGPRINT_TREEGETX) printf(">");
			switch ((reader->tokens + (r + 1))->type)
			{
			default:
				finished_loop = 1;
				/*if (DEBUGPRINT_TREEGETX)
				{
					printf("R[%d]: %s <???>\n", r, ex_txt);
					printf("Clearly off now.\n");
				}*/
				break;
			case JSMN_PRIMITIVE:
				//if (DEBUGPRINT_TREEGETX) printf("R [%d]: %s <PRIMITIVE>\n", r, ex_txt);
				if (selected_token)
				{
					free(ex_txt);
					ex_txt = jsmnreader_extract((reader->tokens + r + 1)->start, (reader->tokens + r + 1)->end, reader);
					//if (DEBUGPRINT_TREEGETX) printf("Found it! [%d] [%s]\n", r + 1, ex_txt);
					loc = r + 1;
					finished_loop = 1;
				}
				r += 2;
				objs--;
				break;
			case JSMN_STRING:
				//if (DEBUGPRINT_TREEGETX) printf("R [%d]: %s\n", r, ex_txt);
				if (selected_token)
				{
					free(ex_txt);
					ex_txt = jsmnreader_extract((reader->tokens + r + 1)->start, (reader->tokens + r + 1)->end, reader);
					//if (DEBUGPRINT_TREEGETX) printf("Found it! [%d] [%s]\n", r + 1, ex_txt);
					loc = r + 1;
					finished_loop = 1;
				}
				r += 2;
				objs--;
				break;
			case JSMN_OBJECT:
				//if (DEBUGPRINT_TREEGETX) printf("R [%d]: %s <OBJECT>\n", r, ex_txt);
				if (!selected_token)
				{
					r++;
					jsmnreader_dataskip(&r, &objs, 0, reader);
					objs--;
				}
				else
				{
					if (i == path_count - 1)
					{
						//if (DEBUGPRINT_TREEGETX) printf("Found it! [%d]\n", r + 1);
						loc = r + 1;
						finished_loop = 1;
					}
					else
					{
						//if (DEBUGPRINT_TREEGETX) printf("\n");
						i += 1;
						if (i > path_count - 1)
							i = path_count - 1;
						offset = r + 1;
						objs = (reader->tokens + offset)->size;
						r += 2;
					}
				}
				break;
			case JSMN_ARRAY:
				//if (DEBUGPRINT_TREEGETX) printf("R [%d]: %s <ARRAY>\n", r, ex_txt);
				if (!selected_token)
				{
					r++;
					jsmnreader_dataskip(&r, &objs, 1, reader);
					objs--;
				}
				else
				{
					if (i == path_count - 1)
					{
						//if (DEBUGPRINT_TREEGETX) printf("Found it! [%d]\n", r + 1);
						loc = r + 1;
						finished_loop = 1;
					}
					else
					{
						finished_loop = 1;
					}
				}
				break;
			}
			if (finished_loop)
			{

				free(ex_txt);
				break;
			}

			free(ex_txt);
		}
		//if (DEBUGPRINT_TREEGETX) printf("---\n");
		for (i = 0; i < path_count; i++)
			free(path_list[i]);
		free(path_list);
		/*if (DEBUGPRINT_READERDATA)
		{

			printf("Tokens: %d\n", reader->tokens_count);
			for (i = 0; i < reader->tokens_count; i++)
			{
				printf("%d: (%d,%d,%d,%d)", i, (reader->tokens + i)->type, (reader->tokens + i)->start, (reader->tokens + i)->end, (reader->tokens + i)->size);
				if ((reader->tokens + i)->type == JSMN_STRING)
				{
					printf(" STR: ");
					ex_txt = jsmnreader_extract((reader->tokens + i)->start, (reader->tokens + i)->end, reader);
					printf("%s", ex_txt);
					free(ex_txt);
				}
				printf("\n");
			}
		}*/

		return loc;
	}

	JSMN_API void jsmnreader_token_array_tokens(unsigned int ** arrays, unsigned int * arrays_size, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		int * edit;
		int can_do;
		//char * temp_str; //Delete later???
		unsigned int size;
		unsigned int i;
		unsigned int objs;
		unsigned int r;
		size = 0;
		can_do = 0;
		r = 0;
		/*if (DEBUGPRINT_ARRAYLIST)
		{
			jsmnreader_print_tokens(reader);
			printf("Offset: %d\n", offset);
		}*/

		*arrays = (int *)malloc(0);
		*arrays_size = 0;
		if (jsmnreader_token_get_array(offset, reader) != -1)
		{
			size = (reader->tokens + offset)->size;
			*arrays = (int *)realloc(*arrays,size*sizeof(int));
			*arrays_size = size;
			edit = *arrays;
			i = 0;
			objs = size;
			r = offset + 1;
			while (objs > 0 && r < reader->tokens_count)
			{
				switch ((reader->tokens + r)->type)
				{
					default:
						break;
					case JSMN_STRING:
						/*if (DEBUGPRINT_ARRAYLIST)
						{
							temp_str = jsmnreader_token_get_string(r, reader);
							printf("%d STRING: %s\n", r, temp_str);
							free(temp_str);
						}*/
						*(edit + i) = r; i++;
						r++;
						break;

					case JSMN_PRIMITIVE:
						/*if (DEBUGPRINT_ARRAYLIST)
						{
							temp_str = jsmnreader_token_get_string(r, reader);
							printf("%d PRIMITIVE: %s\n", r, temp_str);
							free(temp_str);
						}*/
						*(edit + i) = r; i++;
						r++;
						break;

					case JSMN_OBJECT:
						//if (DEBUGPRINT_ARRAYLIST) printf("%d OBJECT\n", r);
						*(edit + i) = r; i++;
						jsmnreader_dataskip(&r, &objs, 0, reader);
						break;

					case JSMN_ARRAY:
						//if (DEBUGPRINT_ARRAYLIST) printf("%d ARRAY\n", r);
						*(edit + i) = r; i++;
						jsmnreader_dataskip(&r, &objs, 1, reader);
						break;
				}
				objs--;
			}
		}
	}

	JSMN_API unsigned int jsmnreader_token_array(unsigned int index, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		int can_do;
		//char * temp_str; //Delete later???
		unsigned int size;
		unsigned int i;
		unsigned int objs;
		unsigned int r;
		size = 0;
		can_do = 0;
		r = 0;
		/*if (DEBUGPRINT_ARRAYLIST)
		{
			jsmnreader_print_tokens(reader);
			printf("Offset: %d\n", offset);
		}*/
		if (jsmnreader_token_get_array(offset, reader) != -1)
		{
			size = (reader->tokens + offset)->size;
			i = 0;
			objs = size;
			r = offset + 1;
			while (objs > 0 && r < reader->tokens_count)
			{
				switch ((reader->tokens + r)->type)
				{
				default:
					break;
				case JSMN_STRING:
					/*if (DEBUGPRINT_ARRAYLIST)
					{
						temp_str = jsmnreader_token_get_string(r, reader);
						printf("%d STRING: %s\n", r, temp_str);
						free(temp_str);
					}*/
					if (i == index)
						return r;
					i++;
					r++;
					break;

				case JSMN_PRIMITIVE:
					/*if (DEBUGPRINT_ARRAYLIST)
					{
						temp_str = jsmnreader_token_get_string(r, reader);
						printf("%d PRIMITIVE: %s\n", r, temp_str);
						free(temp_str);
					}*/
					if (i == index)
						return r;
					i++;
					r++;
					break;

				case JSMN_OBJECT:
					//if (DEBUGPRINT_ARRAYLIST) printf("%d OBJECT\n", r);
					if (i == index)
						return r;
					i++;
					jsmnreader_dataskip(&r, &objs, 0, reader);
					break;

				case JSMN_ARRAY:
					//if (DEBUGPRINT_ARRAYLIST) printf("%d ARRAY\n", r);
					if (i == index)
						return r;
					i++;
					jsmnreader_dataskip(&r, &objs, 1, reader);
					break;
				}
				objs--;
			}
		}
		return -1;
	}

	JSMN_API unsigned int jsmnreader_tree_anyprint(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		int loc;
		int type;
		char * txt;
		float num;
		loc = jsmnreader_tree_get_x(mypath, offset, reader);
		printf("%s -> ", mypath);
		if (loc != -1)
		{
			type = (reader->tokens + loc)->type;
			switch (type)
			{

			case JSMN_OBJECT:
				printf("(Object) [%d]\n", loc);
				break;

			case JSMN_ARRAY:
				printf("(Array) [%d]\n", loc);
				break;

			case JSMN_STRING:
				txt = jsmnreader_token_get_string(loc, reader);
				printf("(String) [%s] [%d]\n", txt, loc);
				free(txt);
				break;

			case JSMN_PRIMITIVE:
				num = jsmnreader_token_get_float(loc, reader);
				printf("(Primitive) [%g] [%d]\n", num, loc);
				break;
			}
		}
		else
		{
			printf("Bad Path\n");
		}
		return loc;
	}

	JSMN_API int jsmnreader_tree_get_int(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		int loc;
		int type;
		int num;
		num = 0;
		loc = jsmnreader_tree_get_x(mypath, offset, reader);
		if (loc != -1)
		{
			type = (reader->tokens + loc)->type;

			if (type == JSMN_PRIMITIVE)
			{
				num = jsmnreader_token_get_int(loc, reader);
			}
		}
		return num;
	}

    JSMN_API unsigned int jsmnreader_tree_get_uint(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		int loc;
		int type;
		unsigned int num;
		num = 0;
		loc = jsmnreader_tree_get_x(mypath, offset, reader);
		if (loc != -1)
		{
			type = (reader->tokens + loc)->type;

			if (type == JSMN_PRIMITIVE)
			{
				num = jsmnreader_token_get_uint(loc, reader);
			}
		}
		return num;
	}

	JSMN_API float jsmnreader_tree_get_float(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		int loc;
		int type;
		float num;
		num = 0;
		loc = jsmnreader_tree_get_x(mypath, offset, reader);
		if (loc != -1)
		{
			type = (reader->tokens + loc)->type;

			if (type == JSMN_PRIMITIVE)
			{
				num = jsmnreader_token_get_float(loc, reader);
			}
		}
		return num;
	}

	JSMN_API char * jsmnreader_tree_get_string(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		int loc;
		int type;
		char * txt;
		loc = jsmnreader_tree_get_x(mypath, offset, reader);
		if (loc != -1)
		{
			type = (reader->tokens + loc)->type;

			if (type == JSMN_STRING || type == JSMN_PRIMITIVE)
			{
				txt = jsmnreader_token_get_string(loc, reader);
			}
		}
		else
		{
			txt = (char *) malloc(0);
		}
		return txt;
	}

    JSMN_API char * jsmnreader_tree_get_raw(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		int loc;
		int type;
		char * txt;
		loc = jsmnreader_tree_get_x(mypath, offset, reader);
		if (loc != -1)
		{
			type = (reader->tokens + loc)->type;

			if (type == JSMN_STRING || type == JSMN_PRIMITIVE)
			{
				txt = jsmnreader_token_get_raw(loc, reader);
			}
		}
		else
		{
			txt = (char *) malloc(0);
		}
		return txt;
	}

	JSMN_API unsigned int jsmnreader_tree_get_object(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		unsigned int loc;
		int type;
		loc = jsmnreader_tree_get_x(mypath, offset, reader);
		if (loc != -1)
		{
			type = (reader->tokens + loc)->type;
			if (type == JSMN_OBJECT)
				return loc;
		}
		return -1;
	}

	JSMN_API unsigned int jsmnreader_tree_get_array(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		unsigned int loc;
		int type;
		loc = jsmnreader_tree_get_x(mypath, offset, reader);
		if (loc != -1)
		{
			type = (reader->tokens + loc)->type;
			if (type == JSMN_ARRAY)
				return loc;
		}
		return -1;
	}

	JSMN_API unsigned int jsmnreader_tree_get_any(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		//jsmnreader_tree_get_x() is functionally the same. Implemented anyways for consistency.
		unsigned int loc;
		loc = jsmnreader_tree_get_x(mypath, offset, reader);
		if (loc != -1)
		{
			return loc;
		}
		return -1;
	}

	JSMN_API void jsmnreader_token_object_tokens(unsigned int ** arrays, unsigned int * arrays_size, unsigned int offset, jsmnreaderobjread_t read_setting, struct jsmnreader_obj_struct * reader)
    {
        unsigned int * edit;
        unsigned int z;
		int in_offset;
		int finished_loop;
		int selected_token;
		char * ex_txt;

		char ** path_list;
		int path_count;
		int i;
		unsigned int objs;
		unsigned int r;
		*arrays = (unsigned int *)malloc(0);
		*arrays_size = 0;
		edit = *arrays;
		path_list=malloc(0);
		r = 0;
        z = 0;
		in_offset = 0;
		if (reader->tokens_count > 0)
		{
			if (offset >= 0 && offset < reader->tokens_count)
			{
				switch ((reader->tokens + offset)->type)
				{
				case JSMN_OBJECT:
					in_offset = 1;
					break;
				//case JSMN_ARRAY:
				//	in_offset = 1;
				//	break;
				}
			}
		}

		if (!in_offset)
        {
            free(path_list);
			return; //Invalid offset
        }
		path_count = 0;
		path_list = jsmnreader_tree_parsepath("", path_list, &path_count);
		objs = (reader->tokens + offset)->size;
		r = offset + 1;
		i = 0;
		while (objs > 0 && r<reader->tokens_count)
		{
			finished_loop = 0;
			selected_token = 0;
			ex_txt = jsmnreader_extract((reader->tokens + r)->start, (reader->tokens + r)->end, reader);
			if (path_count > 0 && i<path_count)
			{
				if (strcmp(path_list[i], ex_txt) == 0) selected_token = 1;
			}
			switch ((reader->tokens + (r + 1))->type)
			{
			default:
				if (i == path_count)
				{

				    if (read_setting==JSMNR_BOTH || read_setting==JSMNR_KEYONLY)
                    {
                        *arrays = (int *)realloc(*arrays,(*arrays_size+1)*sizeof(int));*arrays_size+=1;edit = *arrays;*(edit + z) = r; z++;
                    }
				    if (read_setting==JSMNR_BOTH || read_setting==JSMNR_ITEMONLY)
                    {
                        *arrays = (int *)realloc(*arrays,(*arrays_size+1)*sizeof(int));*arrays_size+=1;edit = *arrays;*(edit + z) = r+1; z++;
                    }

					//printf("R [%d]: %s <???>\n", r, ex_txt);
				}
				break;
			case JSMN_PRIMITIVE:
				if (i == path_count)
				{
				    if (read_setting==JSMNR_BOTH || read_setting==JSMNR_KEYONLY)
                    {
                        *arrays = (int *)realloc(*arrays,(*arrays_size+1)*sizeof(int));*arrays_size+=1;edit = *arrays;*(edit + z) = r; z++;
                    }
				    if (read_setting==JSMNR_BOTH || read_setting==JSMNR_ITEMONLY)
                    {
                        *arrays = (int *)realloc(*arrays,(*arrays_size+1)*sizeof(int));*arrays_size+=1;edit = *arrays;*(edit + z) = r+1; z++;
                    }
				    //*arrays[*arrays_size-1]=r;
					//printf("R [%d]: %s <PRIMITIVE>\n", r, ex_txt);
				}
				r += 2;
				objs--;
				break;
			case JSMN_STRING:
				if (i == path_count)
				{
				    //printf("STR %d\n",*arrays_size);
				    if (read_setting==JSMNR_BOTH || read_setting==JSMNR_KEYONLY)
                    {
                        *arrays = (int *)realloc(*arrays,(*arrays_size+1)*sizeof(int));*arrays_size+=1;edit = *arrays;*(edit + z) = r; z++;
                    }
				    if (read_setting==JSMNR_BOTH || read_setting==JSMNR_ITEMONLY)
                    {
                        *arrays = (int *)realloc(*arrays,(*arrays_size+1)*sizeof(int));*arrays_size+=1;edit = *arrays;*(edit + z) = r+1; z++;
                    }

				}
				r += 2;
				objs--;
				break;
			case JSMN_OBJECT:
				if (i == path_count)
				{
				    if (read_setting==JSMNR_BOTH || read_setting==JSMNR_KEYONLY)
                    {
                        *arrays = (int *)realloc(*arrays,(*arrays_size+1)*sizeof(int));*arrays_size+=1;edit = *arrays;*(edit + z) = r; z++;
                    }
				    if (read_setting==JSMNR_BOTH || read_setting==JSMNR_ITEMONLY)
                    {
                        *arrays = (int *)realloc(*arrays,(*arrays_size+1)*sizeof(int));*arrays_size+=1;edit = *arrays;*(edit + z) = r+1; z++;
                    }

					//printf("R [%d]: %s <OBJECT>\n", r, ex_txt);
				}
				if (!selected_token)
				{
					r++;
					jsmnreader_dataskip(&r, &objs, 0, reader);
					objs--;
				}
				else
				{
					if (i == path_count - 1)
					{
						i += 1;
						offset = r + 1;
						objs = (reader->tokens + offset)->size;
						r += 2;
					}
					else
					{
						if (i == path_count-1)
                        {
							printf("\n");
                        }
						i += 1;
						if (i > path_count - 1)
							i = path_count - 1;
						offset = r + 1;
						objs = (reader->tokens + offset)->size;
						r += 2;
					}
					//}
				}
				break;
			case JSMN_ARRAY:
				if (i == path_count)
				{
				    if (read_setting==JSMNR_BOTH || read_setting==JSMNR_KEYONLY)
                    {
                        *arrays = (int *)realloc(*arrays,(*arrays_size+1)*sizeof(int));*arrays_size+=1;edit = *arrays;*(edit + z) = r; z++;
                    }
				    if (read_setting==JSMNR_BOTH || read_setting==JSMNR_ITEMONLY)
                    {
                        *arrays = (int *)realloc(*arrays,(*arrays_size+1)*sizeof(int));*arrays_size+=1;edit = *arrays;*(edit + z) = r+1; z++;
                    }
					//printf("R [%d]: %s <ARRAY>\n", r, ex_txt);
				}
				if (!selected_token)
				{
					r++;
					jsmnreader_dataskip(&r, &objs, 1, reader);
					objs--;
				}
				break;
			}

			free(ex_txt);

		}
        free(path_list);
		return;
	}

    JSMN_API unsigned int jsmnreader_token_object(unsigned int index, unsigned int offset, jsmnreaderobjread_t read_setting, struct jsmnreader_obj_struct * reader)
    {
    unsigned int * arrays;
    unsigned int arrays_size;
    unsigned int num;
    num=-1;
    jsmnreader_token_object_tokens(&arrays, &arrays_size, offset, read_setting, reader);
    if (arrays_size>0)
    {
        if (index>=0 && index<arrays_size)
        {
            num=arrays[index];
            free(arrays);
            return num;
        }
    }
    free(arrays);
    return num;
    }

	JSMN_API void jsmnreader_tree_print(char * mypath, unsigned int offset, struct jsmnreader_obj_struct * reader)
	{
		int in_offset;
		int finished_loop;
		int selected_token;
		char * ex_txt;

		char ** path_list;
		int path_count;
		int i;
		unsigned int objs;
		unsigned int r;
		path_list=malloc(0);
		r = 0;

		in_offset = 0;
		if (reader->tokens_count > 0)
		{
			if (offset >= 0 && offset < reader->tokens_count)
			{
				switch ((reader->tokens + offset)->type)
				{
				default:
					printf("Error! The offset is not on an object or array!\n");
					break;
				case JSMN_OBJECT:
					in_offset = 1;
					break;
				case JSMN_ARRAY:
					in_offset = 1;
					break;
				}
			}
			else
			{
				printf("Error! The offset is out of range!\n");
			}
		}

		if (!in_offset)
		{
			printf("Error! Invalid offset!\n");
            free(path_list);
			return;
		}

		path_count = 0;
		path_list = jsmnreader_tree_parsepath(mypath, path_list, &path_count);
		objs = (reader->tokens + offset)->size;
		r = offset + 1;
		i = 0;
		while (objs > 0 && r<reader->tokens_count)
		{
			finished_loop = 0;
			selected_token = 0;
			ex_txt = jsmnreader_extract((reader->tokens + r)->start, (reader->tokens + r)->end, reader);
			if (path_count > 0 && i<path_count)
			{
				if (strcmp(path_list[i], ex_txt) == 0) selected_token = 1;
			}
			switch ((reader->tokens + (r + 1))->type)
			{
			default:
				if (i == path_count)
				{
					printf("R [%d]: %s <???>\n", r, ex_txt);
				}
				break;
			case JSMN_PRIMITIVE:
				if (i == path_count)
				{
					printf("R [%d]: %s <PRIMITIVE>\n", r, ex_txt);
				}
				r += 2;
				objs--;
				break;
			case JSMN_STRING:
				if (i == path_count)
				{
					printf("R [%d]: %s\n", r, ex_txt);
				}
				r += 2;
				objs--;
				break;
			case JSMN_OBJECT:
				if (i == path_count)
				{
					printf("R [%d]: %s <OBJECT>\n", r, ex_txt);
				}
				if (!selected_token)
				{
					r++;
					jsmnreader_dataskip(&r, &objs, 0, reader);
					objs--;
				}
				else
				{
					if (i == path_count - 1)
					{
						i += 1;
						offset = r + 1;
						objs = (reader->tokens + offset)->size;
						r += 2;
					}
					else
					{
						if (i == path_count-1)
                        {
							printf("\n");
                        }
						i += 1;
						if (i > path_count - 1)
							i = path_count - 1;
						offset = r + 1;
						objs = (reader->tokens + offset)->size;
						r += 2;
					}
					//}
				}
				break;
			case JSMN_ARRAY:
				if (i == path_count)
				{
					printf("R [%d]: %s <ARRAY>\n", r, ex_txt);
				}
				if (!selected_token)
				{
					r++;
					jsmnreader_dataskip(&r, &objs, 1, reader);
					objs--;
				}
				break;
			}

			free(ex_txt);

		}
        free(path_list);
		return;
	}


#endif /* JSMN_HEADER */

#ifdef __cplusplus
}
#endif

#endif /* JSMN_H */
