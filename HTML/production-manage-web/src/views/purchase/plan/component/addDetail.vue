<template>
  <div>
    <el-form
      ref="form"
      :model="form"
      :rules="rules"
      label-width="100px"
      style="width: 94%;margin-left:3%"
      class="editForm"
      label-position="top"
    >
      <el-row :gutter="50">
        <el-col :span="12">
          <el-form-item
            label="资产名称"
            prop="materialName"
          >
            <el-input
              v-model.trim="form.materialName"
              placeholder="请选择"
              maxlength="15"
              @focus="showType"
              @input="changeMessage($event)"
            >  <i slot="suffix" class="iconfont icon-danchu1 select-icon" @click="showType" /></el-input>
          </el-form-item>
        </el-col>
        <el-col :span="12">
          <el-form-item label="资产类别">
            <el-input
              v-model="form.materialCategory"
              readonly
              placeholder="选择资产后自动关联"
            />
          </el-form-item>
        </el-col>
      </el-row>
      <el-row :gutter="50">
        <el-col :span="12">
          <el-form-item label="品牌">
            <el-input
              v-model="form.brandName"
              readonly
              placeholder="选择资产后自动关联"
            />
          </el-form-item>
        </el-col>
        <el-col :span="12">
          <el-form-item label="规格型号">
            <el-input
              v-model="form.materialStandard"
              readonly
              placeholder="选择资产后自动关联"
            />
          </el-form-item>
        </el-col>
      </el-row>
      <el-row :gutter="50">
        <el-col :span="12">
          <el-form-item label="单位" prop="unitName">
            <el-input v-model="form.unitName" readonly placeholder="选择资产后自动关联" />
          </el-form-item>
        </el-col>
        <el-col :span="12"> <el-form-item label="计划数量" prop="planQty">
          <el-input
            v-model.number="form.planQty"
            v-trim.positiveInt="form.planQty"
            placeholder="请输入"
            maxlength="11"
            @input="(v) => (form.planQty = v.replace(/[^\d]/g, ''))"
          />
          <!-- -->
        </el-form-item></el-col>
      </el-row>
      <el-row :gutter="50">
        <el-col :span="12"> <el-form-item label="参考供应商" prop="supplierName">
          <el-input v-model.trim="form.supplierName" placeholder="请输入" @focus="supplierShow = true">
            <i slot="suffix" class="iconfont icon-danchu1" style="line-height: 36px;cursor: pointer;" @click="supplierShow = true" />
          </el-input>
        </el-form-item></el-col>
        <el-col :span="12"> <el-form-item label="预期完成时间" prop="expectedCompletionDate">
          <div class="date_box">
            <el-date-picker
              ref="picker"
              v-model="form.expectedCompletionDate"
              size="small"
              :clearable="false"
              :picker-options="pickerOptions"
              value-format="yyyy-MM-dd"
              placeholder="请选择"
            /><i class="el-icon-date data_icon" @click="focusMethod" />
          </div>
        </el-form-item></el-col>
      </el-row>
      <el-row :gutter="50">
        <el-col :span="24"> <el-form-item label="备注" prop="remark">
          <el-input
            v-model="form.remark"
            placeholder="请输入"
            type="textarea"
            maxlength="500"
            show-word-limit
          />
        </el-form-item></el-col>
      </el-row>
      <el-row :gutter="50">
        <el-col :span="24"><el-form-item label="附件" prop="file">
          <attachment-upload v-model="fileList" />
        </el-form-item></el-col>
      </el-row>
    </el-form>
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
    <!-- 供应商选择 -->
    <el-dialog
      title="供应商选择"
      width="1200px"
      :visible.sync="supplierShow"
      append-to-body
      @close="closeSupplier"
    >
      <selectApply ref="selectApply" />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="cancelSupplier">取 消</el-button>
        <el-button v-debounce size="small" type="primary" @click="submitSupplier">保 存</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
// import { getFixedAssetTree } from '@/api/basicdata/index'
import selectMaterial from '@/views/basicdata/material/component/selectMaterial.vue'
import { getToken } from '@/utils/auth'
import { getSupplierList } from '@/api/basicdata/index'
import selectApply from '@/views/purchase/apply/component/selectApply.vue'
import AttachmentUpload from '@/components/AttachmentUpload'
export default {
  components: { selectMaterial, selectApply, AttachmentUpload },
  props: {
    // 接受数据
    forms: {
      type: Object,
      default: () => {}
    }
  },
  data() {
    return {
      pickerOptions: { // 禁用当前日期之前的日期
        disabledDate(time) {
          // Date.now()是javascript中的内置函数，它返回自1970年1月1日00:00:00 UTC以来经过的毫秒数。
          return time.getTime() < Date.now()
        }
      },
      selectIndex: '',
      // 表单数据
      form: {
        materialName: '',
        brandName: '',
        materialStandard: '',
        model: '',
        unitName: '',
        planQty: '',
        applyAmount: 0,
        univalent: '',
        subtotal: '',
        materialCategory: '',
        supplierName: '',
        expectedCompletionDate: '',
        remark: '',
        file: '',
        materialId: '',
        fileList: []
      },
      totalPrice: 0,
      rules: {
        materialName: [{ required: true, message: '请选择', trigger: ['change', 'blur'] }],
        // brandName: [{ required: true, message: '请输入', trigger: 'blur' }],
        model: [{ required: true, message: '请输入', trigger: ['change', 'blur'] }],
        // materialStandard: [{ required: true, message: '请输入', trigger: 'blur' }],
        planQty: [{ required: true, message: '请输入', trigger: ['change', 'blur'] }

        ],
        univalent: [{ required: true, message: '请输入', trigger: ['change', 'blur'] }]
      },
      isShowType: false,
      fileList: [],
      upload: {
        // 是否显示弹出层（用户导入）
        open: false,
        // 弹出层标题（用户导入）
        title: '',
        // 是否禁用上传
        isUploading: false,
        // 是否更新已经存在的用户数据
        updateSupport: 0,
        // 设置上传的请求头部
        headers: { Authorization: 'Bearer ' + getToken() },
        // 上传的地址
        url: process.env.VUE_APP_BASE_API + '/file/upload'
      },
      fileMessage: {},
      // FixedAsset: [],
      supplierOptions: [], // 供应商
      supplierShow: false,
      formData: {},
      queryParams: {
        enable: 0
      }
    }
  },
  watch: {
    // forms(val) {
    //   this.form = val
    //
    // },
    // form: {
    //   handler(newValue, oldValue) {
    //     //
    //     if (newValue.planQty || newValue.univalent) {
    //       this.form.subtotal = newValue.planQty * newValue.univalent
    //     }
    //   },
    //   deep: true
    //   // this.form.subtotal = this.form.applyAmount * this.form.univalent
    //   // return this.form.applyAmount * this.form.univalent
    // }
  },
  created() {
    // this.getTreeselect()
  },
  mounted() {
    // supplierOptions
    this.getSupplierList()
  },
  methods: {
    /** 获取供应商列表*/
    getSupplierList() {
      getSupplierList(this.queryParams).then((response) => {
        this.supplierOptions = (response.data.records && response.data.records.map(record => {
          return {
            label: record.name || '',
            value: record.id
          }
        })) || []
      }
      )
    },
    /** 选择物料 */
    showType() {
      this.isShowType = true
      // this.getTreeselect()
    },
    /** 关闭选择物料 */
    closeType() {
      this.isShowType = false
    },
    // 强制更新测试文本信息框的值
    changeMessage(e) {
      // this.$forceUpdate()
    },
    clearChosen(value) {
      this.isShowType = value
      this.form = {}
    },
    /** 添加明细最终的保存提交 */
    submitForm() {
      this.$refs['form'].validate((valid) => {
        if (valid) {
          if (this.$route.query.id) {
            this.form.mainid = this.$route.query.id
          }
          if (this.selectIndex !== '') {
            this.$emit('updateData', this.form, this.selectIndex, this.fileList)
          } else {
            this.$emit('addData', this.form, this.fileList)
          }
        }
      })
    },
    /** 取消选择 */
    // clearChosen() {
    //   this.isShowType = false
    //   this.form = {}
    //   this.fixedData = {}
    // },
    /** 确认选择物料对象 */
    // submitUserSet() {
    //   if (!this.fixedData.id) {
    //     this.$message.error('请先选择资产')
    //   } else {
    //     // 资产信息自动带入
    //     this.isShowType = false
    //     this.form.detailNo = this.fixedData.code
    //     this.form.materialCategory = this.fixedData.categoryNameLevel1
    //     this.form.materialName = this.fixedData.name
    //     this.form.materialId = this.fixedData.id
    //     this.form.brandName = this.fixedData.brand
    //     this.form.supplierName = this.fixedData.supplier
    //     this.form.materialStandard = this.fixedData.standard
    //     this.form.unitName = this.fixedData.unit
    //     // this.form.supplierName
    //     this.fixedData = {}
    //     this.$refs.form.clearValidate('materialName')
    //   }
    // },
    // getRow(data) {
    //   this.form.materialCategory = data.name
    // },
    /** 左侧查询下拉树data */
    // getTreeselect() {
    //   getFixedAssetTree().then((response) => {
    //     // 树形data赋值
    //     this.FixedAsset = response.data
    //   })
    // },
    /** 监听子组件选择资产事件 */
    selectMaterial(data) {
      this.isShowType = false
      this.form.detailNo = data.code
      // this.form.materialCategory = data.categoryNameLevel3 ? data.categoryNameLevel3 : data.categoryNameLevel2 ? data.categoryNameLevel2 : data.categoryNameLevel1 ? data.categoryNameLevel1 : ''
      // this.form.materialCategory = `${data.categoryNameLevel1 ? data.categoryNameLevel1 : ''} / ${data.categoryNameLevel2 ? data.categoryNameLevel2 : ''} / ${data.categoryNameLevel3 ? data.categoryNameLevel3 : ''} / ${data.categoryNameLevel4 ? data.categoryNameLevel4 : ''}`
      this.form.materialCategory = this.formatAssetsName(data)
      this.$set(this.form, 'materialName', data.name)
      this.form.materialId = data.id
      this.form.brandName = data.brand
      this.form.supplierName = data.supplier
      this.form.materialStandard = data.standard
      this.form.unitName = data.unit
    },
    closeSupplier() {
      this.showSupplierDialog = false
    },
    cancelSupplier() {
      this.showSupplierDialog = false
    },
    /** 设置供应商*/
    submitSupplier() {
      this.form.supplierName = this.$refs.selectApply.currentRow.name
      this.supplierShow = false
    },
    changeSelect(value) {
      const selectedOption = this.supplierOptions.filter(option => value === option.value)
      this.form.supplierName = selectedOption[0] ? selectedOption[0].label : value
    },
    /** 拼接资产名称 */
    formatAssetsName(data) {
      return data.categoryNameLevel4 || data.categoryNameLevel3 || data.categoryNameLevel2 || data.categoryNameLevel1 || ''
    },
    focusMethod() {
      this.$refs.picker.focus()
    }
  }

}
</script>

<style lang="scss" scoped>
.date_box {
  position: relative;
  width: 100%;
  .data_icon {
    position: absolute;
    top: 50%;
    right: 8px;
    z-index: 9;
    color: #c0c4cc;
    font-size: 14px;
    transform: translateY(-50%);
  }
  // ::v-deep .el-range__close-icon {
  //   position: absolute;
  //   right: 34px;
  //   top: 54%;
  //   transform: translateY(-50%);
  // }
}
.role-dialog {
  &::v-deep .el-dialog__body {
    padding: 30px 20px;
    color: #606266;
    font-size: 14px;
    word-break: break-all;
  }
}

</style>
