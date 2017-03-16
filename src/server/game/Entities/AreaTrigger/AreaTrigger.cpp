/*

 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ObjectAccessor.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "Unit.h"
#include "SpellInfo.h"
#include "Log.h"
#include "AreaTrigger.h"
#include "ScriptMgr.h"
#include "AreaTriggerScript.h"

AreaTrigger::AreaTrigger() : WorldObject(false), m_MaxDuration(0), m_Duration(0), m_Caster(NULL), m_Script(nullptr)
{
    m_objectType  |= TYPEMASK_AREATRIGGER;
    m_objectTypeId = TYPEID_AREATRIGGER;

    m_updateFlag   = UPDATEFLAG_STATIONARY_POSITION | UPDATEFLAG_AREATRIGGER;

    m_valuesCount  = AREATRIGGER_END;
    m_CreatedTime  = 0;
    m_Flags        = 0;

    m_Trajectory   = AREATRIGGER_INTERPOLATION_NONE;
    m_Templates.clear();
}

AreaTrigger::~AreaTrigger()
{
    ASSERT(!m_Caster);
}

void AreaTrigger::AddToWorld()
{
    ///- Register the AreaTrigger for guid lookup and for caster
    if (!IsInWorld())
    {
        GetMap()->GetObjectsStore().Insert<AreaTrigger>(GetGUID(), this);
        WorldObject::AddToWorld();
        BindToCaster();
    }
}

void AreaTrigger::RemoveFromWorld()
{
    ///- Remove the AreaTrigger from the accessor and from all lists of objects in world
    if (IsInWorld())
    {
        UnbindFromCaster();
        WorldObject::RemoveFromWorld();
        GetMap()->GetObjectsStore().Remove<AreaTrigger>(GetGUID());
    }
}

bool AreaTrigger::CreateAreaTriggerFromSpell(uint32 p_GuidLow, Unit* p_Caster, SpellInfo const* p_SpellInfo, uint32 p_EffIndex, Position const& p_Pos, Position const& p_Dest)
{
    SetMap(p_Caster->GetMap());
    Relocate(p_Pos);

    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_GENERAL, "AreaTrigger (spell %u) not created. Invalid coordinates (X: %f Y: %f)", p_SpellInfo->Id, GetPositionX(), GetPositionY());
        return false;
    }

    WorldObject::_Create(p_GuidLow, HighGuid::AreaTrigger, p_Caster->GetPhaseMask());

    m_CreatedTime = getMSTime();

    const AreaTriggerTemplateList* l_Templates = sObjectMgr->GetAreaTriggerTemplatesForSpell(p_SpellInfo->Id);
    if (l_Templates != nullptr)
    {
        for (AreaTriggerTemplateList::const_iterator l_Itr = l_Templates->begin(); l_Itr != l_Templates->end(); l_Itr++)
        {
            const AreaTriggerTemplate& l_AreaTriggerTemplate = *l_Itr;
            if (l_AreaTriggerTemplate.m_EffIndex == p_EffIndex)
                m_Templates.push_back(l_AreaTriggerTemplate);
        }
    }
    else
    {
        // Create default template
        AreaTriggerTemplate l_DefaultAreaTriggerTemplate;
        l_DefaultAreaTriggerTemplate.m_Entry         = p_SpellInfo->Id;
        l_DefaultAreaTriggerTemplate.m_CheckRadius   = 0.5f;
        l_DefaultAreaTriggerTemplate.m_SelectionMode = 0;

        m_Templates.push_back(l_DefaultAreaTriggerTemplate);
    }

    const AreaTriggerTemplate* l_MainTemplate = GetMainTemplate();
    if (l_MainTemplate == nullptr)
        return false;

    m_Flags = l_MainTemplate->m_Flags;

    if (m_Flags & AREATRIGGER_FLAG_ATTACHED)
    {
        m_updateFlag |= UPDATEFLAG_GO_TRANSPORT_POSITION;
        m_movementInfo.transport.guid = p_Caster->GetGUID();
        m_movementInfo.transport.seat = 0;
    }

    SetEntry(l_MainTemplate->m_Entry);
    SetMaxDuration(p_SpellInfo->GetDuration());
    SetDuration(p_SpellInfo->GetDuration());
    SetObjectScale(1);

    SetUInt64Value(AREATRIGGER_CASTER, p_Caster->GetGUID());
    SetUInt32Value(AREATRIGGER_SPELLID, p_SpellInfo->Id);
    SetUInt32Value(AREATRIGGER_SPELLVISUALID, p_SpellInfo->SpellVisual[0]);

    SetSource(p_Pos);
    SetDestination(p_Dest);

    SetTrajectory(p_Pos.ToString() != p_Dest.ToString() ? AREATRIGGER_INTERPOLATION_LINEAR : AREATRIGGER_INTERPOLATION_NONE);
    SetUpdateTimerInterval(60);

    SetUInt32Value(AREATRIGGER_DURATION, p_SpellInfo->GetDuration() != -1 ? p_SpellInfo->GetDuration() : HOUR * IN_MILLISECONDS);

    SetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE, GetFloatValue(OBJECT_FIELD_SCALE_X));

    SetSelectionMode(l_MainTemplate->m_SelectionMode);

    // do it before AddToMap, so we can change object before it creates
    // maybe we should make special script event 'OnInitialize'?
    sScriptMgr->OnSpellAreaTriggerCreate(this);
    
    if (!GetMap()->AddToMap(this))
        return false;

    sScriptMgr->OnCreateAreaTriggerEntity(this);

    return true;
}

bool AreaTrigger::CreateAreaTrigger(uint32 p_Entry, uint32 p_GuidLow, uint32 p_PhaseMask, uint32 p_SpellVisualID, Position const& p_Pos, uint32 p_Duration, Map* p_Map)
{
    ASSERT(p_Map != nullptr);

    SetMap(p_Map);
    Relocate(p_Pos);

    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_GENERAL, "AreaTrigger (entry %u) not created. Invalid coordinates (X: %f Y: %f)", p_Entry, GetPositionX(), GetPositionY());
        return false;
    }

    WorldObject::_Create(p_GuidLow, HighGuid::AreaTrigger, p_PhaseMask);

    const AreaTriggerTemplateList* l_Templates = sObjectMgr->GetAreaTriggerTemplatesForEntry(p_Entry);
    if (l_Templates != nullptr)
    {
        for (AreaTriggerTemplateList::const_iterator l_Itr = l_Templates->begin(); l_Itr != l_Templates->end(); l_Itr++)
            m_Templates.push_back(*l_Itr);
    }
    else
    {
        // Create default template
        AreaTriggerTemplate l_DefaultAreaTriggerTemplate;
        l_DefaultAreaTriggerTemplate.m_Entry = p_Entry;

        m_Templates.push_back(l_DefaultAreaTriggerTemplate);
    }

    const AreaTriggerTemplate* l_MainTemplate = GetMainTemplate();
    if (l_MainTemplate == nullptr)
        return false;

    m_Flags = l_MainTemplate->m_Flags;

    SetEntry(p_Entry);
    SetMaxDuration(p_Duration);
    SetDuration(p_Duration);
    SetObjectScale(1);

    SetUInt32Value(AREATRIGGER_SPELLVISUALID, p_SpellVisualID);

    SetSource(p_Pos);
    SetTrajectory(AREATRIGGER_INTERPOLATION_NONE);
    SetUpdateTimerInterval(60);

    if (p_Duration != -1)
        SetUInt32Value(AREATRIGGER_DURATION, p_Duration);

    SetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE, GetFloatValue(OBJECT_FIELD_SCALE_X));

    sScriptMgr->OnSpellAreaTriggerCreate(this);

    if (!GetMap()->AddToMap(this))
        return false;

    sScriptMgr->OnCreateAreaTriggerEntity(this);

    return true;
}

void AreaTrigger::Update(uint32 p_Time)
{
    if (GetDuration() != -1)
    {
        if (GetDuration() > int32(p_Time))
            m_Duration -= p_Time;
        else
            Remove(true); // expired
    }

    WorldObject::Update(p_Time);

    SpellInfo const* m_spellInfo = sSpellMgr->GetSpellInfo(GetUInt32Value(AREATRIGGER_SPELLID));
    if (!m_spellInfo)
        return;

    /// Must be done before...
    if (!GetCaster())
    {
        Remove(false);
        return;
    }

    if (const AreaTriggerTemplate* l_MainTemplate = GetMainTemplate())
        UpdateTargets(l_MainTemplate->m_CheckRadius);

    Unit* l_Caster = GetCaster();
    float l_Radius = 0.0f;

    sScriptMgr->OnSpellAreaTriggerUpdate(this);
    sScriptMgr->OnUpdateAreaTriggerEntity(this, p_Time);

    m_UpdateTimer.Update(p_Time);

    // ... and after the update, just in case it would be modified in a script
    if (!GetCaster())
    {
        Remove(false);
        return;
    }

    if (m_UpdateTimer.Passed())
    {
        m_UpdateTimer.Reset();

        // Calculate new position
        if (GetMainTemplate()->m_MoveCurveID != 0)
        {
            UpdatePositionWithPathId(m_CreatedTime, this);
        }
        else if (m_Trajectory)
        {
            //uint32 currentTimePos = GetMaxDuration() * ((GetDuration() / GetUpdateInterval()) / (GetMaxDuration() / GetUpdateInterval()));
            uint32 currentTimePos = GetMaxDuration() - GetDuration();
            GetPositionAtTime(currentTimePos/*m_CreatedTime*/, this);
        }
        else if (GetMainTemplate()->HasAttached())
        {
            m_positionX = m_Caster->m_positionX;
            m_positionY = m_Caster->m_positionY;
            m_positionZ = m_Caster->m_positionZ;
            m_orientation = m_Caster->m_orientation;
        }
    }

    if (m_Flags & AREATRIGGER_FLAG_ATTACHED)
        UpdatePositionToCaster();
}

void AreaTrigger::Remove(bool byExpire)
{
    if (IsInWorld())
    {
        for (std::set<uint64>::iterator l_Itr = m_targets.begin(); l_Itr != m_targets.end(); ++l_Itr)
        {
            Unit* l_Target = sObjectAccessor->GetUnit(*this, (*l_Itr));
            if (l_Target)
            {
                sScriptMgr->OnSpellAreaTriggerRemoveTarget(this, l_Target);
            }
        }

        SpellInfo const* m_spellInfo = sSpellMgr->GetSpellInfo(GetUInt32Value(AREATRIGGER_SPELLID));
        if (!m_spellInfo)
            return;

        switch (m_spellInfo->Id)
        {
            case 115460: // Healing sphere
            case 147494: // healing sphere (Glyph of Fortuitous Sphere)
            {
                if (!byExpire)
                    break;

                if (!m_Caster)
                    break;

                m_Caster->CastSpell(GetPositionX(), GetPositionY(), GetPositionZ(), 135914, true);
                break;
            }
            case 119031: // healing sphere
            {
                if (!byExpire)
                    break;

                if (!m_Caster)
                    break;

                m_Caster->CastSpell(GetPositionX(), GetPositionY(), GetPositionZ(), 135920, true);
                break;
            }
            case 116011:// Rune of Power : Remove the buff if caster is still in radius
                if (m_Caster && m_Caster->HasAura(116014))
                    m_Caster->RemoveAura(116014);
                break;
            case 122731:// Create Noise Cancelling Area Trigger
            {
                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, 200.0f);

                for (auto player : playerList)
                    if (player->HasAura(122706))
                        player->RemoveAura(122706);
                break;
            }
            case 123461:// Get Away!
            {
                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, 100.0f);

                Position pos;
                GetPosition(&pos);

                for (auto player : playerList)
                    player->GetForcedMovement().Stop();

                break;
            }
            case 134370:// Down Draft
            {
                Position pos;
                GetPosition(&pos);

                /// Getting players from caster if possible
                if (m_Caster)
                {
                    Map::PlayerList const &l_PlayerList = m_Caster->GetMap()->GetPlayers();

                    for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                    {
                        if (Player* l_Player = l_Itr->getSource())
                            l_Player->GetForcedMovement().Stop();
                    }
                }
                /// Else, getting players around the areatrigger
                else
                {
                    std::list<Player*> l_PlayerList;
                    GetPlayerListInGrid(l_PlayerList, 300.0f);

                    for (Player* l_Player : l_PlayerList)
                        l_Player->GetForcedMovement().Stop();
                }

                break;
            }
            default:
                break;
        }

        sScriptMgr->OnSpellAreaTriggerRemove(this, byExpire);
        sScriptMgr->OnRemoveAreaTriggerEntity(this, byExpire);

        SendObjectDeSpawnAnim(GetGUID());
        RemoveFromWorld();
        AddObjectToRemoveList();
    }
}

void AreaTrigger::BindToCaster()
{
    //ASSERT(!m_caster);
    m_Caster = ObjectAccessor::GetUnit(*this, GetCasterGUID());
    //ASSERT(GetCaster());
    //ASSERT(GetCaster()->GetMap() == GetMap());
    if (m_Caster)
        m_Caster->_RegisterAreaTrigger(this);
}

void AreaTrigger::UnbindFromCaster()
{
    ASSERT(m_Caster);
    m_Caster->_UnregisterAreaTrigger(this);
    m_Caster = NULL;
}

void AreaTrigger::SendMovementUpdate()
{
    WorldPacket l_Data(SMSG_AREA_TRIGGER_MOVEMENT_UPDATE, 24);

    // startX
    // startY
    // endY
    // endZ
    // startZ
    // endX
}

void AreaTrigger::UpdatePositionWithPathId(uint32 p_Time, Position* p_OutPos)
{
    const AreaTriggerTemplate* l_template = GetMainTemplate();

    if (l_template->m_Flags & AREATRIGGER_FLAG_HAS_MOVE_CURVE)
    {
        AreaTriggerMoveTemplate l_MoveTemplate = sObjectMgr->GetAreaTriggerMoveTemplate(l_template->m_MoveCurveID);
        if (l_MoveTemplate.m_path_size == 0)
        {
            sLog->outError(LOG_FILTER_GENERAL, "AreaTrigger Move Template (entry %u) not in DB.", l_template->m_MoveCurveID);
            return; // ERROR.
        }

        int32 l_LocalDuration = l_MoveTemplate.m_Duration / l_MoveTemplate.m_path_size;
        float l_Progress = float(p_Time % l_LocalDuration) / l_LocalDuration;
        uint32 l_PathId = float(p_Time) / l_LocalDuration;

        // We want to interpolate so that to get true position on server side.
        AreaTriggerMoveSplines l_spline0 = sObjectMgr->GetAreaTriggerMoveSplines(l_template->m_MoveCurveID, l_PathId);
        AreaTriggerMoveSplines l_spline1 = sObjectMgr->GetAreaTriggerMoveSplines(l_template->m_MoveCurveID, l_PathId + 1);

        if (l_spline0.m_move_id == 0 || l_spline1.m_move_id == 0)
        {
            sLog->outError(LOG_FILTER_GENERAL, "AreaTrigger Move Splines (entry %u) not in DB.", l_template->m_MoveCurveID);
            return; // ERROR.
        }

        float l_Cos = cos(m_Source.m_orientation - M_PI / 2);
        float l_Sin = sin(m_Source.m_orientation - M_PI / 2);
        float l_X = l_spline0.m_path_x + l_Progress * (l_spline1.m_path_x - l_spline0.m_path_x);
        float l_Y = l_spline0.m_path_y + l_Progress * (l_spline1.m_path_y - l_spline0.m_path_y);

        p_OutPos->m_positionX = m_Source.m_positionX + l_X * l_Cos - l_Y * l_Sin;
        p_OutPos->m_positionY = m_Source.m_positionY + l_X * l_Sin + l_Y * l_Cos;
        p_OutPos->m_positionZ = m_Source.m_positionZ + l_spline0.m_path_z + l_Progress * (l_spline1.m_path_z - l_spline0.m_path_z);
        p_OutPos->m_orientation = m_Source.m_orientation;
    }
}

void AreaTrigger::GetPositionFromPathId(uint32 p_PathId, Position* p_OutPos) const
{
    const AreaTriggerTemplate* l_template = GetMainTemplate();

    if (l_template->m_Flags & AREATRIGGER_FLAG_HAS_MOVE_CURVE)
    {
        AreaTriggerMoveSplines l_spline = sObjectMgr->GetAreaTriggerMoveSplines(l_template->m_MoveCurveID, p_PathId);

        if (l_spline.m_move_id == 0)
        {
            sLog->outError(LOG_FILTER_GENERAL, "AreaTrigger Move Splines (entry %u) not in DB.", l_template->m_MoveCurveID);
            return; // ERROR.
        }

        float l_Cos = cos(m_Source.m_orientation - M_PI / 2);
        float l_Sin = sin(m_Source.m_orientation - M_PI / 2);

        p_OutPos->m_positionX = m_Source.m_positionX + l_spline.m_path_x * l_Cos - l_spline.m_path_y * l_Sin;
        p_OutPos->m_positionY = m_Source.m_positionY + l_spline.m_path_x * l_Sin + l_spline.m_path_y * l_Cos;
        p_OutPos->m_positionZ = m_Source.m_positionZ + l_spline.m_path_z;
        p_OutPos->m_orientation = m_Source.m_orientation;
    }
}

void AreaTrigger::GetPositionAtTime(uint32 p_Time, Position* p_OutPos) const
{
    switch (m_Trajectory)
    {
    case AREATRIGGER_INTERPOLATION_LINEAR:
    {

        int32 l_Duration = GetDuration();
        //float l_Progress = l_Duration ? float(p_Time % l_Duration) / l_Duration : 0.f;
        float l_Progress = l_Duration ? float(p_Time % GetMaxDuration()) / GetMaxDuration() : 0.f;
        float angle = m_Source.GetOrientation();
        float dist = m_Source.GetExactDist(&m_Destination);

        p_OutPos->m_positionX = m_Source.m_positionX + l_Progress * dist * std::cos(angle);
        p_OutPos->m_positionY = m_Source.m_positionY + l_Progress * dist * std::sin(angle);
        p_OutPos->m_positionZ = m_Source.m_positionZ + l_Progress * (m_Destination.m_positionZ - m_Source.m_positionZ);
        p_OutPos->m_orientation = m_Source.m_orientation + l_Progress * (m_Destination.m_orientation - m_Source.m_orientation);
        
        /*
        p_OutPos->m_positionX = m_Source.m_positionX + l_Progress * (m_Destination.m_positionX - m_Source.m_positionX);
        p_OutPos->m_positionY = m_Source.m_positionY + l_Progress * (m_Destination.m_positionY - m_Source.m_positionY);
        p_OutPos->m_positionZ = m_Source.m_positionZ + l_Progress * (m_Destination.m_positionZ - m_Source.m_positionZ);
        p_OutPos->m_orientation = m_Source.m_orientation + l_Progress * (m_Destination.m_orientation - m_Source.m_orientation);
        */
        break;
    }
    default:
        *p_OutPos = m_Source;
        break;
    }
}

void AreaTrigger::UpdatePositionToCaster()
{
    if (!m_Caster)
        return;

    Position l_CasterPos;
    m_Caster->GetPosition(&l_CasterPos);
    Relocate(l_CasterPos);
}

/// Method to check if a Coordinate is located in a polygon
bool AreaTrigger::IsInAreaTriggerPolygon(std::vector<G3D::Vector2> p_Polygon, G3D::Vector2 p_Point, float p_Radius/* = 0.5f*/) const
{
    /// This method uses the ray tracing algorithm to determine if the point is in the polygon
    int l_PointCount = p_Polygon.size();
    int l_J = -999;
    int l_I = -999;
    bool l_IsInPolygon = false;

    for (l_I = 0; l_I < l_PointCount; l_I++)
    {
        /// Repeat loop for all sets of points
        /// If i is the last vertex, let j be the first vertex
        if (l_I == (l_PointCount - 1))
            l_J = 0;
        /// For all-else, let j=(i+1)th vertex
        else
            l_J = l_I + 1;

        float l_VerticeY_I = p_Polygon[l_I].y;
        float l_VerticeX_I = p_Polygon[l_I].x;
        float l_VerticeY_J = p_Polygon[l_J].y;
        float l_VerticeX_J = p_Polygon[l_J].x;
        float l_TestX = p_Point.x;
        float l_TestY = p_Point.y;

        /// Following statement checks if testPoint.Y is below Y-coord of i-th vertex. NB: Y-axis is inverted
        bool l_BelowLowY = l_VerticeY_I < l_TestY + p_Radius;

        /// Following statement checks if testPoint.Y is below Y-coord of i+1-th vertex
        bool l_BelowHighY = l_VerticeY_J < l_TestY + p_Radius;

        /// Following statement is true if testPoint.Y satisfies either (only one is possible)
        /// -->(i).Y < testPoint.Y < (i+1).Y        OR
        /// -->(i).Y > testPoint.Y > (i+1).Y
        ///
        /// (Note)
        /// Both of the conditions indicate that a point is located within the edges of the Y-th coordinate
        /// of the (i)-th and the (i+1)- th vertices of the polygon. If neither of the above
        /// conditions is satisfied, then it is assured that a semi-infinite horizontal line draw
        /// to the right from the testpoint will NOT cross the line that connects vertices i and i+1
        /// of the polygon

        bool l_WithinYsEdges = l_BelowLowY != l_BelowHighY;

        if (l_WithinYsEdges)
        {
            /// This is the slope of the line that connects vertices i and i+1 of the polygon
            float l_sLopeOfLine = (l_VerticeX_J - l_VerticeX_I) / (l_VerticeY_J - l_VerticeY_I);

            /// This looks up the x-coord of a point lying on the above line, given its y-coord
            float l_PointOnLine = (l_sLopeOfLine * (l_VerticeY_I - l_TestY)) + l_VerticeX_I;

            /// Checks to see if x-coord of testPoint is smaller than the point on the line with the same y-coord
            bool l_IsLeftToLine = l_TestX - p_Radius < l_PointOnLine;

            /// This statement changes true to false (and vice-versa)
            if (l_IsLeftToLine)
                l_IsInPolygon = !l_IsInPolygon;
        }
    }

    return l_IsInPolygon;
}

void AreaTrigger::UpdateTargets(float p_Radius)
{
    // Remove not valid targets
    if (m_targets.size() > 0)
    {
        for (std::set<uint64>::iterator l_Itr = m_targets.begin(); l_Itr != m_targets.end();)
        {
            Unit* l_Target = sObjectAccessor->GetUnit(*this, (*l_Itr));
            if (!l_Target)
            {
                l_Itr = m_targets.erase(l_Itr);
                continue;
            }

            if (!l_Target->IsInWorld() || !l_Target->isAlive() || GetDistance(l_Target) > p_Radius)
            {
                if (!sScriptMgr->OnSpellAreaTriggerRemoveTarget(this, l_Target))
                    continue;

                l_Itr = m_targets.erase(l_Itr);
                continue;
            }

            ++l_Itr;
        }
    }

    const AreaTriggerTemplate* l_MainTemplate = GetMainTemplate();

    // Add new targets
    if (!GetCaster() || !l_MainTemplate)
        return;

    std::list<Unit*> l_Newtargets;

    switch (GetSelectionMode())
    {
        case SELECTION_FRIENDLY:
        {
            if (l_MainTemplate->m_Type == AREATRIGGER_TYPE_POLYGON)
            {
                AreaTriggerTemplateList const& l_Templates = GetTemplates();
                std::vector<G3D::Vector2> l_CheckPoints;

                JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(this, GetCaster(), p_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(this, l_Newtargets, l_Check);
                VisitNearbyObject(p_Radius, l_Searcher);

                float l_X = GetPositionX();
                float l_Y = GetPositionY();
                float l_Z = GetPositionZ();
                float l_O = GetOrientation();

                Position l_Pos = { l_X, l_Y, l_Z, 0.0f };

                for (auto l_Template : l_Templates)
                {
                    /// Absolute position according to the current position and the template
                    float l_AbsX = l_X + l_Template.m_PolygonDatas.m_Vertices[0];
                    float l_AbsY = l_Y + l_Template.m_PolygonDatas.m_Vertices[1];

                    /// Calculating the distance between the origin and the absolute position, and the angle between them
                    Position l_VirtualPos = { l_AbsX, l_AbsY, l_Z, 0.0f };
                    float l_Dist = l_Pos.GetExactDist2d(&l_VirtualPos);
                    float l_Angle = l_Pos.GetAngle(&l_VirtualPos);

                    /// Reporting the distance and the angle according to the current orientation
                    float l_NewX = l_X + l_Dist * cos(l_O - l_Angle + M_PI / 2);
                    float l_NewY = l_Y + l_Dist * sin(l_O - l_Angle + M_PI / 2);

                    /// Adding the point to the vector
                    G3D::Vector2 l_Point = G3D::Vector2(l_NewX, l_NewY);
                    l_CheckPoints.push_back(l_Point);
                }
            }
            else
            {
                JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(this, GetCaster(), p_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, l_Newtargets, u_check);
                VisitNearbyObject(p_Radius, searcher);
            }
            break;
        }
        case SELECTION_HOSTILE:
        {
            if (l_MainTemplate->m_Type == AREATRIGGER_TYPE_POLYGON)
            {
                AreaTriggerTemplateList const& l_Templates = GetTemplates();
                std::vector<G3D::Vector2> l_CheckPoints;

                JadeCore::AnyUnfriendlyNoTotemUnitInObjectRangeCheck l_Check(this, GetCaster(), p_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyNoTotemUnitInObjectRangeCheck> l_Searcher(this, l_Newtargets, l_Check);
                VisitNearbyObject(p_Radius, l_Searcher);

                float l_X = GetPositionX();
                float l_Y = GetPositionY();
                float l_Z = GetPositionZ();
                float l_O = GetOrientation();

                Position l_Pos = { l_X, l_Y, l_Z, 0.0f };

                for (auto l_Template : l_Templates)
                {
                    /// Absolute position according to the current position and the template
                    float l_AbsX = l_X + l_Template.m_PolygonDatas.m_Vertices[0];
                    float l_AbsY = l_Y + l_Template.m_PolygonDatas.m_Vertices[1];

                    /// Calculating the distance between the origin and the absolute position, and the angle between them
                    Position l_VirtualPos = { l_AbsX, l_AbsY, l_Z, 0.0f };
                    float l_Dist = l_Pos.GetExactDist2d(&l_VirtualPos);
                    float l_Angle = l_Pos.GetAngle(&l_VirtualPos);

                    /// Reporting the distance and the angle according to the current orientation
                    float l_NewX = l_X + l_Dist * cos(l_O - l_Angle + M_PI / 2);
                    float l_NewY = l_Y + l_Dist * sin(l_O - l_Angle + M_PI / 2);

                    /// Adding the point to the vector
                    G3D::Vector2 l_Point = G3D::Vector2(l_NewX, l_NewY);
                    l_CheckPoints.push_back(l_Point);
                }
            }
            else
            {
                JadeCore::AnyUnfriendlyNoTotemUnitInObjectRangeCheck u_check(this, GetCaster(), p_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyNoTotemUnitInObjectRangeCheck> searcher(this, l_Newtargets, u_check);
                VisitNearbyObject(p_Radius, searcher);
            }
            break;
        }
        default:
        {
            JadeCore::AnyUnitInObjectRangeCheck u_check(this, p_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> searcher(this, l_Newtargets, u_check);
            VisitNearbyObject(p_Radius, searcher);
            break;
        }
    }

    if (l_Newtargets.size() > 0)
    {
        for (std::list<Unit*>::const_iterator l_Itr = l_Newtargets.begin(); l_Itr != l_Newtargets.end(); ++l_Itr)
        {
            Unit* l_Target = (*l_Itr);
            uint64 l_Guid = l_Target->GetGUID();
            if (m_targets.find(l_Guid) == m_targets.end())
            {
                // Skip spectators
                // Maybe check spectators in target selectors ?
                if (l_Target->IsPlayer() && l_Target->ToPlayer()->isSpectator())
                    continue;

                if (!sScriptMgr->OnSpellAreaTriggerAddTarget(this, l_Target))
                    continue;

                m_targets.insert(l_Guid);
            }
        }
    }
}

void AreaTrigger::ScaleCheckRadius(float m_Scale)
{
    // Invalid scale: no change
    if (m_Scale <= 0.f)
        return;

    for (AreaTriggerTemplateList::iterator l_Itr = m_Templates.begin(); l_Itr != m_Templates.end(); ++l_Itr)
        l_Itr->m_CheckRadius *= m_Scale;
}
