#ifndef _SMART_ENUM_H
#define _SMART_ENUM_H

/// \brief Smart enum template
template<typename def>
struct smart_enum : def
{
	/// \brief Type of value provider
    typedef typename def::type type;
	/// \brief Value provider object
    type v;
	/// \brief Construcotr
	/// \param v value
    smart_enum(type v) : v(v) {}
	/// \brief Operator ()
    operator type () const {return v;}
};

#endif
