if [ -z ${version+x} ]; then
    version=2.0.0-alpha
  fi
  os=`uname -s | tr '[:upper:]' '[:lower:]'`
  wget  https://nucivic-binaries.s3-us-west-1.amazonaws.com/ahoy/$version/ahoy-$os-amd64 -O ./ahoy && \
            chmod +x ./ahoy-$os
