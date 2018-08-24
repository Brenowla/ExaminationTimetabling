#ifndef STRUTIL_H
#define STRUTIL_H

#include <vector>
#include <string>

class StrUtil
{
public:
    static char **convert( const std::vector< std::string > &strv );
  
    static const char *shortName( const char *str, char *result );

    
};

#endif // STRUTIL_H
