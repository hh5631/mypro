import { Message } from 'element-ui'
let wsocket// websocket对象
// =========================新增方法
export async function connectPrinterPromise() { // 连接打印机
  // console.log('wsocket', wsocket)
  // 如果已经有连接
  if (wsocket != null) {
    // 连接正常则直接返回
    if (wsocket.readyState === 1) {
      return
    }
    // 异常则直接清空socket准备重建
    wsocket = null
  }

  await connectionPromise()// 连接打印服务
}
function connectionPromise() { // 连接打印服务
  return new Promise((resolve, reject) => {
    const wsServer = 'ws://' + '127.0.0.1' + ':' + '1808' + '/'
    if ('WebSocket' in window) {
      wsocket = new WebSocket(wsServer)
      console.log('wsocket1', wsocket)
    } else if ('MozWebSocket' in window) {
      // eslint-disable-next-line no-undef
      wsocket = new MozWebSocket(wsServer)
      console.log('wsocket2', wsocket)
    } else {
      alert('不支持当前浏览器！')
      reject('不支持当前浏览器')
    }

    // 注册各类回调
    wsocket.onopen = () => {
      Message.success('连接打印服务成功!正在打印请稍后....')
      resolve('连接成功')
    }

    wsocket.onclose = function () {
      console.log('与打印服务断开连接！')
      Message.error('与打印服务断开连接！')
    }

    wsocket.onerror = function () { // 出错的提示
      alert('数据传输错误，请检查打印服务是否已经运行！')
      reject('连接失败!')
    }

    wsocket.onmessage = function (receiveMsg) { // 接收服务端数据返回
      const receiveContent = receiveMsg.data
      if (receiveContent.indexOf('PrinterStatus_USB:') === 0) { // 打印机状态码
        const printerStatus = parseInt(receiveContent.replace('PrinterStatus_USB:', ''))
        if (printerStatus !== 0) { // 0是打印机状态正常
          const statusMsg = checkErrorStatus(printerStatus)
          endPromise('error', { data: statusMsg })
          console.log('打印机异常')
          alert('打印机状态异常：' + statusMsg)
          return
        } else { // 打印标签
          console.log('打印机正常')
          endPromise('success', { type: 1, data: '打印机状态正常' })
        }
      } else if (receiveContent.indexOf('ZM_PrintLabel_Preview:') === 0) { // 返回的标签预览图片数据
        endPromise('success', { type: 2, data: receiveContent.replace('ZM_PrintLabel_Preview:', '') })
      } else {
        // 显示服务传回来的所有数据
        if (receiveContent.indexOf('Error:') === 0) {
          endPromise('error', { data: receiveContent })
          return
        }
        // setTimeout(() => {
        //   console.log('10秒时间到=====')
        //   endPromise('success', { type: 3, data: receiveContent })
        // }, 5000)
        endPromise('success', { type: 3, data: receiveContent })
      }
    }
  })
}
/** 结束promise
 * type有success和error 两种
 * data格式为: { type: number, data: any}
 */
function endPromise(type, data) {
  const firstPromise = promiseList.shift()
  // debugger
  if (!firstPromise) return
  if (type === 'success') {
    firstPromise.resolve({
      code: 200,
      data: data.data,
      type: data.type
    })
  } else {
    firstPromise.reject({
      code: 500,
      data: data.data
    })
  }
}
function checkErrorStatus(printerStatus) {
  let statusMsg = ''
  switch (printerStatus) {
    case 88:
      statusMsg = '打印机已经暂停，请按[暂停]按键，再重新打印!'
      break
    case 82:
      statusMsg = '打印机碳带错误，请重新安装碳带后按[取消]按键，再重新打印!'
      break
    case 83:
      statusMsg = '打印机标签错误，请重新安装标签后按[取消]按键，再重新打印!'
      break
    case 90:
      statusMsg = 'RFID标签读写错误，请按[取消]按键，再重新打印!'
      break
    case 91:
    case 92:
      statusMsg = 'RFID标签校准错误，请按[取消]按键，再重新打印!'
      break
    case -1007:
      statusMsg = '打印机端口异常，请检查打印机是否正常连接且已开机。'
      break
    default:
      statusMsg = '打印机状态异常，错误代码：' + printerStatus
      break
  }
  return statusMsg
}
// promise列表 项为对象{ promise, resolve, reject }
const promiseList = []
// 检测和发送
async function checkAndSendPromise(data) {
  // 新建本次的promise
  let resolve_
  let reject_
  const promise = () => {
    return new Promise((resolve, reject) => {
      resolve_ = resolve
      reject_ = reject
    })
  }
  const promise_ = promise()
  promiseList.push({
    promise: promise_,
    resolve: resolve_,
    reject: reject_
  })
  // 此处需要缓存列表 ,因为promiseList 在任务完成时会移除首位.
  const tmp = [...promiseList]
  // 长度减一, 排除自身,等待其他任务完成
  for (let index = 0; index < tmp.length - 1; index++) {
    await tmp[index].promise
  }
  wsocket.send(data)
  return await promise_
}

// 打印标签，即将JSON字符串发给打印服务处理
// 返回code200为正常500为异常, type 1 2 3 分别为打印机状态, 预览内容, 普通内容
export async function printLabelPromise(data) {
  await connectPrinterPromise()
  const jsonString = JSON.stringify(data)
  const newJsonString = jsonString.replace('"Operate":"preview"', '"Operate":"print"')
  return await checkAndSendPromise(newJsonString)
}
// 预览标签，即将JSON字符串发给打印服务处理，将print替换为preview
export async function previewLabel(data) {
  await connectPrinterPromise()
  const jsonString = JSON.stringify(data)
  const newJsonString = jsonString.replace('"Operate":"print"', '"Operate":"preview"')
  return await checkAndSendPromise(newJsonString)
}
export async function printStartPromise() {
  await connectPrinterPromise()
  return await checkAndSendPromise('ZM_GetPrinterNameAndSN')
  // wsocket.send('ZM_GetPrinterNameAndSN')
  // return await onMessagePromise
}
