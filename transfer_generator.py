from math import cos, asin, sqrt, pi
from grispy import GriSPy
import numpy as np

def distance(lat1, lon1, lat2, lon2):
	p = pi/180
	a = 0.5 - cos((lat2-lat1)*p)/2 + cos(lat1*p) * cos(lat2*p) * (1-cos((lon2-lon1)*p))/2
	return 12742000 * asin(sqrt(a))




directory = "RNV_GTFS_original/"

all_stations = []
data = []

same_stations = {}
with open(directory+"stops.txt", "r") as file:
	for line in file.readlines()[1:]:
		[stop_id,stop_name,stop_lat,stop_lon,location_type,platform_code] = line.strip().split(",")
		if stop_name in same_stations:
			same_stations[stop_name].append([stop_id,stop_lat,stop_lon])
		else:
			same_stations[stop_name] = [[stop_id,stop_lat,stop_lon]]
		# all_stations.append([stop_id, stop_name])
		# data.append([float(stop_lat),float(stop_lon)])

# gsp = GriSPy(np.array(data))

# for current_station in data:
# 	for x in gsp.bubble_neighbors(np.array([current_station]), distance_upper_bound=0.002)[1][0]:
# 		print(all_stations[x])
# 	print()
with open(directory+"transfers.txt", "w") as file:
	file.write("from_stop_id,to_stop_id,transfer_type,min_transfer_time\n")
	for key in same_stations:
		# print(same_stations[key])
		if len(same_stations[key]) > 1:
			for i in range(len(same_stations[key])):
				j = i+1
				while j < len(same_stations[key]):
					# from_stop_id,to_stop_id,transfer_type,min_transfer_time
					zeit = distance(float(same_stations[key][i][1]), float(same_stations[key][i][2]),float(same_stations[key][j][1]), float(same_stations[key][j][2]))//1.3
					file.write(str(same_stations[key][i][0])+","+str(same_stations[key][j][0])+",2,"+str(zeit)+"\n")
					file.write(str(same_stations[key][j][0])+","+str(same_stations[key][i][0])+",2,"+str(zeit)+"\n")
					j += 1