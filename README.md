# XSFML
eXtensible Source for Markup Language

## Basic Usage

```cpp

const char* SOURCE = R"(
@ Materials
(
	DefaultMaterial: "Default"
	

	@
		(
			Name: "Default"
			DiffuseColor: "#FFF"
			DiffuseTexture: "../df_tex_color.png"
			Transparency: "true"
		)
		
	@
		(
			Name: "Alternative"
			DiffuseColor: "#F33446"
			DiffuseTexture: "../df_tex_color_alt.png"
			Transparency: "false"
		)

)


@ MeshNode (
	Parent: "Root"
	Origin: "0,0,0"
	Scale: "1,1,1"
	Rotation: "0,0,0"
	
	@ (Mat: "Default" Pos: "0,0,0" Indx: "0")
	@ (Mat: "Default" Pos: "1,0,0" Indx: "1")
	@ (Mat: "Default" Pos: "1,1,0" Indx: "2")
	@ (Mat: "Default" Pos: "0,1,0" Indx: "3")
)

@ Root ()

)";
```

```cpp
XSFML::XSFMLDocument doc;

XSFML::XSFML_LOAD(doc, SOURCE);
XSFML::XSFML_PARSE(doc);


for (size_t i = 0; i < doc.blocks.size(); i++) {
    std::cout << "Block: " << doc.blocks[i].name << " Size: " << doc.blocks[i].attributes.size() << " [ID: " << doc.blocks[i]._id << " Anonymous?: " << doc.blocks[i].isAnonymous << "]" << std::endl;
    

   for (auto k = doc.blocks[i].attributes.begin(); k != doc.blocks[i].attributes.end(); k++) {
            std::cout << "\tAttribute: " << k->first << " = " << k->second << std::endl;
   }

   std::cout << "\n" << std::endl;
}

XSFML::XSFML_CLEAR(doc);
```

```
Exited with 105 tokens parseds!
Block: Materials@0 Size: 4 [ID: 0 Anonymous?: 1]
        Attribute: Name = Default
        Attribute: DiffuseColor = #FFF
        Attribute: DiffuseTexture = ../df_tex_color.png
        Attribute: Transparency = true


Block: Materials@1 Size: 4 [ID: 0 Anonymous?: 1]
        Attribute: Name = Alternative
        Attribute: DiffuseColor = #F33446
        Attribute: DiffuseTexture = ../df_tex_color_alt.png
        Attribute: Transparency = false


Block: Materials Size: 1 [ID: 0 Anonymous?: 0]
        Attribute: DefaultMaterial = Default


Block: MeshNode@0 Size: 3 [ID: 3 Anonymous?: 1]
        Attribute: Mat = Default
        Attribute: Pos = 0,0,0
        Attribute: Indx = 0


Block: MeshNode@1 Size: 3 [ID: 3 Anonymous?: 1]
        Attribute: Mat = Default
        Attribute: Pos = 1,0,0
        Attribute: Indx = 1


Block: MeshNode@2 Size: 3 [ID: 3 Anonymous?: 1]
        Attribute: Mat = Default
        Attribute: Pos = 1,1,0
        Attribute: Indx = 2


Block: MeshNode@3 Size: 3 [ID: 3 Anonymous?: 1]
        Attribute: Mat = Default
        Attribute: Pos = 0,1,0
        Attribute: Indx = 3


Block: MeshNode Size: 4 [ID: 3 Anonymous?: 0]
        Attribute: Parent = Root
        Attribute: Origin = 0,0,0
        Attribute: Rotation = 0,0,0
        Attribute: Scale = 1,1,1


Block: Root Size: 0 [ID: 8 Anonymous?: 0]

```
