<template>
  <div>
    <div class="title">{{ titleName }}</div>
    <!-- 打印表头部信息 -->
    <div class="top">
      <div class="top-left">
        <div>申请部门：</div>
      </div>
      <div class="time">
        <span style="min-width:30px;display:inline-block;" />
        <span>年</span>
        <span style="min-width:30px;display:inline-block;" />
        <span>月</span>
        <span style="min-width:30px;display:inline-block;" />
        <span>日</span>
      </div>
    </div>
  </div>
</template>

<script>
const titleNameType = {
  'warehousing': '浙江大学嘉兴研究院入库确认表',
  'warehousingInfo': '浙江大学嘉兴研究院出库确认表',
  'outboundOrder': '浙江大学嘉兴研究院领用出库表',
  'assetDisposalOrder': '浙江大学嘉兴研究院固定资产处置申请单',
  'purchaseApply': '浙江大学嘉兴研究院采购申请表',
  'assetWithdrawal': '浙江大学嘉兴研究院资产退库单'
}
export default {
  name: 'CommonTitle',
  props: {
    titleType: {
      type: String,
      default: ''
    }
  },
  data() {
    return {
      titleName: ''
    }
  },
  watch: {
    titleType: {
      immediate: true,
      handler() {
        this.titleName = titleNameType[this.titleType]
      }
    }
  }
}
</script>

<style lang="scss" scoped>
  .title {
    padding-top: 17px;
    text-align: center;
    font-weight: bold;
    font-size: 22px;
  }
  .top{
    display: flex;
    justify-content: space-between;
    .top-left{
      text-align: left;
      padding: 10px 0 10px ;
    }
    .time {
      text-align: right;
      padding: 10px 0px 10px 0;
    }
  }
</style>
