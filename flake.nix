{
    description = "nix barracks";
    outputs = { self, nixpkgs, ...  }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];
      forEachSupportedSystem = f: nixpkgs.lib.genAttrs supportedSystems (system: f {
        pkgs = import nixpkgs { inherit system; };
      });
    in
    {
        devShells = forEachSupportedSystem ({pkgs}: {
            default = pkgs.mkShell {
              packages = with pkgs; [ 
                clang-tools cmake gnumake glfw glew spdlog
                (imgui.override { IMGUI_BUILD_GLFW_BINDING = true; IMGUI_BUILD_OPENGL3_BINDING = true; IMGUI_LINK_GLVND = true;}) 
              ];
            };
        });
    };
}
