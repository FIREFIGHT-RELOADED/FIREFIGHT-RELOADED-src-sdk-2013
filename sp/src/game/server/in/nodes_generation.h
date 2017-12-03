//==== Woots 2016. http://creativecommons.org/licenses/by/2.5/mx/ ===========//

#ifndef NODES_GENERATION_H
#define NODES_GENERATION_H

#ifdef _WIN32
#pragma once
#endif

#define MAX_NAV_AREAS 9999
#define MAX_NODES_PER_AREA 60

class CNodeEnt;

//================================================================================
//================================================================================
class CNodesGeneration
{
public:
    virtual void Start();

    virtual void GenerateWalkableNodes();

protected:
    int m_iNavAreaCount;
    int m_iWalkableNodesCount;

    CUtlVector<Vector> m_WalkLocations;
};

#endif // NODES_GENERATION_H