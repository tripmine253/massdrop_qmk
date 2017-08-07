#/bin/bash
echo "# Keyboard Identifiers

|Keyboard|Vendor ID|Product ID|Device Ver|" > docs/keyboard_identifiers.md
for project in $(find . -regex '\./keyboards/[a-z0-9_\-]+/config.h')
do
    keyboard_name=$(printf $project | sed -r "s/\.\/keyboards\/([a-zA-Z0-9_\-]+).*/\1/g")
    vendor_id=$(grep 'VENDOR_ID' $project | \
        tr -d '\n' | \
        sed -r "s/.*(0x[a-fA-F0-9]+).*/\1/g")
    if [ -z $vendor_id ]; then
        for subproject in $(find . -regex "\./keyboards/$keyboard_name/[a-z0-9_\-]+/config.h")
        do
            subproject_name=$(printf $subproject | sed -r "s/\.\/keyboards\/${keyboard_name}\/([a-zA-Z0-9_\-]+).*/\1/g")
            subproject_vendor_id=$(grep 'VENDOR_ID' $subproject | \
                tr -d '\n' | \
                sed -r "s/.*(0x[a-fA-F0-9]+).*/\1/g")
            vendor_id+=$(printf "%s: %s<br />" $subproject_name $subproject_vendor_id)
        done
    fi
    product_id=$(grep 'PRODUCT_ID' $project | \
        tr -d '\n' | \
        sed -r "s/.*(0x[a-fA-F0-9]+).*/\1/g")
    if [ -z $product_id ]; then
        for subproject in $(find . -regex "\./keyboards/$keyboard_name/[a-z0-9_\-]+/config.h")
        do
            subproject_name=$(printf $subproject | sed -r "s/\.\/keyboards\/${keyboard_name}\/([a-zA-Z0-9_\-]+).*/\1/g")
            subproject_product_id=$(grep 'PRODUCT_ID' $subproject | \
                tr -d '\n' | \
                sed -r "s/.*(0x[a-fA-F0-9]+).*/\1/g")
            product_id+=$(printf "%s: %s<br />" $subproject_name $subproject_product_id)
        done
    fi
    device_ver=$(grep 'DEVICE_VER' $project | \
        tr -d '\n' | \
        sed -r "s/.*(0x[a-fA-F0-9]+).*/\1/g")
    if [ -z $device_ver ]; then
        for subproject in $(find . -regex "\./keyboards/$keyboard_name/[a-z0-9_\-]+/config.h")
        do
            subproject_name=$(printf $subproject | sed -r "s/\.\/keyboards\/${keyboard_name}\/([a-zA-Z0-9_\-]+).*/\1/g")
            subproject_device_ver=$(grep 'DEVICE_VER' $subproject | \
                tr -d '\n' | \
                sed -r "s/.*(0x[a-fA-F0-9]+).*/\1/g")
            device_ver+=$(printf "%s: %s<br />" $subproject_name $subproject_device_ver)
        done
    fi
    printf "|%s|%s|%s|%s|\n" "$keyboard_name" "$vendor_id" "$product_id" "$device_ver" >> docs/keyboard_identifiers.md
done