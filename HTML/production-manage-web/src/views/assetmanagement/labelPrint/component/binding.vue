<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-05 14:26:13
-->
<template>
  <div class="selectLeader">
    <!-- <div class="info"><span class="xing">*</span>提示：选中单条可再次读码，核验RFID</div> -->
    <div>
      <el-table
        ref="table"
        highlight-current-row
        :data="tableData"
        style="margin-top: 10px;"
        max-height="500"
        :row-key="getRowKeys"
        @current-change="handleCurrentChange"
      >
        <!-- <el-table-column type="selection" width="50" :reserve-selection="true" align="center" /> -->
        <!-- <el-table-column type="index" width="100" label="序号" /> -->
        <el-table-column key="internalCode" label="资产编码" align="center" prop="internalCode" :show-overflow-tooltip="true" />
        <el-table-column key="name" label="资产名称" align="center" prop="name" :show-overflow-tooltip="true" />
        <el-table-column key="brandName" label="品牌" align="center" prop="brandName" :show-overflow-tooltip="true" />
        <el-table-column key="standard" label="规格型号" align="center" prop="standard" :show-overflow-tooltip="true" />
        <el-table-column key="dept" label="领用部门" align="center" prop="dept" :show-overflow-tooltip="true" />
        <el-table-column key="people" label="领用人" align="center" prop="people" :show-overflow-tooltip="true" />
        <el-table-column key="rfidCode" label="RFID" align="center" prop="rfidCode" :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <div>
              <el-input v-model="scope.row.rfidCode" placeholder="请用扫描枪" class="noborder" />
            </div>
          </template>
        </el-table-column>
      </el-table>
    </div>
  </div>
</template>

<script>

export default {
  props: {
    tableData: {
      type: Array,
      default: () => []
    }
  },
  data() {
    return {
      currentRow: {}// 选中的当前行
    }
  },
  created() {
  },
  methods: {
    getRowKeys(row) {
      return row.id
    },
    handleCurrentChange(val) {
      this.currentRow = val
    }
  }
}
</script>

<style lang="scss" scoped>
.selectLeader{
  width: 100%;
  height: 100%;
  padding:10px  16px 16px;
  overflow-y: scroll;
}

.info{
 margin-bottom: 30px ;
 .xing{
  color: rgb(255, 0, 0);;
 }
}
.noborder{
::v-deep .el-input__inner{
  border: none;
}
}
.detail-title{
  margin:50px 0 20px ;
}
</style>
