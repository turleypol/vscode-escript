/** @file
 *
 * @par History
 */


#ifndef __SYMCONT_H
#define __SYMCONT_H

#include <cstdio>

#include "../clib/rawtypes.h"

#ifndef POLSERVER_STOREDTOKEN_H
#include "StoredToken.h"
#endif

namespace Pol
{
namespace Bscript
{

class SymbolContainer
{
protected:
  char* s;
  unsigned usedLen;
  unsigned allocLen;
  unsigned growBy;

public:
  explicit SymbolContainer( int growBy = 512 );
  virtual ~SymbolContainer();
  virtual void pack( void ) {}
  void erase();

  virtual void resize( unsigned lengthToAdd );
  bool findexisting( const void* data, int datalen, unsigned& position );
  void append( const char* string, unsigned& position );
  void append( int lvalue, unsigned& position );
  void append( double dvalue, unsigned& position );
  void append( void* data, unsigned datalen, unsigned& position );

  unsigned length( void ) const { return usedLen; }
  void setlength( unsigned newLen ) { usedLen = newLen; }
  const char* array( void ) const { return s; }
  virtual void* detach( void )
  {
    char* t = s;
    s = nullptr;
    usedLen = allocLen = 0;
    return t;
  }
  virtual void write( FILE* fp );
  virtual void write( char* fname );
  unsigned int get_write_length() const;
  virtual void read( FILE* fp );
  virtual void read( char* fname );
};

/* talk to these in statement numbers */
class StoredTokenContainer final : public SymbolContainer
{
public:
  StoredToken* ST;
  explicit StoredTokenContainer( int growBy = 512 ) : SymbolContainer( growBy ), ST( nullptr ) {}
  virtual void pack( void ) override;
  virtual void* detach( void ) override;
  virtual void resize( unsigned lengthToAdd ) override;
  void setcount( unsigned newCount ) { usedLen = newCount * sizeof( StoredToken ); }
  unsigned count( void ) const { return usedLen / sizeof( StoredToken ); }
  void append_tok( const StoredToken& token, unsigned* position = nullptr );
  void atPut1( const StoredToken& token, unsigned position );
  void atGet1( unsigned position, StoredToken& token ) const;
  unsigned next( void ) { return length() / sizeof( StoredToken ); }
  virtual void read( FILE* fp ) override;
};
}
}
#endif
