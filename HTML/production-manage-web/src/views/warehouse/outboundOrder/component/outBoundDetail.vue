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
        <div>
          <div class="add-form">
            <el-form-item label="资产类别" prop="categoryNameLevel">
              <el-input
                v-model.trim="form.categoryNameLevel"
                disabled
                maxlength="15"
                size="small"
                class="input-width"
              />
            </el-form-item>
            <el-form-item label="资产名称" prop="materialName">
              <el-input
                v-model="form.materialName"
                maxlength="11"
                size="small"
                class="input-width"
                disabled
              />
            </el-form-item>
            <el-form-item label="品牌" prop="brandName">
              <el-input
                v-model="form.brandName"
                maxlength="11"
                size="small"
                class="input-width"
                disabled
              />
            </el-form-item>
            <el-form-item label="规格型号" prop="standard">
              <div class="standard">{{ form.standard }}</div>

            </el-form-item>
            <el-form-item label="计量单位" prop="unitName">
              <el-input
                v-model="form.unitName"
                clearable
                size="small"
                class="input-width"
                disabled
              />
            </el-form-item>
          </div>
        </div>
      </el-form>
    </collapse>
    <collapse title="详细信息">
      <div>
        <el-table
          ref="logList"
          style="width: 100%"
          border
          :data="dataList"
          highlight-current-row
          max-height="300"
        >
          <el-table-column type="index" fixed label="序号" width="50" />
          <el-table-column
            prop="internalCode"
            label="资产编码"
            align="center"
          />
          <el-table-column prop="price" label="税前单价（元）" align="center" />
          <el-table-column prop="taxAmount" label="税额（元）" align="center" />
        </el-table>
      </div>
    </collapse>
  </div>
</template>

<script>
import collapse from '@/views/basicdata/material/component/collapse'
export default {
  name: 'AddWarehouseOrder',
  dicts: ['outbound_order_origin', 'outbound_order_status', 'warehouse_classification'],
  components: { collapse },
  props: {
    rowData: {
      type: Object,
      default: () => { }
    },
    dataList: {
      type: Array,
      default: () => []
    }
  },
  data () {
    return {
      // 表单参数
      form: {
        id: undefined,
        orderNo: '',
        orderName: '',
        outType: '',
        bizNo: '',
        dept: '',
        people: '',
        fileList: [],
        company: '',
        companyName: '',
        warehousingStatus: '0'
      },
      // 表单校验
      rules: {
        orderName: [
          { required: true, message: '请填写单据名称', trigger: ['blur', 'change'] }
        ],
        outType: [
          { required: true, message: '请选择出库类型', trigger: ['blur', 'change'] }
        ],
        peopleName: [
          { required: true, message: '请选择申领人', trigger: ['blur', 'input', 'change'] }
        ],
        dept: [
          { required: true, message: '请选择申领部门', trigger: ['blur', 'change'] }
        ]
      }
    }
  },
  created () {
  },
  methods: {
    editWarehouse () {
      this.form = this.rowData
    }
  }
}
</script>

<style lang="scss" scoped>
.add-form {
  display: flex;
  flex-wrap: wrap;
  ::v-deep .el-form-item__label {
    padding: 0;
  }
  ::v-deep .el-form-item {
    margin-right: 35px;
  }
  .noIcon {
    ::v-deep .el-input__icon {
      display: none;
    }
  }
}
.input-width {
  ::v-deep .el-input__inner {
    width: 150px;
  }

}
.standard{
  background-color: rgb(245, 247, 250);
  border: 1px solid rgb(228, 231, 237);
  border-radius:4px;
  height: 35px;
  color: rgb(166, 166, 166);
  padding: 0 16px 0 16px;
  overflow:hidden;//超过隐藏
text-overflow:ellipsis;//溢出显示...，

}
.final {
  margin-left: 12px;
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
.fileName:hover {
  color: #4686ef;
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
::v-deep .el-table th.must > .cell:after {
  content: " *";
  color: #ff1818;
}
.outType{
  ::v-deep .el-select {
    width: 100%;
    .el-input__suffix-inner {
      display: none;
    }
  }
}

</style>
