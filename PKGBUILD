#Forked from https://aur.archlinux.org/packages/extremetuxracer/
# Maintainer: Carlos Aznarán <caznaranl@uni.pe>
# Contributor: Antonio Rojas <arojas@archlinux.org>
# Contributor: Ronald van Haren <ronald.archlinux.org>
# Contributor: Eric Belanger <eric.archlinux.org>
# Contributor: Fabio Scotoni <CCuleX.gmail.com>
# Contributor: Alexander Rødseth <rodseth@gmail.com>
pkgname=extremetuxracer
pkgver=0.8.4
pkgrel=1
pkgdesc="Downhill racing game starring Tux"
arch=(x86_64)
url="https://sourceforge.net/projects/${pkgname}"
license=(GPL-2.0-or-later)
depends=(sfml glu freetype2)
makedepends=(csfml git)
source=(git+https://github.com/jcowgill/extremetuxracer.git#branch=sfml3)
sha512sums=('SKIP')

build() {
  cd $pkgname
  ./autogen.sh
  ./configure --prefix=/usr
  make
}

check() {
  cd $pkgname
  make check
}

package() {
  cd $pkgname
  make DESTDIR="${pkgdir}" install
  install -Dm 644 COPYING -t "${pkgdir}/usr/share/licenses/${pkgname}"
}
