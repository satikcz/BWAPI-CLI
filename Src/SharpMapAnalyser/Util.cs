using System.IO;

namespace SharpMapAnalyser
{
    public static class Util
    {
        public static string GetReadDir()
        {
            if (!Directory.Exists(@"bwapi-data\read\"))
                Directory.CreateDirectory(@"bwapi-data\read\");
            return @"bwapi-data\read\";
        }

        public static string GetWriteDir()
        {
            if (!Directory.Exists(@"bwapi-data\write\"))
                Directory.CreateDirectory(@"bwapi-data\write\");
            return @"bwapi-data\write\";
        }
    }
}
