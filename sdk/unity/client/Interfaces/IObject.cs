using System;

namespace Client.Interfaces
{
    public interface IObject
    {
        int Id { get; set; }
        bool AutoUpdate { get; set; }
        bool ShouldUpdate { get; set; }
        Type Type { get; }

        void OnUpdated();
    }
}