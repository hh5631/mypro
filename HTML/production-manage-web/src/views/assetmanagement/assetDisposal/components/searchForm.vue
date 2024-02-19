<template>
  <div>
    <el-form ref="queryForm" :model="queryParams" :inline="true">
      <el-form-item label="处置单号" prop="orderNo">
        <el-input
          v-model.trim="queryParams.orderNo"
          placeholder="请输入"
          clearable
          maxlength="15"
          size="small"
          style="width: 180px"
          @keyup.enter.native="handleQuery"
        />
      </el-form-item>
      <el-form-item label="处置类型" prop="type">
        <el-select
          v-model="queryParams.type"
          style="width: 180px"
          placeholder="请选择"
          clearable
          size="small"
        >
          <el-option
            v-for="dict in dict.type.asset_disposal_type"
            :key="dict.value"
            :label="dict.label"
            :value="dict.value"
          />
        </el-select>
      </el-form-item>
      <el-form-item label="审批状态" prop="status">
        <el-select
          v-model="queryParams.status"
          style="width: 180px"
          placeholder="请选择"
          clearable
          size="small"
        >
          <el-option
            v-for="dict in dict.type.asset_disposal_status"
            :key="dict.value"
            :label="dict.label"
            :value="dict.value"
          />
        </el-select>
      </el-form-item>
      <el-form-item label="处置时间">
        <div class="date_box">
          <el-date-picker
            v-model="queryParams.getTime"
            size="small"
            style="width: 200px"
            value-format="yyyy-MM-dd"
            type="daterange"
            range-separator="至"
            start-placeholder="开始日期"
            end-placeholder="结束日期"
            :clearable="false"
          /><i class="el-icon-date data_icon" />
        </div>
      </el-form-item>
      <el-form-item>
        <el-button size="small" @click="resetQuery">重置</el-button>
        <el-button
          type="primary"
          plain
          size="small"
          class="search-btn"
          @click="handleQuery"
        >搜索</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>
<script>
export default {
  name: 'SearchForm',
  dicts: ['asset_disposal_type', 'asset_disposal_status'],
  data() {
    return {
      // 初始查询表单对象
      queryParams: {
        pageNum: 0,
        pageSize: 10,
        orderNo: undefined,
        type: undefined,
        status: undefined,
        getTime: []
      }
    }
  },
  methods: {
    /** 搜索按钮操作 */
    handleQuery () {
      this.$emit('search', this.queryParams)
    },
    /** 重置按钮操作 */
    resetQuery() {
      this.resetForm('queryForm')
      this.queryParams.getTime = []
      this.$emit('reset', this.queryParams)
    }
  }
}
</script>
<style lang="scss" scoped>
.date_box {
  position: relative;
  width: fit-content;
  .data_icon {
    position: absolute;
    top: 50%;
    right: 17px;
    z-index: 9;
    color: #c0c4cc;
    font-size: 14px;
    transform: translateY(-50%);
  }
  ::v-deep .el-input__icon.el-range__icon.el-icon-date {
    display: none;
  }
}
</style>
