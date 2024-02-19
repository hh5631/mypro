<template>
  <el-select
    v-model="selectList"
    placeholder="请选择"
    multiple
    :disabled="disabled"
    collapse-tags
    @change="selectChange"
    @visible-change="openSelect"
  >
    <el-option :value="selectValue" style="height: auto" disabled>
      <!-- <el-input
        v-model="filterText"
        size="mini"
        placeholder="搜索部门"
      /> -->
      <el-tree
        ref="tree"
        :data="treeList"
        show-checkbox
        :node-key="nodeKey"
        highlight-current
        :props="defaultProps"
        :filter-node-method="filterNode"
        @check-change="handleCheckChange"
      />
    </el-option>
  </el-select>
</template>
<script>
export default {
  props: {
    treeList: {
      type: Array,
      default: () => []
    },
    nodeKey: {
      type: String,
      default: undefined
    },
    defaultProps: {
      type: Object,
      default: () => {}
    },
    disabled: {
      type: Boolean,
      default: false
    }
  },
  data() {
    return {
      // defaultProps: {
      //   label: 'deptName',
      //   children: 'children'
      // },
      // 勾选选项的值数组
      selectList: [],
      // 树结构选中后，下拉框的数组值
      selectValue: [],
      // 树结构选中id数组
      selectIds: [],
      // 搜索绑定值
      filterText: ''
    }
  },
  watch: {
    filterText(val) {
      this.$refs.tree.filter(val)
    }
  },
  mounted () {
    this.$on('change', function() {
      this.handleCheckChange()
    })
  },
  methods: {
    /** 重置下拉选择树 */
    resetTree() {
      this.selectValue = []
      this.selectList = []
      this.selectIds = []
      // 设置当前勾选节点
      this.$refs.tree.setCheckedNodes([])
    },
    /** 下拉框选中值发生变化时触发 */
    selectChange(e) {
      console.log('e', e)
      const arrNew = []
      const dataLength = this.selectValue.length
      const eleng = e.length
      for (let i = 0; i < dataLength; i++) {
        for (let j = 0; j < eleng; j++) {
          if (e[j] === this.selectValue[i][this.defaultProps.label]) {
            arrNew.push(this.selectValue[i])
          }
        }
      }
      // 设置勾选的值
      this.$refs.tree.setCheckedNodes(arrNew)
    },
    /** 下拉框出现/隐藏时校验 */
    openSelect(value) {
      this.$emit('openSelect', value)
    },
    /** 树节点选中状态发生变化时的回调 */
    handleCheckChange() {
      // 获取目前所有选中节点返回组成的数组
      const res = this.$refs.tree.getCheckedNodes('true')
      console.log('选中res', res)
      // 接收值数组
      const arrLabel = []
      // 接收id数组
      const arrValue = []
      // 重新复制一份当前选中的数组
      const arr = []
      res.forEach((item) => {
        arrValue.push(item[this.nodeKey])
        arrLabel.push(item[this.defaultProps.label])
        arr.push(item)
      })
      this.selectValue = arr
      // 勾选选项的值数组
      this.selectList = arrLabel
      // 勾选选项的id数组
      this.selectIds = arrValue
      // 发射事件getChecked，把选中的id数组值发射出去
      this.$emit('getChecked', this.selectIds)
      this.$emit('getCurrentBox', this.defaultProps.label)
    },
    /** 树节点进行筛选时执行的方法 */
    filterNode(value, data) {
      if (!value) return true
      return data.deptName.indexOf(value) !== -1
    }
  }
}
</script>
