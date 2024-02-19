<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-08-22 15:52:14
-->
<template>
  <div class="selectLeader">
    <el-form ref="tableForm" :model="tableFormData" :rules="tableFormRules">
      <el-table
        ref="asstesData"
        style="width: 100%"
        border
        :data="tableFormData.list"
        highlight-current-row
        max-height="600"
        :header-cell-class-name="cellClass"
      >
        <el-table-column
          key="materialName"
          label="物料名称"
          align="center"
          prop="materialName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="brandName"
          label="品牌"
          align="center"
          prop="brandName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="materialStandard"
          label="规格型号"
          align="center"
          prop="materialStandard"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="unitName"
          label="计量单位"
          align="center"
          prop="unitName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="num"
          label="入库数量"
          align="center"
          prop="num"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="thisQty"
          label="生成资产卡片数量"
          align="center"
          width="150"
          prop="thisQty"
        >
          <template slot-scope="scope">
            <el-form-item
              :prop="'list.' + scope.$index + '.thisQty'"
              :rules="tableFormRules.thisQty"
              class="qty-input"
            >
              <el-input
                v-model.trim="scope.row.thisQty"
                placeholder="请输入"
                class="noborder"
                @input="numberFilter.filterQty(scope.row,'thisQty',scope.row.num)"
                @blur="checkInput(scope.row)"
                @change="handleChange(scope.row)"
              />
            </el-form-item>
          </template>
        </el-table-column>
      </el-table>
    </el-form>
  </div>
</template>

<script>
export default {
  props: {
    asstesData: {
      type: Array,
      default: () => []
    }
  },
  data() {
    const validateQty = (rule, value, callback) => {
      if (isNaN(Number(value))) {
        callback('请输入数字值')
        return
      } else {
        callback()
      }
    }
    return {
      temObj: {},
      tableFormRules: {
        thisQty: [
          {
            required: true,
            message: '请输入数量',
            trigger: ['blur', 'input']
          },
          {
            trigger: ['blur', 'input'],
            validator: validateQty
          },
          {
            trigger: ['blur', 'input'],
            validator: (rule, value, callback) => {
              if (Number(value) > this.temObj.num) {
                callback('生成资产卡片数量不能大于已入库数')
                return
              } else {
                callback()
              }
            }
          }
        ]
      },
      newArray: []
    }
  },
  computed: {
    tableFormData () {
      return {
        list: this.newArray
      }
    }
  },
  created() {
    this.newArray = this.asstesData.filter(function (item) {
      return item.thisQty !== 0
    })
  },
  methods: {
    getRowKeys(row) {
      return row.id + Math.random()
    },
    checkInput(row) {
      if (row.thisQty > row.num) {
        this.$message.error('生成资产卡片数量不得超过入库数量')
        row.thisQty = ''
      }
    },
    /** 必填星号 */
    cellClass(row) {
      if (row.columnIndex === 5 || row.columnIndex === 6) {
        return 'must'
      }
    },
    /** 临时存储 */
    handleChange(row, index) {
      this.temObj = row
    }
  }
}
</script>

<style lang="scss" scoped>
.selectLeader {
  width: 100%;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
}
.search {
  display: flex;
  align-items: center;
  .label {
    margin: 0 8px 0 24px;
  }
}
.detail-title {
  font-size: 16px;
  font-weight: 500;
  color: #6a7697;
  margin-bottom: 15px;
  &.sub-title{
    margin-top: 15px;
  }
}
.noborder {
  ::v-deep .el-input__inner {
    border: none;
  }
}
::v-deep .el-table th.must>.cell:after {
	content: ' *';
	color: #ff1818;
}
</style>
