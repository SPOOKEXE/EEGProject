
from os import path
from random import randint

def write_blank_file( filepath : str ) -> None:
	with open(filepath, "w") as file:
		file.write("")

def check_if_file_exists( filepath : str, createIfMissing=True ) -> None:
	if path.exists( filepath ):
		return
	print(f"File at {filepath} does not exist - creating new file.")
	if createIfMissing:
		write_blank_file( filepath )

def append_data_to_file( timestamp : float, readings : list[float], filepath : str ) -> None:
	check_if_file_exists( filepath )
	with open(filepath, "a") as file:
		file.writelines( str(timestamp) + " " + str(readings).replace(" ", "").replace("0.", ".") + '\n' )

def bulk_append_data_to_file( timestamps : list, bulk_readings : list, filepath : str ) -> None:
	check_if_file_exists( filepath )
	with open(filepath, "a") as file:
		file.writelines([
			str(timestamp) + " " + str(readings).replace(" ", "").replace("0.", ".") + '\n'
			for timestamp, readings in zip( timestamps, bulk_readings )
		])

def read_data_from_file( filepath : str, start=-1, finish=-1 ) -> list:
	check_if_file_exists( filepath )

	# read from file
	with open(filepath, "r") as file:
		lines = file.readlines()

	# start/finish markers
	if start != -1:
		lines = lines[start:]
	if finish != -1:
		lines = lines[:finish]
	
	# double list comprehension for that sweet sweet performance
	items = [ line.split("\n")[0].split(" ") for line in lines ]
	items = [ [ float( data[0] ), [ float(n) for n in data[1].strip('][').split(',') ] ] for data in items ]
	return items

def generate_sample_data( datalen : int ) -> list:
	return [ randint(0, 1000)/1000 for _ in range(datalen) ]

def generate_test_file( filepath : str, samples=1e5, datalen=4 ) -> None:
	write_blank_file( filepath )
	bulk_append_data_to_file(
		list(range(int(samples))),
		[ generate_sample_data( datalen ) for _ in range(int(samples)) ],
		filepath 
	)

if __name__ == '__main__':

	filepath = "test.dat"
	generate_test_file( filepath, samples=1e5, datalen=20 )
	print( read_data_from_file( filepath, finish=5 ) )
