<?php
/**
 * 
 * Componente que monta o Ordenador da Grid
 * @author Ráyner Lima <rayner.lima@squadra.com.br>
 *
 */
class Ordenador {
	
	private $strNomeOrdenador;
	private $arrCamposOrdenacao;
	private $strUrlAjax;
	
	const ORDEM_PADRAO = 'asc';

	/**
	 * 
	 * Recebe um nome para o ordenador com o propósito de ser impresso para nome de classes css e ids para o html
	 * @param str $strNomeOrdenador
	 */
	public function setNomeOrdenador( $strNomeOrdenador ){
		$this->strNomeOrdenador = $strNomeOrdenador;
	}
	
	/**
	 *
	 * Informa à classe um array associativo com campos que servirão de atributos para ordenação,
	 * onde a chave do array é o que será apresentado na página e o valor é o nome do campo no banco de dados.
	 * Caso o o valor de uma chave seja <code>null</code>, o ordenador apresentará na tela apenas um texto com a chave do array, 
	 * caso contrário apresentará um link com o valor do array.
	 * <code>$arrCamposOrdenacao = array('Nome do usuário'=>'no_usuario', 'Data de Nascimento'=>'dt_nascimento');</code>
	 * @param array $arrCamposOrdenacao
	 */
	public function setCamposOrdenacao( $arrCamposOrdenacao ){
		$this->arrCamposOrdenacao = $arrCamposOrdenacao;
	}
	
	/**
	 * 
	 * Informa à classe uma url que será chamada ao clicar num link impresso pelo ordenador
	 * @param str $strUrlAjax
	 */
	public function setUrlAjax( $strUrlAjax ) {
		$this->strUrlAjax = $strUrlAjax;
	}
	
	/**
	 * 
	 * Imprime o cabeçalho na página gerando uma saída html
	 */
	public function montaOrdenador() {
		$elementos = "<thead>";
		$elementos .= 	"<tr id=\"ordenador\">";
		$x = 1;
		$urlAjax = $this->strUrlAjax;
		$nomeOrdenador = $this->strNomeOrdenador;
		foreach ($this->arrCamposOrdenacao as $label => $campo) {
			if ($x==1){
				$ativo = "ativo";
			} else {
				$ativo = "";
			}
			if ($campo == null){
				$elementos .= "<th>$label</th>";
			} else {
				$elementos .= "<th><a id=\"$campo-asc\" href=\"$urlAjax\" class=\"ordenador $nomeOrdenador $ativo\">$label</th>";
				++$x;
			}
		}
		$elementos .= 	"</tr>";
		$elementos .= "</thead>";
		return $elementos;
	}
	
}