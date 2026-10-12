let 
  pkgs = import <nixpkgs> { config = { allowUnfree = false; }; };
  PROJECT_ROOT = builtins.toString ./.;
in
(pkgs.mkShell.override { stdenv = pkgs.gcc13Stdenv; }) {
  name = "dev-shell";

  # needed to disable warning: features.h:435:4: warning: #warning _FORTIFY_SOURCE requires compiling with optimization (-O) [-Wcpp]
  hardeningDisable = [
    "fortify"
  ];

  buildInputs = [
    pkgs.clang-tools
    pkgs.gnumake
  ];

  env = {
  };

  LANG = "en_US.UTF-8";
  LC_ALL = "en_US.UTF-8";

  shellHook = ''
    export PROJECT_ROOT=${PROJECT_ROOT}
  '';
}
