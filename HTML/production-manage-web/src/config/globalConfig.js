import { Base64 } from 'js-base64'
export function filePreview(url) {
  const preview = '/preview'
  return preview + '/onlinePreview?url=' + encodeURIComponent(Base64.encode(url))
}
