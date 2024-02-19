<template>
  <collapse title="基本信息">
    <div class="produce-collapse">
      <el-form ref="mainForm" :model="baseInfo" :rules="rules">
        <el-row style="border:1px solid #DDDDDD">
          <el-col class="colTitle" style="border-bottom:1px solid #DDDDDD" :span="3">标题<span v-if="isDetail!=='1'" class="redDot">*</span></el-col>
          <el-col :span="21">
            <el-form-item prop="title">
              <el-input
                v-model="baseInfo.title"
                :disabled="isDetail==='1'"
                placeholder="请输入"
                maxlength="15"
                show-word-limit
                class="firstInput inputNoBorder"
              >
              <!-- <span v-if="isTitleWarn" slot="suffix" class="warnTip">请填写此项</span> -->
              </el-input>
            </el-form-item>
          </el-col>
          <el-col class="colTitle" :span="3">计划单编号</el-col>
          <el-col :span="9"><el-input v-model="baseInfo.orderNo" class="inputNoBorder" :disabled="true" placeholder="自动生成" /></el-col>
          <el-col class="colTitle" style="border-left:1px solid #DDDDDD" :span="3">预期完成时间<span v-if="isDetail!=='1'" class="redDot">*</span></el-col>
          <el-col :span="9">
            <div class="date_box">
              <el-form-item prop="expectedCompletionDate">
                <el-date-picker
                  ref="picker"
                  v-model="baseInfo.expectedCompletionDate"
                  size="small"
                  type="date"
                  :disabled="isDetail==='1'"
                  value-format="yyyy-MM-dd"
                  placeholder="请选择"
                  :picker-options="pickerOptions"
                  :clearable="false"
                />
                <i v-if="isDetail!=='1'" class="el-icon-date data_icon" @click="focusMethod" />
              </el-form-item>
            </div>
          </el-col>
        </el-row>
      </el-form>
    </div>
  </collapse>
</template>

<script>
import collapse from './collapse.vue'
export default {
  components: { collapse },
  props: {
    baseInfos: {
      type: Object,
      default: () => {}
    },
    isDetail: {
      type: String,
      default: ''
    },
    isTitleWarn: {
      type: Boolean,
      default: false
    },
    isExpectDateWarn: {
      type: Boolean,
      default: false
    }
  },
  data() {
    return {
      baseInfo: {
        title: '',
        orderNo: '',
        expectedCompletionDate: ''
      },
      pickerOptions: { // 禁用当前日期之前的日期
        disabledDate(time) {
        // Date.now()是javascript中的内置函数，它返回自1970年1月1日00:00:00 UTC以来经过的毫秒数。
          return time.getTime() < Date.now()
        }
      },
      rules: {
        title: [
          { required: true, message: '请填写此项' }
        ],
        expectedCompletionDate: [
          { required: true, message: '请选择时间' }
        ]
      }

    }
  },
  watch: {
    baseInfos(val) {
      this.baseInfo = val
    },
    isTitleWarn() {
      this.$refs.mainForm.validate()
    },
    isExpectDateWarn() {
      this.$refs.mainForm.validate()
    }
    // baseInfo(val, val1) {
    //
    // }
  },
  mounted() {
    //
    // this.getInfo()
  },
  methods: {
    clearValidate() {
      this.$refs.mainForm.resetFields()
    },
    // getInfo() {
    //
    //   this.baseInfo = this.baseInfos
    // }

    focusMethod() {
      this.$refs.picker.focus()
    }
  }
}
</script>

<style lang="scss" scoped>
  .colTitle{
    height:36px;
    line-height:36px;
    padding-left:10px;
    background:#F1F2F5;
    border-right:1px solid #DDDDDD;
  }
  .redDot{
    color: #EF5826;
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
    ::v-deep .el-date-editor .el-input__inner{
      border: none;
    }
  }
  .warnTip{
    color: #EF5826;
    font-size: 14px;
    line-height: 36px;
    border-radius: 0;
  }
::v-deep .inputNoBorder .el-input__inner{
    border: none;
    border-radius: 0;
  }
  ::v-deep .warn .el-input__inner{
    border: 1px solid #EF5826 !important;
  }
::v-deep .firstInput .el-input__inner{
  border-bottom: 1px solid #DDDDDD;
}
</style>
