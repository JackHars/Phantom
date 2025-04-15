#!/usr/bin/env python3
import json
import sys
import requests

from common.params import Params

if __name__ == "__main__":
  p = Params()
  address = str(p.get("Address")) 
  resolve = requests.get("https://nominatim.openstreetmap.org/search.php?q=" + address[2:-1] + "&format=jsonv2").json()
  lat,lon = resolve[0]["lat"], resolve[0]["lon"]
  dest = {"latitude": float(lat), "longitude": float(lon)}
  p.put("NavDestination", json.dumps(dest))
