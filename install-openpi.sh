#! @crossShell@
# meant to be used as part of nixos, not ran manually

ln -svf $1/init /init

for file in bootcode.bin rpi2.dtb rpi3.dtb; do
  cp -v @bootdir@/$file /boot/
done

cp -vL $1/initrd /boot/initrd

echo "systemConfig=$1 init=$1/init $(cat $1/kernel-params)" > /boot/cmdline.txt
