#!/usr/bin/env python3
import json
import sys
import time
import random
import re

from common.params import Params

def parse_destination(dest_str):
    """Parse different types of destination inputs"""
    
    # Check if it's a POI search
    if dest_str.startswith("poi:"):
        category = dest_str[4:]
        # In a real implementation, this would query a location service
        # For now, simulate with a random nearby location
        lat = 37.7749 + (random.random() - 0.5) * 0.01
        lon = -122.4194 + (random.random() - 0.5) * 0.01
        dest = {
            "latitude": lat, 
            "longitude": lon,
            "place_name": f"Nearby {category}",
            "place_details": f"Found location",
            "poi_category": category
        }
        return dest
    
    # Check if it's Google Maps URL format
    if "google.com/maps" in dest_str or "goo.gl/maps" in dest_str:
        if "@" in dest_str:
            coords = dest_str.split("/@")[-1].split("/")[0].split(",")
            if len(coords) >= 2:
                try:
                    return {
                        "latitude": float(coords[0]), 
                        "longitude": float(coords[1]),
                        "place_name": "Google Maps Location",
                        "place_details": f"Lat: {coords[0]}, Lon: {coords[1]}"
                    }
                except ValueError:
                    pass
    
    # Try to parse as address or place name
    # In a real implementation, this would use a geocoding service
    # For now, just use dummy coordinates
    return {
        "latitude": 37.7749, 
        "longitude": -122.4194,
        "place_name": dest_str,
        "place_details": "Address lookup"
    }

def handle_waypoints():
    """Process waypoints if they exist"""
    params = Params()
    waypoints_str = params.get("NavWaypoints")
    
    if not waypoints_str:
        return None
    
    try:
        waypoints = json.loads(waypoints_str)
        # Process waypoints here in a real implementation
        return waypoints
    except:
        return None

if __name__ == "__main__":
    address = None
    
    # Check if we're setting a direct destination or have args
    if len(sys.argv) > 1:
        address = sys.argv[1]
    else:
        # Check if there's an address in the params
        params = Params()
        address_bytes = params.get("Address")
        if address_bytes:
            address = address_bytes.decode('utf-8')
    
    if address:
        dest = parse_destination(address)
        waypoints = handle_waypoints()
        
        if waypoints:
            dest["waypoints"] = waypoints
        
        # Get route preferences
        params = Params()
        try:
            dest["avoid_tolls"] = bool(int(params.get("NavAvoidTolls") or "0"))
            dest["avoid_highways"] = bool(int(params.get("NavAvoidHighways") or "0"))
            dest["avoid_ferries"] = bool(int(params.get("NavAvoidFerries") or "0"))
        except:
            dest["avoid_tolls"] = False
            dest["avoid_highways"] = False
            dest["avoid_ferries"] = False
            
        # Save to params
        Params().put("NavDestination", json.dumps(dest))
        print(f"Set destination: {json.dumps(dest)}")
    else:
        print("No destination provided")
