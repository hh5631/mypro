<template>
  <el-form-item label="onMounted" label-width="150px">
    <el-button
      type="info"
      icon="el-icon-edit"
      plain
      round
      size="small"
      @click="editEventHandler('onMounted', eventParams)"
    >
      {{ i18nt('designer.setting.addEventHandler') }}</el-button
    >
  </el-form-item>
</template>

<script>
import i18n from "@/vform/utils/i18n"
import eventMixin from "@/vform/components/form-designer/setting-panel/property-editor/event-handler/eventMixin"

export default {
  name: "onMounted-editor",
  mixins: [i18n, eventMixin],
  props: {
    designer: Object,
    selectedWidget: Object,
    optionModel: Object,
  },
  data () {
    return {
      eventParams: [],
    }
  }
}
</script>

<style scoped>
</style>
