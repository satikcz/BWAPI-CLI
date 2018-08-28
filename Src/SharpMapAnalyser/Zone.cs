using BroodWar.Api;

namespace SharpMapAnalyser
{
    /// <summary>
    /// Zone is largest walkable area. So every zone can be considered as an island.
    /// </summary>
    public class Zone
    {
        /// <summary>
        /// Zone id.
        /// </summary>
        public int Id { get; private set; }

        /// <summary>
        /// Size in walk tiles.
        /// </summary>
        public int WalkSize { get; private set; }

        /// <summary>
        /// Size in build tiles.
        /// </summary>
        public int BuildSize { get; private set; }
        private SharpMapAnalyser analyser;

        public Zone(int id, int walkSize, SharpMapAnalyser analyser)
        {
            this.WalkSize = walkSize;
            this.Id = id;
            this.analyser = analyser;
            BuildSize = walkSize / 16; // todo: real calculation
        }

        /// <summary>
        /// Checks if given player has any non-flying unit in the zone.
        /// </summary>
        /// <param name="player"></param>
        /// <returns></returns>
        public bool IsIslandFor(Player player)
        {
            foreach (var unit in player.Units)
            {
                if (unit.UnitType.IsFlyer || unit.Position.IsInvalid) continue;

                var walkPos = WalkPosition.Rescale(unit.Position);

                if (analyser.WalkGrid[walkPos.X, walkPos.Y].Zone == Id)
                {
                    return false;
                }
            }

            return true;
        }
    }

    /// <summary>
    /// Subzone is largest possible walkable area with the same terrain height. So subzones are usually player main bases or islands.
    /// </summary>
    public class SubZone : Zone
    {
        public int TerrainHeight { get; private set; }

        public SubZone(int id, int walkSize, int terrainHeight, SharpMapAnalyser analyser) : base(id, walkSize, analyser)
        {
            this.TerrainHeight = terrainHeight;
        }
    }


}
