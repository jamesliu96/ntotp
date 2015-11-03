cat << END                                            >> $NGX_MAKEFILE

$ngx_addon_dir/totp/libtotp.so: $ngx_addon_dir/totp
	cd $ngx_addon_dir/totp && $(MAKE)

$ngx_addon_dir/totp:
	git submodule add https://github.com/jamesliu96/totp.git totp

END
