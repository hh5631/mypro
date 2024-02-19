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
      <div>
        <div class="addCss-form">
          <el-form-item label="资产编码" prop="assetCode">
            <el-input v-model="addForm.assetCode" disabled placeholder="系统自动生成" />
          </el-form-item>
          <el-form-item label="资产名称" prop="name">
            <el-input
              v-model="addForm.name"
              placeholder="请选择"
              style="width: 210px;"
              @focus="showType"
            />
          </el-form-item>
          <el-form-item label="资产类别" prop="category">
            <el-input
              v-model="addForm.category"
              placeholder="选择资产后自动关联"
              readonly
            />
          </el-form-item>
          <el-form-item label="品牌" prop="brand">
            <el-input v-model="addForm.brand" readonly placeholder="选择资产后自动关联" />
          </el-form-item>
          <el-form-item label="规格型号" prop="standard">
            <el-input v-model="addForm.standard" readonly placeholder="选择资产后自动关联" />
          </el-form-item>
          <el-form-item label="计量单位" prop="unit">
            <el-input v-model="addForm.unit" readonly placeholder="选择资产后自动关联" />
          </el-form-item>
          <el-form-item label="资产状态" prop="unit">
            <el-select v-model="addForm.status" placeholder="请选择" style="width: 210px;" @change="changeStatus">
              <el-option
                v-for="item in assetStatus"
                :key="item.value"
                :label="item.label"
                :value="item.value"
              />
            </el-select>
          </el-form-item>
          <el-form-item :label="addForm.status==='0'?'所属部门/单位':'使用部门/单位'" prop="deptId">
            <el-select
              v-model="addForm.deptId"
              clearable
              size="small"
            >
              <el-option
                v-for="dict in deptData"
                :key="dict.deptId"
                :label="dict.deptName"
                :value="dict.deptId"
              />
            </el-select>
          </el-form-item>
          <el-form-item v-if="addForm.status==='0'" label="存放仓库" prop="warehouse">
            <el-select v-model="addForm.warehouse" placeholder="请选择" style="width: 210px;">
              <el-option
                v-for="item in warehouseData"
                :key="item.id"
                :label="item.name"
                :value="item.id"
              />
            </el-select>
          </el-form-item>
          <el-form-item v-if="addForm.status==='3'" label="使用人" prop="userName">
            <el-input
              ref="peopleNameRef"
              v-model="addForm.userName"
              placeholder="请选择"
              style="width: 210px;"
              @focus="showLeaderDig"
            >
              <i
                slot="suffix"
                class="iconfont icon-renyuanxuanze"
                @click="showLeaderDig"
              />
            </el-input>
          </el-form-item>

          <el-form-item v-if="addForm.status==='3'" label="存放位置" prop="placeId">
            <el-select
              v-model="addForm.placeId"
              clearable
              size="small"
              @change="changePlace"
            >
              <el-option
                v-for="dict in placeData"
                :key="dict.id"
                :label="dict.placeName"
                :value="dict.id"
              />
            </el-select>
          </el-form-item>
          <el-form-item label="税前单价（元）" prop="price">
            <el-input v-model="addForm.price" placeholder="" />
          </el-form-item>
          <el-form-item label="税额（元）" prop="taxAmount">
            <el-input v-model="addForm.taxAmount" placeholder="" />
          </el-form-item>
        </div>
        <div class="form-footer">
          <el-button type="primary" @click="submit">保存</el-button>
        </div>
      </div></el-form>
    <el-dialog
      title="选择使用人"
      :visible.sync="selectLeaderDig"
      width="800px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeLeaderDig"
    ><selectLeader ref="selectLeader" @checkedUser="getCheckUser" />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="clearSelected">清空已选</el-button>
        <el-button
          size="small"
          type="primary"
          @click="submitCheckLeader"
        >保存</el-button>
      </div>
    </el-dialog>
    <!-- 选择资产列表 -->
    <el-dialog
      title="选择物料"
      :visible.sync="isShowType"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      class="role-dialog"
      @close="closeType"
    >
      <!-- <selectFixedAsset :fix-data="FixedAsset" @selectRow="getRow" /> -->
      <selectMaterial @selectMaterial="selectMaterial" @clearChosen="clearChosen" />
    </el-dialog>
  </div>
</template>

<script>
import { listDept } from '@/api/system/dept'
import { getPlaceList } from '@/api/basicdata/index'
import { listWarehouse } from '@/api/basicdata/warehouse'
import selectMaterial from '@/views/basicdata/material/component/selectMaterial.vue'
import selectLeader from '../../../purchase/apply/component/selectLeader.vue'
import { addCheckPlus, updateCheckPlus } from '@/api/assetmanagement/takeStock.js'
export default {
  name: 'Add',
  components: { selectLeader, selectMaterial },
  dicts: ['assets_enable_status'],
  props: {},
  data () {
    // 金额校验规则
    const validatorPrice = (rule, value, callback) => {
      // let reg = /^(([1-9]{1}\d*)|(0{1}))(\.\d{0,2})$/g; //小数限制0-2位
      const reg = /(^[1-9]{1}[0-9]*$)|(^[0-9]*\.[0-9]{1,}$)/
      if (value && !reg.test(value)) return callback(new Error('请输入正确价格'))
      callback()
    }
    // 金额校验规则
    // const validatorRate = (rule, value, callback) => {
    //   // 0-1之间的小数，最多为三位小数
    //   const reg = /^0\.[0-9]{1,3}$|^0{1}$|^1{1}$|^1\.[0]{1,3}$/
    //   // 百分号
    //   const reg2 = /^(\d|[1-9]\d|100)(\.\d{1,2})?%$/
    //   if (value && !reg.test(value) && !reg2.test(value)) return callback(new Error('请输入正确税率'))
    //   callback()
    // }
    return {
      taskId: '',
      addForm: {
        id: '',
        name: '',
        category: '',
        brand: '',
        standard: '',
        unit: '',
        code: '',
        userId: '',
        userName: '',
        deptId: '',
        deptName: '',
        placeId: '',
        price: '',
        taxAmount: '',
        status: '0', // 0闲置3使用,
        warehouse: ''
      },
      selectLeaderDig: false,
      isShowType: false,
      deptData: [],
      deptDataAll: [],
      placeData: [],
      warehouseData: [],
      checkedUser: {},
      pickerOptions: {
        // 禁用当前日期之前的日期
        // disabledDate(time) {
        //   // Date.now()是javascript中的内置函数，它返回自1970年1月1日00:00:00 UTC以来经过的毫秒数。
        //   return time.getTime() < Date.now() - 8.64e7
        // }
      },
      addFormRule: {
        name: [
          { required: true, message: '请选择资产', trigger: ['change', 'blur'] }
        ],
        deptId: [
          { required: true, message: '请选择部门', trigger: ['change', 'blur'] }
        ],
        warehouse: [
          { required: true, message: '请选择存放仓库', trigger: ['change', 'blur'] }
        ],
        price: [
          { required: true, message: '请填写单价', trigger: ['change', 'blur'] },
          { validator: validatorPrice, message: '请填写正确的金额', trigger: ['change', 'blur'] }
        ],
        taxAmount: [
          { validator: validatorPrice, message: '请填写正确的税额', trigger: ['change', 'blur'] }
        ]
      },
      assetStatus: [
        { label: '使用', value: '3' },
        { label: '闲置', value: '0' }
      ]
    }
  },
  computed: {
  },
  watch: {
  },
  created () {
    this.getListDept()
    this.getPlaceList()
    this.getListWarehouse()
  },
  methods: {
    getFormData(data) {
      this.addForm = {
        id: data.id,
        name: data.name,
        category: data.type,
        brand: data.brandName,
        standard: data.standard,
        unit: data.unit,
        code: data.code,
        userId: data.userId,
        userName: data.userName,
        deptId: Number(data.deptId),
        deptName: data.deptName,
        placeId: Number(data.placeId),
        price: data.price,
        taxAmount: data.taxAmount,
        warehouse: data.warehouseId || '',
        status: data.usageStatus
      }
    },
    // 重置表单，清除校验
    resetAddForm() {
      this.addForm = {
        name: '',
        category: '',
        brand: '',
        standard: '',
        unit: '',
        code: '',
        userId: '',
        userName: '',
        deptId: '',
        deptName: '',
        placeId: '',
        price: '',
        taxAmount: '',
        status: '0', // 0闲置3使用,
        warehouse: ''
      }
      this.$refs.addForm.resetFields()
    },
    /** 选择物料 */
    showType() {
      this.isShowType = true
    },
    /** 关闭选择物料 */
    closeType() {
      this.isShowType = false
    },
    /** 监听子组件选择资产事件 */
    selectMaterial(data) {
      this.isShowType = false
      this.$set(this.addForm, 'name', data.name)
      this.$set(this.addForm, 'brand', data.brand)
      this.$set(this.addForm, 'standard', data.standard)
      this.$set(this.addForm, 'unit', data.unit)
      this.$set(this.addForm, 'code', data.code)
      const category = data.categoryNameLevel4 || data.categoryNameLevel3 || data.categoryNameLevel2 || data.categoryNameLevel1
      this.$set(this.addForm, 'category', category)
    },
    clearChosen(value) {
      this.isShowType = value
    },
    changePlace(e) {

    },
    // 添加或修改盘盈资产
    addOrUpdateCheckPlus() {
      const data = {
        taskId: this.taskId,
        id: this.addForm.id || undefined,
        goodCode: this.addForm.code,
        userId: this.addForm.userId || undefined,
        deptId: this.addForm.deptId || undefined,
        placeId: this.addForm.placeId || undefined,
        taxAmount: this.addForm.taxAmount || undefined,
        price: this.addForm.price || undefined,
        usageStatus: this.addForm.status,
        warehouseId: this.addForm.warehouse || undefined
      }

      const Api = data.id ? updateCheckPlus : addCheckPlus
      Api(data).then(
        (response) => {
          if (response.code === 200) {
            this.$emit('addsucess')
            this.$modal.msgSuccess('保存成功')
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    submit() {
      this.$refs.addForm.validate((valid) => {
        if (valid) {
          this.addOrUpdateCheckPlus()
        } else {
          return false
        }
      })
    },
    // 获取部门信息
    getListDept () {
      listDept({}).then(response => {
        this.deptDataAll = response.data
        if (this.addForm.status === '0') {
          this.deptData = this.deptDataAll.filter(e => e.parentId === 100)// 取树形第二级
        } else {
          this.deptData = this.deptDataAll.filter(e => e.ancestors.split(',').length === 3)// 取树形最后一级
        }
      })
    },
    // 获取位置信息
    getPlaceList () {
      getPlaceList({}).then(response => {
        this.placeData = response.data
      })
    },
    /* 获取仓库信息 */
    getListWarehouse () {
      listWarehouse({ pageSize: 999 }).then(response => {
        this.warehouseData = response.data.records
      })
    },
    changeStatus(value) {
      console.log(value, this.deptDataAll, 2222)
      if (value === '0') {
        this.deptData = this.deptDataAll.filter(e => e.parentId === 100)// 取树形第二级
      } else {
        this.deptData = this.deptDataAll.filter(e => e.ancestors.split(',').length === 3)// 取树形最后一级
      }
    },
    showLeaderDig () {
      this.selectLeaderDig = true
    },
    closeLeaderDig () {
      this.selectLeaderDig = false
    },
    // 选择申领人
    getCheckUser (items) {
      this.checkedUser = items[0]
    },
    // 保存申领人
    submitCheckLeader () {
      this.$set(this.addForm, 'userName', this.checkedUser.realName)
      this.$set(this.addForm, 'userId', this.checkedUser.userId)
      this.$set(this.addForm, 'deptName', this.checkedUser.deptName)
      this.$set(this.addForm, 'deptId', this.checkedUser.deptId)

      this.selectLeaderDig = false
    },
    // 清空已选
    clearSelected () {
      this.checkedUser = {}
      this.$refs.selectLeader.$refs.rowRef.setCurrentRow()
    }

  }
}
</script>

<style lang="scss" scoped>
.addCss{
  &-form{
  width: 660px;
    display: flex;
  flex-wrap: wrap;
  ::v-deep .el-form-item {
      margin-right: 10px;
    }
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
    ::v-deep .el-date-editor .el-input__inner {
      border: none;
    }
  }
   .form-footer{
    text-align: end;
  }
}
</style>
