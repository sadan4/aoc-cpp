{
  description = "A basic flake with a shell";
  inputs = {
    nixpkgs = {
      url = "github:NixOS/nixpkgs/nixos-unstable";
    };
    systems = {
      url = "github:nix-systems/default";
    };
    flake-utils = {
      url = "github:numtide/flake-utils";
      inputs.systems.follows = "systems";
    };
  };

  outputs =
    {
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            clang
            clang-tools
            meson
            ninja
            bear
            fmt
            (writeShellScriptBin "build" ''
                set -x
                ${pkgs.bear}/bin/bear -- ${pkgs.gnumake}/bin/make -j -C dist
            '')
            (writeShellScriptBin "configure" ''
                set -x
                ${pkgs.cmake}/bin/cmake -B dist -G "Unix Makefiles"
            '')
            (writeShellScriptBin "clean" ''
                set -x
                rm -rf dist
            '')
          ];
          shellHook = ''
              #shell config here
              export CC=clang
              export CXX=clang++
          '';
        };
      }
    );
}
