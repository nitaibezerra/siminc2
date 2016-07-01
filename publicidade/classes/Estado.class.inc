<?php

/**
 * Modelo de Estados(UF's)
 * @author Alysson Rafael
 */
class Estado extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'territorios.estado';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('estuf');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos	= array(
		'muncodcapital' => null,
		'regcod' => null,
		'estcod' => null,
		'estdescricao' => null
	);
	
	
	/**
     * Gerar a combo de uf
     * @name monta_combo_uf
     * @author Alysson Rafael
     * @access public
     * @param string $estuf       - Qual option receberá o selected
     * @param string $obrigatorio - Se a combo vai ou não exibir a imagem de obrigatório(S ou N)
     * @param string $origem      - De onde foi chamada a função
     * @return void
     */
	public function monta_combo_uf($estuf='',$obrigatorio='N',$origem=''){
		
		$sql = "SELECT estuf AS codigo,estdescricao AS descricao FROM $this->stNomeTabela ORDER BY estdescricao ";

		$resultado = $this->carregar($sql);

		if($origem == 'cadastro'){
			$this->monta_combo('estuf',$resultado,'S','Selecione...','carregarMunicipio(this.value,\'\',\''.$obrigatorio.'\');','','','',$obrigatorio,'estuf','',$estuf);
		}
		else{
			$this->monta_combo('estuf',$resultado,'S','Selecione...','','','','',$obrigatorio,'estuf','',$estuf);
		}
	}
	
	
	
	

	
}