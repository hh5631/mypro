<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: lirioing
 * @LastEditTime: 2022-09-06 13:46:31
-->
<template>
  <div>
    <!-- 退库单表格 -->
    <el-table
      ref="table"
      border
      :data="detailData"
      style="width: 100%"
      max-height="400"
    >
      <el-table-column prop="internalCode" label="资产编码" width="120" align="center" />
      <el-table-column prop="name" label="资产名称" align="center" />
      <el-table-column prop="brandName" label="品牌" align="center" />
      <el-table-column prop="standard" label="规格型号" align="center" />
      <el-table-column prop="unit" label="计量单位" align="center" />
      <el-table-column prop="deptName" label="使用部门" align="center" />
      <el-table-column prop="warehouseName" label="出库仓库" align="center" />
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
