#include "c/methods/CommentsMethods.h"

extern "C" {
void modioGetAllModComments(void *object, u32 mod_id, ModioFilterCreator filter, void (*callback)(void *object, ModioResponse response, ModioComment comments[], u32 comments_size))
{
    std::string filter_string = modio::getFilterString(&filter);
    std::string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods/" + modio::toString(mod_id) + "/comments?" + filter_string + "&api_key=" + modio::API_KEY;

    u32 call_number = modio::curlwrapper::getCallNumber();

    get_all_mod_comments_callbacks[call_number] = new GetAllModCommentsParams;
    get_all_mod_comments_callbacks[call_number]->callback = callback;
    get_all_mod_comments_callbacks[call_number]->object = object;
    get_all_mod_comments_callbacks[call_number]->url = url;
    get_all_mod_comments_callbacks[call_number]->is_cache = false;

    std::string cache_filename = modio::getCallFileFromCache(url, filter.cache_max_age_seconds);
    if (cache_filename != "")
    {
        std::ifstream cache_file(modio::getModIODirectory() + "cache/" + cache_filename);
        json cache_file_json;
        if (cache_file.is_open())
        {
            cache_file >> cache_file_json;
            get_all_mod_comments_callbacks[call_number]->is_cache = true;
            modioOnGetAllModComments(call_number, 200, cache_file_json);
            return;
        }
    }
    modio::curlwrapper::get(call_number, url, modio::getHeaders(), &modioOnGetAllModComments);
}

void modioDeleteModComment(void* object, u32 mod_id, u32 comment_id, void(*callback)(void* object, ModioResponse response))
{
    std::map<std::string, std::string> data;
    u32 call_number = modio::curlwrapper::getCallNumber();

    delete_mod_comment_callbacks[call_number] = new DeleteModCommentParams;
    delete_mod_comment_callbacks[call_number]->callback = callback;
    delete_mod_comment_callbacks[call_number]->object = object;

    std::string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods/" + modio::toString(mod_id) + "/dependencies/" + modio::toString(comment_id);

    modio::curlwrapper::deleteCall(call_number, url, modio::getUrlEncodedHeaders(), &modioOnDeleteModComment);
}
}
