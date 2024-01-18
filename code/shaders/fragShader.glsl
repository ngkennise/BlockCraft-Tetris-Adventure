#version 330
in vec3 colors;
in vec2 textures;

uniform sampler2D blockTextures;

out vec4 frag_color;

void main() {
  frag_color = texture(blockTextures, textures);
}

