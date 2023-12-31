# JSMN Reader

An extension of the [JSON parser JSMN](https://github.com/zserge/jsmn) for C, made into a library for advanced use in JSON reading. Being built from it, it still retains the simple, light-weight token-based design the original parser had.

Similar to the original parser, it's designed to be usuable in a wide range of compilers by being written within ANSI-C. It still retains the simple design for installation, and is likely to share similar compatiblity.

The JSMN reader adds functions that utilize the token data generated by JSMN. Do note however, JSMN Reader uses dynamically allocated data.

## Features
* Original JSMN features are still applicable.
* Tokens can be grabbed from paths using `jsmnreader_tree_get_<type>` functions.
* Tokens can be grabbed individually from their IDs, using `jsmnreader_token_get_<type>` functions.
* Token IDs can be obtained from arrays and objects within. 
	* Options exist to get array data from a generated array, or specifiying location from within a function.
* Debug printing functions for outputing the contents of a path, or displaying the raw JSON string and tokens.
* Built-in parsing functions from file or string input.

## Installation & Usage
The installation is more or less the same as jsmn, as you only need to include `jsmnreader.h` to your source file.

Additionally, the macros `JSMN_STATIC` (for making symbols static) and `JSMN_HEADER` (for avoiding duplicate symbols) are still present, because it is still a library of a single header file.

You can initalize JSMN Reader with `jsmnreader_init()`, and do need to free it with `jsmnreader_free()` when it's done, due to allocating data on initalization.

```
#include "jsmnreader.h"

...

jsmnreader_obj myjsmn;
jsmnreader_init(&myjsmn);

...

jsmnreader_free(&myjsmn);
```

## Documentation

*(That's right, the documentation is in the README.md file.)*

### Initalization/Loading

* `jsmnreader_init(&reader)`: Initalizes the reader data, as well as sets up **malloc()**. Should be the first function used.
* `jsmnreader_free(&reader)`: Frees the reader data from memory. Should be the last function used.
* `jsmnreader_load(str, str_size, &reader)`: Loads a C string to populate the tokens within the reader. Can return an int for checking errors loading.
* `jsmnreader_fileload(filepath, &reader)`: Loads a C string from a text file to populate the tokens within the reader. Can return an int for checking errors loading.

### Tree Grabbing

* `jsmnreader_tree_get_x(mypath, offset, &reader)`: Returns the token's ID if the token was successfully found. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
* `jsmnreader_tree_get_int(mypath, offset, &reader)`: Returns an int if the token successfully found. Returns 0 in failure.
* `jsmnreader_tree_get_uint(mypath, offset, &reader)`: Returns an unsigned int if the token successfully found. Returns 0 in failure.
* `jsmnreader_tree_get_float(mypath, offset, &reader)`: Returns a float if the token was successfully found. Returns 0 in failure.
* `jsmnreader_tree_get_string(mypath, offset, &reader)`: Returns an allocated C string if the token was successfully found. Returns as a blank string in failure. Compatible with other types of items. Remember to free the C string after usage.
* `jsmnreader_tree_get_raw(mypath, offset, &reader)`: Returns an allocated C string of "raw" contents (strings with quotations, true/false/null, etc.) if the token was successfully found. Returns as a blank string in failure. Remember to free the C string after usage.
* `jsmnreader_tree_get_object(mypath, offset, &reader)`: Returns the token's ID if the object token was successfully found. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
* `jsmnreader_tree_get_array(mypath, offset, &reader)`: Returns the token's ID if the array token was successfully found. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
* `jsmnreader_tree_get_any(mypath, offset, &reader)`: Returns the token's ID if the token was successfully found. Is arguably redundant to **jsmnreader_tree_get_x()**, but was implemented for naming consistency. On failure to locate the token, it returns as -1 (or unsigned 4294967295).

**mypath** usage appears as `"repository\\type"` like a filepath, use a blank string `""` if you want to grab from the root from the `offset`. Generally, **offset** comes from object/array-related output.

### Token Grabbing

* `jsmnreader_token_get_int(index, &reader)`: Returns an int if the token successfully found. Returns 0 in failure.
* `jsmnreader_token_get_uint(index, &reader)`: Returns an unsigned int if the token successfully found. Returns 0 in failure.
* `jsmnreader_token_get_float(index, &reader)`: Returns a float if the token successfully found. Returns 0 in failure.
* `jsmnreader_token_get_string(index, &reader)`: Returns an allocated C string if the token was successfully found; it also can be used to grab the key string. Returns as a blank string in failure. Compatible with other types of items. Remember to free the C string after usage.
* `jsmnreader_token_get_raw(index, &reader)`: Returns an allocated C string of "raw" contents (strings with quotations, true/false/null, etc.) if the token was successfully found; it also can be used to grab the key string. Returns as a blank string in failure. Remember to free the C string after usage.
* `jsmnreader_token_get_object(index, &reader)`: Returns the token's ID if the object was successfully found. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
* `jsmnreader_token_get_array(index, &reader)`: Returns the token's ID if the array was successfully found. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
* `jsmnreader_token_size(index, &reader)`: Returns the token's size if the object or array was successfully found. Returns 0 in failure.
* `jsmnreader_token_is_null(index, &reader)`: Returns a 1 if the null token was successfully found. Returns 0 in failure.
* `jsmnreader_token_is_special(index, &reader)`: Returns a 1 if a special token (true/false/null) was successfully found. Returns 0 in failure.

It grabs the token's data directly if successfully found, without the use of a path. To be used especially with arrays. Generally, **index** comes from object/array-related output.

### Token Index Grabbing

* `jsmnreader_token_array(index, offset, &reader)`: Returns a token ID from a specified index within the array's tokens. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
* `jsmnreader_token_array_tokens(&arrays, &arrays_size, offset, &reader)`: Populates an unsigned int array with the indexes of the array's tokens.
* `jsmnreader_token_object(index, offset, read_setting, &reader)`: Returns a token ID from a specified index within the object's tokens. On failure to locate the token, it returns as -1 (or unsigned 4294967295).
* `jsmnreader_token_object_tokens(&arrays, &arrays_size, offset, read_setting, &reader)`: Populates an unsigned int array with the indexes of the object's tokens.

To be used with the token grabbing functions. On failure to locate the token, it returns as -1 (or unsigned 4294967295).

**read_setting** makes use of the `jsmnreaderobjread_t` enum (`JSMNR_BOTH`, `JSMNR_KEYONLY`, `JSMNR_ITEMONLY`) for listing the tokens within the object.

### Debug Output

* `jsmnreader_print_string(&reader)`: Outputs the raw string contents of the reader's JSON string.
* `jsmnreader_print_tokens(&reader)`: Outputs a list of the reader's tokens.
* `jsmnreader_tree_print(mypath, offset, &reader)`: Outputs a list of the visible tokens from the path.
* `jsmnreader_tree_anyprint(mypath, offset, &reader)`: Outputs the token if the token was successfully found from the path.

### Error Constants

* `JSMN_ERROR_NOMEM`: Not enough memory, or too much memory to parse. (-1)
* `JSMN_ERROR_INVAL`: Invalid format. (-2)
* `JSMN_ERROR_PART`: Fragmented JSON. (-3)
* `JSMN_ERROR_NOFILE`: File not found. (-4)
* `JSMN_SUCCESS`: JSON parsed successfully. Not an error, but listed for consistency. (0)

Constants for loading errors, to be used with **jsmnreader_load()** or **jsmnreader_fileload()**.

## Misc. Info

This software is distributed under [MIT license](http://www.opensource.org/licenses/mit-license.php), so feel free to integrate it in your commercial products.