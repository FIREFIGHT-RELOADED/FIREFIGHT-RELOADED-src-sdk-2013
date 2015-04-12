//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "iclassmap.h"
#include "utldict.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class classentry_t
{
public:
	classentry_t()
	{
		mapname[ 0 ] = 0;

		classname[ 0 ] = 0;

		factory = 0;
		size = -1;

		scripted = false;

	}

	char const *GetMapName() const
	{
		return mapname;
	}

	void SetMapName( char const *newname )
	{
		Q_strncpy( mapname, newname, sizeof( mapname ) );
	}


	char const *GetClassName() const
	{
		return classname;
	}

	void SetClassName( char const *newname )
	{
		Q_strncpy( classname, newname, sizeof( classname ) );
	}


	DISPATCHFUNCTION	factory;
	int					size;

	bool				scripted;

private:
	char				mapname[ 40 ];

	char				classname[ 40 ];

};

class CClassMap : public IClassMap
{
public:

	virtual void			Add( const char *mapname, const char *classname, int size, DISPATCHFUNCTION factory /*= 0*/, bool scripted );
	virtual void			RemoveAllScripted( void );

	virtual const char		*Lookup( const char *classname );

	virtual DISPATCHFUNCTION FindFactory( const char *classname );

	virtual C_BaseEntity	*CreateEntity( const char *mapname );
	virtual int				GetClassSize( const char *classname );

private:
	CUtlDict< classentry_t, unsigned short > m_ClassDict;
};

IClassMap& GetClassMap( void )
{
	static CClassMap g_Classmap;
	return g_Classmap;
}

void CClassMap::Add( const char *mapname, const char *classname, int size, DISPATCHFUNCTION factory = 0, bool scripted = false )
{

	for ( int i=m_ClassDict.First(); i != m_ClassDict.InvalidIndex(); i=m_ClassDict.Next( i ) )
	{
		classentry_t *lookup = &m_ClassDict[ i ];
		if ( !lookup )
			continue;

		if ( !Q_stricmp( lookup->GetMapName(), mapname ) )
		{
			m_ClassDict.RemoveAt( i );
		}
	}


	classentry_t element;
	element.SetMapName( mapname );
	element.factory = factory;
	element.size = size;

	element.SetClassName( classname );
	element.scripted = scripted;
	m_ClassDict.Insert( mapname, element );

}

void CClassMap::RemoveAllScripted( void )
{
	int c = m_ClassDict.Count();
	int i;

	for ( i = 0; i < c; i++ )
	{
		classentry_t *lookup = &m_ClassDict[ i ];
		if ( !lookup )
			continue;

		if ( lookup->scripted )
		{
			m_ClassDict.RemoveAt( i );
		}
	}
}


const char *CClassMap::Lookup( const char *classname )
{
	int c = m_ClassDict.Count();
	int i;

	for ( i = 0; i < c; i++ )
	{
		classentry_t *lookup = &m_ClassDict[ i ];
		if ( !lookup )
			continue;

		if ( Q_stricmp( lookup->GetClassName(), classname ) )
			continue;

		return lookup->GetMapName();
	}

	return NULL;
}

DISPATCHFUNCTION CClassMap::FindFactory( const char *classname )
{
	for ( int i=m_ClassDict.First(); i != m_ClassDict.InvalidIndex(); i=m_ClassDict.Next( i ) )
	{
		classentry_t *lookup = &m_ClassDict[ i ];
		if ( !lookup )
			continue;

		if ( Q_stricmp( lookup->GetMapName(), classname ) )
			continue;

		return lookup->factory;
	}

	return NULL;
}


C_BaseEntity *CClassMap::CreateEntity( const char *mapname )
{
	int c = m_ClassDict.Count();
	int i;

	for ( i = 0; i < c; i++ )
	{
		classentry_t *lookup = &m_ClassDict[ i ];
		if ( !lookup )
			continue;

		if ( Q_stricmp( lookup->GetMapName(), mapname ) )
			continue;

		if ( !lookup->factory )
		{
#if defined( _DEBUG )
			Msg( "No factory for %s/%s\n", lookup->GetMapName(), m_ClassDict.GetElementName( i ) );
#endif
			continue;
		}

		return ( *lookup->factory )();
	}

	return NULL;
}

int CClassMap::GetClassSize( const char *classname )
{
	int c = m_ClassDict.Count();
	int i;

	for ( i = 0; i < c; i++ )
	{
		classentry_t *lookup = &m_ClassDict[ i ];
		if ( !lookup )
			continue;

		if ( Q_strcmp( lookup->GetMapName(), classname ) )
			continue;

		return lookup->size;
	}

	return -1;
}
