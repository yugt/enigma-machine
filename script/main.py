from samson.analyzers.english_analyzer import EnglishAnalyzer
import subprocess, sys

analyzer = EnglishAnalyzer()
def crack_one(c):
	return (c, analyzer.analyze(bytearray(c)))

def crack(cypher, top=1):
	return [(cypher, p[0].decode(), p[1])
	 	for p in sorted([ crack_one(c) for c in
		subprocess.run(['./build/enigma', cypher], stdout=subprocess.PIPE)\
		.stdout.split(b'\n')], key=lambda tup: tup[1])[-top:]]

for cypher in sys.stdin:
	[ print(c,t,s) for (c,t,s) in crack(cypher.rstrip()) ]