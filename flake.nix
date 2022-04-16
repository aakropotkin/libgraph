{
  description = "Graph algorithm library in C";

  outputs = { self, nixpkgs }: {

    packages.x86_64-linux.libgraph =
	( import nixpkgs { system = "x86_64-linux"; } ).callPackage ./default.nix {};
    packages.x86_64-linux.default = self.packages.x86_64-linux.libgraph;

  };
}
