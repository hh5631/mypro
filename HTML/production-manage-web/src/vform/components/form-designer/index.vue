<!--
/**
 * author: vformAdmin
 * email: vdpadmin@163.com
 * website: http://www.vform666.com
 * date: 2021.08.18
 * remark: 如果要分发VForm源码，需在本文件顶部保留此文件头信息！！
 */
-->

<template>
  <div class="form-design">
    <el-container class="full-height">
      <el-container>
        <el-aside class="side-panel-left">
          <widget-panel :designer="designer" />
        </el-aside>

        <el-container class="center-layout-container">
          <el-header class="toolbar-header">
            <toolbar-panel :designer="designer" ref="toolbarRef">
              <template #toolButton
                ><slot name="customToolButtons"></slot
              ></template>
            </toolbar-panel>
          </el-header>
          <el-main class="form-widget-main">
             <v-form-widget
                :designer="designer"
                :form-config="designer.formConfig"
              >
              </v-form-widget>
          </el-main>
        </el-container>

        <el-aside class="side-panel-right">
          <setting-panel
            :designer="designer"
            :selected-widget="designer.selectedWidget"
            :form-config="designer.formConfig"
          />
        </el-aside>
      </el-container>
    </el-container>
  </div>
</template>

<script>
import WidgetPanel from './widget-panel/index'
import ToolbarPanel from './toolbar-panel/index'
import SettingPanel from './setting-panel/index'
import VFormWidget from './form-widget/index'
import { createDesigner } from "@/vform/components/form-designer/designer";
import { addWindowResizeHandler, deepClone, getQueryParam } from "@/vform/utils/util";
import { MOCK_CASE_URL, VARIANT_FORM_VERSION } from "@/vform/utils/config";
import i18n, { changeLocale } from "@/vform/utils/i18n";

export default {
  name: "VFormDesigner",
  componentName: "VFormDesigner",
  mixins: [i18n],
  components: {
    WidgetPanel,
    ToolbarPanel,
    SettingPanel,
    VFormWidget,
  },
  props: {
    /* 后端字段列表API */
    fieldListApi: {
      type: Object,
      default: null,
    },

    /* 禁止显示的组件名称数组 */
    bannedWidgets: {
      type: Array,
      default: () => []
    },

    designerConfig: {
      type: Object,
      default: () => {
        return {
          languageMenu: true,  //是否显示语言切换菜单
          externalLink: true,  //是否显示GitHub、文档等外部链接
          formTemplates: true,  //是否显示表单模板
          eventCollapse: true,  //是否显示组件事件属性折叠面板
          clearDesignerButton: true,  //是否显示清空设计器按钮
          previewFormButton: true,  //是否显示预览表单按钮
          importJsonButton: true,  //是否显示导入JSON按钮
          exportJsonButton: true,  //是否显示导出JSON器按钮
          exportCodeButton: true,  //是否显示导出代码按钮
          generateSFCButton: true,  //是否显示生成SFC按钮
        }
      }
    },

  },
  data() {
    return {
      vFormVersion: VARIANT_FORM_VERSION,
      curLangName: '',

      vsCodeFlag: false,
      caseName: '',

      docUrl: 'http://www.vform666.com/document.html',
      gitUrl: 'https://github.com/vform666/variant-form',
      chatUrl: 'http://www.vform666.com/chat-group.html',
      subScribeUrl: 'http://www.vform666.com/subscribe.html',

      scrollerHeight: 0,

      designer: createDesigner(this),

      fieldList: []
    }
  },
  provide() {
    return {
      serverFieldList: this.fieldList,
      getDesignerConfig: () => this.designerConfig,
      getBannedWidgets: () => this.bannedWidgets,
    }
  },
  inject: ['saveForm'],
  created() {
    this.vsCodeFlag = getQueryParam('vscode') == 1
    this.caseName = getQueryParam('case')
  },
  mounted() {
    this.initLocale()

    this.scrollerHeight = window.innerHeight - 86 - 36 + 'px'
    addWindowResizeHandler(() => {
      this.$nextTick(() => {
        this.scrollerHeight = window.innerHeight - 86 - 36 + 'px'
      })
    })
  },
  methods: {
    editVForm(data) {
      if (!data) {
        this.designer.clearDesigner()
      }
      else if (typeof data === 'string') {
        this.designer.loadFormJson(JSON.parse(data))
      } else if (data.constructor === Object) {
        this.designer.loadFormJson(data)
      }
    },
    /**
     * 区别于原saveVForm
     * 此方法不再依赖saveForm的注入
     * 直接返回数据,由上一层去执行
     */
    saveVFormNew() {
      let widgetList = this.$deepClone(this.designer.widgetList)
      let formConfig = this.$deepClone(this.designer.formConfig)
      return JSON.stringify({ widgetList, formConfig }, null, '  ')
    },
    saveVForm() {
      let widgetList = this.$deepClone(this.designer.widgetList)
      let formConfig = this.$deepClone(this.designer.formConfig)
      let jsonContent = JSON.stringify({ widgetList, formConfig }, null, '  ')
      this.saveForm(jsonContent)
    },
    showLink(configName) {
      if (this.designerConfig[configName] === undefined) {
        return true
      }

      return !!this.designerConfig[configName]
    },

    openHome() {
      if (!!this.vsCodeFlag) {
        const msgObj = {
          cmd: 'openUrl',
          data: {
            url: 'http://www.vform666.com/'
          }
        }
        window.parent.postMessage(msgObj, '*')
      }
    },

    openUrl(event, url) {
      if (!!this.vsCodeFlag) {
        const msgObj = {
          cmd: 'openUrl',
          data: {
            url
          }
        }
        window.parent.postMessage(msgObj, '*')
      } else {
        let aDom = event.currentTarget
        aDom.href = url
        //window.open(url, '_blank') //直接打开新窗口，会被浏览器拦截
      }
    },

    loadCase() {
      if (!this.caseName) {
        return
      }

      axios.get(MOCK_CASE_URL + this.caseName + '.txt').then(res => {
        if (!!res.data.code) {
          this.$message.error(this.i18nt('designer.hint.sampleLoadedFail'))
          return
        }

        this.setFormJson(res.data)
        this.$message.success(this.i18nt('designer.hint.sampleLoadedSuccess'))
      }).catch(error => {
        this.$message.error(this.i18nt('designer.hint.sampleLoadedFail') + ':' + error)
      })
    },

    initLocale() {
      let curLocale = localStorage.getItem('v_form_locale')
      if (!!this.vsCodeFlag) {
        curLocale = curLocale || 'en-US'
      } else {
        curLocale = curLocale || 'zh-CN'
      }
      this.curLangName = this.i18nt('application.' + curLocale)
      this.changeLanguage(curLocale)
    },

    loadFieldListFromServer() {
      if (!this.fieldListApi) {
        return
      }

      axios.get(this.fieldListApi.URL).then(res => {
        let labelKey = this.fieldListApi.labelKey || 'label'
        let nameKey = this.fieldListApi.nameKey || 'name'

        res.data.forEach(fieldItem => {
          this.fieldList.push({
            label: fieldItem[labelKey],
            name: fieldItem[nameKey]
          })
        })
      }).catch(error => {
        this.$message.error(error)
      })
    },

    handleLanguageChanged(command) {
      this.changeLanguage(command)
      this.curLangName = this.i18nt('application.' + command)
    },

    changeLanguage(langName) {
      changeLocale(langName)
    },

    setFormJson(formJson) {
      let modifiedFlag = false
      if (!!formJson) {
        if (typeof formJson === 'string') {
          modifiedFlag = this.designer.loadFormJson(JSON.parse(formJson))
        } else if (formJson.constructor === Object) {
          modifiedFlag = this.designer.loadFormJson(formJson)
        }

        if (modifiedFlag) {
          this.designer.emitHistoryChange()
        }
      }
    },

    getFormJson() {
      return {
        widgetList: deepClone(this.designer.widgetList),
        formConfig: deepClone(this.designer.formConfig)
      }
    },

    clearDesigner() {
      this.$refs.toolbarRef.clearFormWidget()
    },


    /**
     * 刷新表单设计器
     */
    refreshDesigner() {
      //this.designer.loadFormJson( this.getFormJson() )  //只有第一次调用生效？？

      let fJson = this.getFormJson()
      this.designer.clearDesigner(true)  //不触发历史记录变更
      this.designer.loadFormJson(fJson)
    },

    /**
     * 预览表单
     */
    previewForm() {
      
      this.$refs.toolbarRef.previewForm()
    },

    /**
     * 导入表单JSON
     */
    importJson() {
      this.$refs.toolbarRef.importJson()
    },

    /**
     * 导出表单JSON
     */
    exportJson() {
      this.$refs.toolbarRef.exportJson()
    },

    /**
     * 导出Vue/HTML代码
     */
    exportCode() {
      this.$refs.toolbarRef.exportCode()
    },

    /**
     * 生成SFC代码
     */
    generateSFC() {
      this.$refs.toolbarRef.generateSFC()
    },

    //TODO: 增加更多方法！！

  }
}
</script>

<style lang="scss" scoped>
.form-design {
  height: 100%;
  .el-container.full-height {
    height: 100%;
    overflow-y: hidden;
  }
    .side-panel-left {
      margin-right: 16px;
      background-color: #fff;
      padding: 0;
      width: 260px;
      margin-bottom: 0;
      overflow: hidden;
    }
    .side-panel-right {
      margin-left: 16px;
      background-color: #fff;
      padding: 0;
      overflow: hidden;
      width: 300px;
      margin-bottom: 0;
    }
    .center-layout-container {
      background-color: #fff;
      height: 100%;
      display: flex;
      flex-direction: column;
    }
    // ::v-deep .no-widget-hint {
    //   background-color: #fff;
    // }
    .form-widget-main {
      position: relative;
      padding: 0;
    }
}

// .el-main.form-widget-main {
//   padding: 0;

//   position: relative;
//   overflow-x: hidden;
// }

.el-container.center-layout-container {
  min-width: 380px;
}

.el-header.main-header {
  border-bottom: 2px dotted #ebeef5;
  height: 48px !important;
  line-height: 48px !important;
  min-width: 800px;
}

div.main-title {
  font-size: 18px;
  color: #242424;
  display: flex;
  align-items: center;
  justify-items: center;

  img {
    cursor: pointer;
    width: 36px;
    height: 36px;
  }

  span.bold {
    font-size: 20px;
    font-weight: bold;
    margin: 0 6px 0 6px;
  }

  span.version-span {
    font-size: 14px;
    color: #101f1c;
    margin-left: 6px;
  }
}

.float-left {
  float: left;
}

.float-right {
  float: right;
}

.el-dropdown-link {
  margin-right: 12px;
  cursor: pointer;
}

div.external-link a {
  font-size: 13px;
  text-decoration: none;
  margin-right: 10px;
  color: #606266;
}

.el-header.toolbar-header {
  font-size: 14px;
  border-bottom: 2px solid #dfe4ed;
  height: 40px !important;
  //line-height: 42px !important;
}


</style>
