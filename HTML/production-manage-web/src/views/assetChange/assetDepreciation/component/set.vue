<!-- eslint-disable vue/no-mutating-props -->
<template>
  <div>
    <!-- 设置折旧 -->

    <el-form
      ref="form"
      :validate-on-rule-change="false"
      :model="depreciation"
      :rules="rules"
      label-width="80px"
      label-position="top"
    >
      <el-row class="form-row" :gutter="12">
        <el-col :span="12">
          <el-form-item label="资产编码">
            <el-input
              v-model="depreciation.internalCode"
              disabled
              style="width: 250px;"
            />
          </el-form-item>
          <el-form-item label="购买日期" prop="purchaseDate">
            <el-date-picker v-model="depreciation.purchaseDate" type="date" style="width: 250px;" size="small" disabled value-format="yyyy-MM-dd" :placeholder="addTimeNow" class="input-width" />
          </el-form-item>
          <el-form-item label="规定年限" prop="serviceLife">
            <el-input
              v-model.number="depreciation.serviceLife"
              style="width: 250px;"
              onkeyup="value=value.replace(/^(0+)|[^\d]+/g,'')"
              maxlength="4"
            />
          </el-form-item>
          <el-form-item label="折旧方法" prop="depreciationType">
            <el-select
              v-model="depreciation.depreciationType"
              style="width: 250px;"
              placeholder="请选择"
              clearable
              size="small"
              class="input-width"
            >
              <el-option
                v-for="dict in dict.type.sys_depreciation_type"
                :key="dict.value"
                :label="dict.label"
                :value="dict.value"
              />
            </el-select>
          </el-form-item>
        </el-col>
        <el-col :span="12">
          <el-form-item label="资产名称" prop="assetName">
            <el-input
              v-model="depreciation.assetName"
              disabled
              style="width: 250px;"
            />
          </el-form-item>
          <el-form-item label="购买单价(元)" prop="price">
            <el-input
              v-model="depreciation.price"
              disabled
              style="width: 250px;"
            />
          </el-form-item>
          <el-form-item label="残值率" prop="residualValueRate">
            <el-input
              v-model.number="depreciation.residualValueRate"
              style="width: 250px;"
              onkeyup="value=value.replace(/[^\d]+/g,'')"
              maxlength="2"
            >
              <template slot="append">%</template>
            </el-input>
          </el-form-item>
          <el-form-item label="折旧率">
            <el-input
              v-model.number="depreciation.depreciationRate"
              onkeyup="value=value.replace(/[^\d]+/g,'')"
              style="width: 250px;"
              maxlength="2"
            >
              <template slot="append">%</template>
            </el-input>
          </el-form-item>
        </el-col>
      </el-row>
    </el-form>

  </div>
</template>
<script>
export default {
  name: 'AddDialog',
  dicts: ['sys_depreciation_type'],
  props: {
    data: {
      type: Object,
      default: () => {
        return {}
      }
    },
    warehouseData: {
      type: Array,
      default: () => []
    },
    selectRow: {
      type: Object,
      default: () => {}
    },
    isEdit: {
      type: Boolean,
      default: false
    },

    addTimeNow: {
      type: String,
      default: ''
    }
  },
  data() {
    return {
      rules: {
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
      // selectLeaderDig: false,
      tempPeopleObj: {
        realName: '',
        userId: '',
        deptId: '',
        deptName: ''
      },
      // 选择明细弹框显示隐藏
      openDetail: false,
      selectedData: [],
      materialKeyWords: '',
      loading: false,
      detailData: [],

      depreciation: {
        id: null,
        internalCode: null, // 编号
        assetName: null, // 名称
        purchaseDate: null, // 日期
        price: null, // 单价
        serviceLife: null,
        residualValueRate: null,
        depreciationType: null,
        depreciationRate: null
      }
    }
  },
  computed: {
    form() {
      return this.data
    },
    tableDatas() {
      return this.data.wareBackDetails
    }

  },
  watch: {
    selectRow: {
      immediate: true,
      handler(val) {
        console.log('val..222......', val)
        if (val) {
          this.depreciation.residualValueRate = val.residualValueRate
          this.depreciation.serviceLife = val.serviceLife
          this.depreciation.depreciationType = val.depreciationType
          this.depreciation.depreciationRate = val.depreciationRate
          this.depreciation.internalCode = val.internalCode
          this.depreciation.purchaseDate = val.purchaseDate
          this.depreciation.assetName = val.assetName
          this.depreciation.price = val.price
          this.depreciation.id = val.id
        }
      }
    }
  },
  methods: {

  }
}
</script>

  <style lang="scss" scoped>
  // 基础信息
  .form-row {
    padding: 0 20px;
  }
  // 添加明细
  .addOrRemove {
    display: flex;
    justify-content: flex-end;
    margin-bottom: 15px;
    margin-right: 10px;
  }
  </style>

