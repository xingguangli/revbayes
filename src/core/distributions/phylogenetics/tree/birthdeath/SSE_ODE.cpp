

#include <cmath>
#include "SSE_ODE.h"

using namespace RevBayesCore;


SSE_ODE::SSE_ODE( const std::vector<double> &m, const RateGenerator* q, double r, bool ext ) :
    mu( m ),
    num_states( q->getNumberOfStates() ),
    Q( q ),
    rate( r ),
    extinction_only( ext ),
    use_backward( true )
{
    
}


void SSE_ODE::operator()(const state_type &x, state_type &dxdt, const double t)
{
    // ClaSSE equations A1 and A2 from Goldberg and Igic, 2012
    
    double age = 0.0;
    for (size_t i = 0; i < num_states; ++i)
    {
        
        // calculate sum of speciation rates
        // lambda_ijk for all possible values of j and k
        double lambda_sum = 0.0;
        
        if ( use_speciation_from_event_map == true )
        {
            std::map<std::vector<unsigned>, double>::iterator it;
            for (it = event_map.begin(); it != event_map.end(); it++)
            {
                const std::vector<unsigned>& states = it->first;
                double curr_lambda = it->second;
            
                if (i == states[0])
                {
                    lambda_sum += curr_lambda;
                }
            }
        }
        else
        {
            lambda_sum = lambda[i];
        }
        
        /**** Extinction ****/
        /**** equation A2 ***/
        
        // extinction event
        dxdt[i] = mu[i];
        
        // no event
        double no_event_rate = mu[i] + lambda_sum;
        for (size_t j=0; j<num_states; ++j)
        {
            if ( i != j )
            {
                no_event_rate += Q->getRate(i, j, age, rate);
            }
        }
        dxdt[i] -= no_event_rate * x[i];
        
        // speciation event
        if ( use_speciation_from_event_map == true )
        {
            std::map<std::vector<unsigned>, double>::iterator it;
            for (it = event_map.begin(); it != event_map.end(); it++)
            {
                const std::vector<unsigned>& states = it->first;
                double curr_lambda = it->second;
                if (i == states[0])
                {
                    dxdt[i] += curr_lambda * x[states[1]] * x[states[2]];
                }
            }
        }
        else
        {
            dxdt[i] += lambda[i]*x[i]*x[i];
        }
        
        // anagenetic state change
        for (size_t j = 0; j < num_states; ++j)
        {
            if ( i != j )
            {
                if ( use_backward == true )
                {
                    dxdt[i] += Q->getRate(i, j, age, rate) * x[j];
                }
                else
                {
                    dxdt[i] += Q->getRate(j, i, age, rate) * x[j];
                }
            }
        
        }

        if ( use_backward == false )
        {
            dxdt[i] = -dxdt[i];
        }
        
        if ( extinction_only == false )
        {
            /**** Observation ****/
            /**** equation A1 ****/
        
            // no event
            dxdt[i + num_states] = -no_event_rate * x[i + num_states];
        
            // speciation event
            if ( use_speciation_from_event_map == true )
            {
                std::map<std::vector<unsigned>, double>::iterator it;
                for (it = event_map.begin(); it != event_map.end(); it++)
                {
                    const std::vector<unsigned>& states = it->first;
                    double lambda = it->second;
                    if (i == states[0])
                    {
                        double term1 = x[states[1] + num_states] * x[states[2]];
                        double term2 = x[states[2] + num_states] * x[states[1]];
                        dxdt[i + num_states] += lambda * (term1 + term2 );
                    }
                }
            }
            else
            {
                dxdt[i+num_states] += 2*lambda[i]*x[i]*x[i+num_states];
            }
        
            // anagenetic state change
            for (size_t j = 0; j < num_states; ++j)
            {
                if ( i != j )
                {
                    if ( use_backward == true )
                    {
                        dxdt[i + num_states] += Q->getRate(i, j, age, rate) * x[j + num_states];
                    }
                    else
                    {
                        dxdt[i + num_states] += Q->getRate(j, i, age, rate) * x[j + num_states];
                    }
                }
                
            }
            
        } // end if extinction_only
        
    } // end for num_states
    
}


void SSE_ODE::setEventMap( const std::map<std::vector<unsigned>, double> &e )
{
    
    use_speciation_from_event_map = true;
    event_map = e;
}


void SSE_ODE::setSpeciationRate( const std::vector<double> &s )
{
    
    use_speciation_from_event_map = false;
    lambda = s;
    
}

