#! /bin/csh 

echo "first, remove excess tilde files..."
rm -i *~ */*~
echo "now build a tarball..."
tar cvzf /tmp/all.tgz *
echo "finally, copy it over for public consumption..."
# scp /tmp/all.tgz ${rac}:public/html/OSTEP/Educators/lecture-notes.tgz



