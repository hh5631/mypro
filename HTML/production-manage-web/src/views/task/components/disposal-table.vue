<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: 751371771@qq.com 751371771@qq.com
 * @LastEditTime: 2022-10-19 11:17:16
-->
<template>
  <div>
    <!-- 处置单表格 -->
    <el-table
      ref="table"
      border
      :data="detailData"
      style="width: 100%"
      max-height="400"
    >
      <el-table-column
        prop="internalCode"
        label="资产编码"
        align="center"
      />
      <el-table-column
        prop="name"
        label="资产名称"
        align="center"
      />
      <el-table-column
        prop="unit"
        label="计量单位"
        align="center"
      />
      <el-table-column
        prop="purchaseDate"
        label="购置日期"
        align="center"
      >
        <template #default="scope">
          <span>
            {{ dayjsFormat(scope.row.purchaseDate,'YYYY-MM-DD') }}
          </span>
        </template>
      </el-table-column>
      <el-table-column
        prop="price"
        label="资产原值"
        align="center"
      />
      <el-table-column
        prop="accumulatedDepreciation"
        label="累计折旧"
        align="center"
      >
        <template slot-scope="scope">
          <span>{{ accumulatedDepreciation(scope.row) }}</span>
        </template>
      </el-table-column>
      <el-table-column
        prop="netResidualValue"
        label="资产净值"
        align="center"
      />
      <el-table-column prop="income" label="处置收入(元)" align="center" />
    </el-table>
  </div>
</template>
<script>
import { dayjsFormat } from '@/utils/index'
export default {
  props: ['detailData'],
  data() {
    return {
    }
  },
  methods: {
    /** 累计折旧计算 */
    accumulatedDepreciation(row) {
      return row.price - row.netResidualValue || 0
    },
    /** 创建时间格式化 */
    dayjsFormat
  }
}
</script>
