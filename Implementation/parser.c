#include "parser.h"

struct Result parse_result(char * server_reply)
{
    struct Result ret;
    char *res = strtok(server_reply, "\n");
    while (res != NULL)
    {
        cJSON *root = cJSON_Parse(res);
        if (root != NULL)
        {
            cJSON *result = cJSON_GetObjectItem(root, "result");
            if (result != NULL)
            {
                if (cJSON_IsNull(result))
                {
                    cJSON *error = cJSON_GetObjectItem(root, "error");
                    ret.success = 0;
                    ret.error = cJSON_GetArrayItem(error,1)->valuestring;
                    return ret;
                }
                else if (cJSON_IsTrue(result))
                {
                    ret.success = 1;
                    return ret;
                }
            }
        }
        res = strtok(NULL, "\n");
    }
    ret.success = 0;
    return ret;
}

struct Job parse_data(char * server_reply)
{
    struct Job job;
    job.valid = 1;
    // printf("%s\n",server_reply);
    char *res = strtok(server_reply, "\n");
    while (res != NULL)
    {
        cJSON *root = cJSON_Parse(res);
        if (root != NULL)
        {
            cJSON *method = cJSON_GetObjectItem(root, "method");
            if (method!=NULL)
            {
                cJSON *params = cJSON_GetObjectItem(root, "params");
                if (strcmp(method->valuestring,"mining.set_difficulty")==0)
                {
                    int share_diff = cJSON_GetArrayItem(params,0)->valueint;
                    job.difficulty = share_diff;
                    //printf("share_diff: %d\n", share_diff);
                    
                }
                else if (strcmp(method->valuestring,"mining.notify")==0)
                {  

                    char *job_id = cJSON_GetArrayItem(params,0)->valuestring;
                    job.job_id = job_id;
                    //printf("job id: %lu\n",job.job_id);
                    char *prev_hash = cJSON_GetArrayItem(params,1)->valuestring;
                    for (int i = 0;i<32;i++)
                    {
                        job.prev_hash[i] = str2bytes(prev_hash).data[i];
                    }
                    //printf("prev_hash: %lu\n",job.prev_hash);

                    char *coinb1 = cJSON_GetArrayItem(params,2)->valuestring;
                    //printf("coinb1: %lu\n",job.coinb1);
                    job.coinb1 = str2bytes(coinb1);

                    char *coinb2 = cJSON_GetArrayItem(params,3)->valuestring;
                    //printf("coinb2: %lu\n",job.coinb2);
                    job.coinb2 = str2bytes(coinb2);

                    cJSON *merkle_branch = cJSON_GetArrayItem(params,4);
                    int merkle_branch_len = cJSON_GetArraySize(merkle_branch);
                    struct merkletree merkle_tree;
                    unsigned char ** tree = (unsigned char **)calloc(merkle_branch_len, sizeof(unsigned char **));
                    for (int i = 0; i < cJSON_GetArraySize(merkle_branch); i++)
                    {
                        char * branch = cJSON_GetArrayItem(merkle_branch,i)->valuestring;
                        tree[i] = (unsigned char *) malloc (32*sizeof(unsigned char));
                        for (int j = 0; j < 32; j++)
                        {
                            tree[i][j] = str2byte(branch[2*i], branch[2*i+1]);
                        }
                        //printf("HERE: %lu\n",merkle_tree[i]);
                    }
                    merkle_tree.size = merkle_branch_len;
                    merkle_tree.hashes = tree;
                    job.merkle_tree = merkle_tree;

                    char *version = cJSON_GetArrayItem(params,5)->valuestring;
                    //printf("version: %lu\n",job.version);
                    job.version = str2uint32(version);

                    char *nbits = cJSON_GetArrayItem(params,6)->valuestring;
                    //printf("nbits: %lu\n",job.nbits);
                    job.nbits = str2uint32(nbits);

                    char *ntime = cJSON_GetArrayItem(params,7)->valuestring;
                    job.ntime = str2uint32(ntime);

                    cJSON * clean_job = cJSON_GetArrayItem(params,8);
                    if (clean_job->type == cJSON_True)
                    {
                        //printf("clean_job: true\n");
                        job.clean = 1;
                    }
                    else
                    {
                        //printf("clean_job: false\n");
                        job.clean = 0;
                    }
                }
                else
                {
                    

                }
                
                
            }
            else 
            {
                cJSON *result = cJSON_GetObjectItem(root, "result");
                if (result!=NULL)
                {
                    cJSON *exnonce1 = cJSON_GetArrayItem(result,1);
                    cJSON *exnonce2_size = cJSON_GetArrayItem(result,2);
                    int extranonce2_size = exnonce2_size->valueint;
                    uint32_t extranonce1 = (uint32_t)strtoll(exnonce1->valuestring, NULL, 16);
                    //printf("extranonce1: %lu\n",extranonce1);
                    //printf("%d\n",extranonce2_size);
                    unsigned char *extranonce2 = (unsigned char *)malloc(extranonce2_size*sizeof(char));
                    memset(extranonce2, 0, extranonce2_size);
                    //printf("extranonce2: %lu\n",*extranonce2);
                    struct Extranonce extranonce;
                    extranonce.extranonce1 = extranonce1;
                    extranonce.extranonce2.data = extranonce2;
                    extranonce.extranonce2.size = extranonce2_size;
                    job.extranonce = extranonce;

                }
            }
            

        }
        else
        {
            //printf("Error");
            job.valid = 0;
        }
        res = strtok(NULL, "\n");
    }

    return job;
}
