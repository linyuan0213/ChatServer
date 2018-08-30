#include <iostream>  
#include <cstdlib>  
#include <cstring>  
#include <map>
  
#define MAX_BUF_LEN 1024  
#define MAX_KEY_LEN 64  
#define MAX_VAL_LEN 256  
  
int trim(char *s)  
{  
    int n;  
	if (s == nullptr)
		return -1;

    for(n = strlen(s) - 1; n >= 0; n--)  
    {  
        if(s[n] != ' ' && s[n] != '\t' && s[n] != '\n')  
            break;  
        s[n+1] = '\0';  
    }  

    return n;  
}  
  
int load_config(const char* config_path, std::map<std::string, std::string>& map)  
{  
    FILE * file = fopen(config_path, "r");  
    if (file == NULL)  
    {  
		fprintf(stderr, "[Error]open %s failed.\n", config_path);  
        return -1;  
    }  
  
  
    char buf[MAX_BUF_LEN];  
    int text_comment = 0;  
    while(fgets(buf, MAX_BUF_LEN, file) != nullptr)  
    {  
        trim(buf);  
        // to skip text comment with flags /* ... */  
        if (buf[0] != '#' && (buf[0] != '/' || buf[1] != '/'))  
        {  
            if (strstr(buf, "/*") != nullptr)  
            {  
                text_comment = 1;  
                continue;  
            }  
            else if (strstr(buf, "*/") != nullptr)  
            {  
                text_comment = 0;  
                continue;  
            }  
        }  
        if (text_comment == 1)  
        {  
            continue;  
        }  
  
  
        int buf_len = strlen(buf);  
        // ignore and skip the line with first chracter '#', '=' or '/'  
        if (buf_len <= 1 || buf[0] == '#' || buf[0] == '=' || buf[0] == '/')  
        {  
            continue;  
        }  
        buf[buf_len-1] = '\0';  
  
  
        char paramk[MAX_KEY_LEN] = {0}, paramv[MAX_VAL_LEN] = {0};  
        int kv = 0, klen = 0, vlen = 0;  
        int i = 0;  
        for (i = 0; i < buf_len; ++i)  
        {  
            if (buf[i] == ' ')  
                continue;  
            // scan param key name  
            if (kv == 0 && buf[i] != '=')  
            {  
                if (klen >= MAX_KEY_LEN)  
                    break;  
                paramk[klen++] = buf[i];  
                continue;  
            }  
            else if (buf[i] == '=')  
            {  
                kv = 1;  
                continue;  
            }  
            // scan param key value  
            if (vlen >= MAX_VAL_LEN || buf[i] == '#')  
                break;  
            paramv[vlen++] = buf[i];  
        }  
        if (strcmp(paramk, "") == 0 || strcmp(paramv, "") == 0)  
            continue;  
       // printf("%s=%s\n", paramk, paramv);  
		map.insert(std::pair<std::string, std::string>(paramk, paramv));
    }  

    return 0;  
}  

