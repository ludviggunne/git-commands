{ pkgs ? import <nixpkgs> {}, }:

pkgs.stdenv.mkDerivation {
  name = "git-commands";
  version = "1.0";
  src = ./.;
  phases = [ "unpackPhase" "installPhase" ];
  buildInputs = with pkgs; [ fzf ];
  installPhase = ''
    make install PREFIX=$out
  '';
}

