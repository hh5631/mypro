<template>
  <div class="user-select">
    <div class="dept-left">
      <el-input v-model.trim="deptName" maxlength="15" placeholder="请输入搜索关键字" size="mini">
        <i slot="suffix" class="el-input__icon el-icon-search" />
      </el-input>
      <div class="head-container">
        <el-tree
          ref="dept"
          :data="deptOptions"
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
      <el-input v-model.trim="leaderName" maxlength="15 " placeholder="请输入姓名或工号" size="mini">
        <i slot="suffix" class="el-input__icon el-icon-search" />
      </el-input>
      <el-table
        ref="rowRef"
        v-loading="loading"
        :row-key="getRowKeys"
        highlight-current-row
        :data="userList2"
        style="margin-top: 10px;"
        @row-click="handleRoleSelectionChange"
      >
        <!-- <el-table-column type="selection" width="50" :reserve-selection="true" align="center" /> -->
        <el-table-column type="index" width="100" label="序号" />
        <el-table-column key="realName" label="姓名" align="center" prop="realName" :show-overflow-tooltip="true" />
        <el-table-column key="jobNumber" label="工号" align="center" prop="jobNumber" />
      </el-table>
    </div>
  </div>
</template>

<script>
import { treeselect as deptTreeselect } from '@/api/system/dept'
import { listUser } from '@/api/system/user'
export default {
  data() {
    return {
      deptName: '',
      deptOptions: undefined,
      defaultProps: {
        children: 'children',
        label: 'label'
      },
      expandedArr: [],
      queryParams: {},
      loading: false,
      userList: [],
      userList2: [],
      leaderName: '',
      submitCheckedUser: [],
      currentNodeKey: ''
    }
  },
  watch: {
    leaderName() {
      this.filterUser()
    },
    deptName(val) {
      this.$refs.dept.filter(val)
    }
  },
  created() {
    this.getTreeselect()
    this.getAllUser()
  },
  methods: {
    // 搜索过滤
    filterUser() {
      const val = this.leaderName
      this.userList2 = this.userList.filter(item => {
        if (item.userId === val || item.realName.indexOf(val) !== -1 || item.jobNumber.indexOf(val) !== -1) {
          return true
        }
      })
    },
    getAllUser() {
      listUser().then(response => {
        // this.userList = response.rows
        this.userList = response.rows
        this.filterUser()
        this.userMap = {}
        this.userList.forEach(element => {
          if (this.$store.state.user.userId === element.userId) {
            this.$refs.rowRef.toggleRowSelection(element)
          }
          // element.label = element.userName
          element.label = element.realName
          element.id = 'userId-' + element.userId
          this.userMap[element.deptId] = this.userMap[element.deptId] || []
          this.userMap[element.deptId].push(element)
        })
      }
      )
    },
    getTreeselect() {
      deptTreeselect().then(response => {
        this.deptOptions = response.data
        response.data.forEach(item => {
          this.expandedArr.push(item.id)
        })
      })
    },
    filterNode(value, data) {
      if (!value) return true
      return data.label.indexOf(value) !== -1
    },
    handleNodeClick(data) {
      this.currentNodeKey = data.id
      this.queryParams.deptId = data.id
      this.queryParams.label = data.label
      this.handleQuery()
    },
    handleQuery() {
      this.loading = true
      listUser(this.addDateRange(this.queryParams)).then(response => {
        this.userList = response.rows
        this.filterUser()
        this.loading = false
      })
    },
    getRowKeys(row) {
      return row.userId
    },
    handleRoleSelectionChange(row) {
      this.submitCheckedUser = []
      this.submitCheckedUser.push(row)
      this.$emit('checkedUser', this.submitCheckedUser)
      // 选中
      this.$refs.dept.setCurrentKey(row.deptId)

      const selected = this.$refs.dept.getCurrentNode()
      /*
      const res = this.$refs.dept.getNode(selected).parent.store.currentNode
      res.expanded = true */
      const exKey = this.$refs.dept.getNode(selected).parent.key
      this.expandedArr = [exKey]
      // this.$refs.dept.setChecked(row.deptId, true, false)
      // this.currentNodeKey = row.deptId
    },
    /** 打开时重置搜索框 */
    reset() {
      this.deptName = ''
      this.leaderName = ''
    }
  }
}
</script>

<style lang="scss" scoped>
.user-select {
  height: 500px;
  display: flex;
}
.dept-left {
  width: 400px;
  height: 100%;
  padding: 16px;
  border-right: 1px solid #DDDDDD;
  overflow-y: scroll;
}

.user-right {
  width: 440px;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
}
</style>
