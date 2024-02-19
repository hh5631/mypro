<template>
  <div class="material-table">
    <el-table
      ref="multipleTable"
      highlight-current-row
      height="100%"
      :data="materialTable"
      @selection-change="handleSelectionChange"
    >
      <el-table-column type="selection" align="center" />
      <el-table-column label="序号" sortable align="left" prop="index" type="index" />
      <el-table-column label="物料编码" align="left" prop="code" :show-overflow-tooltip="true" />
      <el-table-column label="物料名称" align="left" prop="name" :show-overflow-tooltip="true" />
      <el-table-column label="品牌" align="left" prop="brand" :show-overflow-tooltip="true" />
      <el-table-column label="规格型号" align="left" prop="standard" :show-overflow-tooltip="true" />
      <el-table-column label="计量单位" align="left" prop="unit" :show-overflow-tooltip="true" />
      <el-table-column label="供应商" align="left" prop="supplier" :show-overflow-tooltip="true" />
      <!-- <el-table-column label="状态" align="left" prop="enable">
        <template slot-scope="scope">
          <div class="statePoint">
            <el-switch v-model="scope.row.enable" active-value="0" inactive-value="1" @change="switchChange(scope.row)" />
          </div>
        </template>
      </el-table-column> -->
      <el-table-column label="操作" align="left" class-name="small-padding">
        <template slot-scope="scope">
          <el-button
            v-hasPermi="['system:user:query']"
            size="small"
            type="text"
            @click="handleUpdate(scope.row,0)"
          >详情</el-button>
          <el-button
            v-hasPermi="['system:user:edit']"
            size="small"
            type="text"
            style="color:#4686EF"
            @click="handleUpdate(scope.row,1)"
          >编辑</el-button>
          <el-button
            v-hasPermi="['system:user:remove']"
            size="small"
            type="text"
            style="color:#EF5826"
            @click="handleDelete(scope.row)"
          >删除</el-button>
        </template>
      </el-table-column>
    </el-table>
    <!-- 分页 -->
    <div class="common-pagination">
      <!-- <div class="chooseCount">已选{{ selectedLength }}条</div> -->
      <pagination
        v-show="total > 0"
        :total="total"
        :page.sync="queryParams.pageNum"
        :limit.sync="queryParams.pageSize"
        @pagination="getList"
      />
    </div>
  </div>
</template>
<script>
export default {
  name: 'MaterialTable',
  props: {
    materialTable: {
      type: Array,
      default: () => []
    },
    total: {
      type: Number,
      default: 0
    },
    queryParams: {
      type: Object,
      required: true
    }
  },
  data() {
    return {
      selectedLength: 0
    }
  },
  created() {
  },
  methods: {
    /** 多选框选中数据*/
    handleSelectionChange(selection) {
      this.selectedLength = selection.length
      this.$emit('selectOptions', selection)
    },
    /** 切换分页*/
    getList() {
      this.$emit('changePage', this.queryParams, this.currentPage)
    },
    /** 详情和编辑*/
    handleUpdate(row, index) {
      this.$emit('update', row, index)
    },
    /** 删除*/
    handleDelete(row) {
      this.$emit('handleDelete', row)
    },
    /** 切换状态*/
    switchChange(row) {
      this.$emit('changeStatus', row)
    }
  }
}
</script>

<style lang="scss" scoped>
.material-table {
  display: flex;
  flex-direction: column;
  flex: 1;
  margin-top: 8px;
}
.statePoint .point{
  width:8px;
  height:8px;
  margin:0 8px 2px 0;
}
</style>
