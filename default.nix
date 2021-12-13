{ stdenv, autoreconfHook }:
stdenv.mkDerivation {
  pname = "libgraph";
  version = "0.1.0";
  src = ./.;
  nativeBuildInputs = [autoreconfHook];
  doCheck = true;
}
