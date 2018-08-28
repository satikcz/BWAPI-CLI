using System;

namespace SharpMapAnalyser
{
    /// <summary>
    /// Tile info
    /// </summary>
    public class TileInfoBase
    {
        /// <summary>
        /// If there is a doodad on this tile.
        /// </summary>
        public bool Doodad { get; set; }

        /// <summary>
        /// Last frame when was this frame seen.
        /// </summary>
        public int LastFrameSeen { get; set; }

        /// <summary>
        /// Zone this tile is in.
        /// </summary>
        public int Zone { get; set; }

        /// <summary>
        /// Subzone this tile is in.
        /// </summary>
        public int SubZone { get; set; }

        /// <summary>
        /// You associate anything with this tile via this property.
        /// </summary>
        public Object Tag { get; set; }

        /// <summary>
        /// Terrain height of this tile.
        /// </summary>
        public int TerrainHeight { get; set; }
    }

    public class BuildTileInfo : TileInfoBase
    {
        /// <summary>
        /// If this tile is fully (= all pixels) walkable.
        /// </summary>
        public bool FullyWalkable { get; set; }

        /// <summary>
        /// If this tile is buildable.
        /// </summary>
        public bool Buildable { get; set; }

        /// <summary>
        /// If resource depot can be built on this tile.
        /// </summary>
        public bool DepotBuildable { get; set; }

        internal WalkTileInfo Clone()
        {
            return (WalkTileInfo)this.MemberwiseClone();
        }
    }

    public class WalkTileInfo : TileInfoBase
    {
        /// <summary>
        /// If this tile is walkable.
        /// </summary>
        public bool Walkable { get; set; }

        /// <summary>
        /// Altitude is distance from nearest unwalkable tile.
        /// </summary>
        public float Altitude { get; set; }

        /// <summary>
        /// If this tile is not walkable, this gives you distance from nearest walkable tile. Can be good for hiding overlords from marines range etc.
        /// </summary>
        public int Inaccessibility { get; set; } // distance from nearest easily accessible ground tile

        internal WalkTileInfo Clone()
        {
            return (WalkTileInfo)this.MemberwiseClone();
        }
    }
}
