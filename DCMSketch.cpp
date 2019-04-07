/* Refer this site for bloom filter 'http://www.partow.net/programming/bloomfilter/index.html'

*/


#include <iostream>
#include "bloom_filter.hpp"
#include "CMSketch.h"
#include <vector>
#include <pthread.h>

using namespace std;

typedef struct ArgsStruct {
    const char* str;
    int index;
    int estimation;
}argsStruct;

class DCMSketch {
    private:
        vector <CMSketch *> cmss;
        vector <bloom_filter *> bfs;
        vector <int> countsInCms;
        int maxCount;
        int w;
        int d;

    public:
        DCMSketch(int w, int d, int maxCount) {
                CMSketch *cmsketch;
                cmsketch = new CMSketch(w , d);
                cmss.push_back(cmsketch);

                bloom_parameters parameters;
                parameters.projected_element_count = maxCount;
                parameters.false_positive_probability = 0.1/(maxCount);
                parameters.compute_optimal_parameters();
                bloom_filter *filter = new bloom_filter(parameters);
                bfs.push_back(filter);

                countsInCms.push_back(0);
                this->maxCount = maxCount;
                this->w = w;
                this->d = d;
        }

        void Insert(const char *str) {
            bool inserted = 0;
            for(int i = 0 ; i < cmss.size() ; i++ ) {
                if(countsInCms[i] >= maxCount) {
                    continue;
                }

                if (!bfs[i]->contains(str) ) {
                    bfs[i]->insert(str);
                }



                cmss[i]->Insert(str);
                countsInCms[i]++;
                inserted = 1;
                break;
            }
            if(inserted) {
                return ;
            }

            CMSketch *cmsketch;
            cmsketch = new CMSketch(w , d);
            bloom_parameters parameters;
            parameters.projected_element_count = maxCount;
            parameters.false_positive_probability = 0.1/(maxCount);
            parameters.compute_optimal_parameters();
            bloom_filter *filter = new bloom_filter(parameters);
            bfs.push_back(filter);
            cmss.push_back(cmsketch);

            countsInCms.push_back(0);

            bfs[bfs.size()-1]->insert(str);
            cmss[cmss.size()-1]->Insert(str);
            countsInCms[countsInCms.size()-1]++;
            if (strcmp("}+$#55#pmm#}7~um+upp<#5m#}p~#~}u7$<5pu@d",str) == 0)
                cout<<"Insert : "<<countsInCms.size()-1<<endl;
                
        }

        // void *QueryParallelHelper(void *args) {

        //     argsStruct *actualArgs = (argsStruct*) args;

        //     if(!bfs[actualArgs->index]->contains(actualArgs->str)) {
        //         return NULL;
        //     }

        //     actualArgs->estimation = cmss[actualArgs->index]->Query(actualArgs->str);
        //     return NULL;
            
        // }

        // int QueryParallel(const char *str) {
        //     pthread_t tid[cmss.size()];
        //     argsStruct* arg[cmss.size()];
        //     int estimate = 0;

        //     for(int i = 0 ; i < cmss.size() ; i++) {
        //         arg[i] = new argsStruct();
        //         arg[i]->index = i;
        //         arg[i]->str = str;
        //         arg[i]->estimation = 0;

        //         pthread_create(&tid[i], NULL, QueryParallelHelper, (void *) (arg + i));

        //     }

        //     for(int i = 0 ; i < cmss.size() ; i++) {
        //         pthread_join(tid[i], NULL);
        //         estimate += arg[i]->estimation;

        //         delete arg[i];
        //     }

        //     delete arg;

        //     pthread_exit(0);

        //     return estimate;

        // }

        int Query(const char *str) {

            int estimation = 0;
            
            for(int i = 0 ; i < cmss.size() ; i++ ) {

                if (!bfs[i]->contains(str) ) {
                    continue;
                }
                estimation += cmss[i]->Query(str);
            }

            return estimation;
        }

        int numberOfCMS() {
            return countsInCms.size();
        }

        void printSingleCMS() {

            return cmss[0]->Print();

        }

};

// int main() {
//     DCMSketch* d = new DCMSketch(10,10,1000);
//     char words[][100] = {"helloo", "haii", "awsdf", "asdkall", "cnxmesa", "kalkskdl", "aslooqwd", "jksdfkk", "nmnmhkg", "dslflsdlf"};
//     for(int i = 0 ; i<10 ; i++) {
//         for(int j = 0; j < 100 ; j++) {
//                 d->Insert(words[i]);
//         }
//     }

//     for(int i = 0 ; i < 10 ; i++) {
//         cout << "Estimate of  " << words[i] << "---------" << d->estimate(words[i]) << endl;
//     }
//     return 0;
// }


