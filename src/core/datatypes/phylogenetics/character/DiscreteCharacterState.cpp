#include "DiscreteCharacterState.h"
#include "RbException.h"

using namespace RevBayesCore;



/** Constructor */
DiscreteCharacterState::DiscreteCharacterState(size_t n) : CharacterState(),
    num_observed_states( 0 ),
    index_single_state( 0 ),
    state(n)
{
    
    
}

/** Equals comparison */
bool DiscreteCharacterState::operator==(const CharacterState& x) const
{
    
    const DiscreteCharacterState* derivedX = dynamic_cast<const DiscreteCharacterState*>( &x );
    
    if (derivedX != NULL)
    {
        return derivedX->state == state;
    }
    
    return false;
}


/** Not equals comparison */
bool DiscreteCharacterState::operator!=(const CharacterState& x) const
{
    
    return !operator==(x);
}


bool DiscreteCharacterState::operator<(const CharacterState &x) const
{
    
    const DiscreteCharacterState* derivedX = static_cast<const DiscreteCharacterState*>(&x);
    if ( derivedX != NULL )
    {
        RbBitSet myState = state;
        RbBitSet yourState = derivedX->state;
        return ( myState < yourState );
    }
    
    return false;
}


void DiscreteCharacterState::operator++( void )
{
    
    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot increment an ambiguous character.");
    }
    
    // unset the current state
    state.unset( index_single_state );
    
    // incremement our state index;
    ++index_single_state;
    
    // now set the bit of the state
    state.set(index_single_state);
    
}


void DiscreteCharacterState::operator++( int i )
{
    
    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot increment an ambiguous character.");
    }
    
    // unset the current state
    state.unset( index_single_state );
    
    // incremement our state index;
    ++index_single_state;
    
    // now set the bit of the state
    state.set(index_single_state);
    
}

void DiscreteCharacterState::operator+=( int i )
{
    
    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot increment an ambiguous character.");
    }
    
    // unset the current state
    state.unset( index_single_state );
    
    // incremement our state index;
    index_single_state += i;
    
    // now set the bit of the state
    state.set(index_single_state);
    
}

void DiscreteCharacterState::operator--( void )
{
    
    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot decrement an ambiguous character.");
    }
    
    // unset the current state
    state.unset( index_single_state );
    
    // incremement our state index;
    --index_single_state;
    
    // now set the bit of the state
    state.set(index_single_state);
    
}


void DiscreteCharacterState::operator--( int i )
{
    
    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot decrement an ambiguous character.");
    }
    
    // unset the current state
    state.unset( index_single_state );
    
    // incremement our state index;
    --index_single_state;
    
    // now set the bit of the state
    state.set(index_single_state);
    
}

void DiscreteCharacterState::operator-=( int i )
{
    
    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot decrement an ambiguous character.");
    }
    
    // unset the current state
    state.unset( index_single_state );
    
    // incremement our state index;
    index_single_state -= i;
    
    // now set the bit of the state
    state.set(index_single_state);
    
}


void DiscreteCharacterState::addState(const std::string &symbol)
{
    ++num_observed_states;
    
    std::string labels = getStateLabels();
    size_t pos = labels.find(symbol);
    
    state.set( pos );
    index_single_state = pos;
}


size_t DiscreteCharacterState::getNumberObservedStates(void) const
{
    return num_observed_states;
}


size_t DiscreteCharacterState::getNumberOfStates(void) const
{
    return state.size();
}


const RbBitSet& DiscreteCharacterState::getState(void) const
{
    return state;
}


size_t DiscreteCharacterState::getStateIndex(void) const
{
    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot get the index of an ambiguous state.");
    }
    
    return index_single_state;
}



std::string DiscreteCharacterState::getStringValue(void) const
{
    
    if ( isMissingState() )
    {
        return "?";
    }
    
    if ( isGapState() )
    {
        return "-";
    }
    
    std::string tmp_val = "";
    std::string labels = getStateLabels();
    size_t size = labels.size();
    for (size_t i = 0; i < size; ++i)
    {
        bool isSet = state.isSet( i );
        if (isSet)
        {
            tmp_val += labels[i];
        }
    }
    
    std::string val = "";
    if ( tmp_val.size() > 1 )
    {
        val = "(" + tmp_val.substr(0,1);
        for ( size_t i = 1; i<tmp_val.size(); ++i )
        {
            val += " " + tmp_val.substr(i,1);
        }
        val += ")";
    }
    else
    {
        val = tmp_val;
    }
    
    return val;
}


bool DiscreteCharacterState::isAmbiguous( void ) const
{
    
    return isMissingState() || isGapState() || ( getNumberObservedStates() > 1 );
}


bool DiscreteCharacterState::isStateSet(size_t index) const
{
    return state.isSet(index);
}


void DiscreteCharacterState::setToFirstState(void)
{
    num_observed_states = 1;
    index_single_state = 0;
    state.clear();
    state.set( 0 );
}


void DiscreteCharacterState::setStateByIndex(size_t index)
{
    
    num_observed_states = 1;
    index_single_state = index;
    state.clear();
    state.set( index );
}



void DiscreteCharacterState::setState(const std::string &s)
{
    
    std::string labels = getStateLabels();
    
    num_observed_states = 0;
    state.clear();
    
    for (size_t i = 0; i < s.size(); i++)
    {
        ++num_observed_states;
        
        size_t pos = labels.find(s[i]);
        state.set(pos);
        index_single_state = pos;
    }
    
}
