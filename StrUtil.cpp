#include <cassert>
#include <cstring>
#include <cstdlib>
#include "StrUtil.hpp"

using namespace std;

#define MAX_NAME_SIZE 512

char** StrUtil::convert(const std::vector< std::string >& strv)
{
    if ( strv.size() == 0 )
        return nullptr;
    
    size_t strSpace = 0;
    for ( auto itS=strv.cbegin() ; (itS!=strv.cend()) ; ++itS )
        strSpace += (itS->size()+1);

    assert( strSpace>0 );
    
    // allocating
    char **res = new char*[strv.size()+1];
    assert( res );
    res[0] = new char[strSpace];
    assert( res[0] );
    for ( size_t i=1 ; (i<strv.size()) ;++i)
        res[i] = (res[i-1]) + (strv[i-1].size() + 1);

    for ( size_t i=0 ; (i<strv.size()) ;++i)
        strcpy( res[i], strv[i].c_str() );
    
    return res;
}

const char *StrUtil::shortName( const char *str, char *result )
{
    //memset( result, '\0', sizeof(char)*MAX_NAME_SIZE );
    int l = strlen(str);
    int added = 0;
    for ( int i=0 ; i<l && added<MAX_NAME_SIZE-1; ++i )
        if ( isdigit(str[i]) || isalpha(str[i]) )
            result[added++] = tolower(str[i]);
    result[added] = '\0';
    
    return result;
}
