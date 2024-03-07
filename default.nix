
with (import <nixpkgs> {});
stdenv.mkDerivation {
  name = "ring-buf-dev";
  buildInputs = [
    gcc
    cmake
    gnumake
    pkg-config
  ];

  # shellHook = ''
  #   export SOME_VAR="value"
  # '';
}
