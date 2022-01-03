#include <iostream>
extern "C"
{
    #include <libavformat/avformat.h>
}

#define EXIT_SUCCESS 0
#define ARGUMENTS_ERROR -1
#define ALLOCATE_FORMAT_CONTEXT_ERROR -2
#define OPEN_INPUT_FILE_ERROR -3
#define FIND_STREAM_INFO_ERROR -4

int open_input_media_file(AVFormatContext** format_context, const char* input_file);
void clean_memory(AVFormatContext* format_context);

int open_input_media_file(AVFormatContext** format_context, const char* input_file)
{
    int response;

    *format_context = avformat_alloc_context();
    if(!(*format_context))
    {
        av_log(NULL, AV_LOG_ERROR, "Could not allocate memory for the input format context.\n");
        return ALLOCATE_FORMAT_CONTEXT_ERROR;
    }

    response = avformat_open_input(format_context, input_file, NULL, NULL);
    if(response != 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Could not open the input file: %s.\n", input_file);
        return OPEN_INPUT_FILE_ERROR;
    }

    response = avformat_find_stream_info(*format_context, NULL);
    if(response < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Could not find the input stream information.\n");
        return FIND_STREAM_INFO_ERROR;
    }

    return EXIT_SUCCESS;
}

void clean_memory(AVFormatContext* format_context)
{
    if(format_context != NULL)
    {
        avformat_close_input(&format_context);
        avformat_free_context(format_context);
    }
}

int main(int argc, char* argv[])
{
    AVFormatContext* format_context = NULL;

    if(argc != 2)
    {
        av_log(NULL, AV_LOG_ERROR, "Usage: <input file>\n");
        return ARGUMENTS_ERROR;
    }
    const char* input_file = argv[1];

    int response = open_input_media_file(&format_context, input_file);
    if(response != EXIT_SUCCESS)
    {
        clean_memory(format_context);
        return response;
    }

    av_dump_format(format_context, 0, input_file, 0);

    clean_memory(format_context);
    return EXIT_SUCCESS;
}