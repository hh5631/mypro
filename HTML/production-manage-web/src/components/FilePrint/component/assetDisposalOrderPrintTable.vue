<template>
  <div>
    <CommonTitle :title-type="titleType" />
    <!-- 具体打印表格内容 -->
    <div class="table-con">
      <!-- 采购申请表 -->
      <table class="table3" style="border-collapse:collapse;" cellspacing="0" cellpadding="0">
        <!-- 采购表表头 -->
        <thead>
          <tr>
            <td rowspan="2" style="width: 70px;" align="center">资产编号</td>
            <td rowspan="2" style="width: 70px;" align="center">资产名称</td>
            <td rowspan="2" style="width: 70px;" align="center">规格型号</td>
            <td rowspan="2" style="width: 70px;" align="center">购置日期</td>
            <td colspan="6" align="center">申请处置方式</td>
          </tr>
          <tr>
            <td align="center">报废</td>
            <td align="center">出售</td>
            <td align="center">转让</td>
            <td align="center">置换</td>
            <td align="center">对外捐赠</td>
            <td align="center">其他</td>
          </tr>
        </thead>
        <!-- 采购表表格内容 -->
        <tbody>
          <div v-if="infoItem.handleDetailList && infoItem.handleDetailList.length>0">
            <tr v-for="(item) in infoItem.handleDetailList" :key="item.id" style="height:40px;">
              <td align="center">{{ item.code }}</td>
              <td align="center">{{ item.name }}</td>
              <td align="center">{{ item.unit }}</td>
              <td align="center">{{ dayjsFormat(item.purchaseDate,'YYYY-MM-DD') }}</td>
              <td align="center">{{ infoItem.type === '1'? '√': '' }}</td>
              <td align="center">{{ infoItem.type === '0'? '√': '' }}</td>
              <td align="center">{{ infoItem.type === '3'? '√': '' }}</td>
              <td align="center">{{ infoItem.type === '5'? '√': '' }}</td>
              <td align="center">{{ infoItem.type === '2'? '√': '' }}</td>
              <td align="center">{{ infoItem.type === '100'? '√': '' }}</td>
            </tr>
          </div>
          <div v-else>
            <tr v-for="(item,index) in 3" :key="index" style="height:40px;">
              <td />
              <td />
              <td />
              <td />
              <td />
              <td />
              <td />
              <td />
              <td />
              <td />
            </tr>
          </div>
          <!-- 处置原因（详细） -->
          <tr>
            <td style="height:60px;" align="center">处置原因 <br>（详细）</td>
            <td colspan="9" />
          </tr>
          <!-- 处置方案（详细） -->
          <tr>
            <td style="height:60px;" align="center">处置方案<br>（详细）</td>
            <td colspan="9" />
          </tr>
          <!-- 申请资产处置部门、处置人、管理员 -->
          <tr>
            <td colspan="10" class="t3-td">
              <span class="t3-top1">申请资产处置部门：</span>
              <span class="t3-top2">申请资产处置人：</span>
              <span class="t3-top3">归口部门资产管理员（签字）：</span>
            </td>
          </tr>
          <!-- 归口部门财务室签字 -->
          <tr>
            <td colspan="10" class="t3-td">归口部门财务室（签字）：</td>
          </tr>
          <!-- 申请资产处置归口部门负责人签字 -->
          <tr>
            <td colspan="10" class="t3-td">申请资产处置归口部门负责人（签字）：</td>
          </tr>
          <!-- 研究院办公室签字 -->
          <tr>
            <td colspan="10" class="t3-td1">研究院办公室（签字）：</td>
          </tr>
          <!-- 分管院领导签字 -->
          <tr>
            <td colspan="10" class="t3-td1">分管院领导（签字）：</td>
          </tr>
          <!-- 院领导签字 -->
          <tr>
            <td colspan="10" class="t3-td1">院领导（签字）：</td>
          </tr>
        </tbody>
      </table>
    </div>
    <div>
      <div style="padding: 10px 15px;">本表一式两份，申请处置（归口）部门、研究院办公室各一份。</div>
    </div>
  </div>
</template>

<script>
import CommonTitle from './commonTitle.vue'
import { dayjsFormat } from '@/utils/index.js'
export default {
  name: 'AssetDisposalOrderPrintTable',
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
.table3 {
    border-top: 1px solid #999;
  .t3-td{
    width:100%;height:30px;
  }
  .t3-td1{
    width:100%;height:50px;
  }
  .t3-top1{
    padding-left: 5px;
  }
  .t3-top2{
    padding-left: 130px;
  }
  .t3-top3{
    padding-left: 100px;
  }
}
</style>
