using BroodWar.Api;
using System;
using System.Collections.Generic;
using System.Linq;

namespace SharpMapAnalyser
{
    /// <summary>
    /// Base is a position in map that allways has some minerals (int he begining) and can has gas.
    /// </summary>
    public class Base
    {
        /// <summary>
        /// List of minerals for this base.
        /// </summary>
        public List<Unit> Minerals { get; private set; }

        /// <summary>
        /// List of gases for this base.
        /// </summary>
        public List<Unit> Geysers { get; private set; }

        /// <summary>
        /// Best depot position for this base.
        /// </summary>
        public TilePosition DepotPosition { get; private set; }

        /// <summary>
        /// If there are some blocking minerals for this base (usually with 8 or 16 minerals left)
        /// </summary>
        public List<Unit> BlockingMinerals { get; private set; }

        /// <summary>
        /// True if this base is also starting location.
        /// </summary>
        public bool IsStartingLocation { get; private set; }

        /// <summary>
        /// Subzone this base belongs to.
        /// </summary>
        public SubZone SubZone { get; private set; }

        /// <summary>
        /// Zone this base belongs to.
        /// </summary>
        public Zone Zone { get; private set; }

        private SharpMapAnalyser Analyser;

        public Base(TilePosition position, bool starting, List<Unit> minerals, List<Unit> geysers, Zone zone, SubZone subZone, SharpMapAnalyser analyser)
        {
            this.DepotPosition = position;
            this.IsStartingLocation = starting;
            this.Minerals = minerals;
            this.Geysers = geysers;
            this.Zone = zone;
            this.SubZone = subZone;
            this.Natural = this;
            this.Analyser = analyser;

            Analyser.OnAnalysisFinished += Analyser_OnAnalysisFinished;
        }

        private void Analyser_OnAnalysisFinished(object sender, EventArgs e)
        {
            FindNatural();
        }

        private void FindNatural()
        {
            var distancesToMainBases = Analyser.OtherMainBases.
              Select(x => Analyser.FindPath(this.DepotPosition, x.DepotPosition)).
              Where(x => x != null && x.Any()).
              Select(x => x.Last().G).
              OrderBy(x => x);

            int distanceToNearestMainBase = distancesToMainBases.FirstOrDefault();

            if (distanceToNearestMainBase > 0)
            {
                Natural = Analyser.OtherBases.
                  Where(x => x.IsReachableFrom(Analyser.SelfBase)).
                  OrderBy(x => x.DepotPosition.CalcDistance(Game.Self.StartLocation)).FirstOrDefault();
            }

            if (Natural == null) Natural = this;
        }

        /// <summary>
        /// Returns true if this base is reachable from given tile.
        /// </summary>
        public bool IsReachableFrom(TilePosition t)
        {
            return Analyser.BuildGrid[t.X, t.Y].Zone == Analyser.BuildGrid[DepotPosition.X, DepotPosition.Y].Zone;
        }

        /// <summary>
        /// Returns true if this base is reachable from given base.
        /// </summary>
        public bool IsReachableFrom(Base b)
        {
            return IsReachableFrom(b.DepotPosition);
        }

        /// <summary>
        /// Natural (nearest base for players main base)
        /// </summary>
        public Base Natural { get; private set; }

        /// <summary>
        /// Minerals together remaining on base.
        /// </summary>
        public int MineralsRemaining()
        {
            return Minerals.Where(x => x.Exists).Sum(x => x.Resources);
        }

        /// <summary>
        /// Returns true if the base is already taken by any player.
        /// </summary>
        public bool IsTaken()
        {
            return IsTakenBy() != null;
        }

        /// <summary>
        /// Returns Player, that currently owns (has resource depot here) this base.
        /// </summary>
        public Player IsTakenBy()
        {
            var depot = Game.AllUnits.
              Where(x => x.Distance(Position.Rescale(DepotPosition)) <= 15 * 32).
              Where(x => x.UnitType.IsResourceDepot).
              OrderBy(x => x.Distance(Position.Rescale(DepotPosition))).
              FirstOrDefault();

            if (depot != null)
                return depot.Player;

            return null;
        }
    }
}
