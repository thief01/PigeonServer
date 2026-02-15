using System.Collections.Generic;
using Client.Interfaces;

namespace Client
{
    public class ObjectManager
    {
        public List<IObject> observedObjects = new List<IObject>();

        public ObjectManager()
        {
            
        }

        public void UpdateFromData(byte[] buffer)
        {
            
        }
    }
}