<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-09-07 13:32:13
-->
<template>
  <div class="addCss">
    <el-form ref="addForm" label-position="top" :model="addForm" :rules="addFormRule">
      <div class="form-top">
        <div class="form-top-item">
          <el-form-item label="盘点单号">
            <el-input v-model="addForm.checkNo" disabled placeholder="系统自动生成" style="width: 220px;" />
          </el-form-item>
          <el-form-item label="盘点人" prop="peopleName">
            <el-input
              v-model="addForm.peopleName"
              placeholder="请选择"
              style="width: 220px;"
              @focus="showLeaderDig"
            >
              <i
                slot="suffix"
                class="iconfont icon-renyuanxuanze"
                @click="showLeaderDig"
              />
            </el-input>
          </el-form-item>
        </div>
        <div class="form-top-item">
          <el-form-item label="计划时间" prop="dateRange">
            <el-date-picker
              v-model="addForm.dateRange"
              class="date-class"
              style="width:220px"
              type="daterange"
              value-format="yyyy-MM-dd"
              align="right"
              start-placeholder="开始日期"
              end-placeholder="结束日期"
            />
          </el-form-item>
        </div>
      </div>
      <div>
        <el-form-item label="盘点范围" prop="range">
          <el-radio-group v-model="addForm.range" @change="rangeChange">
            <el-radio label="0">全部</el-radio>
            <el-radio label="1">指定范围</el-radio>
          </el-radio-group>
        </el-form-item>
      </div>
      <!-- 盘点范围-指定范围 -->
      <div v-if="addForm.range==='1'">
        <!-- 指定部门 -->
        <div class="checkbox-item">
          <el-checkbox v-model="appointDept" label="指定部门" :disabled="appointPlace" @change="checkedRange($event,'dept','selectTree')" />
          <select-tree
            ref="selectTree"
            :tree-list="deptList"
            :node-key="'deptId'"
            :default-props="{
              label: 'deptName',
              children: 'children'
            }"
            class="checkbox-input"
            :disabled="!appointDept"
            @getChecked="getCheckedRange($event,'dept')"
            @openSelect="openSelect1"
          />
        </div>
        <!-- 指定位置 -->
        <div class="checkbox-item">
          <el-checkbox v-model="appointPlace" label="指定位置" :disabled="appointDept" @change="checkedRange($event,'place','selectTree1')" />
          <select-tree
            ref="selectTree2"
            :tree-list="placeList"
            :node-key="'id'"
            :default-props="{
              label: 'placeName',
              children: 'children'
            }"
            class="checkbox-input"
            :disabled="!appointPlace"
            @getChecked="getCheckedRange($event,'place')"
            @openSelect="openSelect2"
          />
        </div>
        <!-- 指定类别 -->
        <div class="checkbox-item">
          <el-checkbox v-model="appointCategory" label="指定类别" @change="checkedRange($event,'category','selectTree3')" />
          <select-tree
            ref="selectTree3"
            :tree-list="categoryList"
            :node-key="'code'"
            class="checkbox-input"
            :default-props="{
              label: 'name',
              children: 'child'
            }"
            :disabled="!appointCategory"
            @getChecked="getCheckedRange($event,'category')"
            @openSelect="openSelect3"
          />
        </div>
        <!-- 指定仓库  -->
        <div class="checkbox-item">
          <el-checkbox v-model="appointWarehouse" label="指定仓库" @change="checkedRange($event,'warehouse')" />
          <el-select
            v-model="addForm.warehouse"
            class="checkbox-input"
            :disabled="!appointWarehouse"
            multiple
            collapse-tags
            @visible-change="openSelect4"
          >
            <el-option
              v-for="item in warehouseList"
              :key="item.id"
              :label="item.name"
              :value="item.id"
            />
          </el-select>
        </div>
      </div>
      <!-- 备注 -->
      <div>
        <el-form-item label="备注">
          <el-input
            v-model="addForm.remark"
            type="textarea"
            :rows="4"
          />
        </el-form-item>
      </div>
      <div class="form-footer">
        <el-button @click="cancleAdd">取消</el-button>
        <el-button v-debounce type="primary" @click.native="submit">保存</el-button>
      </div>
    </el-form>
    <!-- 选择盘点人弹窗 -->
    <el-dialog
      title="选择盘点人"
      :visible.sync="selectLeaderDig"
      width="800px"
      append-to-body
      destroy-on-close
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeLeaderDig"
    >
      <selectLeader ref="selectLeader" @checkedUser="getCheckUser" />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="clearSelected">清空已选</el-button>
        <el-button
          size="small"
          type="primary"
          @click="submitCheckLeader"
        >保存</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import selectLeader from '@/views/purchase/apply/component/selectLeader.vue'
import selectTree from './selectTree.vue'
import { getPlaceList, getFixedAssetTree } from '@/api/basicdata/index'
import { listDept } from '@/api/system/dept'
import { listWarehouse } from '@/api/basicdata/warehouse'
export default {
  name: 'Add',
  components: { selectLeader, selectTree },
  props: {},
  data () {
    var validateRange = (rule, value, callback) => {
      if (!this.appointDept && !this.appointPlace && !this.appointCategory && !this.appointWarehouse && value === '1') {
        callback(new Error('请选择指定的范围'))
      } else {
        callback()
      }
    }
    return {
      // 表单数据
      addForm: {
        checkNo: '', // 盘点单号
        peopleName: this.$store.state.user.realName, // 盘点人姓名
        peopleId: this.$store.state.user.userId, // 盘点人id
        dateRange: [],
        range: '', // 盘点范围0全部1指定
        dept: [], // 部门
        place: [], // 位置
        category: [], // 类别
        warehouse: [], // 仓库
        remark: ''// 备注
      },
      appointDept: false, // 指定部门
      appointPlace: false, // 指定位置
      appointCategory: false, // 指定类别
      appointWarehouse: false, // 指定仓库

      checkedUser: {}, // 选中的
      // 盘点人弹窗
      selectLeaderDig: false,
      pickerOptions: {
        // 禁用当前日期之前的日期
        // disabledDate(time) {
        //   // Date.now()是javascript中的内置函数，它返回自1970年1月1日00:00:00 UTC以来经过的毫秒数。
        //   return time.getTime() < Date.now() - 8.64e7
        // }
      },
      deptList: [], // 部门信息
      placeList: [], // 位置信息
      categoryList: [], // 类别信息
      warehouseList: [], // 仓库信息
      addFormRule: {
        range: [
          { required: true, message: '请选择盘点范围', trigger: ['change', 'blur'] },
          { validator: validateRange, trigger: ['change', 'blur'] }
        ],
        dateRange: [
          { required: true, message: '请选择计划时间', trigger: ['change', 'blur'] }
        ]

      }
    }
  },
  created () {
  },

  methods: {
    /** 初始化数据 */
    initData() {
      this.getListDept()
      this.getPlaceList()
      this.getFixedAssetTree()
      this.getWareList()
    },
    /** 重置表单，清除校验 */
    resetAddForm() {
      this.addForm = {
        checkNo: '',
        peopleName: this.$store.state.user.realName, // 盘点人姓名
        peopleId: this.$store.state.user.userId, // 盘点人id
        range: '', // 盘点范围0全部1指定
        dept: [], // 部门
        place: [], // 位置
        category: [], // 类别
        warehouse: [] // 仓库
      }
      this.appointDept = false // 指定部门
      this.appointPlace = false// 指定位置
      this.appointCategory = false // 指定类别
      this.appointWarehouse = false // 指定仓库
      this.$refs.addForm.resetFields()
    },
    // 取出树形数据的children组成新的数组
    deepTraversal(data) {
      const result = []
      data.forEach(item => {
        const loop = data => {
          result.push(data)
          const child = data.children || data.child
          if (child) {
            for (let i = 0; i < child.length; i++) {
              loop(child[i])
            }
          }
        }
        loop(item)
      })
      return result
    },
    /** 编辑时格式化数据 */
    getFormData(data) {
      this.addForm = {
        id: data.id,
        checkNo: data.checkNo, // 盘点单号
        peopleName: data.checkPersonName, // 盘点人姓名
        peopleId: data.checkPerson, // 盘点人id
        dateRange: [data.planStartTime, data.planEndTime], // 计划开始、结束时间
        range: data.checkRange, // 盘点范围0全部1指定
        dept: data.deptIds, // 部门id字符串数组
        place: data.placeIds, // 位置id字符串数组
        category: data.goodCodes, // 类别id字符串数组
        categoryNames: data.goodCodeNames, // 资产类别名称字符串数组
        warehouse: data.warehouseIds ? data.warehouseIds.map(e => Number(e)) : [], // 仓库id字符串数组
        remark: data.remark// 备注
      }
      // 全部范围不用做其它了
      if (data.checkRange === '0') return
      // 盘点范围-指定部门
      if (data.deptIds && data.deptIds.length > 0) {
        this.appointDept = true
        this.$nextTick(() => {
          this.$refs.selectTree.selectList = data.deptNames
        })
      }
      // 盘点范围-指定位置
      if (data.placeIds && data.placeIds.length > 0) {
        this.appointPlace = true
        this.$nextTick(() => {
          this.$refs.selectTree2.selectList = data.placeNames
        })
      }
      // 盘点范围-指定资产类别
      if (data.goodCodes && data.goodCodes.length > 0) {
        this.appointCategory = true
        this.$nextTick(() => {
          this.$refs.selectTree3.selectList = data.goodCodeNames
        })
      }
      // 盘点范围-指定仓库
      if (data.warehouseIds && data.warehouseIds.length > 0) {
        this.appointWarehouse = true
      }
    },
    /** 选择盘点范围切换 */
    rangeChange(value) {
      // 切换按钮要清空之前的选择
      if (value === '0') {
        this.appointDept = false
        this.appointPlace = false
        this.appointCategory = false
        this.appointWarehouse = false
        this.addForm.dept = []
        this.addForm.place = []
        this.addForm.category = []
        this.addForm.warehouse = []
      }
    },
    /** 根据选项情况清除范围校验 */
    clearRangValid() {
      if ((this.appointDept || this.appointPlace || this.appointCategory || this.appointWarehouse) && this.addForm.range === '1') {
        this.$refs.addForm.clearValidate('range')
      }
    },
    /** 当指定盘点范围多选框绑定值变化时触发的事件
     * value  更新后的值
     * name  指定盘点范围的id字符串数组 ，dept-指定部门、place-指定位置、category-指定类别、warehouse-指定仓库
     * ref  下拉选择树组件 el-select-tree的ref名称
    */
    checkedRange(value, name, ref) {
      // 切换清除校验
      this.clearRangValid()
      if (!value) {
        this.addForm[name] = []
        // 如果使用了下拉选择树组件 el-select-tree，
        if (ref) {
          this.$nextTick(() => {
            this.$refs[ref].resetTree()
          })
        }
      }
    },
    /** 显示选择盘点人弹窗 */
    showLeaderDig () {
      this.selectLeaderDig = true
    },
    /** 关闭选择盘点人弹窗 */
    closeLeaderDig () {
      this.selectLeaderDig = false
    },
    /** 选择申领人 */
    getCheckUser (items) {
      this.checkedUser = items[0]
    },
    /** 保存选中盘点人 */
    submitCheckLeader () {
      this.addForm.peopleId = this.checkedUser.userId
      this.addForm.peopleName = this.checkedUser.realName
      this.selectLeaderDig = false
    },
    /** 清空已选 */
    clearSelected () {
      this.$refs.selectLeader.$refs.rowRef.setCurrentRow()
      this.checkedUser = {}
    },
    /** 获取部门信息 */
    getListDept () {
      listDept({ pageNum: 1, pageSize: 999 }).then(response => {
        this.deptList = this.handleTree(response.data, 'deptId')
      })
    },
    /** 获取位置信息 */
    getPlaceList() {
      getPlaceList({}).then(
        (response) => {
          if (response.code === 200) {
            this.placeList = this.handleTree(response.data, 'id')
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    /** 获取资产类别 */
    getFixedAssetTree() {
      getFixedAssetTree({
        parentCode: '',
        pageNum: 1,
        pageSize: 999
      }).then(
        (response) => {
          if (response.code === 200) {
            // this.categoryList = this.handleTree(response.data, 'code', 'parentCode', 'child')
            this.categoryList = response.data
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    /** 获取仓库信息 */
    getWareList () {
      listWarehouse({ pageNum: 1, pageSize: 999 }).then(
        res => {
          this.warehouseList = res.data.records
        }
      )
    },
    onCateSelect() {},
    /** 指定范围多选
     * data 下拉选择树选择的选项id数组
     * name 表单数据字段名：dept-指定部门、place-指定位置、category-指定类别
    */
    getCheckedRange(data, name) {
      this.addForm[name] = data
    },
    /** 指定部门选择下拉框出现/隐藏时验证 */
    openSelect1(value) {
      if (!value) {
        return
      }
      const selectValue = []
      const dataList = this.deepTraversal(this.deptList)
      dataList.map((e) => {
        if (this.addForm.dept && this.addForm.dept.includes(String(e.deptId))) {
          selectValue.push(e)
          this.$nextTick(() => {
            // 数据回显
            this.$refs.selectTree.selectValue = selectValue
            this.$refs.selectTree.$refs.tree.setChecked(e, true, false)
          })
        }
      })
    },
    /** 指定位置选择下拉框出现/隐藏时验证 */
    openSelect2(value) {
      if (!value) {
        return
      }
      const selectValue = []
      const dataList = this.deepTraversal(this.placeList)
      dataList.map((e) => {
        if (this.addForm.place && this.addForm.place.includes(String(e.id))) {
          selectValue.push(e)
          this.$nextTick(() => {
            // 数据回显
            this.$refs.selectTree2.selectValue = selectValue

            this.$refs.selectTree2.$refs.tree.setChecked(e, true, false)
          })
        }
      })
    },
    /** 指定类别选择下拉框出现/隐藏时验证 */
    openSelect3(value) {
      if (!value) {
        return
      }
      const selectValue = []
      const dataList = this.deepTraversal(this.categoryList)
      dataList.map((e) => {
        if (this.addForm.category && this.addForm.category.includes(e.code)) {
          selectValue.push(e)
          this.$nextTick(() => {
            // 数据回显
            this.$refs.selectTree3.selectValue = selectValue
            this.$refs.selectTree3.$refs.tree.setChecked(e, true, false)
          })
        }
      })
    },
    /** 指定仓库选择下拉框出现/隐藏时验证 */
    openSelect4(value) { },
    /** 保存，发射事件把表单数据传出去*/
    submit() {
      this.$refs.addForm.validate((valid) => {
        if (valid) {
          // 发射事件，把表单对象传出去。
          this.$emit('submitForm', this.addForm)
          this.resetForm('addForm')
        } else {
          return false
        }
      })
    },
    /** 取消新建盘点 */
    cancleAdd() {
      this.$emit('cancelAdd')
    }
  }
}
</script>

<style lang="scss" scoped>
.addCss{
  .form-top{
    &-item{
      display: flex;
      justify-content: space-between;
      .date-class{
      width: 220px;
    }
    ::v-deep .el-range-editor.el-input__inner{
        padding: 3px 5px;
      }
    }

  }
  .checkbox-item{
    display: flex;
    align-items: center;
    margin-bottom: 20px;
  }
 .checkbox-input{
  width: 550px;
  margin-left: 20px;
 }
  .date_box {
    position: relative;
    width: 100%;
    .data_icon {
      position: absolute;
      top: 50%;
      right: 17px;
      z-index: 9;
      color: #c0c4cc;
      font-size: 14px;
      transform: translateY(-50%);
    }
    ::v-deep .el-range__close-icon {
      position: absolute;
      right: 34px;
      top: 54%;
      transform: translateY(-50%);
    }
  }
  .form-footer{
    text-align: end;
  }
}
</style>
