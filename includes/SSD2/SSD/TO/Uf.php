<?php
	class Uf {
		/**
		 * @var string
		 */
		private $descricao;

		/**
		 * @var string
		 */
		private $sigla;

		public static function loadUf($descricao, $sigla) {
			$uf = new Uf();
			$uf->setDescricao($descricao);
			$uf->setSigla($sigla);
			return($uf);
		}

		/**
		 * @return string
		 */
		public function getDescricao() {
			return $this->descricao;
		}

		/**
		 * @return string
		 */
		public function getSigla() {
			return $this->sigla;
		}

		/**
		 * @param string $descricao
		 */
		public function setDescricao($descricao) {
			$this->descricao = $descricao;
		}

		/**
		 * @param string $sigla
		 */
		public function setSigla($sigla) {
			$this->sigla = $sigla;
		}
	}
?>