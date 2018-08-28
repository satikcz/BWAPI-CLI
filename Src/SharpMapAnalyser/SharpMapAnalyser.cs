using Algorithms;
using BroodWar.Api;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;

namespace SharpMapAnalyser
{
    public class SharpMapAnalyser
    {
        public int Width { get; private set; } = 0;
        public int Height { get; private set; } = 0;

        public BuildTileInfo[,] BuildGrid = null;
        public WalkTileInfo[,] WalkGrid = null;
        public int zoneCount = 0;
        public int ZoneCount = 0;

        public List<Zone> Zones { get; } = new List<Zone>();
        public List<SubZone> SubZones { get; } = new List<SubZone>();

        /// <summary>
        /// List of Start locations tiles.
        /// </summary>
        public List<TilePosition> StartLocations { get { return Game.StartLocations; } }

        /// <summary>
        /// List of all bases. Base allways has some minerals and can has gas.
        /// </summary>
        public List<Base> Bases { get; private set; } = new List<Base>();

        /// <summary>
        /// List of all bases except player's starting location.
        /// </summary>
        public IEnumerable<Base> OtherBases
        {
            get
            {
                // There is some tolerance, since the start location can not allways be the same as optimal depot location.
                return Bases.Where(x => x.DepotPosition.CalcDistance(Game.Self.StartLocation) > 5);
            }
        }

        /// <summary>
        /// Returns all main bases except the player's starting location.
        /// </summary>
        public IEnumerable<Base> OtherMainBases
        {
            get
            {
                return OtherBases.Where(x => x.IsStartingLocation);
            }
        }

        /// <summary>
        /// Returns all bases that are on starting location.
        /// </summary>
        public IEnumerable<Base> MainBases
        {
            get
            {
                return Bases.Where(x => x.IsStartingLocation);
            }
        }

        public float InitLength { get; private set; } = 0;

        private sbyte[,] directions = new sbyte[8, 2] { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 }, { 1, -1 }, { 1, 1 }, { -1, 1 }, { -1, -1 } };

        private PathFinderFast PathFinderTile;
        private byte[,] PathFinderTileData;

        /// <summary>
        /// Gets map name.
        /// </summary>
        public string MapName { get { return Game.MapName; } }

        /// <summary>
        /// This eventhandler is called on map analysis finishes.
        /// </summary>
        public event EventHandler OnAnalysisFinished;
        protected virtual void AnalysisFinished()
        {
            OnAnalysisFinished?.Invoke(null, null);
        }

        /// <summary>
        /// Returns true for island maps = maps, where terrain restricts walking from one main base to another.
        /// </summary>
        public bool IsIslandMap
        {
            get
            {
                return StartLocations.All(x => BuildGrid[x.X * 4, x.Y * 4].Zone == BuildGrid[StartLocations.First().X * 4, StartLocations.First().Y * 4].Zone);
            }
        }

        public Base SelfBase { get; private set; }

        /// <summary>
        /// Analyses the map. Can take up a few seconds.
        /// </summary>
        public void Analyse()
        {
            DateTime now = DateTime.Now;
            Width = Game.MapWidth;
            Height = Game.MapHeight;

            int minSize = Math.Max(Width, Height);
            int power = 1;
            while (power < minSize)
                power *= 2;

            PathFinderTileData = new byte[power, power];
            PathFinderTile = new PathFinderFast(PathFinderTileData);

            BuildGrid = new BuildTileInfo[Width, Height];
            WalkGrid = new WalkTileInfo[Width * 4, Height * 4];
            UpdateWalkability();
            CalculateInaccessibility();
            CalculateAltitude();
            InitLength = (DateTime.Now.Ticks - now.Ticks) / 10000.0f;

            FindBases();

#if DEBUG
            Game.SendText($"Map analyser: {InitLength}ms");
            SaveToFile(Game.MapFileName);
#endif

            SelfBase = Bases.OrderBy(x => x.DepotPosition.CalcDistance(Game.Self.StartLocation)).First();

            AnalysisFinished();
        }

        /// <summary>
        /// Checks if the position is valid depot building position according to the game.
        /// </summary>
        /// <param name="pos">Position to check.</param>
        /// <returns>Returns true if the position is valid.</returns>
        public bool IsValidDepotPos(TilePosition pos)
        {
            return Game.CanBuildHere(pos, Game.Self.Race.ResourceDepot.Type, null, false);
        }

        /// <summary>
        /// Returns nearest valid depot position from given position.
        /// </summary>
        /// <param name="pos">Position to find valid depot position arround.</param>
        /// <returns>Returns nearest valid tile position or null if not found.</returns>
        public TilePosition GetDepotPosition(TilePosition pos)
        {
            if (IsValidDepotPos(pos))
                return pos;

            var valid = new List<TilePosition>();

            for (int x = -12; x < 13; x++)
                for (int y = -12; y < 13; y++)
                {
                    var examined = new TilePosition(pos.X + x, pos.Y + y);
                    if (IsValidDepotPos(examined))
                        valid.Add(examined);
                }

            return valid.OrderBy(x => x.CalcDistance(pos)).FirstOrDefault();
        }

        private Base FindBase(List<Unit> resources, TilePosition startPos, bool starting = false)
        {
            const int resDist = 15;
            var nearRes = resources.Where(x => startPos.CalcDistance(x.TilePosition) <= resDist);
            TilePosition center = new TilePosition(nearRes.Sum(x => x.TilePosition.X) / nearRes.Count(), nearRes.Sum(x => x.TilePosition.Y) / nearRes.Count());
            center = starting ? startPos : GetDepotPosition(center);

            if (center == null)
            {
                resources.RemoveAll(x => startPos.CalcDistance(x.TilePosition) <= resDist);
                return null;
            }

            var b = new Base(center, starting, nearRes.Where(x => x.UnitType.IsMineralField).ToList(),
              nearRes.Where(x => x.UnitType.IsGas()).ToList(),
              Zones[BuildGrid[center.X, center.Y].Zone],
              SubZones[BuildGrid[center.X, center.Y].SubZone], this);
            resources.RemoveAll(x => startPos.CalcDistance(x.TilePosition) <= resDist);
            return b;
        }

        /// <summary>
        /// Finds bases.
        /// </summary>
        public void FindBases()
        {
            var resources = Game.Minerals.Where(x => x.Resources > 8).Concat(Game.Geysers).ToList();

            foreach (var sl in Game.StartLocations)
            {
                Bases.Add(FindBase(resources, sl, true));
            }

            while (resources.Any())
            {
                var b = FindBase(resources, resources.First().TilePosition, false);
                if (b != null)
                    Bases.Add(b);
                else
                    Game.SendText($"Base center not found for {resources.First().TilePosition}.");
            }
        }

        /// <summary>
        /// Finds a path between two positions.
        /// </summary>
        public List<PathFinderNode> FindPath(TilePosition start, TilePosition end)
        {
            return PathFinderTile.FindPath(start.ToPoint(), end.ToPoint());
        }

        /// <summary>
        /// Returns true if the tile is valid (= is in map)
        /// </summary>
        public bool IsValidWalkTile(int x, int y)
        {
            return (x >= 0 && y >= 0 && x < Width * 4 && y < Height * 4);
        }

        /// <summary>
        /// Returns true if tile is walkable.
        /// </summary>
        public bool IsWalkable(int x, int y)
        {
            return WalkGrid[x, y].Walkable;
        }

        private bool IsValidAndNotWalkable(int x, int y)
        {
            return IsValidWalkTile(x, y) && !IsWalkable(x, y);
        }

        private bool IsValidAndWalkable(int x, int y)
        {
            return IsValidWalkTile(x, y) && IsWalkable(x, y);
        }

        private bool IsBorderWalkTile(int x, int y)
        {
            if (!IsWalkable(x, y))
                return false;

            return IsValidAndNotWalkable(x + 1, y) || IsValidAndNotWalkable(x - 1, y) || IsValidAndNotWalkable(x, y + 1) || IsValidAndNotWalkable(x, y - 1);
        }

        private void CalculateAltitude()
        {
            // initiate openset
            var openSet = new Queue<WalkPosition>();
            for (int y = 0; y < Height * 4; y++)
                for (int x = 0; x < Width * 4; x++)
                {
                    bool isBorder = IsBorderWalkTile(x, y) && !IsSmallZone(x, y);
                    WalkGrid[x, y].Altitude = isBorder ? 0 : -1;
                    if (isBorder)
                        openSet.Enqueue(new WalkPosition(x, y));
                }

            // search altitude by floodfill
            while (openSet.Any())
            {
                var pos = openSet.Dequeue();
                float altitude = WalkGrid[pos.X, pos.Y].Altitude;

                for (int d = 0; d < 4; d++)
                {
                    int x = pos.X + directions[d, 0];
                    int y = pos.Y + directions[d, 1];

                    if (!IsValidWalkTile(x, y) || !IsWalkable(x, y) || IsSmallZone(x, y) || WalkGrid[x, y].Altitude != -1) continue;

                    openSet.Enqueue(new WalkPosition(x, y));
                    WalkGrid[x, y].Altitude = altitude + 1;
                }
            }
        }

        private void CalculateInaccessibility()
        {
            // initiate openset
            var openSet = new Queue<WalkPosition>();
            for (int y = 0; y < Height * 4; y++)
                for (int x = 0; x < Width * 4; x++)
                {
                    bool isBorder = IsBorderWalkTile(x, y) && !IsSmallZone(x, y);
                    WalkGrid[x, y].Inaccessibility = isBorder ? 0 : -1;
                    if (isBorder)
                        openSet.Enqueue(new WalkPosition(x, y));
                }

            // search inaccessibility by floodfill
            while (openSet.Any())
            {
                var pos = openSet.Dequeue();
                int inaccessibility = WalkGrid[pos.X, pos.Y].Inaccessibility;

                for (int d = 0; d < 4; d++)
                {
                    int x = pos.X + directions[d, 0];
                    int y = pos.Y + directions[d, 1];

                    if (!IsValidWalkTile(x, y)) continue;

                    if ((!IsWalkable(x, y) || IsSmallZone(x, y)) && WalkGrid[x, y].Inaccessibility < 0)
                    {
                        openSet.Enqueue(new WalkPosition(x, y));
                        WalkGrid[x, y].Inaccessibility = inaccessibility + 1;
                    }
                }
            }
        }

        private bool IsSmallZone(int x, int y)
        {
            return WalkGrid[x, y].Zone >= 0 && Zones[WalkGrid[x, y].Zone].BuildSize < 100;
        }

        // Updates walkability info
        public void UpdateWalkability()
        {
            ZoneCount = 0;
            UpdateWalkableGrid(); // Read real walkability from BW
            UpdateBuildGrid();
            FindZones();
        }

        // Some info about the build grid tiles.
        private void UpdateBuildGrid()
        {
            for (int y = 0; y < Height; y++)
                for (int x = 0; x < Width; x++)
                {
                    var tileInfo = new BuildTileInfo()
                    {
                        Buildable = Game.IsBuildable(x, y, true),
                        FullyWalkable = IsFullyWalkable(x, y),
                        DepotBuildable = Game.IsBuildable(x, y, true),
                        LastFrameSeen = -1,
                        TerrainHeight = Game.GetGroundHeight(x, y) / 2,
                        Doodad = Game.GetGroundHeight(x, y) % 2 == 1,
                        Zone = -1,
                        SubZone = -1,
                    };
                    BuildGrid[x, y] = tileInfo;
                }
        }

        // Read walkable info from BW and process it with some small filtering
        private void UpdateWalkableGrid()
        {
            var temp = new WalkTileInfo[Width * 4, Height * 4];

            for (int y = 0; y < Height * 4; y++)
                for (int x = 0; x < Width * 4; x++)
                {
                    temp[x, y] = new WalkTileInfo();
                    temp[x, y].Altitude = -1;
                    temp[x, y].LastFrameSeen = -1;
                    temp[x, y].TerrainHeight = 0;
                    temp[x, y].Walkable = Game.IsWalkable(x, y);
                    temp[x, y].Zone = -1;
                    temp[x, y].SubZone = -1;
                }

            // Slight filtering to ignore some small walkable pixels
            for (int y = 0; y < Height * 4; y++)
                for (int x = 0; x < Width * 4; x++)
                {
                    WalkGrid[x, y] = temp[x, y].Clone();

                    bool walkUp = y > 0 && temp[x, y - 1].Walkable;
                    bool walkDown = y < Height * 4 - 1 && temp[x, y + 1].Walkable;
                    bool walkLeft = x > 0 && temp[x - 1, y].Walkable;
                    bool walkRight = x < Width * 4 - 1 && temp[x + 1, y].Walkable;
                    bool walkable = WalkGrid[x, y].Walkable;

                    if (((!walkUp && !walkDown) || (!walkLeft && !walkRight)))
                        walkable = false;

                    WalkGrid[x, y].Walkable = walkable;
                }
        }

        private void FindSubZones()
        {
            foreach (var sub in Zones)
            {
                // todo: fix
                SubZones.Add(new SubZone(sub.Id, sub.WalkSize, 0, this));
            }
        }

        private void FindZones()
        {
            // find walk zones
            for (int y = 0; y < Height * 4; y++)
                for (int x = 0; x < Width * 4; x++)
                {
                    if (Game.IsWalkable(x, y) && WalkGrid[x, y].Zone == -1)
                    {
                        int areaSize = FloodFill(x, y, ZoneCount);
                        Zone zone = new Zone(ZoneCount, areaSize, this);

                        if (areaSize > 3)
                        {
                            Zones.Add(zone);
                            ZoneCount++;
                        }
                    }
                }

            FindSubZones();
        }

        private int FloodFill(int x, int y, int zone)
        {
            Stack<Tuple<int, int>> tileStack = new Stack<Tuple<int, int>>();

            tileStack.Push(new Tuple<int, int>(x, y));

            int zoneSize = 0;

            while (tileStack.Any())
            {
                var t = tileStack.Pop();
                x = t.Item1;
                y = t.Item2;

                if (!WalkGrid[x, y].Walkable || WalkGrid[x, y].Zone != -1) continue;
                WalkGrid[x, y].Zone = zone;
                WalkGrid[x, y].SubZone = zone;

                BuildGrid[x / 4, y / 4].Zone = zone;
                BuildGrid[x / 4, y / 4].SubZone = zone;

                zoneSize++;

                if (x < Width * 4 - 1 && WalkGrid[x + 1, y].Zone == -1)
                    if (WalkGrid[x + 1, y].Walkable) tileStack.Push(new Tuple<int, int>(x + 1, y));

                if (x > 0 && WalkGrid[x - 1, y].Zone == -1)
                    if (WalkGrid[x - 1, y].Walkable) tileStack.Push(new Tuple<int, int>(x - 1, y));

                if (y < Height * 4 - 1 && WalkGrid[x, y + 1].Zone == -1)
                    if (WalkGrid[x, y + 1].Walkable) tileStack.Push(new Tuple<int, int>(x, y + 1));

                if (y > 0 && WalkGrid[x, y - 1].Zone == -1)
                    if (WalkGrid[x, y - 1].Walkable) tileStack.Push(new Tuple<int, int>(x, y - 1));
            }

            return zoneSize;
        }

        /// <summary>
        /// True for tiles that are fully walkable.
        /// </summary>
        public bool IsFullyWalkable(int x, int y)
        {
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    if (!WalkGrid[x * 4 + i, y * 4 + j].Walkable)
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        // various colors for representating of areas in bitmap.
        private List<Color> GenerateColors(int count)
        {
            List<Color> colors = new List<Color>();
            int phases = (int)(Math.Pow(count + 1, 1.0f / 3.0f) + 1);
            if (phases <= 1) phases = 2;
            int colorCount = phases * phases * phases;
            for (int i = 1; i < colorCount; i++)
            {
                int r = (i % phases) * 255 / (phases - 1);
                int g = ((i / phases) % phases) * 255 / (phases - 1);
                int b = i / phases / phases * 255 / (phases - 1);
                colors.Add(Color.FromArgb(r, g, b));
            }

            return colors;
        }

        /// <summary>
        /// Saves colorful bitmap of areas to file with given filename.
        /// </summary>
        public void SaveToFile(string name)
        {
            int zoneColors = 27;

            var colors = GenerateColors(zoneColors);
            {
                Bitmap bmp = new Bitmap(Width * 4, Height * 4);

                // zone colors
                for (int y = 0; y < Height * 4; y++)
                {
                    for (int x = 0; x < Width * 4; x++)
                    {
                        int zone = WalkGrid[x, y].Zone;
                        if (zone < 0)
                            bmp.SetPixel(x, y, Color.Black);
                        else
                            bmp.SetPixel(x, y, colors[zone % zoneColors]);
                    }
                }

                // markup minerals, geysers and bases
                using (var g = Graphics.FromImage(bmp))
                {
                    foreach (var m in Game.Minerals)
                    {
                        g.FillRectangle(new SolidBrush(Color.Aqua), new Rectangle(m.TilePosition.X * 4, m.TilePosition.Y * 4, 2 * 4, 1 * 4));
                    }

                    foreach (var m in Game.Geysers)
                    {
                        g.FillRectangle(new SolidBrush(Color.Olive), new Rectangle(m.TilePosition.X * 4, m.TilePosition.Y * 4, 2 * 4, 1 * 4));
                    }

                    foreach (var b in Bases)
                    {
                        var pen = new Pen(new SolidBrush(b.IsStartingLocation ? Color.Orange : Color.Purple));
                        g.DrawEllipse(pen, new Rectangle(b.DepotPosition.X * 4, b.DepotPosition.Y * 4, 4 * 4, 3 * 4));
                    }
                }

                bmp.Save(Path.Combine(Util.GetWriteDir(), name + "_zones.png"));

                // Altitude = distance from nearest unwalkable tile.
                bmp = new Bitmap(Width * 4, Height * 4);
                for (int y = 0; y < Height * 4; y++)
                {
                    for (int x = 0; x < Width * 4; x++)
                    {
                        float alt = WalkGrid[x, y].Altitude * 2;
                        if (alt <= 0)
                        {
                            if (alt == 0)
                            {
                                bmp.SetPixel(x, y, Color.FromArgb(0, 0, 255));
                            }
                            else
                            {
                                alt = WalkGrid[x, y].Inaccessibility * 2;
                                bmp.SetPixel(x, y, Color.FromArgb((int)alt, 0, 0));
                            }
                        }
                        else
                            bmp.SetPixel(x, y, Color.FromArgb(0, (int)alt, 0));
                    }
                }
                bmp.Save(Path.Combine(Util.GetWriteDir(), name + "_alt.png"));
            }
        }
    }
}