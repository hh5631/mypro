<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: lirioing
 * @LastEditTime: 2022-09-06 13:57:08
-->
<template>
  <div>
    <!-- 采购申请表格 -->
    <el-table
      ref="table"
      border
      :data="detailData"
      style="width: 100%"
      max-height="400"
    >
      <el-table-column
        prop="materialName"
        label="资产名称"
        align="center"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        prop="brandName"
        label="品牌"
        align="center"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        prop="standard"
        label="规格型号"
        align="center"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        prop="unitName"
        label="计量单位"
        align="center"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        prop="getNum"
        align="center"
        label="领用数量"
        :show-overflow-tooltip="true"
      />

      <!-- <el-table-column
        prop="note"
        label="领用说明"
        align="center"
        :show-overflow-tooltip="true"
      /> -->
    </el-table>
  </div>
</template>
<script>
export default {
  props: ['detailData'],
  data() {
    return {
    }
  },
  methods: {

  }
}
</script>
