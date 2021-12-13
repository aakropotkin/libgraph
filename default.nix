{ stdenv, autoreconfHook, writeText }:
stdenv.mkDerivation {
  pname = "libgraph";
  version = "0.1.0";
  src = ./.;
  nativeBuildInputs = [autoreconfHook];
  doCheck = true;
  setupHook = writeText "setupHook.sh" ''

    setLibgraphEnv() {
      LIBGRAPH_PATH=$i
      export LIBGRAPH_PATH
    }

    addEnvHooks "$targetOffset" addLibgraphEnv
  '';
}
