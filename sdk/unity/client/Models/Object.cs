using System;
using Client.Interfaces;

namespace Client.Models
{
    public class Object : IObject
    {
        public int Id { get; set; }
        public bool AutoUpdate { get; set; }
        public bool ShouldUpdate { get; set; }
        public Type Type { get; }
        
        public void OnUpdated()
        {
            throw new NotImplementedException();
        }
    }
}