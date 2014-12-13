'''
The MIT License (MIT)

Copyright (c) 2014 Frederick Cook <a.o.d.6060@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
'''

import bpy

bl_info = {
    "name": "Static Mesh Format Development",
    "author": "Fred R. Cook",
    "blender": (2, 59, 0),
    "location": "File > Import-Export",
    "description": "This is the developmental version of the static format for my first person opensource game.",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "support": 'TESTING',
    "category": "Import-Export"}
    
version = 1
def write_some_data(context, filepath, use_some_setting):
    # Object
    obj = bpy.context.active_object
    # Check to see if the object is a mesh
    if obj.type != "MESH":
        print("Not a Mesh\n")
        return {'FINISHED'}
    oldmode = obj.mode
    
    #change mode to edit
    #and convert to tris
    bpy.ops.object.mode_set(mode="EDIT")
    bpy.ops.mesh.select_all(action="SELECT")
    bpy.ops.mesh.quads_convert_to_tris()
    bpy.ops.object.mode_set(mode="OBJECT")
    
    #grab data from mesh
    mesh = obj.to_mesh(bpy.context.scene, True, 'PREVIEW')
    
    mat4 = obj.matrix_world.to_4x4()
    
    if mesh.uv_layers is None:
        print("You need to have a uv map.\n")
        return {'FINISHED'}
    
    #grab arrays from mesh object
    vert = mesh.vertices
    layer = mesh.uv_layers[0].data
    poly = mesh.polygons
    
    #open file
    file = open(filepath, "w")
    
    #write version number to file
    file.write("version %d\n"%(version))
    #write transform section
    file.write("transform\n")
    file.write("%f %f %f %f\n"%(mat4[0][0], mat4[2][0], mat4[1][0], mat4[3][0]))
    file.write("%f %f %f %f\n"%(mat4[0][2], mat4[2][2], mat4[1][2], mat4[3][2]))
    file.write("%f %f %f %f\n"%(mat4[0][1], mat4[2][1], mat4[1][1], mat4[3][1]))
    file.write("%f %f %f %f\n"%(mat4[0][3], mat4[2][3], mat4[1][3], mat4[3][3]))
    file.write("end\n")
    #write vertex section
    file.write("vertex\n")
    for v in vert:
        file.write("%f %f %f\n"%(v.co[0], v.co[2], v.co[1]))
    file.write("end\n")
    #write normal section (note: vertex normals)
    file.write("normal\n")
    for n in vert:
        file.write("%f %f %f\n"%(n.normal[0], n.normal[2], n.normal[1]))
    file.write("end\n")
    #write texCoord section
    file.write("texCoord\n")
    for tc in layer:
        file.write("%f %f\n"%(tc.uv[0], tc.uv[1]))
    file.write("end\n")
    #write face section
    file.write("face\n")
    for f in poly:
        file.write("%d %d %d %d %d %d\n"%(f.vertices[0], f.vertices[2], f.vertices[1], f.loop_indices[0], f.loop_indices[2], f.loop_indices[1]))
    file.write("end\n")
    
    #change mode back to normal
    bpy.ops.object.mode_set(mode="EDIT")
    bpy.ops.mesh.select_all(action="SELECT")
    bpy.ops.mesh.tris_convert_to_quads()
    
    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator


class ExportStaticMesh(Operator, ExportHelper):
    """This appears in the tooltip of the operator and in the generated docs"""
    bl_idname = "export_test.static_mesh_dev"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Export Shooter Static Mesh Development"

    # ExportHelper mixin class uses this
    filename_ext = ".smesh"

    filter_glob = StringProperty(
            default="*.smesh",
            options={'HIDDEN'},
            )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    use_setting = BoolProperty(
            name="Example Boolean",
            description="Example Tooltip",
            default=True,
            )

    type = EnumProperty(
            name="Example Enum",
            description="Choose between two items",
            items=(('OPT_A', "First Option", "Description one"),
                   ('OPT_B', "Second Option", "Description two")),
            default='OPT_A',
            )

    def execute(self, context):
        return write_some_data(context, self.filepath, self.use_setting)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportStaticMesh.bl_idname, text="Export Shooter Static Mesh Development")


def register():
    bpy.utils.register_class(ExportStaticMesh)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportStaticMesh)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()

    # test call
    bpy.ops.export_test.static_mesh_dev('INVOKE_DEFAULT')
