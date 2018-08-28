using BroodWar.Api;
using System;
using System.Drawing;

namespace SharpMapAnalyser
{
    public static class Extensions
    {
        #region Player
        public static bool IsZerg(this Player player)
        {
            return player.Race.Type == BroodWar.Api.Enum.RaceType.Zerg;
        }

        public static bool IsProtoss(this Player player)
        {
            return player.Race.Type == BroodWar.Api.Enum.RaceType.Protoss;
        }

        public static bool IsTerran(this Player player)
        {
            return player.Race.Type == BroodWar.Api.Enum.RaceType.Terran;
        }

        public static bool IsRandom(this Player player)
        {
            return player.Race.Type == BroodWar.Api.Enum.RaceType.Unknown;
        }
        #endregion

        #region Unit
        public static bool Move(this Unit unit, TilePosition position)
        {
            return unit.Move(Position.Rescale(position), false);
        }

        public static bool Move(this Unit unit, WalkPosition position)
        {
            return unit.Move(Position.Rescale(position), false);
        }

        //public static int GetBuildingStartFramex(this Unit unit)
        //{
        //  if (unit.IsCompleted || !unit.UnitType.IsBuilding) return -1;

        //  float hpFromStart = unit.HitPoints - unit.UnitType.MaxHitPoints * 0.1f;
        //  float buildRate = (float)Math.Ceiling(0.9f * 256 * unit.UnitType.MaxHitPoints / unit.UnitType.Price.TimeFrames);
        //  float framesFromStart = hpFromStart * 256 / buildRate;

        //  return (int)(Game.FrameCount - framesFromStart + 0.5f);
        //}

        public static int GetBuildingEndFrame(this Unit unit)
        {
            if (unit.IsCompleted || !unit.UnitType.IsBuilding) return Game.FrameCount;

            float hpFromStart = unit.HitPoints - unit.UnitType.MaxHitPoints * 0.1f;
            float buildRate = (float)Math.Ceiling(0.9f * 256 * unit.UnitType.MaxHitPoints / unit.UnitType.Price.TimeFrames);
            float framesFromStart = hpFromStart * 256 / buildRate;

            return (int)(Game.FrameCount - framesFromStart + 0.5f) + unit.UnitType.Price.TimeFrames;
        }
        #endregion

        #region UnitType
        public static bool IsWarUnit(this UnitType type)
        {
            return (type.CanAttack || type.IsSpellcaster) && !type.IsWorker;
        }

        public static bool IsMorphedBuilding(this UnitType type)
        {
            return type.Type == BroodWar.Api.Enum.UnitType.Zerg_Sunken_Colony ||
                    type.Type == BroodWar.Api.Enum.UnitType.Zerg_Spore_Colony ||
                    type.Type == BroodWar.Api.Enum.UnitType.Zerg_Lair ||
                    type.Type == BroodWar.Api.Enum.UnitType.Zerg_Hive ||
                    type.Type == BroodWar.Api.Enum.UnitType.Zerg_Greater_Spire;
        }

        public static bool IsTank(this UnitType type)
        {
            return type.Type == BroodWar.Api.Enum.UnitType.Terran_Siege_Tank_Tank_Mode || type.Type == BroodWar.Api.Enum.UnitType.Terran_Siege_Tank_Siege_Mode;
        }

        public static bool IsGas(this UnitType type)
        {
            return type.Type == BroodWar.Api.Enum.UnitType.Resource_Vespene_Geyser;
        }

        public static bool IsMelee(this UnitType type)
        {
            return type.CanAttack && ((type.GroundWeapon.DamageAmount > 0 && type.GroundWeapon.MaxRange <= 32) || (type.AirWeapon.DamageAmount > 0 && type.AirWeapon.MaxRange <= 32));
        }
        #endregion

        public static Point ToPoint(this TilePosition pos)
        {
            return new Point(pos.X, pos.Y);
        }
    }
}
