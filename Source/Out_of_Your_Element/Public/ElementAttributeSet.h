#pragma once

#include "AttributeSet.h"
#include "ElementAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#define NOTIFY_UPDATE(PropertyName) \
	On##PropertyName##Changed.Broadcast(this, OldValue, NewValue);

#define CHECK_AND_NOTIFY_UPDATE1(PropertyName) \
	if (Attribute == Get##PropertyName##Attribute()) \
		NOTIFY_UPDATE(PropertyName)

#define CHECK_AND_NOTIFY_UPDATE2(P1, P2) \
	CHECK_AND_NOTIFY_UPDATE1(P1) \
	else CHECK_AND_NOTIFY_UPDATE1(P2)

#define CHECK_AND_NOTIFY_UPDATE3(P1, P2, P3) \
	CHECK_AND_NOTIFY_UPDATE1(P1) \
	else CHECK_AND_NOTIFY_UPDATE1(P2) \
	else CHECK_AND_NOTIFY_UPDATE1(P3)

#define CHECK_AND_NOTIFY_UPDATE4(P1, P2, P3, P4) \
	CHECK_AND_NOTIFY_UPDATE1(P1) \
	else CHECK_AND_NOTIFY_UPDATE1(P2) \
	else CHECK_AND_NOTIFY_UPDATE1(P3) \
	else CHECK_AND_NOTIFY_UPDATE1(P4)

// Based on source from - https://stackoverflow.com/a/11763277
// Posted by netcoder, modified by community. See post 'Timeline' for change history
// Retrieved 2025-11-12, License - CC BY-SA 4.0
#define EXPAND(x) x
#define GET_MACRO(_1, _2, _3, _4, name, ...) name
#define CHECK_AND_NOTIFY_UPDATE(...) EXPAND(GET_MACRO(__VA_ARGS__, CHECK_AND_NOTIFY_UPDATE4, CHECK_AND_NOTIFY_UPDATE3, CHECK_AND_NOTIFY_UPDATE2, CHECK_AND_NOTIFY_UPDATE1)(__VA_ARGS__))

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChangedEvent,
                                               UAttributeSet*, AttributeSet,
                                               float, OldValue,
                                               float, NewValue);

UCLASS(Abstract)
class OUT_OF_YOUR_ELEMENT_API UElementAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
};
