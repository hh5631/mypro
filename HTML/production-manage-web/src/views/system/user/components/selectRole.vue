<template>

  <div class="roleContainer">
    <div class="roleLeftAndRight">
      <div class="left">
        <el-input
          v-model.trim="roleName"
          placeholder="请输入角色名称"
          clearable
          size="small"
          maxlength="15"
          suffix-icon="el-icon-search"
          style="margin-bottom: 20px"
        />
        <el-checkbox
          v-if="!roleName"
          v-model="check"
          @change="selectAll"
        ><span style="font-size: 16px">全选</span>
        </el-checkbox>
        <el-tree
          ref="roletree"
          :data="roleOptions"
          show-checkbox
          :props="props"
          :filter-node-method="filterRoleNode"
          node-key="roleName"
          highlight-current
          @check-change="handleCheckChange"
        />
      </div>
      <div class="right">
        <div
          style="color: #7D8592; font-size: 14px; padding-bottom: 26px"
        >
          已选角色({{ checkNodes.length }})
        </div>
        <div
          v-for="item in checkNodes"
          :key="item.roleId"
          style="padding: 5px 0; font-size: 14px;color: #333333;"
        >
          <!-- <el-checkbox v-model="checked"></el-checkbox> -->
          <span>{{ item.roleName }}</span>
        </div>
      </div>
    </div>
    <div slot="footer" class="dialog-footer" style="float: right">
      <el-button size="small" @click="clearChoose">清空已选</el-button>
      <el-button size="small" type="primary" @click="confirmRole">保 存</el-button>
    </div>
  </div>

</template>

<script>
export default {
  name: 'SelectRole',
  props: {
    roleOptions: {
      type: Array,
      default: () => []
    },
    // show: {
    //   type: Boolean,
    //   default: false
    // },
    checkedNodes: {
      type: Array,
      default: () => []
    }
  },
  data() {
    return {
      roleName: '',
      props: {
        // children: 'children',
        label: 'roleName'
      },
      check: false,
      isChecked: false,
      checkNodes: []
      // checkedNodes: []
    }
  },
  watch: {
    roleName(val) {
      this.$refs.roletree.filter(val)
    }
  },
  mounted() {
    // this.showDig = this.show
    // this.$nextTick(() => {
    //
    //   this.$refs.roletree.setCheckedNodes(this.checkedNodes)
    // })
  },
  methods: {
    filterRoleNode(value, data) {
      if (!value) return true
      return data.roleName.indexOf(value) !== -1
    },
    //  角色选择
    handleCheckChange() {
      this.checkNodes = this.$refs.roletree.getCheckedNodes()
      if (this.checkNodes.length === this.roleOptions.filter(item => item.status === '0').length) {
        this.check = true
        this.isChecked = true
      } else {
        this.check = false
        this.isChecked = false
      }
    },
    //  确定角色
    confirmRole() {
      this.$emit('checkRole', this.checkNodes)
    },
    //  角色弹框全选
    selectAll() {
      if (this.isChecked === false) {
        this.isChecked = true
        this.check = true
        const data = this.roleOptions
        this.$refs.roletree.setCheckedNodes(data.filter(item => item.status === '0'))
      } else {
        this.isChecked = false
        this.check = false
        this.$refs.roletree.setCheckedNodes([])
      }
    },
    //  清空选择
    clearChoose() {
      this.$refs.roletree.setCheckedNodes([])
    }
    // closeDig() {
    //   this.$emit('closeDig', false)
    // }
  }
}
</script>
<style lang="scss" scoped>
.roleContainer{
  height: 500px;
  .roleLeftAndRight{
    display: flex;
    height: 90%;
    border-bottom: 1px solid #DDDDDD;
  }
  .left{
    width: 50%;
    border-right: 1px solid #DDDDDD;
    padding: 12px 24px 0 24px;
    overflow: scroll;
  }
  .right{
    width: 50%;
    margin-left: 24px;
    padding-top: 28px;
    overflow: scroll;
  }
}
.el-checkbox{
  margin: 0 0 10px 24px
}
.dialog-footer{
  margin: 10px 16px 0 0;
}
</style>
