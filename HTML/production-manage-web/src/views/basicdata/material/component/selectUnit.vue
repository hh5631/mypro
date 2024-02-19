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
      <el-form-item label="基准单位" prop="base">
        <el-input
          v-model.trim="queryParams.base"
          placeholder="请输入"
          clearable
          size="small"
          style="width:200px"
          maxlength="15"
          @keyup.enter.native="handleQuery"
        />
      </el-form-item>
      <el-form-item label="辅助单位" prop="assist">
        <el-input
          v-model.trim="queryParams.assist"
          placeholder="请输入"
          clearable
          size="small"
          style="width:200px"
          maxlength="15"
          @keyup.enter.native="handleQuery"
        />
      </el-form-item>
      <!-- <el-form-item label="状态" prop="status">
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
    <el-table ref="multiplePlaceTable" v-loading="tableLoading" :highlight-current-row="highlightFlog" height="500" :data="placeTable" @current-change="handleSelectionChange">
      <el-table-column label="单选" align="center" width="60">
        <template slot-scope="scope">
          <el-radio v-model="radio" :label="scope.row.id" />
        </template>
      </el-table-column>
      <el-table-column label="序号" sortable align="left" prop="index" type="index" />
      <el-table-column label="基准单位" align="left" prop="base" :show-overflow-tooltip="true" />
      <el-table-column label="辅助单位" align="left" prop="assist" :show-overflow-tooltip="true" />
      <el-table-column label="比例" align="left" prop="ratio" :show-overflow-tooltip="true" />
      <el-table-column label="单位类别" align="left" prop="unitType" :show-overflow-tooltip="true" />
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
      <el-button size="small" @click="cancelUnit">取 消</el-button>
      <el-button size="small" type="primary" @click="submitUnit">保 存</el-button>
    </div>
  </div>
</template>
<script>
import { getUnitByCondition } from '@/api/basicdata/index'
export default {
  name: 'SelectBrand',
  dicts: ['enable_status'],
  props: {
    form: {
      type: Object,
      default: () => {}
    }
  },
  data() {
    return {
      total: 0,
      // 右侧头部表单查询参数
      queryParams: {
        assist: undefined,
        // enable: '0',
        status: undefined,
        pageNum: 1,
        pageSize: 10
      },
      placeTable: [],
      radio: '',
      selectObj: {},
      highlightFlog: true,
      tableLoading: false
    }
  },
  watch: {
    form: {
      immediate: true,
      deep: true,
      handler (value) {
        this.radio = this.form.unitId
        this.highlightFlog = !!this.form.unitId
      }
    }
  },
  created() {
    this.getList()
  },
  methods: {
    /** 右侧查询list结构 */
    getList() {
      this.tableLoading = true
      getUnitByCondition(this.queryParams).then(
        (response) => {
          if (response.code === 200) {
            this.placeTable = response.data.records
            this.total = response.data.total
          } else {
            this.$message.error(response.msg)
          }
        }
      ).finally(() => {
        this.tableLoading = false
      })
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
      // this.radio = ''
      this.getList()
      this.handleQuery()
    },
    cancelUnit() {
      this.$emit('cancelUnit', false)
    },
    /** 设置计量单位id*/
    submitUnit() {
      //
      // if (!this.selectObj.id) {
      //   this.$message.error('请先选择计量单位')
      // }
      //
      // if (this.form.unit === '') {
      //
      // }
      this.$emit('selectUnit', this.selectObj, true)
      //
      //
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
