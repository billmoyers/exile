IMAGES = $(shell ls entity/*.png terrain/*.png)
MATERIALS = $(subst .png,.material,$(IMAGES))

all: $(MATERIALS) $(MESHES)

$(MATERIALS): %.material: %.png
	m4 -DNAME=`echo "$<" | sed -e 's/.png//g'` -DIMAGE=`echo "$<" | sed -e 's/.*\///g'` template.material > $@

clean: 
	rm -f $(MATERIALS)
