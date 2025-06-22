{ pkgs ? import <nixpkgs> {}, }:

pkgs.stdenv.mkDerivation {
  name = "git-commands";
  version = "1.0";
  src = ./.;
  phases = [ "unpackPhase" "buildPhase" "installPhase" ];
  buildInputs = with pkgs; [ fzf libgit2 ];
  installPhase = ''
    make install PREFIX=$out
  '';
}

