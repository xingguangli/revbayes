#ifndef PhyloCTMCSiteHomogeneous_H
#define PhyloCTMCSiteHomogeneous_H

#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDistribution.h"
#

namespace RevBayesCore {
    
    template<class charType>
    class PhyloCTMCSiteHomogeneous : public AbstractPhyloCTMCSiteHomogeneous<charType> {
        
    public:
        PhyloCTMCSiteHomogeneous(const TypedDagNode< Tree > *t, size_t nChars, bool c, size_t nSites, bool amb);
        virtual                                            ~PhyloCTMCSiteHomogeneous(void);                                                                   //!< Virtual destructor
        
        // public member functions
        PhyloCTMCSiteHomogeneous*                           clone(void) const;                                                                          //!< Create an independent clone
        
        
    protected:
        
        virtual void                                        computeRootLikelihood(size_t root, size_t l, size_t r);
        virtual void                                        computeRootLikelihood(size_t root, size_t l, size_t r, size_t m);
        virtual void                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
        virtual void                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m);
        virtual void                                        computeTipLikelihood(const TopologyNode &node, size_t nIdx);
        
        
    private:        

    
    
    };
    
}


#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "RateMatrix_JC.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>

template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneous<charType>::PhyloCTMCSiteHomogeneous(const TypedDagNode<Tree> *t, size_t nChars, bool c, size_t nSites, bool amb) : AbstractPhyloCTMCSiteHomogeneous<charType>(  t, nChars, 1, c, nSites, amb )
{
    
}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneous<charType>::~PhyloCTMCSiteHomogeneous( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneous<charType>* RevBayesCore::PhyloCTMCSiteHomogeneous<charType>::clone( void ) const
{
    
    return new PhyloCTMCSiteHomogeneous<charType>( *this );
}



template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneous<charType>::computeRootLikelihood( size_t root, size_t left, size_t right)
{
    
    // get the root frequencies
    const std::vector<double> &f                    = this->getRootFrequencies();
    std::vector<double>::const_iterator f_end       = f.end();
    std::vector<double>::const_iterator f_begin     = f.begin();
    
    // get the pointers to the partial likelihoods of the left and right subtree
          double* p        = this->partialLikelihoods + this->activeLikelihood[root]  * this->activeLikelihoodOffset + root  * this->nodeOffset;
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]  * this->activeLikelihoodOffset + left  * this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right] * this->activeLikelihoodOffset + right * this->nodeOffset;
    
    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(this->num_patterns,0.0);
    
    // get pointers the likelihood for both subtrees
          double*   p_mixture          = p;
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture) 
    {
        
        // get pointers to the likelihood for this mixture category
              double*   p_site_mixture          = p_mixture;
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        // iterate over all sites
        for (size_t site = 0; site < this->pattern_block_size; ++site)
        {
            // get the pointer to the stationary frequencies
            std::vector<double>::const_iterator f_j             = f_begin;
            // get the pointers to the likelihoods for this site and mixture category
                  double* p_site_j        = p_site_mixture;
            const double* p_site_left_j   = p_site_mixture_left;
            const double* p_site_right_j  = p_site_mixture_right;
            // iterate over all starting states
            for (; f_j != f_end; ++f_j) 
            {
                // add the probability of starting from this state
                *p_site_j = *p_site_left_j * *p_site_right_j * *f_j;
                
                // increment pointers
                ++p_site_j; ++p_site_left_j; ++p_site_right_j;
            }
            
            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset; p_site_mixture_left+=this->siteOffset; p_site_mixture_right+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset; p_mixture_left+=this->mixtureOffset; p_mixture_right+=this->mixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
    
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneous<charType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle)
{
    
    // get the root frequencies
    const std::vector<double> &f                    = this->getRootFrequencies();
    std::vector<double>::const_iterator f_end       = f.end();
    std::vector<double>::const_iterator f_begin     = f.begin();
    
    // get the pointers to the partial likelihoods of the left and right subtree
          double* p        = this->partialLikelihoods + this->activeLikelihood[root]   * this->activeLikelihoodOffset + root   * this->nodeOffset;
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]   * this->activeLikelihoodOffset + left   * this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right]  * this->activeLikelihoodOffset + right  * this->nodeOffset;
    const double* p_middle = this->partialLikelihoods + this->activeLikelihood[middle] * this->activeLikelihoodOffset + middle * this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
          double*   p_mixture          = p;
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    const double*   p_mixture_middle   = p_middle;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        
        // get pointers to the likelihood for this mixture category
              double*   p_site_mixture          = p_mixture;
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        const double*   p_site_mixture_middle   = p_mixture_middle;
        // iterate over all sites
        for (size_t site = 0; site < this->pattern_block_size; ++site)
        {
            
            // get the pointer to the stationary frequencies
            std::vector<double>::const_iterator f_j = f_begin;
            // get the pointers to the likelihoods for this site and mixture category
                  double* p_site_j        = p_site_mixture;
            const double* p_site_left_j   = p_site_mixture_left;
            const double* p_site_right_j  = p_site_mixture_right;
            const double* p_site_middle_j = p_site_mixture_middle;
            // iterate over all starting states
            for (; f_j != f_end; ++f_j)
            {
                // add the probability of starting from this state
                *p_site_j = *p_site_left_j * *p_site_right_j * *p_site_middle_j * *f_j;
                
                // increment pointers
                ++p_site_j; ++p_site_left_j; ++p_site_right_j; ++p_site_middle_j;
            }
            
            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset; p_site_mixture_left+=this->siteOffset; p_site_mixture_right+=this->siteOffset; p_site_mixture_middle+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset; p_mixture_left+=this->mixtureOffset; p_mixture_right+=this->mixtureOffset; p_mixture_middle+=this->mixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneous<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t node_index, size_t left, size_t right)
{   
    
    // compute the transition probability matrix
    this->updateTransitionProbabilities( node_index, node.getBranchLength() );
    
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[node_index]*this->activeLikelihoodOffset + node_index*this->nodeOffset;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*    tp_begin                = this->transition_prob_matrices[mixture].theMatrix;
        
        // get the pointers to the likelihood for this mixture category
        size_t offset = mixture*this->mixtureOffset;
        double*          p_site_mixture          = p_node + offset;
        const double*    p_site_mixture_left     = p_left + offset;
        const double*    p_site_mixture_right    = p_right + offset;
        // compute the per site probabilities
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {
            
            // get the pointers for this mixture category and this site
            const double*       tp_a    = tp_begin;
            // iterate over the possible starting states
            for (size_t c1 = 0; c1 < this->num_chars; ++c1) 
            {
                // temporary variable
                double sum = 0.0;
                
                // iterate over all possible terminal states
                for (size_t c2 = 0; c2 < this->num_chars; ++c2 ) 
                {
                    sum += p_site_mixture_left[c2] * p_site_mixture_right[c2] * tp_a[c2];
                    
                } // end-for over all distination character 
                
                // store the likelihood for this starting state
                p_site_mixture[c1] = sum;
                
                // increment the pointers to the next starting state
                tp_a+=this->num_chars;
                
            } // end-for over all initial characters
            
            // increment the pointers to the next site
            p_site_mixture_left+=this->siteOffset; p_site_mixture_right+=this->siteOffset; p_site_mixture+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
    } // end-for over all mixtures (=rate-categories)
    
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneous<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t node_index, size_t left, size_t right, size_t middle)
{
    
    // compute the transition probability matrix
    this->updateTransitionProbabilities( node_index, node.getBranchLength() );
    
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left      = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_middle    = this->partialLikelihoods + this->activeLikelihood[middle]*this->activeLikelihoodOffset + middle*this->nodeOffset;
    const double*   p_right     = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double*         p_node      = this->partialLikelihoods + this->activeLikelihood[node_index]*this->activeLikelihoodOffset + node_index*this->nodeOffset;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*    tp_begin                = this->transition_prob_matrices[mixture].theMatrix;
        
        // get the pointers to the likelihood for this mixture category
        size_t offset = mixture*this->mixtureOffset;
        double*          p_site_mixture          = p_node + offset;
        const double*    p_site_mixture_left     = p_left + offset;
        const double*    p_site_mixture_middle   = p_middle + offset;
        const double*    p_site_mixture_right    = p_right + offset;
        // compute the per site probabilities
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {
            
            // get the pointers for this mixture category and this site
            const double*       tp_a    = tp_begin;
            // iterate over the possible starting states
            for (size_t c1 = 0; c1 < this->num_chars; ++c1)
            {
                // temporary variable
                double sum = 0.0;
                
                // iterate over all possible terminal states
                for (size_t c2 = 0; c2 < this->num_chars; ++c2 )
                {
                    sum += p_site_mixture_left[c2] * p_site_mixture_middle[c2] * p_site_mixture_right[c2] * tp_a[c2];
                    
                } // end-for over all distination character
                
                // store the likelihood for this starting state
                p_site_mixture[c1] = sum;
                
                // increment the pointers to the next starting state
                tp_a+=this->num_chars;
                
            } // end-for over all initial characters
            
            // increment the pointers to the next site
            p_site_mixture_left+=this->siteOffset; p_site_mixture_middle+=this->siteOffset; p_site_mixture_right+=this->siteOffset; p_site_mixture+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
    } // end-for over all mixtures (=rate-categories)
    
}




template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneous<charType>::computeTipLikelihood(const TopologyNode &node, size_t node_index) 
{    
    
    double* p_node = this->partialLikelihoods + this->activeLikelihood[node_index]*this->activeLikelihoodOffset + node_index*this->nodeOffset;
    
    const std::vector<bool> &gap_node = this->gap_matrix[node_index];
    const std::vector<unsigned long> &char_node = this->char_matrix[node_index];
    const std::vector<RbBitSet> &amb_char_node = this->ambiguous_char_matrix[node_index];
    
    // compute the transition probabilities
    this->updateTransitionProbabilities( node_index, node.getBranchLength() );
    
    double*   p_mixture      = p_node;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*                       tp_begin    = this->transition_prob_matrices[mixture].theMatrix;
        
        // get the pointer to the likelihoods for this site and mixture category
        double*     p_site_mixture      = p_mixture;
        
        // iterate over all sites
        for (size_t site = 0; site != this->pattern_block_size; ++site) 
        {
            
            // is this site a gap?
            if ( gap_node[site] ) 
            {
                // since this is a gap we need to assume that the actual state could have been any state
                
                // iterate over all initial states for the transitions
                for (size_t c1 = 0; c1 < this->num_chars; ++c1) 
                {
                    
                    // store the likelihood
                    p_site_mixture[c1] = 1.0;
                    
                }
            } 
            else // we have observed a character
            {
                
                // iterate over all possible initial states
                for (size_t c1 = 0; c1 < this->num_chars; ++c1) 
                {
                    
                    if ( this->using_ambiguous_characters == true )
                    {
                        // compute the likelihood that we had a transition from state c1 to the observed state org_val
                        // note, the observed state could be ambiguous!
                        const RbBitSet &val = amb_char_node[site];
                        
                        // get the pointer to the transition probabilities for the terminal states
                        const double* d  = tp_begin+(this->num_chars*c1);
                        
                        double tmp = 0.0;
                        
                        for( size_t i=0; i<val.size(); ++i )
                        {
                            // check whether we observed this state
                            if ( val.isSet(i) == true )
                            {
                                // add the probability
                                tmp += *d;
                            }
                            
                            // increment the pointer to the next transition probability
                            ++d;
                        } // end-while over all observed states for this character
                        
                        // store the likelihood
                        p_site_mixture[c1] = tmp;
                         
                    }
                    else // no ambiguous characters in use
                    {
                        unsigned long org_val = char_node[site];
                        
                        // store the likelihood
                        p_site_mixture[c1] = tp_begin[c1*this->num_chars+org_val];
                        
                    }
                    
                } // end-for over all possible initial character for the branch
                
            } // end-if a gap state
            
            // increment the pointers to next site
            p_site_mixture+=this->siteOffset; 
            
        } // end-for over all sites/patterns in the sequence
        
        // increment the pointers to next mixture category
        p_mixture+=this->mixtureOffset;
        
    } // end-for over all mixture categories
    
}


#endif
