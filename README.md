import bpy
import json
import base64
import struct
import os
# from io import BytesIO

meshes = {}
lines = {}
materials = {}
textures = {}
images = {}

x_scale = 0.001
y_scale = 0.001
z_scale = 0.001

# 加载数据


def load(input_file_path):
    with open(input_file_path, "r", encoding="utf-8") as json_file:
        json_data = json.load(json_file)
        scene = json_data["Scene"]
        scene_data = json_data["Data"]
        return scene, scene_data


scene, scene_data = load("D:/scene.json")

# 清空场景


def clear():
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.object.delete()
    collections = bpy.data.collections
    for collection in collections:
        bpy.context.scene.collection.children.unlink(collection)
    for collection in collections:
        bpy.data.collections.remove(collection)


def post_process():
    for obj in bpy.data.objects:
        if "helper|boundary" in obj.name:
            obj.hide_set(True)
            obj.hide_render = True
        elif "outline" in obj.name:
            obj.hide_set(True)
            obj.hide_render = True
        elif obj.name == "sky.box" or obj.name == "grid.ground.plane" or obj.name == "grid.lines":
            obj.hide_set(True)
            obj.hide_render = True
    # pack image
    for image in bpy.data.images:
        image.pack()
    print("setting Render")
    bpy.context.scene.render.engine = "CYCLES"
    bpy.context.scene.cycles.device = "GPU"
    bpy.context.scene.cycles.preview_samples = 500
    bpy.context.scene.cycles.use_preview_denoising = True
    bpy.context.scene.render.film_transparent = True
    print("finish post process")


def convert(value):
    tokens = value.split(',')
    return (float(tokens[0]), float(tokens[1]), float(tokens[2]))


def convert_quaternion(value):
    tokens = value.split(',')
    return (float(tokens[3]), float(tokens[0]), float(tokens[1]), float(tokens[2]))


def covert_location(value):
    tokens = value.split(',')
    return (float(tokens[0])*x_scale, float(tokens[1])*y_scale, float(tokens[2])*z_scale)


def convert_color(value):
    tokens = value.split(',')
    return (float(tokens[0]), float(tokens[1]), float(tokens[2]), 1)
# 创建材质


def create_material(mesh_node):
    material_data = None
    for mat_data in scene_data["Materials"]:
        if mat_data["Ptr"] == mesh_node["Material"]:
            material_data = mat_data
            break
    if not material_data:
        raise ValueError("没找到材质数据")
    mat_type = material_data["Type"]
    mat_name = material_data["Ptr"]
    if mat_type == "FStageMeshBasicMaterial":
        material = bpy.data.materials.new(name=mat_name)
        material.use_nodes = True
        nodes = material.node_tree.nodes
        diffuse_node = nodes.get(
            "Principled BSDF") or nodes.get("Diffuse BSDF")
        if diffuse_node:
            pass
        materials[mat_name] = material
        return material
    elif mat_type == "FStageMeshPhongMaterial":
        material = bpy.data.materials.new(name=mat_name)
        material.use_nodes = True
        nodes = material.node_tree.nodes
        diffuse_node = nodes.get(
            "Principled BSDF") or nodes.get("Diffuse BSDF")
        if diffuse_node:
            diffuse_node.inputs["Base Color"].default_value = convert_color(material_data["Diffuse"])
            if "Texture" in material_data:
                image = get_image(material_data["Texture"])
                texture_node = nodes.new(type="ShaderNodeTexImage")
                texture_node.image = image
                texture_node.location = (154, 271)
                material.node_tree.links.new(
                    texture_node.outputs["Color"], diffuse_node.inputs["Base Color"])
            else:
                # 没有贴图
                print(material_data)
        materials[mat_name] = material
        return material
    else:
        return bpy.data.materials["Material"]


def create_texture(texture_node):
    pass


def create_image(image_node):
    image_name = image_node["Ptr"]
    for texture_data in scene_data["Textures"]:
        if texture_data["Ptr"] == image_name:
            image_data = texture_data
            break
    if not image_data:
        raise ValueError("没找到贴图")
    urls = texture_data["Url"].split(",")
    width = int(urls[1][2:])
    height = int(urls[2][2:])
    decoded_data = base64.b64decode(texture_data["Data"])
    temp_file_path = os.path.join(bpy.app.tempdir, f"{image_name}.jpg")
    with open(temp_file_path, "wb") as temp_file:
        temp_file.write(decoded_data)
    image = bpy.data.images.load(temp_file_path)
    #  os.remove(temp_file_path)
    # image = bpy.data.images.new(name=image_name, width=width, height=height)
    # image.pixels = bytes(decoded_data)
    images[image_name] = image
    # pack the image into the .blend file
    # bpy.ops.image.pack({'image':image})
    return image

# 创建网格


def create_mesh(meshNode):
    for mdata in scene_data["Meshes"]:
        if mdata["Ptr"] == meshNode["Mesh"]:
            mesh = mdata
            break
    if not mesh:
        raise ValueError("没有找到mesh数据")
    positionDecoded = base64.b64decode(mesh["Position"])
    faceDecoded = base64.b64decode(mesh["Index"])
    normalDecoded = base64.b64decode(mesh["Normal"])
    uvDecoded = base64.b64decode(mesh["UV"])
    useIndex16 = mesh["UseIndex16"]
    mesh_name = mesh["Ptr"]

    v_count = int(len(positionDecoded)/12)
    n_count = int(len(normalDecoded)/12)
    uv_count = int(len(uvDecoded)/8)
    #  if v_count != n_count:
    #      print("Mesh:", mesh_name)

    if useIndex16:
        f_count = int(len(faceDecoded)/6)
    else:
        f_count = int(len(faceDecoded)/12)

    verts = []
    faces = []
    normals = []
    uvs = []

    position_values = struct.unpack(f"{v_count*3}f", positionDecoded)
    normal_values = struct.unpack(f"{n_count*3}f", normalDecoded)
    uv_values = struct.unpack(f"{uv_count*2}f", uvDecoded)
    if useIndex16:
        face_values = struct.unpack(f"{f_count*3}H", faceDecoded)
    else:
        face_values = struct.unpack(f"{f_count*3}I", faceDecoded)
    for i in range(v_count):
        idx = 3*i
        x = position_values[idx] * x_scale
        y = position_values[idx+1] * y_scale
        z = position_values[idx+2] * z_scale
        verts.append((x, y, z))
    for i in range(n_count):
        idx = 3*i
        normals.append(
            (normal_values[idx], normal_values[idx+1], normal_values[idx+2]))
    for i in range(uv_count):
        idx = 2*i
        uvs.append((uv_values[idx], uv_values[idx+1]))
    for i in range(f_count):
        idx = 3*i
        v1 = face_values[idx]
        v2 = face_values[idx+1]
        v3 = face_values[idx+2]
        faces.append((v3, v2, v1))
    bmesh = bpy.data.meshes.new(name=mesh_name)
    bmesh.from_pydata(verts, [], faces)
    bmesh.uv_layers.new(name="UVMap")
    uv_layer = bmesh.uv_layers.active.data
    for face in bmesh.polygons:
        for loop_index in face.loop_indices:
            vertex_index = bmesh.loops[loop_index].vertex_index
            uv = uvs[vertex_index]
            uv_layer[loop_index].uv = uv
            # bmesh.vertices[vertex_index].normal = normals[vertex_index]
    for loop in bmesh.loops:
        loop.normal = normals[loop.vertex_index]
    bmesh.update()
    meshes[mesh_name] = bmesh
    return bmesh
# 创建线


def create_line(lineNode):
    for ldata in scene_data["Lines"]:
        if ldata["Ptr"] == lineNode["Lines"]:
            line = ldata
            break
    if not line:
        raise ValueError("没找到line数据")

    positionDecoded = base64.b64decode(line["Position"])
    edgeDecoded = base64.b64decode(line["Index"])
    useIndex16 = line["UseIndex16"]
    v_count = int(len(positionDecoded)/12)
    if useIndex16:
        e_count = int(len(edgeDecoded)/4)
    else:
        e_count = int(len(edgeDecoded)/8)
    verts = []
    edges = []
    position_values = struct.unpack(f"{v_count*3}f", positionDecoded)
    if useIndex16:
        edge_values = struct.unpack(f"{e_count*2}H", edgeDecoded)
    else:
        edge_values = struct.unpack(f"{e_count*2}I", edgeDecoded)
    for i in range(v_count):
        idx = 3*i
        x = position_values[idx] * x_scale
        y = position_values[idx+1] * y_scale
        z = position_values[idx+2] * z_scale
        verts.append((x, y, z))
    for i in range(e_count):
        idx = 2*i
        v1 = edge_values[idx]
        v2 = edge_values[idx+1]
        edges.append((v1, v2))
    mesh = bpy.data.meshes.new(name=line["Ptr"])
    mesh.from_pydata(verts, edges, [])
    mesh.update()
    lines[line["Ptr"]] = mesh
    return mesh
# 获取材质


def get_material(mesh_node):
    if mesh_node["Material"] in materials:
        return materials[mesh_node["Material"]]
    else:
        return create_material(mesh_node)
# 获取贴图


def get_textrue(texture_node):
    if texture_node["Ptr"] in textures:
        return textures[texture_node["Ptr"]]
    else:
        return create_texture(texture_node)
# 获取贴图


def get_image(image_node):
    if image_node["Ptr"] in images:
        return images[image_node["Ptr"]]
    else:
        return create_image(image_node)
# 获取网格


def get_mesh(meshNode):
    if meshNode["Mesh"] in meshes:
        return meshes[meshNode["Mesh"]]
    else:
        return create_mesh(meshNode)
# 获取线条


def get_line(line):
    if line["Ptr"] in meshes:
        return meshes[line["Ptr"]]
    else:
        return create_line(line)

# 创建容器


def create_container(container, parent, collection):
    container_obj = bpy.data.objects.new(
        name=container["Name"], object_data=None)
    collection.objects.link(container_obj)
    container_obj.parent = parent
    container_obj.location = covert_location(container["Location"])
    container_obj.rotation_mode = "QUATERNION"
    container_obj.rotation_quaternion = convert_quaternion(
        container["Rotation"])
    container_obj.scale = convert(container["Scale"])
    container_obj.hide_set(True)
    container_obj.hide_render = True
    container_obj["name"] = container["Name"]
    # container_obj["pickable"] = container["Pickable"]
    if "Children" in container:
        for container_child in container["Children"]:
            type = container_child["Type"]
            container_child_name = container_child["Name"]
            if type == "FStageMesh" or type == "FStageMeshInstance":
                mesh_obj = bpy.data.objects.new(
                    name=container_child_name, object_data=get_mesh(container_child))
                collection.objects.link(mesh_obj)
                mesh_obj.parent = container_obj
                if not container_child["Visible"]:
                    mesh_obj.hide_set(True)
                    mesh_obj.hide_render = True
                mesh_obj.rotation_mode = "QUATERNION"
                mesh_obj.location = covert_location(
                    container_child["Location"])
                mesh_obj.rotation_quaternion = convert_quaternion(
                    container_child["Rotation"])
                mesh_obj.scale = convert(container_child["Scale"])
                mesh_obj["pickable"] = container_child["Pickable"]
                mesh_obj["name"] = container_child["Name"]
                if container_child["Material"] != "NULL":
                    material = get_material(container_child)
                    mesh_obj.data.materials.append(material)
                    mesh_obj.active_material = material
                    mesh_obj.material_slots[0].material = material
                    if type == "FStageMesh":
                        mesh_obj.material_slots[0].link = "DATA"
                    else:
                        mesh_obj.material_slots[0].link = 'OBJECT'
                    mesh_obj.active_material_index = 0
            elif type == "FStageLines":
                line_obj = bpy.data.objects.new(
                    name=container_child_name, object_data=get_line(container_child))
                collection.objects.link(line_obj)
                line_obj.parent = container_obj
                if not container_child["Visible"]:
                    line_obj.hide_set(True)
                line_obj.hide_render = True
                line_obj.rotation_mode = "QUATERNION"
                line_obj.location = covert_location(
                    container_child["Location"])
                line_obj.rotation_quaternion = convert_quaternion(
                    container_child["Rotation"])
                line_obj.scale = convert(container_child["Scale"])
                line_obj["pickable"] = container_child["Pickable"]
                line_obj["name"] = container_child["Name"]
            elif type == "FStageContainer":
                create_container(container_child, container_obj)

# 创建内容


def create_content(layer, collection):
    if "Children" in layer:
        for child in layer["Children"]:
            type = child["Type"]
            child_name = child["Name"]
            if type == "FStageMesh" or type == "FStageMeshInstance":
                mesh_obj = bpy.data.objects.new(
                    name=child_name, object_data=get_mesh(child))
                collection.objects.link(mesh_obj)
                if not child["Visible"]:
                    mesh_obj.hide_set(True)
                    mesh_obj.hide_render = True
                mesh_obj.rotation_mode = "QUATERNION"
                mesh_obj.location = covert_location(child["Location"])
                mesh_obj.rotation_quaternion = convert_quaternion(
                    child["Rotation"])
                mesh_obj.scale = convert(child["Scale"])
                mesh_obj["pickable"] = child["Pickable"]
                mesh_obj["name"] = child["Name"]
                if child["Material"] != "NULL":
                    material = get_material(child)
                    mesh_obj.data.materials.append(material)
                    mesh_obj.active_material = material
                    mesh_obj.material_slots[0].material = material
                    if type == "FStageMesh":
                        mesh_obj.material_slots[0].link = "DATA"
                    else:
                        mesh_obj.material_slots[0].link = 'OBJECT'
                    mesh_obj.active_material_index = 0
            elif type == "FStageLines":
                line_obj = bpy.data.objects.new(
                    name=child_name, object_data=get_line(child))
                collection.objects.link(line_obj)
                if not child["Visible"]:
                    line_obj.hide_set(True)
                    line_obj.hide_render = True
                line_obj.rotation_mode = "QUATERNION"
                line_obj.location = covert_location(child["Location"])
                line_obj.rotation_quaternion = convert_quaternion(
                    child["Rotation"])
                line_obj.scale = convert(child["Scale"])
                line_obj["pickable"] = child["Pickable"]
                line_obj["name"] = child["Name"]
            elif type == "FStageContainer":
                container_obj = bpy.data.objects.new(
                    name=child_name, object_data=None)
                collection.objects.link(container_obj)
                container_obj.rotation_mode = "QUATERNION"
                container_obj.location = covert_location(child["Location"])
                container_obj.rotation_quaternion = convert_quaternion(
                    child["Rotation"])
                container_obj.scale = convert(child["Scale"])
                container_obj.hide_set(True)
                container_obj.hide_render = True
                container_obj["name"] = child["Name"]
                # container_obj["pickable"] = child["Pickable"]
                if "Children" in child:
                    for container_child in child["Children"]:
                        type = container_child["Type"]
                        container_child_name = container_child["Name"]
                        if type == "FStageMesh" or type == "FStageMeshInstance":
                            mesh_obj = bpy.data.objects.new(
                                name=container_child_name, object_data=get_mesh(container_child))
                            collection.objects.link(mesh_obj)
                            mesh_obj.parent = container_obj
                            if not container_child["Visible"]:
                                mesh_obj.hide_set(True)
                                mesh_obj.hide_render = True
                            mesh_obj.rotation_mode = "QUATERNION"
                            mesh_obj.location = covert_location(
                                container_child["Location"])
                            mesh_obj.rotation_quaternion = convert_quaternion(
                                container_child["Rotation"])
                            mesh_obj.scale = convert(container_child["Scale"])
                            mesh_obj["pickable"] = container_child["Pickable"]
                            mesh_obj["name"] = container_child["Name"]
                            if container_child["Material"] != "NULL":
                                material = get_material(container_child)
                                mesh_obj.data.materials.append(material)
                                mesh_obj.active_material = material
                                mesh_obj.material_slots[0].material = material
                                if type == "FStageMesh":
                                    mesh_obj.material_slots[0].link = "DATA"
                                else:
                                    mesh_obj.material_slots[0].link = 'OBJECT'
                                mesh_obj.active_material_index = 0
                        elif type == "FStageLines":
                            line_obj = bpy.data.objects.new(
                                name=container_child_name, object_data=get_line(container_child))
                            collection.objects.link(line_obj)
                            line_obj.parent = container_obj
                            if not container_child["Visible"]:
                                line_obj.hide_set(True)
                            line_obj.hide_render = True
                            line_obj.rotation_mode = "QUATERNION"
                            line_obj.location = covert_location(
                                container_child["Location"])
                            line_obj.rotation_quaternion = convert_quaternion(
                                container_child["Rotation"])
                            line_obj.scale = convert(container_child["Scale"])
                            line_obj["pickable"] = container_child["Pickable"]
                            line_obj["name"] = container_child["Name"]
                        elif type == "FStageContainer":
                            create_container(
                                container_child, container_obj, collection)

# 创建层


def create_layer(scene):
    for layer in scene["Children"]:
        if layer["Type"] == "FStageLayer":
            lay_name = layer["Name"]
            print(f"Begin create {lay_name}")
            if lay_name in bpy.data.collections:
                ptr = layer["Ptr"]
                lay_name = f"{lay_name}-{ptr}"
            bpy.ops.collection.create(name=lay_name)
            collection = bpy.data.collections.get(lay_name)
            # 切换到创建的 Collection
            bpy.context.scene.collection.children.link(collection)
            bpy.context.view_layer.active_layer_collection = bpy.context.view_layer.layer_collection.children[-1]
            create_content(layer, collection)
            print(f"End create {lay_name}")


print("clear scene")
clear()
print("create scene")
create_layer(scene)
# 后处理
print("post process")
post_process()
# if __name__ == "__main__":
# 清空场景
# clear()
# 替换为你要读取的 JSON 文件的路径

#  in_file_path = "D:/scene.json"
#  with open(in_file_path,'r') as json_file:
#      json_data = json.load(json_file)
#      scene = json_data["Scene"]
#      scene_data = json_data["Data"]
#      for texture in scene_data["Textures"]:
#          decoded_data = base64.b64decode(texture["Data"])
#          with open(texture["Ptr"]+".jpg","wb") as out_image:
#              out_image.write(decoded_data)
#      print(scene_data)

