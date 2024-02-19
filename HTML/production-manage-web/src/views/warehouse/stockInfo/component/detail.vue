<!--
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-07-25 13:55:12
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-09-09 09:10:50
 * @FilePath: \production-manage-web\src\views\warehouse\stockInfo\component\detail.vue
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
-->
<template>
  <el-dialog
    title="详情"
    width="60%"
    :visible.sync="visible"
    append-to-body
    destroy-on-close
    class="my-dialog"
  >
    <my-collapse title="基本信息">
      <stock-form ref="form" :colums="formColums" :rules="rules" :data="formData" />
    </my-collapse>
    <my-collapse title="出入库记录">
      <el-table
        :data="hisToryData"
        height="340px"
        size="mini"
      >
        <el-table-column
          v-for="(table,index) in colums"
          :key="index"
          :type="table.type"
          :label="table.label"
          :align="table.align"
          :prop="table.prop"
          :show-overflow-tooltip="table.overflow"
        />
      </el-table>
      <div class="common-pagination">
        <pagination
          v-show="total > 0"
          :total="total"
          :page.sync="queryParams.pageNum"
          :limit.sync="queryParams.pageSize"
          @pagination="getList"
        />
      </div>
    </my-collapse>
    <template slot="footer">
      <el-button
        size="small"
        @click="visible = false"
      >关 闭</el-button>
    </template>
  </el-dialog>
</template>
<script>
import MyCollapse from '@/components/MyCollapse/index.vue'
import StockForm from '@/components/StockForm/index.vue'
import { getStockDetail, getStockHistory } from '@/api/warehouse'

const changeTypeArr = [
  { txt: '入库', val: '0' },
  { txt: '出库', val: '1' },
  { txt: '盘盈', val: '2' },
  { txt: '处置', val: '3' },
  { txt: '退库', val: '4' }
]

export default {
  components: {
    MyCollapse,
    StockForm
  },
  data() {
    return {
      colums: [
        { type: 'index', label: '序号' },
        { prop: 'changeType', label: '流转类型', align: 'center' },
        { prop: 'inOutNo', label: '单号', align: 'center' },
        { prop: 'changeQty', label: '操作数量', align: 'center' },
        { prop: 'afterQty', label: '库存数量', align: 'center' },
        { prop: 'oper', label: '操作人', align: 'center' },
        { prop: 'changeTime', label: '操作时间', align: 'center' }
      ],
      formColums: [{
        label: '物料分类',
        value: 'materialCategory',
        readonly: true
      },
      {
        label: '物料编码',
        value: 'materialCode',
        readonly: true
      },
      {
        label: '物料名称',
        value: 'materialName',
        readonly: true
      },
      {
        label: '品牌',
        value: 'brand',
        readonly: true
      },
      {
        label: '规格型号',
        value: 'materialStandard',
        readonly: true
      },
      {
        label: '计量单位',
        value: 'unit',
        readonly: true
      },
      // {
      //   label: '入库时间',
      //   value: 'name2',
      //   readonly: true
      // },
      {
        label: '所属单位/部门',
        value: 'dept',
        readonly: true
      },
      {
        label: '存放仓库',
        value: 'warehouseName',
        readonly: true
      },
      {
        label: '库存总量',
        value: 'num',
        readonly: true
      }
      ],
      rules: {
        name: [{ required: true, trigger: ['blur', 'change'] }]
      },
      formData: {
        dept: '浙大嘉兴研究院'
      },
      visible: false,
      hisToryData: [],
      total: 0,
      queryParams: {
        pageNum: 0,
        pageSize: 10,
        stockId: ''
      },
      tempStockId: ''
    }
  },
  methods: {
    open(val) {
      this.tempStockId = val?.id
      this.visible = true
      getStockDetail(val?.id).then(da => {
        this.formData = da.data
        this.formData.dept = '浙大嘉兴研究院'
      })
      getStockHistory({ stockId: val?.id }).then(da => {
        console.log(da)
        this.total = da.data.total
        da.data.records.map(item => { item.changeType = changeTypeArr.find(itemx => itemx.val === item.changeType).txt })
        this.hisToryData = da.data.records
      })
    },
    getList() {
      this.queryParams.stockId = this.tempStockId
      console.log(this.queryParams)
      getStockHistory(this.queryParams).then(da => {
        console.log(da)
        da.data.records.map(item => { item.changeType = changeTypeArr.find(itemx => itemx.val === item.changeType).txt })
        this.hisToryData = da.data.records
      })
    }
  }
}
</script>
<style lang="scss">
.readOnly {
  .el-input__inner {
    background: #f5f8fc;
  }
}
</style>
