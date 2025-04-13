#pragma once
#include "vdf_parser.hpp"

#include <Toshi/T2String.h>
#include <Toshi/TString8.h>

#include <format>

namespace tyti
{

namespace vdf
{

TINT getInt( tyti::vdf::object& rObject, Toshi::T2StringView strAttrib, TINT iDefault )
{
	auto it = rObject.attribs.find( strAttrib.Get() );

	if ( it == rObject.attribs.end() )
		return iDefault;

	return Toshi::T2String8::StringToInt( it->second.c_str() );
}

TFLOAT getFloat( tyti::vdf::object& rObject, Toshi::T2StringView strAttrib, TFLOAT fDefault )
{
	auto it = rObject.attribs.find( strAttrib.Get() );

	if ( it == rObject.attribs.end() )
		return fDefault;

	return Toshi::T2String8::StringToFloat( it->second.c_str() );
}

TBOOL getBool( tyti::vdf::object& rObject, Toshi::T2StringView strAttrib, TBOOL bDefault )
{
	auto it = rObject.attribs.find( strAttrib.Get() );

	if ( it == rObject.attribs.end() )
		return bDefault;

	return it->second.c_str()[ 0 ] == '1' || it->second.c_str() == "true";
}

std::string getString( tyti::vdf::object& rObject, Toshi::T2StringView strAttrib, Toshi::T2StringView strDefault )
{
	auto it = rObject.attribs.find( strAttrib.Get() );

	if ( it == rObject.attribs.end() )
		return strDefault.Get();

	return it->second.c_str();
}

std::string toString( TBOOL bValue )
{
	return bValue ? "1" : "0";
}

std::string toString( TINT iValue, TINT iRadix = 10 )
{
	Toshi::T2String8::IntToString( iValue, Toshi::T2String8::ms_aScratchMem, iRadix );
	return Toshi::T2String8::ms_aScratchMem;
}

std::string toString( TFLOAT fValue )
{
	return std::format( "{}", fValue ).c_str();
}

} // namespace vdf

} // namespace tyti
