<?php
	class UrlInfo {
		
		/**
		 * @var string
		 */
		private $url;
		
		public static function loadUrlInfo($url) {
			$urlInfo = new UrlInfo();
			$urlInfo->setUrl($url);
			return ($urlInfo);
		}
		
		/**
		 * @return string
		 */
		public function getUrl() {
			return $this->url;
		}
		
		/**
		 * @param string $url
		 */
		public function setUrl($url) {
			$this->url = $url;
		}
	}
?>