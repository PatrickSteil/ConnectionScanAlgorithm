# Classes

## Connection

- departure			[Station]
- arrival			[Station]
- dep_time			[Date]
- arr_time			[Date]

## Station

- stop_id			[int]
- location_type 	[int]
- stop_name			[string]
- stop_lat 			[string]
- stop_lon 			[string]
- platform_code 	[string]

## CSA

- connections 		[vector Connection]
- stations 			[vector Station]


## Parser

- filename 			[string]