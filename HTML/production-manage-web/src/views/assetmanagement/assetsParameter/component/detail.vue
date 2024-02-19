<template>
  <div>
    <!-- 基本信息 -->
    <collapse title="基本信息">
      <el-form
        ref="form"
        :model="form"
        label-width="100px"
        label-position="top"
      >
        <div class="base-form">
          <div class="add-form">
            <el-form-item label="资产类别" prop="type">
              <el-input
                v-model="form.category"
                class="input-width"
                disabled
              />
            </el-form-item>
            <el-form-item label="资产编码" prop="internalCode">
              <el-input
                v-model.number="form.internalCode"
                class="input-width"
                disabled
              />
            </el-form-item>
            <el-form-item label="资产名称" prop="name">
              <el-input
                v-model="form.name"
                class="input-width"
                disabled
              />
            </el-form-item>
            <el-form-item label="计量单位" prop="unit">
              <el-input
                v-model.number="form.unit"
                class="input-width"
                disabled
              />
            </el-form-item>
            <el-form-item label="规格型号" prop="standard">
              <el-input
                v-model.trim="form.standard"
                class="input-width"
                disabled
              />
            </el-form-item>
            <el-form-item label="品牌" prop="brandName">
              <el-input
                v-model.trim="form.brandName"
                class="input-width"
                disabled
              />
            </el-form-item>
            <el-form-item label="资产状态" prop="city">
              <el-select
                v-model="form.status"
                clearable
                size="small"
                class="input-width"
                disabled
              >
                <el-option
                  v-for="dict in dict.type.assets_enable_status"
                  :key="dict.value"
                  :label="dict.label"
                  :value="dict.value"
                />
              </el-select>
            </el-form-item>
            <el-form-item label="存放仓库" prop="warehouseName">
              <el-input
                v-model.trim="form.warehouseName"
                disabled
                class="input-width"
              />
            </el-form-item>
            <!-- <el-form-item label="购置金额（元）" prop="unit">
              <el-input v-model="form.unit" placeholder="请输入" class="input-width" disabled />
            </el-form-item> -->
            <el-form-item label="购置时间" prop="purchaseDate" class="final">
              <el-input
                v-model.number="form.purchaseDate"
                disabled
              />
            </el-form-item>
            <el-form-item label="税前单价（元）" prop="price" class="final">
              <el-input
                v-model.number="form.price"
                disabled
              />
            </el-form-item>
            <el-form-item label="税额（元）" prop="rate" class="final">
              <el-input
                v-model.number="form.taxAmount"
                disabled
              />
            </el-form-item>
            <!-- <el-form-item label="税后金额" prop="ratePrice" class="final">
              <el-input
                v-model.number="form.ratePrice"
                placeholder="系统自动生成"
                disabled
              />
            </el-form-item> -->
          </div>

          <div class="qrcode">
            <div>资产二维码</div>
            <div class="qrcode-img">
              <img :src="form.codeIcon" width="156px" height="156px">
            </div>
          </div>
        </div>
      </el-form>
    </collapse>

    <!-- 操作日志 新增/编辑时不显示-->
    <collapse title="操作记录">
      <div>
        <el-table
          ref="useRecords"
          style="width: 100%"
          border
          :data="useRecords"
          highlight-current-row
          max-height="200"
        >
          <el-table-column prop="company" label="单位" align="center" />
          <el-table-column prop="dept" label="部门" align="center" />
          <el-table-column prop="people" label="操作人" align="center" />
          <el-table-column prop="type" label="业务类型" align="center" />
          <el-table-column prop="time" label="操作时间" align="center" />
        </el-table>
      </div>
    </collapse>
  </div>
</template>

<script>
import collapse from '@/views/basicdata/material/component/collapse'
import { listDept } from '@/api/system/dept'
import { listWarehouse } from '@/api/basicdata/warehouse'
export default {
  name: 'AddWarehouseOrder',
  dicts: ['material_type', 'assets_enable_status'],
  components: { collapse },
  props: {
    baseForm: {
      type: Object,
      default: () => { }
    },
    useRecords: {
      type: Array,
      default: () => []
    }

  },
  data () {
    return {
      selectDetailDig: false,
      multipleSelection: [], // 多选物料
      // 表单参数
      form: {
        id: undefined,
        type: '',
        code: '',
        unit: '',
        standard: '',
        brandName: '',
        status: '',
        place: '',
        codeIcon: '',
        price: null,
        rate: null,
        ratePrice: ''
        // 购置金额
        // acquireDate: '',
      },
      assetList: [
        // { qrCode: '', rfid: '22564', code: 'sjahdauh' },
        // { qrCode: '', rfid: '22564', code: 'sjahdauh' }
      ],
      deptData: [],
      warehouseData: []

    }
  },
  watch: {
    form: {
      immediate: true,
      deep: true,
      handler (val) {
        val.ratePrice = val.price * (1 + val.rate).toFixed(2) || 0
        //
      }
    }
  },
  created () {
    this.getListDept()
    this.getListWarehouse()
  },
  methods: {
    /** 删除行 */
    deleteRow (index, rows) {
      rows.splice(index, 1)
    },

    /** 删除行 */
    removeDetail () {
      if (this.multipleSelection.length === 0) {
        this.$message.error('请至少选择一个物料')
        return
      }
      this.multipleSelection.forEach(e => {
        this.delList(e)
      })
    },
    // 删除数据
    delList (row) {
      this.$emit('delDetail', row)
    },
    getForm () {
      this.form = this.baseForm
    },
    showApplyDig () {
      this.selectDetailDig = true
    },

    handleSelectionChange (val) {
      this.multipleSelection = val
    },

    // 获取部门信息
    getListDept () {
      listDept({}).then(response => {
        this.deptData = response.data
      })
    },
    /* 获取仓库信息 */
    getListWarehouse () {
      listWarehouse({ pageSize: 999 }).then(response => {
        this.warehouseData = response.data.records
      })
    }
  }
}
</script>

<style lang="scss" scoped>
.base-form {
  display: flex;
  align-items: center;
  .qrcode {
    width: 23%;
    .qrcode-img {
      width: 272px;
      height: 188px;
      background: #ffffff;
      border-radius: 2px;
      border: 1px solid #dddddd;
      display: flex;
      justify-content: center;
      align-items: center;
    }
  }
}
.add-form {
  width: 70%;
  display: flex;
  flex-wrap: wrap;
  ::v-deep .el-form-item__label {
    padding: 0;
  }
  ::v-deep .el-form-item {
    margin-right: 35px;
  }
}
.input-width {
  ::v-deep .el-input__inner {
    width: 200px;
  }
}
.final {
  ::v-deep .el-input__inner {
    width: 200px;
  }
  ::v-deep .el-form-item__label {
    padding: 0;
  }
}
.addOrRemove {
  display: flex;
  justify-content: flex-end;
  margin-bottom: 15px;
  margin-right: 10px;
}
.input-class {
  ::v-deep .el-input__inner {
    border: none;
  }
}
.fileTip {
  color: #b2b5b9;
  font-size: 14px;
  padding-left: 8px;
}
.fileName {
  cursor: pointer;
}
.file-list {
  display: flex;
  justify-content: space-between;
  margin-top: 10px;
}
.popover-title {
  display: flex;
  justify-content: space-between;
  margin-bottom: 10px;
}
.asset-item {
  width: 296px;
  height: 88px;
  border-bottom: 1px solid #dddddd;
  padding: 12px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  .item-left {
    width: 80%;
    display: flex;
    align-items: center;
    justify-content: space-between;
  }
}
.assetSelected {
  background: #f1f6fd;
}
.asset-qrcode {
  width: 56px;
  height: 56px;
}
.asset-icon {
  width: 20px;
  height: 20px;
  color: #4686ef;
}
.print {
  margin-top: 20px;
}
</style>
