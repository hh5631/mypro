<template>
  <div v-if="visible" style="display:none;">
    <!-- 需要打印的页面内容    -->
    <div id="printStatic">
      <div v-for="infoItem in infoList" :id="`printStatic${infoItem.id}`" :key="infoItem.id" class="my-content" style="page-break-after:always;" :style="`min-height:${(Math.ceil((infoItem.length-33)/40) + 1) * 270}mm`">
        <component :is="printTableName" :info-item="infoItem" :title-type="type" />
      </div>
    </div>
    <!-- 打印取消按钮 -->
    <div slot="footer">
      <el-button size="mini" @click="close">取消</el-button>
      <el-button
        :id="`printStaticButton`"
        v-print="`#printStatic`"
        size="mini"
        type="primary"
      >打印
      </el-button>
    </div>
  </div>
</template>
<script>
import print from 'vue-print-nb'
import comps from './component/index'
const componentType = {
  // 传入类型 : 组件name属性名
  'warehousing': 'WarehousingPrintTable',
  'warehousingInfo': 'WarehousingInfoPrintTable',
  'outboundOrder': 'OutboundOrderPrintTable',
  'assetDisposalOrder': 'AssetDisposalOrderPrintTable',
  'purchaseApply': 'PurchaseApplyPrintTable',
  'assetWithdrawal': 'AssetWithdrawalPrintTable'
}
export default {
  name: 'FilePrint',
  components: {
    ...comps
  },
  directives: {
    print
  },
  props: {
    type: {
      type: String,
      default: 'warehousing'
    }
  },
  data () {
    return {
      visible: false,
      info: {},
      infoList: [],
      printTableName: ''
    }
  },
  watch: {
    type: {
      immediate: true,
      handler() {
        this.printTableName = componentType[this.type]
      }
    }
  },
  methods: {
    // 渲染dom，展示打印预览
    show (infoList) {
      // console.log('infoList', infoList)
      this.infoList = infoList
      this.visible = true
      // 入库单
      if (this.type === 'warehousing') {
        for (const infoItem of this.infoList) {
          infoItem.totalPrice = 0
          for (const itemx of infoItem.detailVOS) {
            infoItem.totalPrice += itemx.price * itemx.qty
          }
        }
      }
      // 出库单
      if (this.type === 'outboundOrder') {
        for (const infoItem of this.infoList) {
          infoItem.totalPrice = 0
          for (const itemx of infoItem) {
            infoItem.totalPrice += itemx.price * 1
          }
        }
      }
      // 采购申请确认单
      if (this.type === 'purchaseApply') {
        for (const infoItem of this.infoList) {
          infoItem.totalPrice = infoItem.totalAmount
        }
      }
      // 点击打印按钮
      this.$nextTick(() => {
        document.querySelector(`button[id="printStaticButton"]`).click()
        this.close()
      })
    },
    // 取消打印，移除dom
    close () { this.visible = false }
  }
}

</script>
<style lang="scss" scoped>
@media print {
  @page {
    size: a4;
    // margin: 5mm 12mm 5mm 12mm;
    // 去除页眉页脚
    margin: 0;
  }
  body {
    height: auto;
  }
}
#printStatic {
  padding: 5mm 12mm 0 12mm;
}
// 打印表格样式
.my-content {
  color: #333;
  font-size: 12px;
  //padding: 20px;
  // overflow-y: scroll;
  min-height: 270mm; // 297mm或210mm会导致多打印一页白纸
  //min-height: 799px;
  &:last-child {
    min-height: 0mm;
  }
}

</style>
