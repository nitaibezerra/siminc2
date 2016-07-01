<?php 
/**
 * 
 * Componente que monta o Paginador da Grid
 * @author Ráyner Lima <rayner.lima@squadra.com.br>
 *
 */
class Paginador {
	
	private $qtdeTotalRegistros;
	private $qtdePorPagina;
	private $strUrlAjax;
	private $strNomePaginador;
	
	const LIMIT_PADRAO = 20;
	const OFFSET_PADRAO = 0;
	
	/**
	 * 
	 * Informa a quantidade total de registros a serem paginados
	 * @param int $qtdeTotalRegistros
	 */
	public function setQtdeTotalRegistros( $qtdeTotalRegistros ){
		$this->qtdeTotalRegistros = $qtdeTotalRegistros;
	}
	
	/**
	 * 
	 * Retorna a quantidade Total de Registros a serem paginados
	 */
	public function getQtdeTotalRegistros(){
		return $this->qtdeTotalRegistros;
	}
	
	/**
	 * 
	 * Informa a quantidade de registros que devem ser apresentados por página.
	 * @param int $qtdePorPagina
	 */
	public function setQtdePorPagina( $qtdePorPagina ){
		$this->qtdePorPagina = $qtdePorPagina;
	}
	
	/**
	 * 
	 * Retorna a quantidade de registros que devem ser apresentados por página.
	 */
	public function getQtdePorPagina(){
		if ($this->qtdePorPagina == null || $this->qtdePorPagina == 0 || 
				$this->qtdePorPagina == '') {
			$qtdePorPagina = Paginador::LIMIT_PADRAO;
		} else {
			$qtdePorPagina = $this->qtdePorPagina;
		}
		return $qtdePorPagina;
	}
	
	/**
	 * 
	 * Retorna a quantidade de páginas.
	 */
	public function getQtdeDePaginas(){
		if ($this->getQtdeTotalRegistros() % $this->getQtdePorPagina() > 0) {
			$qtdePagina = floor($this->getQtdeTotalRegistros() / $this->getQtdePorPagina());
			return ++$qtdePagina;
		} else {
			return $this->getQtdeTotalRegistros() / $this->getQtdePorPagina();
		}
	}
	
	/**
	 * 
	 * Informa a url de uma action ajax onde os registros de uma determinada página serão apresentados.
	 * @param str $strUrlAjax
	 */
	public function setUrlAjax( $strUrlAjax ){
		$this->strUrlAjax = $strUrlAjax;
	}
	
	/**
	 * 
	 * Retorna a url de uma action ajax onde os registros de uma determinada página serão apresentados.
	 */
	public function getUrlAjax(){
		return $this->strUrlAjax;
	}
	
	/**
	 * 
	 * Retorna uma string html que será impresso na tela.
	 */
	public function montaPaginador(){
		$nomePaginador = $this->getNomePaginador();	
		$listaPaginada = $this->geraListaPaginada();
		
		$qtdeDePaginas = $this->getQtdeDePaginas();		
		$elemento  = "<tfoot class=\"$nomePaginador\">";
		$elemento .=	"<tr id=\"paginador\" style=\"background-color: #FFF; border-top: 2px solid #404040;\">";
		$elemento .=	 	"<th class=\"paginador registros\" style=\"background-color: #FFF; text-align: left;\"> ";
		$elemento .=			"<span class=\"label\">Total de Registros:</span><span class=\"qtdeTotalRegistros\">" . $this->getQtdeTotalRegistros() . "</span>";
		$elemento .=		"</th>";
		$elemento .= 		"<th class=\"paginador paginas\" style=\"background-color: #FFF; text-align: right;\">";
		$elemento .=			"$listaPaginada ";
		$elemento .=		"</th>";
		$elemento .=	"</tr>";
		$elemento .= "</tfoot>";
		return $elemento;
	}
	
	/**
	 * 
	 * Informa um nome para o paginador. O nome será utilizado para identificar com atributos <code>id</code> e <code>class</code>.
	 * @param str $strNomePaginador
	 */
	public function setNomePaginador( $strNomePaginador ) {
		$this->strNomePaginador = $strNomePaginador;
	}
	
	/**
	 * 
	 * Retorn um nome para o paginador. O nome será utilizado para identificar com atributos <code>id</code> e <code>class</code>.
	 */
	public function getNomePaginador(){
		return $this->strNomePaginador;
	}
	
	/**
	 * 
	 * Cria links para o paginador, números que serão apresentados no rodapé da tabela.
	 */
	private function geraListaPaginada() {
		$nomePaginador = $this->getNomePaginador();		
		$urlAjax = $this->getUrlAjax();
		$limit = $this->getQtdePorPagina();
		
		
		if ( $this->getQtdeDePaginas() == 1 ){
			$paginas = "Página: <a id=\"0-$limit\" href=\"$urlAjax\" class=\"paginador ativo ultima unico $nomePaginador\" >1</a>";
		} else {
			$paginas = "Páginas: ";
			for ($i = 1; $i <= $this->getQtdeDePaginas(); $i++){
				$offset = $i * $limit - $limit;
				
				//Ao renderizar links de paginação atribui class="ativo" ao primeiro elemento HTML
				if ($i == 1) {
					$ativo = "ativo";
				} else {
					$ativo = "";
				}
				
				//Atribui class="ultima" ao último elemento HTML
				if ($i == $this->getQtdeDePaginas()) {
					$ultima = "ultima";
				}
				
				//if ($i < 11){
				//	$agrupamento = "agrupamento-1";
				//}
				
				$paginas .= "<a id=\"$offset-$limit\" href=\"$urlAjax\" class=\"paginador $ativo $ultima $nomePaginador\" >$i</a>";
			}
		}
		return $paginas;
	}
	
}
