#include "Dist_phyloCTMC.h"
#include "RlDistributionMemberFunction.h"
#include "PhyloCTMCSiteHomogeneous.h"
#include "FilteredPhyloCTMCSiteHomogeneous.h"
#include "PhyloCTMCSiteHomogeneousNucleotide.h"
#include "PhyloCTMCSiteHomogeneousRestriction.h"
#include "OptionRule.h"
#include "Probability.h"
#include "RevNullObject.h"
#include "RlAminoAcidState.h"
#include "RlBoolean.h"
#include "RlDnaState.h"
#include "RlRateGenerator.h"
#include "RlRnaState.h"
#include "RlString.h"
#include "RlTree.h"
#include "StandardState.h"
#include "RlSimplex.h"
#include "PomoState.h"
#include "NaturalNumbersState.h"

using namespace RevLanguage;

Dist_phyloCTMC::Dist_phyloCTMC() : TypedDistribution< AbstractHomologousDiscreteCharacterData >()
{

}


Dist_phyloCTMC::~Dist_phyloCTMC()
{

}



Dist_phyloCTMC* Dist_phyloCTMC::clone( void ) const
{

    return new Dist_phyloCTMC(*this);
}


RevBayesCore::TypedDistribution< RevBayesCore::AbstractHomologousDiscreteCharacterData >* Dist_phyloCTMC::createDistribution( void ) const
{

    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    size_t n = size_t( static_cast<const Natural &>( nSites->getRevObject() ).getValue() );
    const std::string& dt = static_cast<const RlString &>( type->getRevObject() ).getValue();
    bool ambig = static_cast<const RlBoolean &>( treatAmbiguousAsGap->getRevObject() ).getValue();
    size_t nNodes = tau->getValue().getNumberOfNodes();
    const std::string& code = static_cast<const RlString &>( coding->getRevObject() ).getValue();

    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* site_ratesNode = NULL;
    if ( site_rates != NULL && site_rates->getRevObject() != RevNullObject::getInstance() )
    {
        site_ratesNode = static_cast<const ModelVector<RealPos> &>( site_rates->getRevObject() ).getDagNode();
    }
    RevBayesCore::TypedDagNode< double >* p_invNode = NULL;
    if ( p_inv != NULL && p_inv->getRevObject() != RevNullObject::getInstance() )
    {
        p_invNode = static_cast<const Probability &>( p_inv->getRevObject() ).getDagNode();
    }

    RevBayesCore::TypedDistribution< RevBayesCore::AbstractHomologousDiscreteCharacterData > *d = NULL;
    const RevBayesCore::TypedDagNode< RevBayesCore::RbVector< double > > *rf = NULL;
    if ( root_frequencies->getRevObject() != RevNullObject::getInstance() )
    {
        rf = static_cast<const Simplex &>( root_frequencies->getRevObject() ).getDagNode();
    }

    if ( dt == "DNA" )
    {
        RevBayesCore::PhyloCTMCSiteHomogeneousNucleotide<RevBayesCore::DnaState> *dist =
        new RevBayesCore::PhyloCTMCSiteHomogeneousNucleotide<RevBayesCore::DnaState>(tau, true, n, ambig);

        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );

        // set the probability for invariant site (by default this p_inv=0.0)
        dist->setPInv( p_invNode );

        // set the clock rates
        if ( rate->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != clockRates->getValue().size() )
            {
                throw RbException( "The number of clock rates does not match the number of branches" );
            }
            
            dist->setClockRate( clockRates );
        }
        else
        {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
            dist->setClockRate( clockRate );
        }

        // set the rate matrix
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != rm->getValue().size() )
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }

            // sanity check
            if ( root_frequencies == NULL || root_frequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }

            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( site_ratesNode != NULL && site_ratesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( site_ratesNode );
        }

        d = dist;
    }
    else if ( dt == "RNA" )
    {
        RevBayesCore::PhyloCTMCSiteHomogeneousNucleotide<RevBayesCore::RnaState> *dist = new RevBayesCore::PhyloCTMCSiteHomogeneousNucleotide<RevBayesCore::RnaState>(tau, true, n, ambig);

        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );

        // set the probability for invariant site (by default this p_inv=0.0)
        dist->setPInv( p_invNode );

        if ( rate->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != clockRates->getValue().size() )
            {
                throw RbException( "The number of clock rates does not match the number of branches" );
            }

            dist->setClockRate( clockRates );
        }
        else
        {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
            dist->setClockRate( clockRate );
        }

        // set the rate matrix
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != rm->getValue().size() )
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            // sanity check
            if ( root_frequencies == NULL || root_frequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( site_ratesNode != NULL && site_ratesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( site_ratesNode );
        }

        d = dist;
    }
    else if ( dt == "AA" || dt == "Protein" )
    {
        RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::AminoAcidState> *dist = new RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::AminoAcidState>(tau, 20, true, n, ambig);

        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );

        // set the probability for invariant site (by default this p_inv=0.0)
        dist->setPInv( p_invNode );

        if ( rate->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != clockRates->getValue().size() )
            {
                throw RbException( "The number of clock rates does not match the number of branches" );
            }

            dist->setClockRate( clockRates );
        }
        else
        {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
            dist->setClockRate( clockRate );
        }

        // set the rate matrix
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != rm->getValue().size() )
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            // sanity check
            if ( root_frequencies == NULL || root_frequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( site_ratesNode != NULL && site_ratesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( site_ratesNode );
        }

        d = dist;
    }
    else if ( dt == "Pomo" )
    {

        // we get the number of states from the rate matrix (we don't know, because Pomo is flexible about its rates)
        // set the rate matrix
        size_t nChars = 1;
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue()[0].getNumberOfStates();
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue().getNumberOfStates();
        }

        RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::PomoState> *dist = new RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::PomoState>(tau, nChars, true, n, ambig);

        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );

        // set the probability for invariant site (by default this p_inv=0.0)
        dist->setPInv( p_invNode );

        if ( rate->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != clockRates->getValue().size() )
            {
                throw RbException( "The number of clock rates does not match the number of branches" );
            }

            dist->setClockRate( clockRates );
        }
        else
        {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
            dist->setClockRate( clockRate );
        }

        // set the rate matrix
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != rm->getValue().size() )
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            // sanity check
            if ( root_frequencies == NULL || root_frequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( site_ratesNode != NULL && site_ratesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( site_ratesNode );
        }

        d = dist;
    }
    else if ( dt == "Standard" )
    {

        
        // we get the number of states from the rates matrix
        // set the rate matrix
        size_t nChars = 1;
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue()[0].getNumberOfStates();
        }

        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue().getNumberOfStates();
        }

                /* need to modify this to a two-part else if: one if there is no statement of filtering. one if there is, in which case we cast to AbstractphyloCTMC and use the asc filters */
        RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<RevBayesCore::StandardState> *dist;
        
        if ( code == "variable" )
        {
            RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<RevBayesCore::StandardState, RevBayesCore::Tree> *dist_tmp = new RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<RevBayesCore::StandardState, RevBayesCore::Tree>(tau, nChars, true, n);
            dist = dynamic_cast< RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<RevBayesCore::StandardState, RevBayesCore::Tree>* >( dist_tmp );
            assert(dist != NULL);
        }
        else
        {
            dist = new RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::StandardState>(tau, nChars, true, n, ambig);
        }


        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );

        // set the probability for invariant site (by default this p_inv=0.0)
        dist->setPInv( p_invNode );

        if ( rate->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != clockRates->getValue().size() )
            {
                throw RbException( "The number of clock rates does not match the number of branches" );
            }

            dist->setClockRate( clockRates );
        }
        else
        {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
            dist->setClockRate( clockRate );
        }

        // set the rate matrix
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != rm->getValue().size() )
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            // sanity check
            if ( root_frequencies == NULL || root_frequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( site_ratesNode != NULL && site_ratesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( site_ratesNode );
        }

        
        d = dist;
    }
    else if ( dt == "NaturalNumbers" )
    {
        // we get the number of states from the rates matrix
        size_t n_chars = 1;
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();
            n_chars = rm->getValue()[0].getNumberOfStates();
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            n_chars = rm->getValue().getNumberOfStates();
        }

        RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::NaturalNumbersState> *dist = new RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::NaturalNumbersState>(tau, n_chars, true, n, ambig);

        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );

        // set the probability for invariant site (by default this p_inv=0.0)
        dist->setPInv( p_invNode );

        if ( rate->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != clockRates->getValue().size() )
            {
                throw RbException( "The number of clock rates does not match the number of branches" );
            }

            dist->setClockRate( clockRates );
        }
        else
        {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
            dist->setClockRate( clockRate );
        }

        // set the rate matrix
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != rm->getValue().size() )
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            // sanity check
            if ( root_frequencies == NULL || root_frequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( site_ratesNode != NULL && site_ratesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( site_ratesNode );
        }

        d = dist;
    }
    else if ( dt == "Restriction" )
    {
        // we get the number of states from the rates matrix
        // set the rate matrix
        size_t nChars = 1;
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue()[0].getNumberOfStates();
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue().getNumberOfStates();
        }

        // sanity check
        if ( nChars != 2 )
        {
            throw RbException( "Only binary characters allowed for type=Restriction" );
        }

        // split the coding option on "|"
        std::string s = code;
        std::vector<std::string> tokens;

        size_t pos = 0;
        while ((pos = s.find("|")) != std::string::npos)
        {
            tokens.push_back(s.substr(0, pos));
            s.erase(0, pos + 1);
        }
        tokens.push_back(s);

        // set the flags for each token
        int cd = RevBayesCore::RestrictionAscertainmentBias::ALL;
        for(size_t i = 0; i < tokens.size(); i++)
        {
            if(tokens[i] == "noabsencesites")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::NOABSENCESITES;
            }
            else if(tokens[i] == "nopresencesites")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::NOPRESENCESITES;
            }
            else if(tokens[i] == "informative")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::INFORMATIVE;
            }
            else if(tokens[i] == "variable")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::VARIABLE;
            }
            else if(tokens[i] == "nosingletonpresence")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::NOSINGLETONPRESENCE;
            }
            else if(tokens[i] == "nosingletonabsence")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::NOSINGLETONABSENCE;
            }
            else if(tokens[i] == "nosingletons")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::NOSINGLETONS;
            }
            else if(tokens[i] != "all")
            {
                std::stringstream ss;
                ss << "Unrecognized coding option \"" << tokens[i] << "\"\n";
                ss << "\tAvailable codings: all, noabsencesites, nopresencesites, informative, variable, nosingletonpresence, nosingletonabsence, nosingletons\n";
                ss << "\tDefault: all. Codings are combined using the vertical bar \'|\'\n";
                throw RbException(ss.str());
            }
        }

        RevBayesCore::PhyloCTMCSiteHomogeneousRestriction *dist = new RevBayesCore::PhyloCTMCSiteHomogeneousRestriction(tau, true, n, ambig, RevBayesCore::RestrictionAscertainmentBias::Coding(cd));

        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );

        // set the probability for invariant site (by default this p_inv=0.0)
        dist->setPInv( p_invNode );

        if ( rate->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != clockRates->getValue().size() )
            {
                throw RbException( "The number of clock rates does not match the number of branches" );
            }

            dist->setClockRate( clockRates );
        }
        else
        {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
            dist->setClockRate( clockRate );
        }

        // set the rate matrix
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();

            // sanity check
            if ( (nNodes-1) != rm->getValue().size() )
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            // sanity check
            if ( root_frequencies == NULL || root_frequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( site_ratesNode != NULL && site_ratesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( site_ratesNode );
        }

        d = dist;
    }



    return d;
}



/* Get Rev type of object */
const std::string& Dist_phyloCTMC::getClassType(void)
{

    static std::string revType = "Dist_phyloCTMC";

    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_phyloCTMC::getClassTypeSpec(void)
{

    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );

    return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_phyloCTMC::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "PhyloCTMC";
    
    return d_name;
}


MethodTable Dist_phyloCTMC::getDistributionMethods( void ) const
{
    
    MethodTable methods = TypedDistribution<AbstractHomologousDiscreteCharacterData>::getDistributionMethods();
    
    // member functions
    ArgumentRules* siteLikelihoodsArgRules = new ArgumentRules();
    methods.addFunction( new DistributionMemberFunction<Dist_phyloCTMC, ModelVector<Real> >( "siteLikelihoods", variable, siteLikelihoodsArgRules, true ) );
    
    return methods;
}


/** Return member rules (no members) */
const MemberRules& Dist_phyloCTMC::getParameterRules(void) const
{

    static MemberRules dist_member_rules;
    static bool rules_set = false;

    if ( !rules_set )
    {
        dist_member_rules.push_back( new ArgumentRule( "tree", Tree::getClassTypeSpec(), "The tree along which the process evolves.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        std::vector<TypeSpec> rateMatrixTypes;
        rateMatrixTypes.push_back( RateGenerator::getClassTypeSpec() );
        rateMatrixTypes.push_back( ModelVector<RateGenerator>::getClassTypeSpec() );
        dist_member_rules.push_back( new ArgumentRule( "Q", rateMatrixTypes, "The global or branch-specific rate matrices.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        // optional argument for the root frequencies
        dist_member_rules.push_back( new ArgumentRule( "rootFrequencies", Simplex::getClassTypeSpec(), "The root specific frequencies of the characters, if applicable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );

        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        dist_member_rules.push_back( new ArgumentRule( "branchRates", branchRateTypes, "The global or branch-specific rate multipliers.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );

        ModelVector<RealPos> *defaultSiteRates = new ModelVector<RealPos>();
        dist_member_rules.push_back( new ArgumentRule( "siteRates", ModelVector<RealPos>::getClassTypeSpec(), "The rate categories for the sites.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultSiteRates ) );
        dist_member_rules.push_back( new ArgumentRule( "pInv", Probability::getClassTypeSpec(), "The probability of a site being invariant.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Probability(0.0) ) );

        dist_member_rules.push_back( new ArgumentRule( "nSites", Natural::getClassTypeSpec(), "The number of sites, used for simulation.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10) ) );

        std::vector<std::string> options;
        options.push_back( "DNA" );
        options.push_back( "RNA" );
        options.push_back( "AA" );
        options.push_back( "Pomo" );
        options.push_back( "Protein" );
        options.push_back( "Standard" );
        options.push_back( "NaturalNumbers" );
        options.push_back( "Restriction" );
        dist_member_rules.push_back( new OptionRule( "type", new RlString("DNA"), options, "The data type, used for simulation and initialization." ) );

        dist_member_rules.push_back( new ArgumentRule( "treatAmbiguousAsGap", RlBoolean::getClassTypeSpec(), "Should we treat ambiguous characters as gaps/missing?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( false ) ) );

        dist_member_rules.push_back( new ArgumentRule("coding", RlString::getClassTypeSpec(), "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("all") ) );

        rules_set = true;
    }

    return dist_member_rules;
}


const TypeSpec& Dist_phyloCTMC::getTypeSpec( void ) const
{

    static TypeSpec ts = getClassTypeSpec();

    return ts;
}


/** Print value for user */
void Dist_phyloCTMC::printValue(std::ostream& o) const
{

    o << "Character-State-Evolution-Along-Tree Process(tree=";
    if ( tree != NULL )
    {
        o << tree->getName();
    }
    else
    {
        o << "?";
    }
    o << ", Q=";
    if ( q != NULL )
    {
        o << q->getName();
    }
    else
    {
        o << "?";
    }
    o << ", branchRates=";
    if ( rate != NULL )
    {
        o << rate->getName();
    }
    else
    {
        o << "?";
    }
    o << ", site_rates=";
    if ( site_rates != NULL )
    {
        o << site_rates->getName();
    }
    else
    {
        o << "?";
    }
    o << ", p_inv=";
    if ( p_inv != NULL )
    {
        o << p_inv->getName();
    }
    else
    {
        o << "?";
    }
    o << ", nSites=";
    if ( nSites != NULL )
    {
        o << nSites->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";

}


/** Set a member variable */
void Dist_phyloCTMC::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "Q" )
    {
        q = var;
    }
    else if ( name == "rootFrequencies" )
    {
        root_frequencies = var;
    }
    else if ( name == "branchRates" )
    {
        rate = var;
    }
    else if ( name == "siteRates" )
    {
        site_rates = var;
    }
    else if ( name == "pInv" )
    {
        p_inv = var;
    }
    else if ( name == "nSites" )
    {
        nSites = var;
    }
    else if ( name == "type" )
    {
        type = var;
    }
    else if ( name == "treatAmbiguousAsGap" )
    {
        treatAmbiguousAsGap = var;
    }
    else if ( name == "coding" )
    {
        coding = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }

}

