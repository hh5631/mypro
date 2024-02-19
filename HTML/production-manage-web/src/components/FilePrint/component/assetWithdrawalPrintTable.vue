<template>
  <!-- 资产退库打印表 -->
  <div>
    <CommonTitle :title-type="titleType" />
    <div class="table-con">
      <table class="table1">
        <tbody>
          <tr>
            <td align="center">退库单号</td>
            <td align="center" class="tr1-content">{{ infoItem.orderNo }}</td>
            <td align="center">申请时间</td>
            <td align="center" class="tr1-content" />
            <td align="center">创建时间</td>
            <td align="center" class="tr1-content">{{ infoItem.createTime }}</td>
          </tr>
          <tr>
            <td align="center">退库人</td>
            <td align="center" class="tr2-content" colspan="2">{{ infoItem.userName }}</td>
            <td align="center">接收部门</td>
            <td align="center" colspan="2">{{ (infoItem.backDept) }}</td>
          </tr>
          <tr>
            <td align="center">退库说明</td>
            <td align="center" class="tr3-content" colspan="5">{{ infoItem.note }}</td>
          </tr>
        </tbody>
      </table>
      <div class="assetDetail">退库明细</div>
      <!-- 资产退库明细表 -->
      <table class="table2">
        <thead>
          <tr>
            <td align="center">序号</td>
            <td align="center">资产编码</td>
            <td align="center">资产名称</td>
            <td align="center">品牌</td>
            <td align="center">规格型号</td>
            <td align="center">计量单位</td>
          </tr>
        </thead>
        <tbody v-if="infoItem.wareBackDetails && infoItem.wareBackDetails.length">
          <tr v-for="(item,index) in infoItem.wareBackDetails" :key="item.id">
            <td align="center">{{ index + 1 }}</td>
            <td align="center">{{ item.internalCode }}</td>
            <td align="center">{{ item.name }}</td>
            <td align="center">{{ item.brandName }}</td>
            <td align="center">{{ item.standard }}</td>
            <td align="center">{{ item.unit }}</td>
          </tr>
        </tbody>
      </table>
    </div>
    <div class="footer">
      <span>
        <span>退库人：(签名)</span>
        <span />
      </span>
      <span class="name2">
        <span>资产管理专员：(签名)</span>
        <span />
      </span>
    </div>
  </div>
</template>

<script>
import CommonTitle from './commonTitle.vue'
import { dayjsFormat } from '@/utils/index.js'
export default {
  name: 'AssetWithdrawalPrintTable',
  components: { CommonTitle },
  props: {
    titleType: {
      type: String,
      default: ''
    },
    infoItem: {
      type: Object,
      default: () => {}
    }
  },
  methods: {
    dayjsFormat
  }
}
</script>

<style lang="scss" scoped>
.table1 {
  border-bottom: 1px solid #999;
  .tr1-content {
    width: 140px;
    height: 40px;
  }
  .tr2-content {
    height: 50px;
  }
  .tr3-content {
    height: 60px;
  }
}
.assetDetail {
  margin-top: 20px;
  margin-bottom: 5px;
}
.footer {
  display: flex;
  margin-top: 50px;
  justify-content: space-around;
  .name2 {
    margin-right: 80px;
  }
}
</style>
