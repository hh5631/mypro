<template>
  <div>
    <!-- 搜索form -->
    <el-form
      ref="queryForm"
      :model="queryParams"
      :inline="true"
      label-width="68px"
      class="searchForm"
    >
      <el-form-item label="物料编码" prop="code">
        <el-input
          v-model.trim="queryParams.code"
          placeholder="请输入"
          clearable
          size="small"
          style="width: 200px"
          maxlength="15"
          @keyup.enter.native="handleQuery"
        />
      </el-form-item>
      <el-form-item label="物料名称" prop="name">
        <el-input
          v-model.trim="queryParams.name"
          placeholder="请输入"
          clearable
          size="small"
          style="width: 200px"
          maxlength="15"
          @keyup.enter.native="handleQuery"
        />
      </el-form-item>
      <el-form-item>
        <el-button size="small" @click="resetQuery">重置</el-button>
        <el-button size="small" class="search-btn" @click="handleQuery">搜索</el-button>
      </el-form-item>
    </el-form>
    <!-- 展示table -->
    <el-table ref="multiplePlaceTable" highlight-current-row height="550" :data="placeTable" @current-change="handleSelectionChange">
      <el-table-column label="单选" align="center" width="60">
        <template slot-scope="scope">
          <el-radio v-model="radio" :label="scope.row.id" />
        </template>
      </el-table-column>
      <el-table-column label="序号" sortable align="left" prop="index" type="index" />
      <el-table-column label="物料编码" align="left" prop="code" :show-overflow-tooltip="true" />
      <el-table-column label="物料名称" align="left" prop="name" :show-overflow-tooltip="true" />
      <el-table-column label="品牌" align="left" prop="brand" :show-overflow-tooltip="true" />
      <el-table-column label="规格型号" align="left" prop="standard" :show-overflow-tooltip="true" />
      <el-table-column label="计量单位" align="left" prop="unit" :show-overflow-tooltip="true" />
      <el-table-column label="供应商" align="left" prop="supplier" :show-overflow-tooltip="true" />
    </el-table>
    <!-- 分页 -->
    <div>
      <pagination
        v-show="total > 0"
        :total="total"
        :page.sync="queryParams.pageNum"
        :limit.sync="queryParams.pageSize"
        @pagination="getList"
      />
    </div>
    <div slot="footer" class="dialog-footer">
      <el-button size="small" @click="clearChosen">取消</el-button>
      <el-button size="small" type="primary" style="margin-right:20px;" @click="submitUserSet">保 存</el-button>
    </div>
  </div>
</template>
<script>
import { getMaterialList } from '@/api/basicdata/material.js'
export default {
  name: 'SelectMaterial',
  dicts: ['enable_status'],
  data() {
    return {
      total: 0,
      // 右侧头部表单查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        name: undefined,
        code: undefined
      },
      placeTable: [],
      radio: '',
      fixedData: {}
    }
  },
  created() {
    this.getList()
  },
  methods: {
    /** 右侧查询list结构 */
    getList() {
      getMaterialList(this.queryParams).then(
        (response) => {
          if (response.code === 200) {
            this.placeTable = response.data.records
            this.total = response.data.total
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    /** 选择物料时发射事件传递选择的值 */
    handleSelectionChange(value) {
      this.radio = value.id
      this.fixedData = value
    },
    /** 搜索 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置 */
    resetQuery() {
      this.resetForm('queryForm')
      this.getList()
      this.handleQuery()
    },
    /** 取消选择 */
    clearChosen() {
      this.radio = ''
      this.fixedData = {}
      this.$emit('clearChosen', false)
    },
    /** 确认选择物料对象 */
    submitUserSet() {
      if (!this.fixedData.id) {
        this.$message.error('请先选择资产')
      } else {
        this.$emit('selectMaterial', this.fixedData)
      }
    }
  }
}
</script>

<style lang="scss" scoped>
::v-deep .el-radio__label{
  display: none;
}
.dialog-footer{
  display: flex;
  justify-content: flex-end;
}
</style>
