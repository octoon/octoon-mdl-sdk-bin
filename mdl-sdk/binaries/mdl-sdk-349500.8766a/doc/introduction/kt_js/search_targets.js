var search_targets = 
[/*jshint multistr: true */

  ["1 Background", [
    ["mdl_introduction", "1001", "Renderers produce images from scene descriptions. A scene description consists of three-dimensional geometric objects and their positioning in space. Common object representations describe objects by their surface geometry, which sometimes only represent surfaces in space and sometimes represent volumetric objects that are enclosed by the surface description."],
    ["mdl_introduction", "1002", "Surfaces and volumes have material properties that determine how they interact with light and, ultimately, how they are rendered in an image. Material properties range from the color of surfaces, to their reflection or refraction properties, light emission of surfaces, scattering and absorption properties of volumes, and even to additional geometric properties of surfaces, such as cut-outs, displacements, or bump maps, which are commonly not modeled in the primary geometric description."],
    ["mdl_introduction", "1003", "For greatest flexibility, rendering systems often use programming languages to describe material properties. These can be general purpose languages, such as C, or domain specific languages, commonly referred to as shading languages, since material authors can program how the actual shading of a surface is computed from material parameters, incoming light, and more. Shader programmers can and do go to the extreme of writing full renderers in shader programs. However, modern renderers can implement techniques such as multiple importance sampling only if they understand the material properties. This analysis is difficult with traditional shading languages, since they define the computation for the shading result itself and not just the material properties. Material Definition Language (MDL) is a domain-specific programming language that can define material properties to serve modern renderers in this sense."],
    ["mdl_introduction", "1004", "This document gives you a functional overview of MDL. The next section provides a quick overview of key features. The following sections provide more detail about those features."]
  ]],
  ["2 Overview of key MDL features", [
    ["mdl_introduction", "1005", "MDL consists of two major parts:"],
    ["mdl_introduction", "1005", "A declarative material definition based on a powerful material model"],
    ["mdl_introduction", "1005", "A procedural programming language to define functions that can compute values for the parameters of the material model"],
    ["mdl_introduction", "1006", "The declarative material definition defines what to compute, not how to compute it, while the procedural programming language preserves the infinite flexibility for creative material authors to define material properties with their own programs."],
    ["mdl_introduction", "1007", "Key features:"],
    ["mdl_introduction", "1007", "MDL is independent of rendering algorithms. The declarative material definition interfaces with a renderer through the underlying material model, which is purely descriptive in terms of physical material properties and agnostic of any renderer algorithm."],
    ["mdl_introduction", "1007", "MDL supports the needs of modern rendering algorithms with a well-defined material model and enough material properties to support path tracing or multiple importance sampling."],
    ["mdl_introduction", "1007", "MDL is supported across a series of renderers developed by NVIDIA and it is designed to be extensible and adoptable by other renderers."],
    ["mdl_introduction", "1007", "Material definitions in MDL can be parameterized to enable flexible, custom-built domain-specific material libraries."],
    ["mdl_introduction", "1007", "MDL has a well-defined module and package concept to support packaging and distribution of material libraries."],
    ["mdl_introduction", "1007", "MDL supports the re-use and further customization of additional materials."],
    ["mdl_introduction", "1007", "MDL supports the customization of materials with predefined parameter sets, such that a material library for a certain material family can be based on one generic material and various parameter sets for specific instances of that material. When such a predefined parameter set is used, it can then still be modified to further adjust the look of the material."],
    ["mdl_introduction", "1007", "The renderer state and the standard modules defined by MDL enable material authors to program a wide variety of functions to initialize material parameters, including procedural methods, noise-based textures, texture projection maps, and texture blend pipelines."],
    ["mdl_introduction", "1007", "MDL is designed for modern highly-parallel machine architectures. The procedural language only allows the definition of pure functions that have access to rendering state and that are free of side effects and global dependencies. These functions can therefore be scheduled and executed depending on the needs of the renderer and machine architecture."],
    ["mdl_introduction", "1008", "Although it emphasizes physically plausible materials, MDL supports traditional computer graphics techniques that are important in conventional rendering and modeling workflows. A MDL material is, in general, applied to a surface and consists of the following properties:"],
    ["mdl_introduction", "1008", "Surface properties that describe the <i>bidirectional scattering distribution function </i> (BSDF) for reflective and transmissive properties, the <i>emissive distribution function </i> (EDF) for emissive properties and the <i>radiant exitance </i> for the amount of emission."],
    ["mdl_introduction", "1008", "A Boolean flag that specifies whether the surface encloses a volume, and thereby defines if the volume properties apply to the enclosed volume or should be ignored."],
    ["mdl_introduction", "1008", "Additional surface properties that can be used to define the back-side material for surfaces that do not enclose a volume."],
    ["mdl_introduction", "1008", "Volume properties that describe the scattering and absorption coefficients and the <i>volume distribution function </i> (VDF), also known as the <i>volume phase function </i>."],
    ["mdl_introduction", "1008", "Geometric properties that describe cut-outs, displacement mapping, and normal mapping."],
    ["mdl_introduction", "1008", "BSDF properties that are set to values composed from a fixed set of elemental BSDFs and operators on them. These operators can be modifiers for tinting or thin-film effect, or they can be combining functions for a weighted mix or layering of BSDFs including Fresnel effects. The layering operator supports an additional normal mapping effect local to the top image. These combinators make this a very flexible material model."],
    ["mdl_introduction", "1008", "EDF and VDF properties can be composed similar to BSDFs."]
  ]],
  ["3 Comparing MDL to shading languages", [
    ["mdl_introduction", "1009", "The use of conventional shading languages can be roughly categorized as follows:"],
    ["mdl_introduction", "1009", "They are used to implement functions to describe the spatial distribution of material features - texture lookups, procedural textures, and projections."],
    ["mdl_introduction", "1009", "They are used to provide re-usable building blocks, called <i>shaders </i>, that describe materials with parameters that correspond to material properties, such as the reflective, emissive and transmissive behavior of an object."],
    ["mdl_introduction", "1009", "They are used to implement the computations needed in shaders, such as light-material interactions or other algorithmic extensions of the renderer. Shaders may even implement full-scale rendering computations, such as volume renderers, particle renderers or complete global illumination solutions."],
    ["mdl_introduction", "1010", "While these areas are typically used by different audiences and require different user skills, traditional shading languages do not explicitly distinguish between these use cases. MDL clearly separates those domains to address the specific needs of the different audiences."],
    ["mdl_introduction", "1011", "In the three numbered areas above, the first corresponds to what MDL offers with its procedural language for implementing functions. The second area corresponds to what MDL offers with its material definition and rich, highly configurable material model. The third area has no correspondence in MDL and is seen as the domain of the renderer."]
  ]],
  ["4 Language elements and re-use", [
    ["mdl_introduction", "1012", "Functions, materials with their components, and modules are the main language elements that MDL offers for material libraries and re-use."],
    ["mdl_introduction", "1013", "Functions can be used individually for the parameters of a renderer, for example, the environment color, or to provide values for material parameters such as a texture lookup for a color parameter for a material. Functions can also provide a mechanism for code re-use and data hiding by encapsulating other functions. The encapsulated functions can be provided with explicit parameter values within the body of the encapsulating function. The resulting function can implemented as a complex calculation with a simpler signature that provides control only for those parameters significant for the intended effect."],
    ["mdl_introduction", "1014", "Materials and their BSDF, EDF, and VDF properties are the main building blocks in MDL. They can be passed to other materials as parameters, which allows the development of generic materials that re-use aspects of a material and extend or change other aspects. For example, a new material could be based on a generic material that takes another material as input and changes it to a thin-walled material by applying the surface properties of the other material on both sides of the surface."],
    ["mdl_introduction", "1015", "A material definition can define input parameters that can be used within the material in expressions and function call parameters to initialize properties of the material model or of other already existing materials. Parameterizing a material definition enables the encapsulation and customization of materials to create custom material libraries."],
    ["mdl_introduction", "1016", "MDL supports the customization of materials with predefined parameter sets, so that a material library for a certain material family can be based on one generic material and various parameter sets for specific instances of that material. When such a predefined parameter set is used, it can then still be modified to further adjust the appearance of the material. For example, a general metal material can be used with a family of parameter sets to offer a material library of different gold and silver alloys. A user can pick a specific material from the library but still modify the final color."],
    ["mdl_introduction", "1017", "All files in MDL are defined to be modules. They define a namespace and shield identifiers from name clashes, which is useful if material libraries are deployed and used together with other libraries or further in-house material developments. Modules can contain materials, functions, and related types and constants."]
  ]],
  ["5 Materials", [
    ["mdl_introduction", "1018", "MDL provides a purely declarative syntax for describing the interaction of an object with light. It relies on predefined building blocks that can be efficiently implemented in a modern renderer, yet are powerful enough to describe a large majority of real-world materials. The renderer-side interface of the material, the material model, is represented by a MDL built-in structure-like type, which contains the different properties of the material:"],
    ["mdl_introduction", "1020", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1023", "<pre>struct material {<br>\
     uniform bool      thin_walled = false;<br>\
     material_surface  surface     = material_surface();<br>\
     material_surface  backface    = material_surface();<br>\
     uniform color     ior         = color(1.0);<br>\
     material_volume   volume      = material_volume();<br>\
     material_geometry geometry    = material_geometry();<br>\
 }; </pre>"],
    ["mdl_introduction", "1024", "Depending on the building blocks used, three different kinds of materials can be distinguished:"],
    ["mdl_introduction", "1024", "Regular materials describing a surface that separates one volume from another"],
    ["mdl_introduction", "1024", "Thin-walled materials where the geometry only describes a soap-bubble like shell"],
    ["mdl_introduction", "1024", "Two-sided materials which also have the thin-walled property, but interact differently with light hitting the geometry from the back side"],
    ["mdl_introduction", "1025", "The surface, emission, volume, and geometry aspects are also defined by built-in structure-like types:"],
    ["mdl_introduction", "1027", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1030", "<pre>struct material_surface {<br>\
     bsdf              scattering = bsdf();<br>\
     material_emission emission   = material_emission();<br>\
 }; </pre>"],
    ["mdl_introduction", "1032", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1035", "<pre>struct material_emission {<br>\
     edf            emission  = edf();<br>\
     color          intensity = color(0.0);<br>\
     intensity_mode mode      = intensity_radiant_exitance;<br>\
 }; </pre>"],
    ["mdl_introduction", "1037", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1040", "<pre>struct material_volume {<br>\
     vdf   scattering             = vdf();<br>\
     color absorption_coefficient = 0.0;<br>\
     color scattering_coefficient = 0.0;<br>\
 }; </pre>"],
    ["mdl_introduction", "1042", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1045", "<pre>struct material_geometry {<br>\
     float3 displacement   = float3(0.0);<br>\
     float  cutout_opacity = 1.0;<br>\
     float3 normal         = state::normal();<br>\
 }; </pre>"]
  ]],
  ["5.1 Elemental distribution functions", [
    ["mdl_introduction", "1046", "To describe the interaction with light, MDL provides a set of <i>elemental distribution functions </i>. The names of the distribution functions end with one of three suffixes:"],
    ["mdl_introduction", "1049", "<tt>bsdf</tt>"],
    ["mdl_introduction", "1050", "Describes the interaction of the light with the surface"],
    ["mdl_introduction", "1051", "<tt>edf</tt>"],
    ["mdl_introduction", "1052", "Describes the emissive properties of the surface"],
    ["mdl_introduction", "1053", "<tt>vdf</tt>"],
    ["mdl_introduction", "1054", "Describes the light distribution in the volume"],
    ["mdl_introduction", "1055", "The elemental distribution functions are used as components of materials. The following three sections use these functions in minimal materials to demonstrate their visual properties."]
  ]],
  ["5.1.1 Bidirectional scattering distribution functions", [
    ["mdl_introduction", "1056", "The elemental BSDFs define the action of light at the surface of an object: how it is reflected from the surface and transmitted through the surface."],
    ["mdl_introduction", "1059", "Fig. __CH.__FG - <tt>diffuse_reflection_bsdf</tt>"],
    ["mdl_introduction", "1061", "<tt>diffuse_reflection_bsdf</tt> A colored diffuse reflection component using roughness based on the Oren-Nayar model. This BSDF also implements pure Lambertian reflection."],
    ["mdl_introduction", "1064", "Fig. __CH.__FG - <tt>diffuse_transmission_bsdf</tt>"],
    ["mdl_introduction", "1066", "<tt>diffuse_transmission_bsdf</tt> A colored diffuse Lambertian transmission component."],
    ["mdl_introduction", "1069", "Fig. __CH.__FG - <tt>specular_bsdf</tt>"],
    ["mdl_introduction", "1071", "<tt>specular_bsdf</tt> A component representing colored specular reflection, specular transmission (refraction) or a combination of both. This image demonstrates pure specular reflection."],
    ["mdl_introduction", "1074", "Fig. __CH.__FG - <tt>specular_bsdf</tt>"],
    ["mdl_introduction", "1076", "<tt>specular_bsdf</tt> The use of the <tt>specular_bsdf</tt> in this image demonstrates pure specular transmission. The index of refraction is controlled as a parameter to the BSDF."],
    ["mdl_introduction", "1079", "Fig. __CH.__FG - <tt>specular_bsdf</tt>"],
    ["mdl_introduction", "1081", "<tt>specular_bsdf</tt> This image combines both the reflective and transmissive factors of <tt>specular_bsdf</tt>. If transmission and reflection are enabled at the same time, they are combined using a Fresnel term based on the index of refraction specified for the volume. This image also demonstrates how the color parameter of a distribution function can scale the function's result."],
    ["mdl_introduction", "1084", "Fig. __CH.__FG - <tt>simple_glossy_bsdf</tt>"],
    ["mdl_introduction", "1086", "<tt>simple_glossy_bsdf</tt> A component representing colored glossy reflection, glossy transmission or a combination of both. As in the specular case, if transmission and reflection are enabled at the same time, they are combined using the Fresnel term."],
    ["mdl_introduction", "1089", "Fig. __CH.__FG - <tt>backscattering_glossy_reflection_bsdf</tt>"],
    ["mdl_introduction", "1091", "<tt>backscattering_glossy_reflection_bsdf</tt> A colored glossy-reflection component capable of simulating back-scattering of light."]
  ]],
  ["5.1.2 Emissive distribution functions", [
    ["mdl_introduction", "1092", "MDL defines a set of elemental emission distribution functions to simulate the light that interacts with MDL's surface and volume definitions. The traditional computer graphics methods of lighting a scene with special purpose constructs (like \"directional lights\" and \"point lights\") is replaced in MDL by defining properties of geometric objects so that they emit light. In this way, the syntactic expression of lighting is unified with the expression of surface and volume appearance. As geometric objects, \"lights\" in MDL are also more readily integrated into the scene definition interface of 3D applications."],
    ["mdl_introduction", "1095", "Fig. __CH.__FG - <tt>diffuse_edf</tt>"],
    ["mdl_introduction", "1097", "<tt>diffuse_edf</tt> Light emitted in all directions from the surface of the object, called <i>Lambertian light distribution </i> by analogy to Lambertian diffuse reflection. The spherical geometric object defined with the emissive material is visible in the image, but without the typical perceptual or photograph effects that provide \"bloom\" or lens distortions."],
    ["mdl_introduction", "1100", "Fig. __CH.__FG - <tt>spot_edf</tt>"],
    ["mdl_introduction", "1102", "<tt>spot_edf</tt> Distribution of the emission based on the cosine between emission direction and surface normal (exponential cosine distribution). The emissive object is visible as a dark gray sphere because the camera is located at the edge of the cone of light distribution."],
    ["mdl_introduction", "1105", "Fig. __CH.__FG - <tt>measured_edf</tt>"],
    ["mdl_introduction", "1107", "<tt>measured_edf</tt> Light distribution based on a measured light profile. Arbitrary geometric structure for light distribution from the emissive object can be specified based on standard industrial formats called <i>light profiles </i>. Here the sharper boundary of the spatial distribution of light energy allows the light reflected by the diffuse surface of the object to be seen as a reddish color in the shadowed area."]
  ]],
  ["5.1.3 Volume distribution functions", [
    ["mdl_introduction", "1108", "BSDFs define the action of light at the object's surface. Volume distribution functions (VDFs) specify the behavior of light within an object. In combination, they provide for all the possibilities of light reflection, transmission, and interior modulation for lighting simulation."],
    ["mdl_introduction", "1111", "Fig. __CH.__FG - <tt>anisotropic_vdf</tt>"],
    ["mdl_introduction", "1113", "<tt>anisotropic_vdf</tt> Addition of absorption effects and subsurface scattering to the material's volume. The BSDF for this material specifies that all light penetrates the object ( <i>transmission scattering mode </i>)."],
    ["mdl_introduction", "1116", "Fig. __CH.__FG - <tt>anisotropic_vdf</tt>"],
    ["mdl_introduction", "1118", "<tt>anisotropic_vdf</tt> Parametric control of the volume distribution function can simulate a wide variety of physical substances. The material of this image only differs from the material of the previous image in the BSDF specifying both reflection and transmission of the light, and in the values of three VDF parameters that define the index of refraction, the degree of scattering, and the scattering direction within the object."],
    ["mdl_introduction", "1121", "Fig. __CH.__FG - <tt>anisotropic_vdf</tt>"],
    ["mdl_introduction", "1123", "<tt>anisotropic_vdf</tt> Even with apparently simple materials made from the elemental BSDFs, the design of the lighting simulation can reveal complex properties of the material. Here the material of the object, <tt>anisotropic_vdf</tt>, is identical to the previous example, lit by the emmisive distribution function <tt>measured_edf</tt> of the previous section."]
  ]],
  ["5.1.4 Measured materials", [
    ["mdl_introduction", "1124", "The previous definitions of light transport used analytic methods which depend upon a mathematical theory of the physics of light. MDL also supports material definitions based on measurement of real-world surfaces, where light interaction with objects is measured over a series of different incident light and viewing angles. The resulting dataset is used as input to an elemental BSDF in MDL that defines a <i>measured BSDF </i>."],
    ["mdl_introduction", "1125", "A measurement dataset can be used as an MDL input parameter by constructing a value of type <tt>bsdf_measurement</tt>. The <tt>bsdf_measurement</tt> constructor takes a file path of a measurement dataset as an input argument:"],
    ["mdl_introduction", "1127", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1130", "<pre>bsdf_measurement(uniform string name) </pre>"],
    ["mdl_introduction", "1131", "A value of <tt>bsdf_measurement</tt> is used as the argument for the <tt>measurement</tt> parameter of the BSDF <tt>measured_bsdf:</tt>"],
    ["mdl_introduction", "1133", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1136", "<pre>bsdf measured_bsdf (<br>\
     uniform bsdf_measurement measurement,<br>\
     uniform float            multiplier = 1.0,<br>\
     uniform scatter_mode     mode       = scatter_reflect,<br>\
     uniform string           handle     = \"\"<br>\
 ); </pre>"],
    ["mdl_introduction", "1139", "Fig. __CH.__FG - <tt>measured_bsdf</tt>"],
    ["mdl_introduction", "1141", "<tt>measured_bsdf</tt> The red material is defined by a measurement of a car paint surface. The materials of the pedestal and ground are defined by measurements of carpet and cement, respectively. Accompanying texture maps supply the level of fine color detail for the carpet and cement."],
    ["mdl_introduction", "1144", "Fig. __CH.__FG - <tt>measured_bsdf</tt>"],
    ["mdl_introduction", "1146", "<tt>measured_bsdf</tt> In this image the pedestal and ground materials have been exchanged. The geometric models define a parametric space for the texture mapping, which is combined with scaling factors for the texture application provided by the material. The blue car paint here uses the same material as the previous example, with a different measured data set provided as the value of the <tt>name</tt> parameter to <tt>bsdf_measurement</tt>."]
  ]],
  ["5.2 Distribution function modifiers and combiners", [
    ["mdl_introduction", "1147", "The previous sections showed simple materials built from the elemental distribution functions based on analytic methods and real-world measurements. MDL also provides building blocks to combine or modify BSDFs to enable the description of more complex light-material interactions. The elemental EDFs can also be mixed together to implement combined emission properties in a single material."],
    ["mdl_introduction", "1148", "Distribution function modifiers and combiners can describe complex reflective and transmissive characteristics (compound distribution functions). Because of their uniform syntax, analytic and measured materials can also be combined. Modifiers can be applied recursively, allowing further modification and combination of compounds."],
    ["mdl_introduction", "1149", "<tt>normalized_mix</tt>"],
    ["mdl_introduction", "1150", "Mix N elemental or compound distribution functions based on N float weights. If the sum of the weights exceeds 1, they are normalized."],
    ["mdl_introduction", "1151", "<tt>clamped_mix</tt>"],
    ["mdl_introduction", "1152", "Mix N elemental or compound distribution functions based on N float weights. Distribution functions and weights are summed in the order they are given. When the sum of weights reaches 1, the remainder, if any, is clamped."],
    ["mdl_introduction", "1153", "<tt>weighted_layer</tt>"],
    ["mdl_introduction", "1154", "Add an elemental or compound BSDF as a layer on top of another elemental or compound BSDF according to a weight w. The base is weighted with 1-w."],
    ["mdl_introduction", "1155", "<tt>fresnel_layer</tt>"],
    ["mdl_introduction", "1156", "Add an elemental or compound BSDF as a layer on top of another elemental or compound BSDF according to a weight w and a Fresnel term using a dedicated index of refraction for the layer. The base is weighted with 1-(w*fresnel(ior))."],
    ["mdl_introduction", "1157", "<tt>custom_curve_layer</tt>"],
    ["mdl_introduction", "1158", "Add an elemental or compound BSDF as a layer on top of another elemental or compound BSDF according to a weight w and a Schlick-style directional-dependent curve function. The base is weighted with 1-(w*curve())."],
    ["mdl_introduction", "1159", "<tt>measured_curve_layer</tt>"],
    ["mdl_introduction", "1160", "Add an elemental or compound BSDF as a layer on top of another elemental or compound BSDF according to a weight w and a directional-dependent measurement of the reflection behavior. The base is weighted with 1-(w*measurement())."],
    ["mdl_introduction", "1161", "<tt>tint</tt>"],
    ["mdl_introduction", "1162", "Tint the result of an elemental or compound distribution function with an additional color."],
    ["mdl_introduction", "1163", "<tt>thin_film</tt>"],
    ["mdl_introduction", "1164", "Add reflective thin film interference color to an elemental or compound BSDF."],
    ["mdl_introduction", "1165", "<tt>directional_factor</tt>"],
    ["mdl_introduction", "1166", "A direction-dependent weight based on a custom curve that is applied to one elemental or compound BSDF."],
    ["mdl_introduction", "1167", "<tt>measured_curve_factor</tt>"],
    ["mdl_introduction", "1168", "A direction-dependent weight based on a measured reflection curve that is applied to one elemental or compound BSDF."],
    ["mdl_introduction", "1169", "The following series demonstrates the sequential combination of a series of BSDFs. Each successive image adds a new layer to the previous one."],
    ["mdl_introduction", "1172", "Fig. __CH.__FG - The base layer is defined by <tt>diffuse_reflection_bsdf</tt> with a tinting color of red."],
    ["mdl_introduction", "1175", "Fig. __CH.__FG - A yellow-tinted <tt>diffuse_reflection_bsdf</tt> is added to the edges with <tt>custom_curve_layer</tt>."],
    ["mdl_introduction", "1178", "Fig. __CH.__FG - A layer of <tt>simple_glossy_bsdf</tt> is added using <tt>weighted_layer</tt>."],
    ["mdl_introduction", "1181", "Fig. __CH.__FG - Another layer of <tt>simple_glossy_bsdf</tt> with sharper highlights is added with <tt>weighted_layer</tt>."],
    ["mdl_introduction", "1184", "Fig. __CH.__FG - Adding a layer of <tt>specular_bsdf</tt> with <tt>fresnel_layer</tt> resembles the clear coat of an automotive finish."],
    ["mdl_introduction", "1185", "Analytic and measured materials can also be combined. Measurement devices for light scattering may have difficulty accurately recording intensities at extreme grazing angles. For example, the measured car paint renderings lack the glossy reflections that are typical at edges. Defining this reflection as a layer to be combined with the measured BSDF can produce the missing effect."],
    ["mdl_introduction", "1190", "Fig. __CH.__FG - Only using measurement data"],
    ["mdl_introduction", "1193", "Fig. __CH.__FG - Adding edge reflection as a layer"]
  ]],
  ["5.3 MDL syntax", [
    ["mdl_introduction", "1194", "MDL provides a notation inspired by functional programming to create a custom material struct and then map an input parameter set to this material. The following example describes a simple material exhibiting Lambertian reflective properties and an input providing a <tt>diffuse_color</tt> input defaulting to red:"],
    ["mdl_introduction", "1196", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1199", "<pre>material diffuse (<br>\
     color diffuse_color = color(0.7,0.0,0.0) )<br>\
 = material (<br>\
     surface :<br>\
         material_surface ( <br>\
             scattering :<br>\
                 diffuse_reflection_bsdf (<br>\
                 tint : diffuse_color ))); </pre>"],
    ["mdl_introduction", "1200", "Note here that MDL structs can have defaults for their members and that the example only provides a value for the <tt>surface</tt> member of the material. All other members of the material struct maintain their default values."],
    ["mdl_introduction", "1201", "A similar syntax can be used to change the parameterization or interface of an already existing material. The following example code creates a blue variant of the preceding example:"],
    ["mdl_introduction", "1203", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1206", "<pre>material blue_diffuse()<br>\
 = diffuse (<br>\
     diffuse_color : color(0.0,0.0,0.7) ); </pre>"],
    ["mdl_introduction", "1207", "A parameter of a material can be a material itself. Since a material functions as a struct, this permits the straightforward reuse of materials. For example, the following material takes an arbitrary base material and adds a reflective clear coat on top:"],
    ["mdl_introduction", "1209", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1212", "<pre>material add_clear_coat (<br>\
     color ior = color(1.5),<br>\
     material base )<br>\
 = material(<br>\
     volume : base.volume,<br>\
     geometry : base.geometry,<br>\
     surface :<br>\
         material_surface (<br>\
             emission : base.surface.emission,<br>\
             scattering :<br>\
                 fresnel_layer (<br>\
                     layer : specular_bsdf (<br>\
                         scatter_mode : scatter_reflect ),<br>\
                     base : base.surface.scattering,<br>\
                     ior : ior ))); </pre>"],
    ["mdl_introduction", "1213", "Measured materials are also defined in a syntactically consistent way in the material definition. The following material defines the measured blue car paint that is combined with a glossy reflection layer, shown. Note that the measured data is provided as a filename parameter of function <tt>bsdf_measurement</tt>."],
    ["mdl_introduction", "1215", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1218", "<pre>material carpaint_blue (<br>\
     float coat_ior = 1.5 )<br>\
 = material (<br>\
     surface :<br>\
         material_surface (<br>\
             scattering :<br>\
                 fresnel_layer (<br>\
                     layer : specular_bsdf (<br>\
                         mode : scatter_reflect,<br>\
                         tint : color(1.0) ),<br>\
                     base : measured_bsdf (<br>\
                         measurement :<br>\
                             bsdf_measurement(\"carpaint_blue.mbsdf\") ),<br>\
                     ior : coat_ior ))); </pre>"],
    ["mdl_introduction", "1219", "MDL provides a <i>let-expression </i> to support the introduction of local variables. The content of the expression is purely declarative, but declarations are evaluated in sequence, allowing access to already declared variables in later declarations. Using a let-expression, the <tt>add_clear_coat</tt> example could be rewritten as:"],
    ["mdl_introduction", "1221", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1224", "<pre>material add_clear_coat (<br>\
     color ior = color(1.5),<br>\
     material base = material() ) <br>\
 = let {<br>\
     bsdf coat =<br>\
         specular_bsdf (<br>\
             scatter_mode : scatter_reflect );<br>\
     bsdf coated_scattering =<br>\
         fresnel_layer (<br>\
             layer : coat,<br>\
             base : base.surface.scattering,<br>\
             ior : ior );<br>\
     material_surface coated_surface (<br>\
         emission : base.surface.emission,<br>\
         scattering : coated_scattering );<br>\
 } in material (<br>\
     volume : base.volume,<br>\
     geometry : base.geometry,<br>\
     surface : coated_surface ); </pre>"],
    ["mdl_introduction", "1225", "Using let-expressions, libraries of materials can be based on a set of small, self-defined, reusable building blocks. For example, putting a layer of rust on the surface of a painted car can be implemented as the combination of two materials."],
    ["mdl_introduction", "1227", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1230", "<pre>material rusty_carpaint( &#47;* ... *&#47; )<br>\
 = let {<br>\
     material base = carpaint_material();<br>\
     material top = rust_material();<br>\
     float blend = rust_blend_function();<br>\
 } in material (<br>\
     surface :<br>\
         material_surface (<br>\
             scattering :<br>\
                 weighted_layer (<br>\
                     layer : top.surface.scattering,<br>\
                     base : base.surface.scattering,<br>\
                     weight : blend ),<br>\
             &#47;* ... *&#47;  )); </pre>"]
  ]],
  ["6 Modules", [
    ["mdl_introduction", "1231", "Modules allow materials and functions to be packaged for re-use in independent libraries that can be used together. Name conflicts can be avoided by choosing between unqualified and qualified in module import statements."],
    ["mdl_introduction", "1232", "MDL's import mechanisms does not offer any name-conflict resolution mechanisms. The purpose of this policy is to have a well-defined module system to enable packaging and re-use of material libraries by independent providers."],
    ["mdl_introduction", "1233", "A directory is considered a package. The name of the package is the name of the directory. Note that this restricts the names of directories that are used as packages to legal identifiers."],
    ["mdl_introduction", "1234", "Modules can be contained in packages and the resulting packages can be nested, allowing comprehensive organization of complex libraries."],
    ["mdl_introduction", "1235", "Extensions to core MDL, such as standard annotations and distribution, math, texture and noise functions are provided in the form of standard modules."]
  ]],
  ["7 Functions", [
    ["mdl_introduction", "1236", "Users needing special texturing functionality are able to provide their own texturing functions written in a C-inspired procedural language. Using this language, implementation of procedural textures is possible as well as the implementation of custom uv-coordinate handling and generation. Texturing functions have access to a limited, read-only rendering state dedicated to the needs of texturing. Texturing functions are pure and free of side-effects. Together with the dedicated texturing state, this makes texturing functions independent from the renderer architecture, making it easier to use them in multiple renderers."],
    ["mdl_introduction", "1237", "Function argument initialization can use other function calls, forming a call graph that is the equivalent to shader graphs which are often provided by traditional shading languages."],
    ["mdl_introduction", "1238", "MDL supports specification of default values for function parameters and a calling syntax using named arguments to provide convenience when using texturing functions."],
    ["mdl_introduction", "1239", "The following is an example of a function with default initializers:"],
    ["mdl_introduction", "1241", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1244", "<pre>float3 texture_lookup(float2 uv, int space = 0); </pre>"],
    ["mdl_introduction", "1245", "Calling function <tt>texture_lookup</tt> in another function body might be expressed as follows:"],
    ["mdl_introduction", "1247", "Listing __CH.__LISTING"],
    ["mdl_introduction", "1250", "<pre>float3 c = texture_lookup(uv : coord); </pre>"]
  ]],
  ["8 Types", [
    ["mdl_introduction", "1251", "The type system of MDL is also inspired by the C language, with additional custom types for domain-specific use, such as vectors and matrices. MDL also provides an abstract type for colors, allowing renderers to choose their own, appropriate format for storing color information."],
    ["mdl_introduction", "1252", "The struct type plays a major role in MDL's material definition syntax. To allow convenient handling of materials, struct types have an automatic constructor which, together with the default values for struct members and the extended calling syntax for functions, allows the relevant code to be short and precise."],
    ["mdl_introduction", "1253", "Variables of an array type can be declared in two ways in MDL. The declarations differ in how the size of the array is specified."],
    ["mdl_introduction", "1253", "In the <i>size-immediate array type </i>, the size of the array is given as a constant value when the array variable is declared. This array type corresponds to the conventional array type in the C language."],
    ["mdl_introduction", "1253", "In the <i>size-deferred array type </i>, the size of the array is given as a symbolic size identifier and bound to a real size on first use. The size identifier can be used when the size of its array is required in other expressions."]
  ]],
  ["9 Annotations", [
    ["mdl_introduction", "1254", "MDL defines a mechanism called <i>annotations </i> to associate meta-data with material definitions and their components."],
    ["mdl_introduction", "1255", "Annotations can be applied to:"],
    ["mdl_introduction", "1255", "Functions"],
    ["mdl_introduction", "1255", "Function input parameters"],
    ["mdl_introduction", "1255", "Function return values"],
    ["mdl_introduction", "1255", "Struct members"],
    ["mdl_introduction", "1255", "Enumeration values"],
    ["mdl_introduction", "1255", "Material definitions"],
    ["mdl_introduction", "1255", "Material definition input parameters"],
    ["mdl_introduction", "1256", "Annotations are a standard mechanism for adding additional semantic information, such as graphical interface specification, documentation data and other integration support, to a program. MDL provides a set of standard annotations (defined in a standard MDL module) as well as a syntax for users to add custom annotations. Syntactically, MDL annotations are inspired by the syntax of <tt>C#</tt> annotations."]
  ]]
];

var section_headers = {
  "1 Background": ["mdl_introduction", "background"],
  "2 Overview of key MDL features": ["mdl_introduction", "overview-of-key-mdl-features"],
  "3 Comparing MDL to shading languages": ["mdl_introduction", "comparing-mdl-to-shading-languages"],
  "4 Language elements and re-use": ["mdl_introduction", "language-elements-and-re-use"],
  "5 Materials": ["mdl_introduction", "materials"],
  "5.1 Elemental distribution functions": ["mdl_introduction", "elemental-distribution-functions"],
  "5.1.1 Bidirectional scattering distribution functions": ["mdl_introduction", "bidirectional-scattering-distribution-functions"],
  "5.1.2 Emissive distribution functions": ["mdl_introduction", "emissive-distribution-functions"],
  "5.1.3 Volume distribution functions": ["mdl_introduction", "volume-distribution-functions"],
  "5.1.4 Measured materials": ["mdl_introduction", "measured-materials"],
  "5.2 Distribution function modifiers and combiners": ["mdl_introduction", "distribution-function-modifiers-and-combiners"],
  "5.3 MDL syntax": ["mdl_introduction", "mdl-syntax"],
  "6 Modules": ["mdl_introduction", "modules"],
  "7 Functions": ["mdl_introduction", "functions"],
  "8 Types": ["mdl_introduction", "types"],
  "9 Annotations": ["mdl_introduction", "annotations"]};
