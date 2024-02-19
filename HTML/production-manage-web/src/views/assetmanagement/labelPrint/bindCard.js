import { batchUpdateRfId, getMaxRfid } from '@/api/assetmanagement/assetsParameter'
import { readUrl, writeUrl } from '@/utils/jri'
import axios from 'axios'
// 读取rfid
async function readRfid() {
  const url = readUrl
  const res = await axios.get(url)
  if (res.data.code === 200) {
    return !!Number(res.data.data > 0)
  } else {
    // 读卡失败
    return '读卡失败'
  }
}
// 绑定rfid
async function handleBind(_this, row, fun) {
  const data = []
  let code = ''
  // 从接口获取一个rfid
  const res = await getMaxRfid()
  if (res.code !== 200) {
    return _this.$message.error('获取RFIDCODE失败')
  } else {
    code = res.data
    const obj = {
      id: row.id,
      rfidCode: res.data
    }
    data.push(obj)
  }
  // 写卡
  const url = writeUrl
  const res1 = await axios.get(url, {
    params: {
      code: code
    }
  })
  const res2 = res1.data
  if (res2.code !== 200) {
    return _this.$message.error('写卡失败')
  }
  // 将写入的rfid传给接口绑定
  const res3 = await batchUpdateRfId(data)
  if (res3.code !== 200) {
    if (row.isBind === '0') {
      _this.$message.success('RFID绑卡失败')
    } else {
      _this.$message.success('RFID更新失败')
    }
    return
  }
  if (row.isBind === '0') {
    _this.$message.success('RFID绑卡成功')
  } else {
    _this.$message.success('RFID更新成功')
  }
  fun()
}
// 绑卡
async function bindCardFun(_this, row, fun) {
  const isBind = await readRfid()
  console.log(isBind, 'isBind')
  if (typeof isBind === 'string') {
    return _this.$message.error('读卡失败')
  }
  if (isBind) {
    return _this.$message.error('此卡已被绑定，无法再次绑定')
  }
  handleBind(_this, row, fun)
}

export { bindCardFun }
