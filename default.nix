{ stdenv, autoreconfHook, srcTarball ? null }:
stdenv.mkDerivation {
  pname = "libgraph";
  version = "0.1.0";
  src = srcTarball or ./.;
  doCheck = true;
}
