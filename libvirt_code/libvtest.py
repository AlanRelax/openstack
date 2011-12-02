import libvirt

conn = libvirt.open('qemu:///system')

for id in conn.listDomainsID():

	dom = conn.lookupByID(id)

	print "Dom %s  State %s" % ( dom.name(), dom.info()[0] )
