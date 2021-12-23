{ stdenv, releaseTools }:
let
  srcTarball = import ./release.nix { inherit (releaseTools) sourceTarball; };
  pname = "libgraph";
  version = "0.1.0";
in stdenv.mkDerivation {
  inherit pname version;
  src = "${srcTarball}/tarballs/${pname}-${version}.tar.gz";
  doCheck = true;
}
