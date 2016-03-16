<?php
/**
 * Modelo de Baixa de Bens RGP
 * @author Silas Matheus
 */
class BensBaixaRgp extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sap.bensbaixargp";	

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("bbrid");

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */    
    protected $arAtributos = array(
	  	'bbrid' => null,
    	'bebid' => null, 
	  	'rgpid' => null
	);

	/**
	 * Salva novo registro
	 * @name salvar
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos para inserir
	 * @return int
	 */
	public function salvar($campos){
		
		extract($campos);

		$sql = "INSERT INTO 
					$this->stNomeTabela 
					(bebid, rgpid)
				VALUES 
					($bebid, $rgpid)
				RETURNING 
					bbrid";
		
	 	$bbrid = $this->pegaUm($sql);
	 	$this->commit(); 
		return $bbrid;
	
	}
	
	/**
	 * Exclui registro
	 * @name excluir
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador da Bens baixa rgp
	 * @return bool
	 */
	public function excluir($id){
	
		$id = addslashes($id);
		
		$sql = " DELETE FROM $this->stNomeTabela WHERE bbrid=$id returning rgpid";
		$rgpid = $this->pegaUm($sql);

		if($rgpid){
			$this->commit();
			$oRgp = new Rgp();
			$oRgp->atualizarSituacao($rgpid, SBM_NORMAL);
		}		
		
		return $return;
		
	}
	
	/**
	 * Recupera dados do registro
	 * @name pegarRegistro
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador da Bens baixa rgp
	 * @return array
	 */
	public function pegarRegistro($id){
	
		$id = addslashes($id);
		
		$sql = "SELECT 
					* 
				FROM 
					$this->stNomeTabela bbr
				INNER JOIN
					sap.bensbaixa beb
					ON beb.bebid=bbr.bebid
				INNER JOIN
					sap.rgp rgp
					ON rgp.rgpid=bbr.rgpid
				WHERE 
					bbrid=$id";
		$return = $this->carregar($sql);
		
		return $return[0];
		
	}
	
	/**
	 * Valida se existem rgps associados a baixa
	 * @name validaTotalBaixas
	 * @author Silas Matheus
	 * @access public
	 * @param int $bebid - Identificador da Baixa
	 * @return int
	 */	
	public function validaTotalBaixas($bebid){
		
		// total de tombamentos
		$sql = "SELECT
					count(*) count
				FROM
					$this->stNomeTabela
				WHERE
					bebid=$bebid";
		
		$result = $this->carregar($sql);
 	   	$qtd = $result[0]['count'];
		
 	   	if($qtd > 0)
 	   		$resultado = true;

		return $resultado;
		
	}
	
	/**
	 * Valida se os rgps associados a baixa estao inserviveis
	 * @name validaMotivoBaixas
	 * @author Silas Matheus
	 * @access public
	 * @param int $bebid - Identificador da Baixa
	 * @return int
	 */	
	public function validaMotivoBaixas($bebid){
		
		// total de tombamentos
		$sql = "SELECT
					count(*) count
				FROM
					$this->stNomeTabela bbr
				INNER JOIN
					sap.rgp rgp
					ON bbr.rgpid=rgp.rgpid
				INNER JOIN
					sap.estadomotivos esm
					ON esm.esmid=rgp.esmid
				INNER JOIN
					sap.bensbaixa beb
					ON beb.bebid=bbr.bebid
				WHERE
					beb.bebid=$bebid
				AND
					beb.tesid=" . TES_SAIDA_DOACAO . "
				AND
					esm.ecoid<>" . ECO_INSERVIVEL;
		
		$result = $this->carregar($sql);
 	   	$qtd = $result[0]['count'];
		
 	   	if($qtd == 0)
 	   		$resultado = true;

		return $resultado;
		
	}
	
	
	
	
	
	
	
	
	/**
	 * Retorna os ids dos rgps relacionados ao processo de baixa
	 * @name pegarRpgsDaBaixa
	 * @author Alysson Rafael
	 * @access public
	 * @param int $bebid - Identificador da Baixa
	 * @return array
	 */	
	public function pegarRpgsDaBaixa($bebid){
		$sql = "SELECT rgpid FROM $this->stNomeTabela WHERE bebid='{$bebid}' ";
		$result = $this->carregar($sql);
		return $result[0];
	}
	
	
	
	
	
	
	
}