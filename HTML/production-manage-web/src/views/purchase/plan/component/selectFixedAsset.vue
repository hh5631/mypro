<template>
  <div>
    <div class="user-select">
      <div class="dept-left">
        <el-input v-model.trim="assetName" maxlength="15" placeholder="请输入资产类别名称" size="mini">
          <i slot="suffix" class="el-input__icon el-icon-search" />
        </el-input>
        <div class="head-container">
          <el-tree
            ref="assetType"
            :data="fixData"
            :props="defaultProps"
            :expand-on-click-node="false"
            :filter-node-method="filterNode"
            node-key="id"
            :default-expanded-keys="expandedArr"
            highlight-current
            @node-click="handleNodeClick"
          />
        </div>
      </div>
      <div class="user-right">
        <el-input v-model.trim="assteCode" maxlength="15 " placeholder="请输入姓名或工号" size="mini">
          <i slot="suffix" class="el-input__icon el-icon-search" />
        </el-input>
        <el-table
          ref="rowRef"
          highlight-current-row
          :data="userList"
          style="margin-top: 10px;"
          @current-change="handleRoleSelectionChange"
        >
          <el-table-column label="单选" align="center" width="60">
            <template slot-scope="scope">
              <el-radio v-model="radio" :label="scope.row.code" />
            </template>
          </el-table-column>
          <el-table-column type="index" label="序号" />
          <el-table-column key="code" label="资产类别编号" align="center" prop="code" :show-overflow-tooltip="true" />
          <el-table-column key="name" label="资产类别名称" align="center" prop="name" :show-overflow-tooltip="true" />
        </el-table>
      </div>
    </div>
  </div>
</template>
<script>
import { getMaterialListByLevel } from '@/api/basicdata/index'

export default {
  name: 'SelectFixedAsset',
  props: {
    fixData: {
      type: Array,
      default: () => []
    }
  },
  data() {
    return {
      assetName: '',
      assteCode: '',
      defaultProps: {
        children: 'child',
        label: 'name'
      },
      expandedArr: [],
      userList: [],
      queryParams: {},
      checkedList: [],
      checkedUserList: [],
      userGroup: null,
      submitCheckedUser: [],
      params: {
        parentCode: '',
        name: ''
      },
      radio: ''
    }
  },
  watch: {
    // 左侧根据名称筛选树
    assetName(val) {
      this.$refs.assetType.filter(val)
    }
  },
  created() {
    this.userList = this.fixData
  },
  methods: {
    handleNodeClick(data) {
      this.params.parentCode = data.code
      getMaterialListByLevel(this.params).then(
        res => {
          if (res.code === 200) {
            this.userList = res.data
          }
        }
      )
    },
    handleQuery() {
      this.getTreeselect()
    },
    filterNode(value, data) {
      if (!value) return true
      return data.name.indexOf(value) !== -1
    },
    handleRoleSelectionChange(value) {
      if (!value) return true
      this.radio = value.code
      this.$emit('selectRow', value)
    }
  }
}
</script>

<style lang="scss" scoped>
.user-select {
  height: 400px;
  display: flex;
}
.dept-left {
  width: 350px;
  padding: 16px;
  border-right: 1px solid #DDDDDD;
  overflow-y: scroll;
}
.user-right {
  width: 700px;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
}
::v-deep .el-table__header-wrapper  .el-checkbox{//找到表头，然后隐藏掉复选框
   display:none
 }
  .fileTip{
    color: #B2B5B9;
    font-size: 14px;
    padding-left: 8px;
  }
.dialog-footer {
  padding: 20px;
  display: flex;
  justify-content: flex-end;
}
::v-deep .el-radio__label{
  display: none;
}
</style>
