/*
 * Copyright (c) 2014-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

/* Tegra vpr routines */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/tegra.h>
#include <asm/arch/mc.h>

#include <fdt_support.h>

static bool _configured;

void tegra_gpu_config(void)
{
	struct mc_ctlr *mc = (struct mc_ctlr *)NV_PA_MC_BASE;

	/* Turn VPR off */
	writel(0, &mc->mc_video_protect_size_mb);
	writel(TEGRA_MC_VIDEO_PROTECT_REG_WRITE_ACCESS_DISABLED,
	       &mc->mc_video_protect_reg_ctrl);
	/* read back to ensure the write went through */
	readl(&mc->mc_video_protect_reg_ctrl);

	debug("configured VPR\n");

	_configured = true;
}

#if defined(CONFIG_OF_LIBFDT)

int tegra_gpu_enable_node(void *blob, const char *gpupath)
{
	int offset;

	if (_configured) {
		offset = fdt_path_offset(blob, gpupath);
		if (offset > 0) {
			fdt_status_okay(blob, offset);
			debug("enabled GPU node %s\n", gpupath);
		}
	}

	return 0;
}

#endif
