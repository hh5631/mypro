<template>
  <div>
    <!-- 搜索form -->
    <el-form
      ref="queryForm"
      :model="queryParams"
      :inline="true"
      label-width="85px"
      class="searchForm"
    >
      <el-form-item label="供应商名称" prop="name">
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
      <el-form-item label="供应商代码" prop="code">
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
      <!-- <el-form-item label="状态" prop="enable">
        <el-select v-model="queryParams.enable" style="width: 200px" placeholder="请选择" clearable size="small">
          <el-option
            v-for="dict in dict.type.enable_status"
            :key="dict.value"
            :label="dict.label"
            :value="dict.value"
          />
        </el-select>
      </el-form-item> -->
      <el-form-item>
        <el-button size="small" @click="resetQuery">重置</el-button>
        <el-button size="small" class="search-btn" @click="handleQuery">搜索</el-button>
      </el-form-item>
    </el-form>
    <!-- 展示table -->
    <el-table ref="multiplePlaceTable" highlight-current-row height="500" :data="placeTable" @current-change="handleSelectionChange">
      <el-table-column label="单选" align="center" width="60">
        <template slot-scope="scope">
          <el-radio v-model="radio" :label="scope.row.id" />
        </template>
      </el-table-column>
      <el-table-column label="序号" sortable align="left" prop="index" type="index" />
      <el-table-column label="供应商名称" align="left" prop="name" :show-overflow-tooltip="true" />
      <el-table-column label="供应商代码" align="left" prop="code" :show-overflow-tooltip="true" />
      <el-table-column label="统一社会信用代码" align="left" prop="account" :show-overflow-tooltip="true" />
      <el-table-column label="经营范围" align="left" prop="businessScope" :show-overflow-tooltip="true" />
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
      <el-button size="small" @click="cancelSupplier">取 消</el-button>
      <el-button size="small" type="primary" plain @click="submitSupplier">保 存</el-button>
    </div>
  </div>
</template>
<script>
import { getSupplierList } from '@/api/basicdata/index'
export default {
  name: 'SelectSupplier',
  dicts: ['enable_status'],
  data() {
    return {
      total: 0,
      // 右侧头部表单查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        name: undefined,
        code: undefined,
        enable: '0'
      },
      placeTable: [],
      radio: '',
      selectObj: {}
    }
  },
  mounted() {
    this.getList()
  },
  methods: {
    /** 右侧查询list结构 */
    getList() {
      getSupplierList(this.queryParams).then(
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
      if (value) {
        this.radio = value.id
        this.selectObj = value
      }
    },
    /** 搜索 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置 */
    resetQuery() {
      this.resetForm('queryForm')
      this.radio = ''
      this.getList()
      this.handleQuery()
    },
    cancelSupplier() {
      this.$emit('cancelSupplier', false)
    },
    /** 设置供应商id*/
    submitSupplier() {
      if (!this.selectObj.id) {
        this.$message.error('请先选择供应商')
      }
      this.$emit('selectSupplier', this.selectObj)
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
