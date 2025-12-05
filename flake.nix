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
            gcc
            meson
            ninja
            bear
            fmt
            fmt.dev
            catch2_3
            (writeShellScriptBin "build" ''
              set -x
              ${pkgs.gnumake}/bin/make -j -C dist "$@"
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
            export RANLIB="${pkgs.gcc}/bin/ranlib"
            export AR="${pkgs.gcc}/bin/ar"
            ln -sf ${pkgs.cmake}/bin/cmake .idea/cmake-link
            ln -sf ${pkgs.gcc}/bin/gcc .idea/cc-link
            ln -sf ${pkgs.gcc}/bin/g++ .idea/cxx-link
          '';
        };
      }
    );
}
