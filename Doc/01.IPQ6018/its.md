# qsdk打包过程

文件【`qsdk\build_dir\target-arm_cortex-a7_musl-1.1.16_eabi\linux-ipq_ipq60xx\fit-qcom-ipq60xx-cpxx.its`】

```bash
/dts-v1/;

/ {
	description = "ARM OpenWrt FIT (Flattened Image Tree)";
	#address-cells = <1>;

	images {
		kernel@1 {
			description = "ARM OpenWrt Linux-4.4.60";
			data = /incbin/("/home/xym/works/01.qsdk/QSDK_r11.4_1.2/qsdk/build_dir/target-arm_cortex-a7_musl-1.1.16_eabi/linux-ipq_ipq60xx/Image.gz");
			type = "kernel";
			arch = "arm";
			os = "linux";
			compression = "gzip";
			load = <0x41008000>;
			entry = <0x41008000>;
			hash@1 {
				algo = "crc32";
			};
			hash@2 {
				algo = "sha1";
			};
		};


		fdt@cp01-c2 {
			description = "ARM OpenWrt qcom-ipq60xx-cpxx device tree blob";
			data = /incbin/("/home/xym/works/01.qsdk/QSDK_r11.4_1.2/qsdk/build_dir/target-arm_cortex-a7_musl-1.1.16_eabi/linux-ipq_ipq60xx/linux-4.4.60/arch/arm/boot/dts//qcom-ipq6018-cp01-c2.dtb");
			type = "flat_dt";
			arch = "arm";
			compression = "none";
			hash@1 {
				algo = "crc32";
			};
			hash@2 {
				algo = "sha1";
			};
		};

		fdt@cp01-c3 {
			description = "ARM OpenWrt qcom-ipq60xx-cpxx device tree blob";
			data = /incbin/("/home/xym/works/01.qsdk/QSDK_r11.4_1.2/qsdk/build_dir/target-arm_cortex-a7_musl-1.1.16_eabi/linux-ipq_ipq60xx/linux-4.4.60/arch/arm/boot/dts//qcom-ipq6018-cp01-c3.dtb");
			type = "flat_dt";
			arch = "arm";
			compression = "none";
			hash@1 {
				algo = "crc32";
			};
			hash@2 {
				algo = "sha1";
			};
		};

		fdt@cp01-c4 {
			description = "ARM OpenWrt qcom-ipq60xx-cpxx device tree blob";
			data = /incbin/("/home/xym/works/01.qsdk/QSDK_r11.4_1.2/qsdk/build_dir/target-arm_cortex-a7_musl-1.1.16_eabi/linux-ipq_ipq60xx/linux-4.4.60/arch/arm/boot/dts//qcom-ipq6018-cp01-c4.dtb");
			type = "flat_dt";
			arch = "arm";
			compression = "none";
			hash@1 {
				algo = "crc32";
			};
			hash@2 {
				algo = "sha1";
			};
		};

		fdt@cp01-c1 {
			description = "ARM OpenWrt qcom-ipq60xx-cpxx device tree blob";
			data = /incbin/("/home/xym/works/01.qsdk/QSDK_r11.4_1.2/qsdk/build_dir/target-arm_cortex-a7_musl-1.1.16_eabi/linux-ipq_ipq60xx/linux-4.4.60/arch/arm/boot/dts//qcom-ipq6018-cp01-c1.dtb");
			type = "flat_dt";
			arch = "arm";
			compression = "none";
			hash@1 {
				algo = "crc32";
			};
			hash@2 {
				algo = "sha1";
			};
		};

		fdt@cp02-c1 {
			description = "ARM OpenWrt qcom-ipq60xx-cpxx device tree blob";
			data = /incbin/("/home/xym/works/01.qsdk/QSDK_r11.4_1.2/qsdk/build_dir/target-arm_cortex-a7_musl-1.1.16_eabi/linux-ipq_ipq60xx/linux-4.4.60/arch/arm/boot/dts//qcom-ipq6018-cp02-c1.dtb");
			type = "flat_dt";
			arch = "arm";
			compression = "none";
			hash@1 {
				algo = "crc32";
			};
			hash@2 {
				algo = "sha1";
			};
		};

		fdt@cp03-c1 {
			description = "ARM OpenWrt qcom-ipq60xx-cpxx device tree blob";
			data = /incbin/("/home/xym/works/01.qsdk/QSDK_r11.4_1.2/qsdk/build_dir/target-arm_cortex-a7_musl-1.1.16_eabi/linux-ipq_ipq60xx/linux-4.4.60/arch/arm/boot/dts//qcom-ipq6018-cp03-c1.dtb");
			type = "flat_dt";
			arch = "arm";
			compression = "none";
			hash@1 {
				algo = "crc32";
			};
			hash@2 {
				algo = "sha1";
			};
		};


	};

	configurations {
		default = "config@cp03-c1";

		config@cp01-c2 {
			description = "OpenWrt";
			kernel = "kernel@1";
			fdt = "fdt@cp01-c2";
		};

		config@cp01-c3 {
			description = "OpenWrt";
			kernel = "kernel@1";
			fdt = "fdt@cp01-c3";
		};

		config@cp01-c4 {
			description = "OpenWrt";
			kernel = "kernel@1";
			fdt = "fdt@cp01-c4";
		};

		config@cp01-c1 {
			description = "OpenWrt";
			kernel = "kernel@1";
			fdt = "fdt@cp01-c1";
		};

		config@cp02-c1 {
			description = "OpenWrt";
			kernel = "kernel@1";
			fdt = "fdt@cp02-c1";
		};

		config@cp03-c1 {
			description = "OpenWrt";
			kernel = "kernel@1";
			fdt = "fdt@cp03-c1";
		};

	};
};

```



```bash
/home/xym/works/01.qsdk/QSDK_r11.4_1.2/qsdk/scripts/mkits.sh -D qcom-ipq6018-cp03-c1 -o /home/xym/works/01.qsdk/QSDK_r11.4_1.2/qsdk/build_dir/target-arm_cortex-a7_musl-1.1.16_eabi/linux-ipq_ipq60xx/fit-qcom-ipq6018-cp03-c1.its -k  /home/xym/works/01.qsdk/QSDK_r11.4_1.2/qsdk/build_dir/target-arm_cortex-a7_musl-1.1.16_eabi/linux-ipq_ipq60xx/Image-initramfs -d  /home/xym/works/01.qsdk/QSDK_r11.4_1.2/qsdk/build_dir/target-arm_cortex-a7_musl-1.1.16_eabi/linux-ipq_ipq60xx/linux-4.4.60/arch/arm/boot/dts//qcom-ipq6018-cp03-c1.dtb -C  none -a 0x41008000 -e 0x41008000 -A arm -v 4.4.60

# mkits.sh 用法
# -A ==> 设置 arch arm
# -C ==> 设置内核压缩格式
# -c ==> set dtb compression type 'comp'"
# -l ==> set dtb load address to 'addr'"
# -a ==> 设置内核加载地址
# -e ==> 设置内核 entry 
# -v ==> 设置内核版本
# -k ==> 指定内核源文件
# -D ==> its里面设备树的名字
# -d ==> 指定设备树源文件
# -o ==> 最终输出的its 文件
```





