{
  description = "Graph algorithm library in C";
  inputs.utils.url = "github:numtide/flake-utils";
  inputs.utils.inputs.nixpkgs.follows = "/nixpkgs";
  outputs = { self, nixpkgs, utils }: let
    inherit (utils.lib) eachDefaultSystemMap;
  in {
    packages = eachDefaultSystemMap ( system: let
      pkgsFor = nixpkgs.legacyPackages.${system};
    in {
      libgraph = pkgsFor.callPackage ./default.nix {};
      default  = self.packages.${system}.libgraph;
    } );
  };
}
