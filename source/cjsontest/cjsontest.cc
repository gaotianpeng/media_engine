#include <iostream>
#include <stdlib.h>
#include "cJSON.h"

using namespace std; 
char* read_file(const char *filename) {
	FILE *file = NULL;
	long length = 0;
	char *content = NULL;
	size_t read_chars = 0;

	/* open in read binary mode */
	file = fopen(filename, "rb");
	if (file == NULL)
	{
		goto cleanup;
	}

	/* get the length */
	if (fseek(file, 0, SEEK_END) != 0)
	{
		goto cleanup;
	}
	length = ftell(file);
	if (length < 0)
	{
		goto cleanup;
	}
	if (fseek(file, 0, SEEK_SET) != 0)
	{
		goto cleanup;
	}

	/* allocate content buffer */
	content = (char*)malloc((size_t)length  sizeof(""));
	if (content == NULL)
	{
		goto cleanup;
	}

	/* read the file into memory */
	read_chars = fread(content, sizeof(char), (size_t)length, file);
	if ((long)read_chars != length)
	{
		free(content);
		content = NULL;
		goto cleanup;
	}
	content[read_chars] = '\0';


cleanup:
	if (file != NULL)
	{
		fclose(file);
	}

	return content;
}
static cJSON *parse_file(const char *filename)
{
	cJSON *parsed = NULL;
	char *content = read_file(filename);

	parsed = cJSON_Parse(content);

	if (content != NULL)
	{
		free(content);
	}

	return parsed;
}


int main(int argc, char* argv[]) {
	//cJSON *tree = parse_file("e:/test.json");
	char* startrecord_str = "{\"opcode\": 1,\"accessKey\": \"123456\",\"width\": 1280,\"height\": 720,\"appid\": \"synative\",\"bitrate\": 1500000,\"cpuopt\": true}";
	cout << "--------------------------" << strlen(startrecord_str) << endl;
	system("pause"); 
	return 0; 
}