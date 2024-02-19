<template>
  <el-form-item label="onUploadSuccess" label-width="150px">
    <el-button
      type="info"
      icon="el-icon-edit"
      plain
      size="small"
      round
      @click="editEventHandler('onUploadSuccess', eventParams)"
    >
      {{ i18nt('designer.setting.addEventHandler') }}</el-button
    >
  </el-form-item>
</template>

<script>
import i18n from "@/vform/utils/i18n"
import eventMixin from "@/vform/components/form-designer/setting-panel/property-editor/event-handler/eventMixin"

export default {
  name: "onUploadSuccess-editor",
  mixins: [i18n, eventMixin],
  props: {
    designer: Object,
    selectedWidget: Object,
    optionModel: Object,
  },
  data () {
    return {
      eventParams: ['result', 'file', 'fileList'],
    }
  }
}
</script>

<style scoped>
</style>
