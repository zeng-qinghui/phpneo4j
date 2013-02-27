dnl
dnl $Id$ 
dnl

PHP_ARG_WITH(neo4j, for neo4j support,
[  --with-neo4j[=DIR]       Include cURL support])

dnl Temporary option while we develop this aspect of the extension
if test "$PHP_NEO4J" != "no"; then

  if test "$PHP_NEO4JWRAPPERS" != "no" ; then
    AC_DEFINE(PHP_NEO4JWRAPPERS,1,[ ])
  fi

  PHP_NEW_EXTENSION(neo4j,php_neo4j.c neo4j.c neo4jNode.c neo4jRelationship.c neo4jPath.c neo4j_curl.c neo4jIndex.c, $ext_shared)
fi
