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

version = 1

def write_some_data(context, filepath, use_some_setting):
    # Grab Active Object (Must be the Mesh)
    obj = bpy.context.active_object
    
    if obj.type != "MESH":
        print("Not a Mesh!")
        return {'CANCELLED'}
    
    # Save Current Mode
    old_mode = obj.mode
    
    # Change Modes
    bpy.ops.object.mode_set(mode="EDIT")
    bpy.ops.mesh.select_all(action="SELECT")
    bpy.ops.mesh.quads_convert_to_tris()
    bpy.ops.object.mode_set(mode="OBJECT")
    
    # Grabing Armature
    if bpy.data.armatures == None:
        print("There is no armatures\n")
        return {'CANCELLED'}
    
    armature = bpy.data.armatures[0]
    
    # Grab Mesh from object
    mesh = obj.to_mesh(bpy.context.scene, True, 'PREVIEW')
    # Grab 4x4 World Matrix from object
    mat4 = obj.matrix_world.to_4x4()
    # Grab vertices from mesh
    vert = mesh.vertices
    # Grab Layers from mesh
    if mesh.uv_layers.data == None:
        print("There is no uv map for the mesh\n")
        return {"CANCELLED"}
    layer = mesh.uv_layers[0].data
    # Grab Faces
    face = mesh.polygons
    
    # Opening file
    file = open(filepath, "w")
    
    # writing version to the mesh format
    file.write("version %d\n"%(version))
    
    # Bones Section
    file.write("bone\n")
    
    i = 0
    
    bone_table = {}
    
    for b in armature.bones:
        file.write("%d %s\n"%(i, b.name))
        bone_table[b.name] = i
        i += 1
    
    file.write("end\n")
    # Transform Section
    file.write("transform\n")
    file.write("%f %f %f %f\n"%(mat4[0][0], mat4[2][0], mat4[1][0], mat4[3][0]))
    file.write("%f %f %f %f\n"%(mat4[0][2], mat4[2][2], mat4[1][2], mat4[3][2]))
    file.write("%f %f %f %f\n"%(mat4[0][1], mat4[2][1], mat4[1][1], mat4[3][1]))
    file.write("%f %f %f %f\n"%(mat4[0][3], mat4[2][3], mat4[1][3], mat4[3][3]))
    file.write("end\n")
    # vertex section
    file.write("vertex\n")
    for v in vert:
        file.write("%d %f %f %f %f %f %f\n"%(v.groups[0].group, v.co[0], v.co[2], v.co[1], v.normal[0], v.normal[2], v.normal[1]))
    
    file.write("end\n")
    # texCoord section
    file.write("texCoord\n")
    for tc in layer:
        file.write("%f %f\n"%(tc.uv[0], tc.uv[1]))
    
    file.write("end\n")
    # face section
    file.write("face\n")
    for f in face:
        file.write("%d %d %d %d %d %d\n"%(f.vertices[0], f.vertices[2], f.vertices[1], f.loop_indices[0], f.loop_indices[2], f.loop_indices[1]))
    
    file.write("end\n")
    # Close file
    file.close()
    
    # Change Back to 
    bpy.ops.object.mode_set(mode="EDIT")
    bpy.ops.mesh.select_all(action="SELECT")
    bpy.ops.mesh.tris_convert_to_quads()
    bpy.ops.object.mode_set(mode=old_mode)
    
    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator

class ExportAnimMeshDev(Operator, ExportHelper):
    """This appears in the tooltip of the operator and in the generated docs"""
    bl_idname = "export_test.anim_mesh_dev"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Export Animated Mesh"

    # ExportHelper mixin class uses this
    filename_ext = ".amesh"

    filter_glob = StringProperty(
            default="*.amesh",
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
    self.layout.operator(ExportAnimMeshDev.bl_idname, text="Export Animated Mesh Development")


def register():
    bpy.utils.register_class(ExportAnimMeshDev)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportAnimMeshDev)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()

    # test call
    bpy.ops.export_test.anim_mesh_dev('INVOKE_DEFAULT')
