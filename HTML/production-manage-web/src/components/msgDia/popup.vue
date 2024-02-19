
<template>
  <div class="test" />
</template>

<script>
import { getNumByReadType } from '@/api/assetChange/index'
import eventBus from '@/utils/bus.js'
export default {
  name: 'Test',
  data() {
    return {
      websock: null,
      msgData: {
        content: null,
        receiveTime: null
      }
    }
  },
  created() {
    this.initWebSocket()
  },
  destroyed() {
    this.websock.close() // 离开路由之后断开websocket连接
  },
  methods: {
    initWebSocket() { // 初始化weosocket
      const wsuri = 'ws://172.16.8.123:38080/message/websocket/admin'
      this.websock = new WebSocket(wsuri)
      // console.log(this.websock)
      this.websock.onmessage = this.websocketonmessage
      this.websock.onopen = this.websocketonopen
      this.websock.onerror = this.websocketonerror
      this.websock.onclose = this.websocketclose
    },
    websocketonopen() { // 连接建立之后执行send方法发送数据
      const actions = { 'test': '12345' }
      this.websocketsend(JSON.stringify(actions))
    },
    websocketonerror() { // 连接建立失败重连
      this.initWebSocket()
    },
    websocketonmessage(e) { // 数据接收
      const redata = JSON.parse(e.data)
      // console.log(' this.websock', redata)
      //
      if (redata) {
        this.msgData.content = redata.content
        this.msgData.receiveTime = redata.receiveTime
        // 获得未读数据
        this.getMsgNum()
        // 消息
        this.$notify.info({
          title: '消息提醒',
          offset: 100,
          dangerouslyUseHTMLString: true,
          message: `<div style="margin-bottom:8px;">${redata.content}</div>
      <div style="text-align: right;">
      ${redata.receiveTime}
      </div>`,
          position: 'bottom-right',
          duration: 8000
        })
      }

      //
    },
    websocketsend(Data) { // 数据发送
      this.websock.send(Data)
      //
    },
    websocketclose(e) { // 关闭
      // console.log('关闭', e)
    },
    getMsgNum() {
      // 已读未读数据查询
      getNumByReadType({ readStatus: 0 }).then(res => {
        if (res.code === 200) {
          eventBus.$emit('getMsgCount', res.data)
        }
      })
    }
  }
}
</script>

