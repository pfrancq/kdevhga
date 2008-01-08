#!/usr/bin/python
# -*- coding: iso-8859-1 -*-

# Import Modules
import _mysql
import sys,string
import time;


		#<Object Id="Réseaux">
			#<Include Attribute="réseaux" />
			#<Include Attribute="neurones"/>
			#<Include Attribute="méta-heurisitique"/>
		#</Object>

# Concept
class Concept:
	"Class representing a concept"
	def __init__(self,Id,Type,w):
		self.Id=Id
		self.Type=Type
		self.Weight=w
	def __cmp__(self,other):
		if(self.Type==other.Type):
			return(self.Id-other.Id)
		return(self.Type-other.Type)

# Treat a particular topic
def TreatTopic(db,out,topicid,name,maxconcepts):
	nbdocs=0           # Number of documents
	Concepts={}       # Dictionnary of concepts (key="id|type")
	print "Treat "+name+"...",
	sys.stdout.flush()

	# Go through the documents of the topic
	db.query("SELECT htmlid FROM topicsbyhtmls WHERE topicid="+topicid)
	for doc in db.store_result().fetch_row(0):
		if(nbdocs==0):
			out.write('\t\t<Object Id="'+name+'">\n')
		nbdocs=nbdocs+1

		# Get all concepts of the current document
		db.query("""SELECT concepts.conceptid,concepts.typeid,weight*log10(concepttypes.refdocs/concepts.refdocs)
		 FROM htmlsbyconcepts,concepts,concepttypes
		  WHERE htmlsbyconcepts.conceptid=concepts.conceptid AND htmlsbyconcepts.typeid=concepts.typeid AND concepts.typeid=concepttypes.typeid
		   AND htmlid="""+doc[0])
		for concept in db.store_result().fetch_row(0):
			# Skip everything outside words
			typeid=int(concept[1])
			if(typeid>12):
				continue;
			key=concept[0]+"|"+concept[1]
			if Concepts.has_key(key):
				Concepts[key].Weight=Concepts[key].Weight+float(concept[2])
			else:
				Concepts[key]=Concept(int(concept[0]),int(concept[1]),float(concept[2]))
	print "OK ("+str(nbdocs)+" documents, "+str(len(Concepts))+" concepts)"

	# Build a list of concept in List to be sorted by descending weight
	List=[]
	for c in Concepts:
		List.append(Concepts[c])
	List.sort (lambda x, y : cmp(y.Weight,x.Weight))

	# Put the first non-null weighted concepts in the XML file
	i=0
	for w in List:
		if(i>maxconcepts or w.Weight==0.0):
			break
		i=i+1
		db.query("SELECT name FROM concepts WHERE conceptid="+str(w.Id)+" AND typeid="+str(w.Type))
 		for name in db.store_result().fetch_row(0):
			out.write('\t\t\t<Include Attribute="'+name[0]+'" />\n')
	if(nbdocs>0):
			out.write('\t\t</Object>\n')

# Export topics
def Export(dbname,filename,maxconcepts):
	out=open(filename,"w")
	out.write("<!DOCTYPE Hierarchical-Data>\n<Hierarchical-Data>\n\t<Objects>\n")
	db=db=_mysql.connect(db=dbname,host="127.0.0.1",user="root",passwd="")
	db.query("SELECT topicid,name FROM topics")
	for topic in db.store_result().fetch_row(0):
		TreatTopic(db,out,topic[0],topic[1],maxconcepts)
	out.write("\t</Objects>\n</Hierarchical-Data>")
	out.close()

def Test2(c,w):
	global Concepts
	print Concepts["1|1"]
	if Concepts.has_key(c):
		Concepts[c]=Concepts[c]+w
	else:
		print  "Not found"
		Concepts[c]=w
	print Concepts["1|1"]

def Test():
	global Concepts
	Concepts={}
	Concepts["1|1"]=float("2.0")
	Concepts["1|3"]=float("2.0")
	Test2("1|1",float("1.0"))
	Test2("1|4",float("1.0"))
	Test2("1|1",float("5.0"))
	for c in Concepts:
		print Concepts[c]

# Main function
if(__name__=='__main__'):
	#Test()
	if(((len(sys.argv)==2) and (sys.argv[1]=="--help")) or ((len(sys.argv)!=3) and (len(sys.argv)!=4))):
		print "Syntax: export.py dbname filename [nb_concepts]"
	else:
		maxconcepts=20
		if(len(sys.argv)==4):
			maxconcepts=int(sys.argv[3])
		print "Export database "+sys.argv[1]+ " in file "+sys.argv[2]+" "+str(maxconcepts)+" concepts per topic"
		Export(sys.argv[1],sys.argv[2],maxconcepts)
