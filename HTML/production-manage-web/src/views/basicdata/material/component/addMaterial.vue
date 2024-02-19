<template>
  <div>
    <div class="produce-collapse">
      <el-form ref="mainForm" :model="form" :rules="rules" :validate-on-rule-change="false">
        <collapse title="基本信息">
          <el-descriptions class="base_info_desc" :column="2" border>
            <el-descriptions-item>
              <template slot="label"> 物料编码 </template>
              <el-input v-model="form.code" disabled placeholder="系统自动生成" />
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                物料名称<span v-if="isEdit" class="redDot">*</span>
              </template>
              <el-form-item prop="name">
                <el-input
                  v-model.trim="form.name"
                  :disabled="isDisabled"
                  clearable
                  :class="{ priceInputClass: isNameWarn }"
                  :placeholder="isEdit?'请输入':''"
                  minlength="2"
                  maxlength="15"
                  @blur="validateForm()"
                  @input="nameInput()"
                >
                  <!-- <span v-if="isNameWarn" slot="suffix" class="warnTip">{{
                    warnText
                  }}</span> -->
                </el-input>
              </el-form-item>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                物料类型<span v-if="isEdit" class="redDot">*</span>
              </template>
              <el-form-item prop="type">
                <el-select
                  v-if="isEdit"
                  v-model.trim="form.type"
                  clearable
                  :class="{ typeSelectClass: isTypeWarn }"
                  :disabled="isDisabled"
                  :placeholder="isEdit?'请选择':''"
                  style="width: 100%"
                  class="select-width"
                  @change="getMaterialCode"
                  @input="typeInput()"
                >
                  <el-option
                    v-for="dict in dict.type.material_type"
                    :key="dict.value"
                    :label="dict.label"
                    :value="dict.value"
                  />
                </el-select>
                <el-input v-else v-model.trim="materialName" :disabled="isDisabled" />
              </el-form-item>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                供应商<span v-if="isEdit" class="redDot">*</span>
              </template>
              <el-form-item v-if="isEdit" prop="supplier">
                <div class="my-select-con">
                  <el-select
                    v-model.trim="form.supplier"
                    filterable
                    clearable
                    style="width: 100%"
                    placeholder="请输入"
                    class="select-width"
                    @change="getSupplierLabel"
                  >
                    <el-option
                      v-for="item in supplierOptions"
                      :key="item.id"
                      :label="item.label"
                      :value="item.value"
                      width="340"
                    />
                  </el-select>
                  <i slot="suffix" class="iconfont icon-bumenxuanze" style="font-size: 14px;line-height: 36px;cursor: pointer;" @click.stop="showSupplier" />
                </div>
              </el-form-item>
              <span
                v-else
                class="spandisabled"
                style="margin-left: 15px; color: #606266"
              >
                {{ form.supplier }}
              </span>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">品牌<span v-if="isEdit" class="redDot">*</span> </template>
              <el-form-item v-if="isEdit" prop="brandId">
                <div class="my-select-con">
                  <el-select
                    v-model.trim="form.brand"
                    filterable
                    clearable
                    style="width: 100%"
                    placeholder="请输入"
                    class="select-width"
                    @change="getBrandLabel"
                  >
                    <el-option
                      v-for="item in brandOptions"
                      :key="item.id"
                      :label="item.label"
                      :value="item.value"
                      width="340"
                    />
                  </el-select>
                  <i slot="suffix" class="iconfont icon-bumenxuanze" style="font-size: 14px;line-height: 36px;cursor: pointer;" @click.stop="showBrand" />
                </div>
              </el-form-item>
              <span
                v-else
                class="spandisabled"
                style="margin-left: 15px; color: #606266"
              >
                {{ form.brand }}
              </span>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                规格型号<span v-if="isEdit" class="redDot">*</span>
              </template>
              <el-form-item prop="standard">
                <el-input
                  v-model.trim="form.standard"
                  minlength="1"
                  maxlength="200"
                  show-word-limit
                  clearable
                  :class="{ standardInputClass: isStandard }"
                  :disabled="isDisabled"
                  :placeholder="isEdit?'请输入':''"
                  @input="standardInput()"
                >
                  <!-- <span v-if="isStandard" slot="suffix" class="warnTip">{{
                    warnText
                  }}</span> -->
                </el-input>
              </el-form-item>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label"> 生产商 </template>
              <el-input
                v-model.trim="form.manufacturer"
                maxlength="15"
                :class="{ manufacturerInputClass: isManufacturer }"
                :disabled="isDisabled"
                :placeholder="isEdit?'请输入':''"
                @blur="checkManufacturer()"
              >
                <span v-if="isManufacturer" slot="suffix" class="warnTip">{{
                  warnText
                }}</span>
              </el-input>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label"> 产地 </template>
              <el-form-item prop="originPlace">
                <el-input
                  ref="originPlaceInputRef"
                  v-model.trim="form.originPlace"
                  :class="{ originPlaceInputClass: isOriginPlace }"
                  maxlength="6"
                  :disabled="isDisabled"
                  :placeholder="isEdit?'请输入':''"
                  @blur="originPlaceInput"
                >
                  <span v-if="isOriginPlace" slot="suffix" class="warnTip">{{
                    warnText
                  }}</span>
                </el-input>
              </el-form-item>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label"> 计量单位 </template>
              <!-- <el-input
                v-if="isEdit"
                v-model.trim="form.unit"
                class="addInputItem"
                clearable
                disabled
                :placeholder="isEdit?'点击图标选择':''"
                @focus="showUnit"
              >
                <template v-if="!!form.unit">
                  <span
                    slot="suffix"
                    style="line-height: 48px; cursor: pointer; margin-right: 5px"
                    @click="deleteUnit()"
                  >x</span>
                </template>
                <i
                  slot="suffix"
                  class="iconfont icon-bumenxuanze"
                  style="line-height: 48px; cursor: pointer"
                  @click="showUnit"
                />
              </el-input> -->
              <el-form-item v-if="isEdit">
                <div class="my-select-con">
                  <el-select
                    ref="unitIdRef"
                    v-model.trim="form.unit"
                    filterable
                    clearable
                    style="width: 100%"
                    placeholder="请输入"
                    class="select-width"
                    @change="getUnitLabel"
                  >
                    <el-option
                      v-for="item in unitOptions"
                      :key="item.id"
                      :label="item.label"
                      :value="item.value"
                      width="340"
                    />
                  </el-select>
                  <i slot="suffix" class="iconfont icon-bumenxuanze" style="font-size: 14px;line-height: 36px;cursor: pointer;" @click.stop="showUnit" />
                </div>
              </el-form-item>
              <span
                v-else
                class="spandisabled"
                style="margin-left: 15px; color: #606266"
              >
                {{ form.unit }}
              </span>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label"> 核算科目 </template>
              <el-select
                v-if="isEdit"
                v-model.trim="form.category"
                clearable
                :disabled="isDisabled"
                :placeholder="isEdit?'请选择':''"
                style="width: 100%"
                class="select-width"
                @change="categoryChange"
              >
                <el-option
                  v-for="dict in dict.type.check_subject_type"
                  :key="dict.value"
                  :label="dict.label"
                  :value="dict.value"
                />
              </el-select>
              <el-input v-else v-model="form.categoryName" :disabled="isDisabled" />
            </el-descriptions-item>

            <el-descriptions-item :span="2">
              <template slot="label"> 条形码 </template>
              <el-input
                v-model.trim="form.barCode"
                maxlength="15"
                show-word-limit
                :class="{ barcodeInputClass: isBarCodeWarn }"
                :disabled="isDisabled"
                :placeholder="isEdit?'请输入':''"
                @input="barCodeInput()"
              >
                <span
                  v-if="isBarCodeWarn"
                  slot="suffix"
                  class="warnTip"
                >请填写数字或英文</span>
              </el-input>
            </el-descriptions-item>
          </el-descriptions>
        </collapse>
        <collapse v-if="!isAdd" title="折旧信息">
          <el-descriptions class="base_info_desc" :column="2" border>
            <el-descriptions-item>
              <template slot="label">
                规定年限<span v-if="isEdit" class="redDot">*</span>
              </template>
              <el-form-item prop="serviceLife">
                <el-input
                  v-model.number="form.serviceLife"
                  :disabled="isDisabled"
                  clearable
                  :placeholder="isEdit?'请输入':''"
                  onkeyup="value=value.replace(/^(0+)|[^\d]+/g,'')"
                  maxlength="4"
                >
                <!-- <span v-if="isNameWarn" slot="suffix" class="warnTip">{{
                    warnText
                  }}</span> -->
                </el-input>
              </el-form-item>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                残值率<span v-if="isEdit" class="redDot">*</span>
              </template>
              <el-form-item prop="residualValueRate">
                <el-input
                  v-model.number="form.residualValueRate"
                  :disabled="isDisabled"
                  clearable
                  :placeholder="isEdit?'请输入':''"
                  onkeyup="value=value.replace(/[^\d]+/g,'')"
                  maxlength="2"
                >
                  <template slot="append">%</template>
                </el-input>
              </el-form-item>
            </el-descriptions-item>

            <el-descriptions-item>
              <template slot="label">
                折旧方法<span v-if="isEdit" class="redDot">*</span>
              </template>
              <el-form-item prop="depreciationType">
                <el-select
                  v-if="isEdit"
                  key="depreciationType "
                  v-model.trim="form.depreciationType"
                  clearable
                  :disabled="isDisabled"
                  :placeholder="isEdit?'请选择':''"
                  style="width: 100%"
                  class="select-width"
                >
                  <el-option
                    v-for="dict in dict.type.sys_depreciation_type"
                    :key="dict.value"
                    :label="dict.label"
                    :value="dict.value"
                  />
                </el-select>
                <el-input
                  v-else
                  :value="dictionary.returnDictLabel(dict.type.sys_depreciation_type,form.depreciationType)"
                  :disabled="isDisabled"
                  class="depreciationType"
                />
              </el-form-item>
            </el-descriptions-item>

            <el-descriptions-item>
              <template slot="label">
                折旧率
              </template>
              <el-form-item>
                <el-input
                  v-model.number="form.depreciationRate"
                  :disabled="isDisabled"
                  clearable
                  :placeholder="isEdit?'请输入':''"
                  onkeyup="value=value.replace(/[^\d]+/g,'')"
                  maxlength="2"
                >
                  <template slot="append">%</template>
                </el-input>
              </el-form-item>
            </el-descriptions-item>
          </el-descriptions>
        </collapse>
        <collapse title="财政分类" style="padding-bottom: 16px">
          <el-descriptions class="base_info_desc" :column="1" border>
            <el-descriptions-item>
              <template slot="label"> 分类编码 <span v-if="isEdit" class="redDot">*</span></template>
              <el-form-item prop="categoryLevel1">
                <el-cascader
                  v-if="isEdit"
                  ref="cascader"
                  v-model="typeCode"
                  :placeholder="isEdit?'请选择':''"
                  :options="treeData"
                  :props="defaultProps"
                  filterable
                  style="width: 100%"
                  :disabled="!isEdit"
                  :filter-method="codeFilter"
                  @change="cascaderChange"
                />
                <span v-else class="categoryName">{{ typeNameCode }}</span>
              </el-form-item>
              <!-- <el-input
                v-else
                :value="getTypeCode()"
                :disabled="isDisabled"
              /> -->
            </el-descriptions-item>

            <el-descriptions-item :span="2">
              <template slot="label"> 备注 </template>
              <el-input
                v-model.trim="form.remark"
                maxlength="500"
                show-word-limit
                :disabled="isDisabled"
                :placeholder="isEdit?'请输入':''"
              />
            </el-descriptions-item>
          </el-descriptions>
        </collapse>

      </el-form>
    </div>
    <!-- 供应商选择 -->
    <el-dialog
      title="供应商选择"
      width="1000px"
      :visible.sync="showSupplierDialog"
      append-to-body
      @close="closeSupplier"
    >
      <selectSupplier
        @selectSupplier="getSupplier"
        @cancelSupplier="cancelSupplier"
      />
    </el-dialog>
    <!-- 品牌选择 -->
    <el-dialog
      title="品牌选择"
      width="1000px"
      :visible.sync="showBrandDialog"
      append-to-body
      @close="closeBrand"
    >
      <selectBrand @selectBrand="getBrand" @cancelBrand="cancelBrand" />
    </el-dialog>
    <!-- 计量单位选择 -->
    <el-dialog
      title="计量单位选择"
      width="1000px"
      :visible.sync="showUnitDialog"
      :close-on-click-modal="false"
      append-to-body
      @close="closeUnit"
    >
      <selectUnit :form="form" @selectUnit="getUnit" @cancelUnit="cancelUnit" />
    </el-dialog>
  </div>
</template>

<script>
import collapse from './collapse'
import selectSupplier from './selectSupplier'
import selectBrand from './selectBrand'
import selectUnit from './selectUnit'
import { addOrUpdateMaterialList, getMaterialCode } from '@/api/basicdata/material'
import { getMaterialListByLevel, getSupplierList, getUnitByCondition } from '@/api/basicdata/index'
import { listBrand } from '@/api/basicdata/brand'
export default {

  components: { collapse, selectSupplier, selectBrand, selectUnit },
  dicts: ['enable_status', 'material_type', 'check_subject_type', 'sys_depreciation_type'],
  props: {
    formData: {
      type: Object,
      default: () => { }
    },
    isDisabled: {
      type: Boolean,
      default: false
    },
    isEdit: {
      type: Boolean
    },
    isAdd: {
      type: Boolean
    },
    treeData: {
      type: Array,
      default: () => []
    }
  },
  data () {
    return {
      title: '',
      form: {
        brandId: '',
        category: '',
        categoryLevel1: '',
        categoryLevel2: '',
        categoryLevel3: '',
        categoryLevel4: '',
        categoryName: '',
        categoryNameLevel1: '',
        categoryNameLevel2: '',
        categoryNameLevel3: '',
        categoryNameLevel4: '',
        code: '',
        enable: '0',
        manufacturer: '',
        name: '', // 物料名称
        originPlace: '', // 产地
        remark: '', // 备注
        standard: '', // 规格型号
        supplierId: '', // 供应商id
        type: '', // 物料类型
        unitId: '', // 计量单位id
        barCode: '',
        supplier: '',
        brand: '',
        unit: '',
        depreciationRate: '',
        depreciationType: '', // 方法
        residualValueRate: '',
        serviceLife: ''
      },

      isCodeWarn: false,
      isNameWarn: false,
      isBarCodeWarn: false,
      isStandard: false,
      isManufacturer: false,
      isTypeWarn: false,
      isSupplierWarn: false,
      isBrandWarn: false,
      isOriginPlace: false,
      warnText: '请填写此项',
      optionsLevel1: [],
      optionsLevel2: [],
      optionsLevel3: [],
      // 选择供应商
      showSupplierDialog: false,
      // 选择品牌
      showBrandDialog: false,
      // 计量单位
      showUnitDialog: false,
      // 层级查询对象
      params: {
        name: '',
        parentCode: ''
      },
      materialName: '',
      materialEnable: '',
      reg1: /^[a-zA-Z0-9\s]+$/,
      rules: {
        name: [{ required: true, message: '请填写此项' },
          { min: 2, max: 15, message: '物料名称长度在2-15个字符' }],
        type: [{ required: true, message: '请选择' }],
        supplier: [{ required: true, message: '请选择' }],
        brandId: [{ required: true, message: '请选择' }],
        standard: [{ required: true, message: '请填写此项' }],
        categoryLevel1: [{ required: true, message: '请选择' }],
        serviceLife: [
          { required: true, message: '请输入年限', trigger: 'blur' }
        ],
        depreciationType: [
          { required: true, message: '请输入折旧方法', trigger: ['change'] }
        ],
        residualValueRate: [
          { required: true, message: '请输入残值率', trigger: 'blur' }
        ]
        // depreciationRate: [
        //   { required: true, message: '请输入折旧率', trigger: 'blur' }
        // ]
      },
      typeCode: [],
      typeNameCode: [],
      defaultProps: {
        children: 'child',
        label: 'name',
        value: 'code'
      },
      // 供应商
      supplierOptions: [],
      brandOptions: [],
      unitOptions: []
    }
  },
  watch: {
    'form': {
      handler(newVal, oldVal) {
        this.form.brandId = newVal.brandId
        this.form.unitId = newVal.unitId
      },
      deep: true,
      immediate: true
    }
  },
  created () {
    this.getMaterialListByLevel()
    this.getSupplierList()
    this.getBrandList()
    this.getUnitByCondition()
  },
  methods: {
    //
    categoryChange(val) {
      this.dict.type.check_subject_type.map(item => {
        if (item.value === val) {
          this.form.categoryName = item.label
        }
      })
      // console.log('this.form.categoryName', this.form.categoryName)
    },
    /** 层级查询*/
    getMaterialListByLevel () {
      getMaterialListByLevel(this.params).then(
        res => {
          this.optionsLevel1 = res.data
        }
      )
    },
    // 自动生成供应商编码
    getMaterialCode () {
      // console.log(this.form.type, 555555555)
      if (!this.form.type) {
        return
      }
      getMaterialCode({ type: this.form.type }).then((res) => {
        if (res.code === 200) {
          this.form.code = res.data
        }
      })
    },
    /** 重置表单*/
    reset () {
      this.form = {
        brandId: '',
        category: '',
        categoryLevel1: '',
        categoryLevel2: '',
        categoryLevel3: '',
        categoryLevel4: '',
        categoryName: '',
        categoryNameLevel1: '',
        categoryNameLevel2: '',
        categoryNameLevel3: '',
        categoryNameLevel4: '',
        code: '',
        enable: '0',
        manufacturer: '',
        name: '', // 物料名称
        originPlace: '', // 产地
        remark: '', // 备注
        standard: '', // 规格型号
        supplierId: '', // 供应商id
        type: '', // 物料类型
        unitId: '', // 计量单位id
        barCode: '',
        supplier: '',
        brand: '',
        unit: '',
        depreciationRate: '',
        depreciationType: '',
        residualValueRate: '',
        serviceLife: ''
      }
      this.isCodeWarn = false
      this.isNameWarn = false
      this.isTypeWarn = false
      this.isStandard = false
      this.isBarCodeWarn = false
      this.isManufacturer = false
      this.isSupplierWarn = false
      this.isBrandWarn = false
      this.isOriginPlace = false
      this.typeCode = []
      this.typeNameCode = []
      this.$refs.mainForm.clearValidate()
    },
    /** 表单校验*/
    validateForm () {
      if (!this.form.name) {
        this.isNameWarn = true
        this.warnText = '请填写此项'
        return false
      }
      if (this.form.name.length < 2 || this.form.name.length > 15) {
        this.isNameWarn = true
        this.warnText = '物料名称长度在2-15个字符'
        return false
      }
      if (!this.form.type) {
        this.isTypeWarn = true
        this.warnText = '请选择'
        return false
      }
      if (!this.form.supplierId) {
        this.isSupplierWarn = true
        this.warnText = '请选择'
        return false
      }
      if (!this.form.brand) {
        this.isBrandWarn = true
        this.warnText = '请选择'
        return false
      }
      if (!this.form.standard) {
        this.isStandard = true
        this.warnText = '请填写此项'
        return false
      }
      if (this.form.originPlace && this.form.originPlace.length > 6) {
        this.isContractNoWarn = true
        this.warnText = '长度要小于6'
        return false
      }
      // if (!this.reg1.test(this.form.barCode)) {
      //   this.isBarCodeWarn = true
      //   this.warnText = '仅支持数字或英文'
      //   return false
      // }
      return true
    },
    /** 生产商输入校验 */
    checkManufacturer () {
      const reg = /^[\u4e00-\u9fa5a-zA-Z]+$/
      if (!reg.test(this.form.manufacturer)) {
        this.isManufacturer = true
        this.warnText = '仅支持汉字和英文'
        return
      }
      this.isManufacturer = false
    },
    /** 提交表单*/
    submitForm () {
      // this.form.map(item => {
      //   item.depreciationRate = this.form.depreciationRat / 100
      //   item.residualValueRate = this.form.residualValueRate / 100
      //   console.log('irem', item)
      // })
      this.$refs.mainForm.validate((valid) => {
        if (valid) {
          // 处理折旧数据
          addOrUpdateMaterialList({ ...this.form, depreciationRate: this.form.depreciationRate / 100, residualValueRate: this.form.residualValueRate / 100 }).then((res) => {
            if (res.code === 200) {
              const obj = {
                code: res.code,
                id: this.form.id
              }
              this.$emit('submitMaterialList', obj)
            }
          })
        }
      })
    },
    numInput () {
      if (this.form.code) {
        this.isCodeWarn = false
      }
    },
    nameInput () {
      if (this.form.name) {
        this.isNameWarn = false
      }
    },
    barCodeInput () {
      if (this.form.barCode && this.reg1.test(this.form.barCode)) {
        this.isBarCodeWarn = false
      } else {
        this.isBarCodeWarn = true
      }
    },
    standardInput () {
      if (this.form.standard) {
        this.isStandard = false
      }
    },
    supplierInput () {
      if (this.form.supplier) {
        this.isSupplierWarn = false
      }
    },
    typeInput () {
      if (this.form.type) {
        this.isTypeWarn = false
      }
    },
    // 产地输入校验
    originPlaceInput () {
      const reg = /^[a-zA-Z\u4e00-\u9fa5]+$/
      if (this.$refs.originPlaceInputRef.value !== '') {
        if (!reg.test(this.form.originPlace)) {
          this.isOriginPlace = true
          this.warnText = '仅支持汉字和英文'
          return
        } else {
          this.isOriginPlace = false
          this.warnText = ''
        }
      } else {
        this.isOriginPlace = false
        this.warnText = ''
      }
    },
    /** 编辑时设置表单对象*/
    editMaterial () {
      this.form = this.formData
      if (this.formData.type === '0') {
        this.materialName = '固定资产'
      } else {
        this.materialName = '低值易耗品'
      }
      if (this.formData.enable === '0') {
        this.materialEnable = '正常'
      } else {
        this.materialEnable = '停用'
      }
      if (this.form.depreciationRate === 0) {
        this.form.depreciationRate = ''
      }
      if (this.form.residualValueRate === 0) {
        this.form.residualValueRate = ''
      }
      // this.form.categoryName = this.getCategoryName(this.form.category)
      this.getTypeCode()
    },
    /** 获取核算科目名称 */
    getCategoryName (value) {
      const map = new Map([
        ['0', '土地、房屋及构筑物'],
        ['1', '通用设备'],
        ['2', '专用设备'],
        ['3', '文物和陈列品'],
        ['4', '图书、档案'],
        ['5', '家具、用具、装具及动植物']
      ])
      return map.get(value)
    },
    /** 改变分类lcode下拉选项*/
    changeLevel1 (value) {
      this.form.categoryNameLevel2 = ''
      this.form.categoryLevel2 = ''
      this.form.categoryNameLevel3 = ''
      this.form.categoryLevel3 = ''
      this.optionsLevel3 = []
      this.optionsLevel1.forEach(option => {
        if (option.code === value) {
          this.form.categoryNameLevel1 = option.name
          // this.form.category = option.name
        }
      })
      this.params.parentCode = value
      getMaterialListByLevel(this.params).then(
        res => {
          this.optionsLevel2 = res.data
        }
      )
    },
    /** 改变分类lname下拉选项*/
    changeNameLevel1 (value) {
      this.form.categoryNameLevel2 = ''
      this.form.categoryLevel2 = ''
      this.form.categoryNameLevel3 = ''
      this.form.categoryLevel3 = ''
      this.optionsLevel3 = []
      // this.form.category = value
      this.optionsLevel1.forEach(option => {
        if (option.name === value) {
          this.form.categoryLevel1 = option.code
          this.params.parentCode = option.code
          getMaterialListByLevel(this.params).then(
            res => {
              this.optionsLevel2 = res.data
            }
          )
        }
      })
    },
    getOne () {
      if (this.form.categoryLevel1 === '' && this.form.categoryNameLevel1 === '') {
        this.$message.error('请先选择1级分类')
      }
    },
    getTwo () {
      if (this.form.categoryNameLevel2 === '' && this.form.categoryNameLevel2 === '') {
        this.$message.error('请先选择2级分类')
      }
    },
    /** 改变分类2下拉选项*/
    changeLevel2 (value) {
      this.form.categoryNameLevel3 = ''
      this.form.categoryLevel3 = ''
      this.params.parentCode = value
      this.optionsLevel2.forEach(option => {
        if (option.code === value) {
          this.form.categoryNameLevel2 = option.name
        }
      })
      getMaterialListByLevel(this.params).then(
        res => {
          this.optionsLevel3 = res.data
        }
      )
    },
    /** 改变分类2name下拉选项*/
    changeNameLevel2 (value) {
      this.form.categoryNameLevel3 = ''
      this.form.categoryLevel3 = ''
      this.optionsLevel2.forEach(option => {
        if (option.name === value) {
          this.form.categoryLevel2 = option.code
          this.params.parentCode = option.code
          getMaterialListByLevel(this.params).then(
            res => {
              this.optionsLevel3 = res.data
            }
          )
        }
      })
    },
    /** 改变分类3下拉选项*/
    changeLevel3 (value) {
      this.optionsLevel3.forEach(option => {
        if (option.code === value) {
          this.form.categoryNameLevel3 = option.name
        }
      })
    },
    /** 改变分类3name下拉选项*/
    changeNameLevel3 (value) {
      this.optionsLevel3.forEach(option => {
        if (option.name === value) {
          this.form.categoryLevel3 = option.code
        }
      })
    },
    /** 显示供应商选择对象*/
    showSupplier () {
      this.showSupplierDialog = true
    },
    closeSupplier () {
      this.showSupplierDialog = false
    },
    /** 取消选择 */
    cancelSupplier (value) {
      this.showSupplierDialog = value
    },
    getSupplier (payload) {
      if (payload.id) {
        this.showSupplierDialog = false
        this.isSupplierWarn = false
      }
      this.form.supplier = payload.name
      this.form.supplierId = payload.id
    },
    /** 显示品牌*/
    showBrand () {
      this.showBrandDialog = true
    },
    cancelBrand (value) {
      this.showBrandDialog = value
    },
    closeBrand () {
      this.showBrandDialog = false
    },
    getBrand (payload) {
      if (payload.id) {
        this.showBrandDialog = false
        this.isBrandWarn = false
      }
      this.form.brand = payload.chineseName || payload.englishName
      this.form.brandId = payload.id
    },
    /** 显示计量单位*/
    showUnit () {
      this.showUnitDialog = true
    },
    closeUnit () {
      this.showUnitDialog = false
    },
    cancelUnit (value) {
      this.showUnitDialog = value
    },
    getUnit (payload, flog) {
      if (flog) {
        this.showUnitDialog = false
      }
      this.form.unit = payload.base
      this.form.unitId = payload.id
    },
    deleteUnit () {
      // console.log('删除')
      this.form.unit = ''
      this.form.unitId = ''
    },
    // 修改级联选择
    cascaderChange(val) {
      for (let i = 4; i > val.length - 1; i--) {
        this.form[`categoryLevel${i}`] = ''
      }
      val.forEach((item, index) => {
        if (index > 0) {
          this.form[`categoryLevel${index}`] = item
        }
      })
    },
    // 获取分类编码
    getTypeCode() {
      const arr = ['0000000']
      const nameArr = ['所有']
      Object.keys(this.form).forEach(item => {
        if (item.includes('categoryLevel') && this.form[item]) {
          arr.push(this.form[item])
        }
        if (item.includes('categoryNameLevel') && this.form[item]) {
          nameArr.push(this.form[item])
        }
      })
      this.typeCode = arr
      this.typeNameCode = nameArr.join(' / ')
    },
    // 搜索过滤
    codeFilter(node, keyword) {
      if (node.text.includes(keyword) || node.value.includes(keyword)) {
        return true
      }
    },
    /** 获取所有的供应商 */
    getSupplierList() {
      getSupplierList({ pageNum: 1, pageSize: 100000, enable: 0 }).then(
        (res) => {
          if (res.code === 200) {
            this.supplierOptions = res.data.records.map((e) => {
              return {
                value: e.id,
                label: e.name
              }
            })
          } else {
            this.$message.error(res.msg)
          }
        }
      )
    },
    /** 获取品牌列表 */
    getBrandList() {
      listBrand({ pageNum: 1, pageSize: 100000 }).then(
        (response) => {
          if (response.code === 200) {
            this.brandOptions = response.data.records.map(e => {
              return {
                value: e.id,
                label: e.chineseName || e.englishName
              }
            })
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    /** 获取计量单位列表 */
    getUnitByCondition() {
      getUnitByCondition({ pageNum: 1, pageSize: 100000 }).then(
        (response) => {
          if (response.code === 200) {
            this.unitOptions = response.data.records.map(e => {
              return {
                value: e.id,
                label: e.base
              }
            })
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    getSupplierLabel(data) {
      this.supplierOptions.forEach(item => {
        if (data === item.value) {
          this.form.supplierId = item.value
          this.form.supplier = item.label
        }
      })
    },
    getBrandLabel(data) {
      this.brandOptions.forEach(item => {
        if (data === item.value) {
          this.form.brandId = item.value
          this.form.brand = item.label
        }
      })
    },
    getUnitLabel(data) {
      this.unitOptions.forEach(item => {
        if (data === item.value) {
          this.form.unitId = item.value
          this.form.unit = item.label
        }
      })
    }
  }
}
</script>
<style lang="scss">
.depreciationType{
  color: black !important;
}

.base_info_desc {
  .addInputItem.el-input.is-disabled .el-input__inner {
    background-color: #fff !important;
  }
}
</style>
<style lang="scss" scoped>
// .base_info_desc :nth-child(3){
//    border: 1px solid red;
//    color: #333333 ;
// }

.el-descriptions-item__cell.el-descriptions-item__label.is-bordered-label {
  color: #333333;
  font-weight: 600;
}
.base_info_desc {
  ::v-deep .el-input__inner {
    border: none;
    border-radius: 0;
    height: 48px;
  }
  ::v-deep .el-descriptions--medium.is-bordered .el-descriptions-item__cell {
    padding: 0px;
  }
  ::v-deep
  .el-descriptions-item__cell.el-descriptions-item__label.is-bordered-label {
    padding-left: 16px;
  }
  .warnTip {
    line-height: 48px;
    color: #ef5826;
  }
  ::v-deep .el-input.is-disabled .el-input__inner,
  ::v-deep .el-range-editor.is-disabled,
  ::v-deep .el-range-editor.is-disabled input,
  .spandisabled {
    cursor: default;
    background: #fff;
    color: #606266;
  }
  .categoryName{
    color:#606266;
    padding-left: 16px;
  }
}
.numInputClass,
.barcodeInputClass,
.manufacturerInputClass,
.standardInputClass,
.typeSelectClass,
.originPlaceInputClass,
.priceInputClass {
  ::v-deep .el-input__inner {
    border: 1px solid #ef5826;
  }
}
.redDot {
  color: #ef5826;
}
.date_box {
  width: 100%;
  position: relative;
  // width: fit-content;
  .data_icon {
    position: absolute;
    top: 50%;
    right: 17px;
    z-index: 9;
    color: #c0c4cc;
    font-size: 14px;
    transform: translateY(-50%);
  }
  ::v-deep .el-input__icon.el-range__icon.el-icon-date {
    display: none;
  }
  ::v-deep .el-range-separator {
    line-height: 40px;
  }
  ::v-deep .el-date-editor.el-input {
    width: 100%;
  }
  ::v-deep .el-input__suffix {
    display: none;
  }
}
.my-select-con{
  .iconfont {
    display: inline-block;
    cursor: pointer;
    width: 22px;
    height: 22px;
    position: absolute;
    right: 6px;
    bottom: 0;
  }
  .icon-bumenxuanze {
    display: inline-block;
    cursor: pointer;
    width: 22px;
    height: 22px;
    position: absolute;
    right: 6px;
    bottom: 0;
    top: 14px;
    background: url("../../../../assets/images/bumen@2x.png") no-repeat;
    background-position: center;
    background-size: 100% 100%;
  }
  .icon-bumenxuanze::before{
    content: '';
  }
  ::v-deep .el-select {
    .el-input__suffix-inner {
      display: none;
    }
  }
}
::v-deep  .el-popper{
  width: 340px !important;
}
</style>
